// Http.cpp
// 2013/11/20

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "Http.h"

//#define VERBOSE
namespace portable
{
/*
 * read a line from file descriptor
 * returns the number of bytes read. negative if a read error occured
 * before the end of line or the max.
 * cariage returns (CR) are ignored.
 */
int Http::http_read_line (int fd,char *buffer,int max) 
{ /* not efficient on long lines (multiple unbuffered 1 char reads) */
  int n=0;
  while (n<max) {
    if (read(fd,buffer,1)!=1) {
      n= -n;
      break;
    }
    n++;
    if (*buffer=='\015') continue; /* ignore CR */
    if (*buffer=='\012') break;    /* LF is the separator */
    buffer++;
  }
  *buffer=0;
  return n;
}

/*
 * read data from file descriptor
 * retries reading until the number of bytes requested is read.
 * returns the number of bytes read. negative if a read error (EOF) occured
 * before the requested length.
 */
int Http::http_read_buffer (int fd,char *buffer,int length) 
{	if(!buffer)
	{	return 0;
	}
	const int bytes=read(fd,buffer,length);
	if(bytes==length)
	{	buffer[length-1]=0;
	}
	else
	{	buffer[bytes]=0;
	}
	return bytes;
}

/* beware that filename+type+rest of header must not exceed MAXBUF */
/* so we limit filename to 256 and type to 64 chars in put & get */
#define MAXBUF 512

/*
 * Pseudo general http query
 *
 * send a command and additional headers to the http server.
 * optionally through the proxy (if http_proxy_server and http_proxy_port are
 * set).
 *
 * Limitations: the url is truncated to first 256 chars and
 * the server name to 128 in case of proxy request.
 */
bool Http::http_query(const char *command, const char *url,const char *additional_header, querymode mode, char* data, int length, int *pfd)
{	struct  sockaddr_in server;
	char header[MAXBUF];
	int hlg;
	if (pfd) 
	{	*pfd=-1;
	}
#pragma warning(disable : 4996)
	hostent* hp = gethostbyname( http_proxy_server.size() ? http_proxy_server.c_str() : http_server.c_str() );
	if(!hp)
	{	ret = ERRHOST;
		return IsGood();
	} 
	memset((char *) &server,0, sizeof(server));
	memmove((char *) &server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_family = hp->h_addrtype;
	server.sin_port = htons( http_proxy_server.size() ? u_short(http_proxy_port):u_short(http_port) );
	int s = (int) socket(AF_INET, SOCK_STREAM, 0);
	if(s < 0)
	{	ret = ERRSOCK;
		return IsGood();
	}
	setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, 0, 0);
	if (connect(s, (const sockaddr*) &server, sizeof(server)) < 0) 
	{	ret=ERRCONN;
		return IsGood();
	}
	if(pfd) 
	{	*pfd=s;
    }
	if (http_proxy_server.size()) 
	{	sprintf(header, "%s http://%.128s:%d/%.256s HTTP/1.1\r\n"
		"Host: %s:%i\r\n"
		"Connection: close\r\n"
		"User-Agent: %s\r\n"
		"%s\r\n\r\n",
		command,http_server.c_str(), http_port, url, 
		http_server.c_str(),http_port,
		http_user_agent, 
		additional_header);
	} 
	else 
	{	sprintf(header, "%s %.256s HTTP/1.1\r\n"
		"Host: %s:%i\r\n"
		"Connection: close\r\n"
		"User-Agent: %s\r\n"
		"%s\r\n\r\n",	
		command,url, 
		http_server.c_str(), http_port, 
		http_user_agent, 
		additional_header);
	}
	hlg=(int)strlen(header);
	if(write(s,header,hlg)!=hlg)
	{	ret=ERRWRHD;
		close(s);
		return IsGood();
	}
	if(length && data && (write(s,data,length)!=length) ) 
	{	ret=ERRWRDT;
		close(s);
		return IsGood();
	}
	const int bytes = http_read_line(s,header,MAXBUF-1);
#ifdef VERBOSE
	fputs(header,stderr);
	putc('\n',stderr);
#endif	
	if(bytes<=0) 
	{	ret=ERRRDHD;
		close(s);
		return IsGood();
	}
	if(sscanf(header,"HTTP/1.%*d %03d",(int*)&ret)!=1) 
	{	ret=ERRPAHD;
		close(s);
		return IsGood();
	}
	if(mode==KEEP_OPEN)
	{	return IsGood();
	}
	close(s);
	return IsGood();
}


/*
 * Put data on the server
 *
 * This function sends data to the http data server.
 * The data will be stored under the ressource name filename.
 * returns a negative error code or a positive code from the server
 *
 * limitations: filename is truncated to first 256 characters 
 *              and type to 64.
 */
bool Http::http_put(char *data,int length,int overwrite,char *type)
{
  char header[MAXBUF];
  if (type) 
    sprintf(header,"Content-length: %d\015\012Content-type: %.64s\015\012%s",
	    length,
	    type  ,
	    overwrite ? "Control: overwrite=1\015\012" : ""
	    );
  else
    sprintf(header,"Content-length: %d\015\012%s",length,
	    overwrite ? "Control: overwrite=1\015\012" : ""
	    );
  return http_query("PUT",filename.c_str(),header,CLOSE, data, length, NULL);
}
#pragma warning(default : 4996)
/*
 * Get data from the server
 *
 * This function gets data from the http data server.
 * The data is read from the ressource named filename.
 * Address of new new allocated memory block is filled in pdata
 * whose length is returned via plength.
 * 
 * returns a negative error code or a positive code from the server
 * 
 *
 * limitations: filename is truncated to first 256 characters
 */

#pragma warning(disable : 4996)

bool Http::http_get(char *pdata,int  *plength,char *typebuf)
{	char header[MAXBUF];
	char *pc;
	int  fd;
	int  n,length=-1;
	if (!pdata) 
	{	ret = ERRNULL; 
		return IsGood();
	}
	*pdata=0;
	if (typebuf) 
	{	*typebuf='\0';
	}
	http_query("GET",filename.c_str(),"",KEEP_OPEN, NULL, 0, &fd);
	if(ret!=200) 
	{	if (ret>=0)
		{	close(fd);
		}
		return IsGood();
	}
	for(;;)
	{	n=http_read_line(fd,header,MAXBUF-1);
#ifdef VERBOSE
		fputs(header,stderr);
		putc('\n',stderr);
#endif	
		if (n<=0) 
		{	close(fd);
			ret = ERRRDHD;
			return IsGood();
		}
		/* empty line ? (=> end of header) */
		if ( n>0 && (*header)=='\0') 
		{	break;
		}
		length+=n;
		/* try to parse some keywords : */
		/* convert to lower case 'till a : is found or end of String */
		for (pc=header; (*pc!=':' && *pc) ; pc++) 
		{	*pc=char(tolower(*pc));
		}
//		sscanf(header,"content-length: %d",&length);
		if (typebuf) 
		{	sscanf(header,"content-type: %s",typebuf);
	}	}
	if (length<=0) 
	{	close(fd);
		ret = ERRNOLG;
		return IsGood();
	}
	if(*plength<length)
	{	length=*plength;
	}
	n=http_read_buffer(fd,pdata,length);
	close(fd);
	if (n<0) 
	{	ret=ERRRDDT;
	}
	return IsGood();
}

/*
 * Request the header
 *
 * This function outputs the header of thehttp data server.
 * The header is from the ressource named filename.
 * The length and type of data is eventually returned (like for http_get(3))
 *
 * returns a negative error code or a positive code from the server
 * 
 * limitations: filename is truncated to first 256 characters
 */
bool Http::http_head(int  *plength,char *typebuf)
{
  char header[MAXBUF];
  char *pc;
  int  fd;
  int  n,length=-1;

  if (plength) *plength=0;
  if (typebuf) *typebuf='\0';

  http_query("HEAD",filename.c_str(),"",KEEP_OPEN, NULL, 0, &fd);
  if (ret==200) {
   for(;;) {
      n=http_read_line(fd,header,MAXBUF-1);
#ifdef VERBOSE
      fputs(header,stderr);
      putc('\n',stderr);
#endif	
      if (n<=0) {
	close(fd);
	ret = ERRRDHD;
	return IsGood();
      }
      /* empty line ? (=> end of header) */
      if ( n>0 && (*header)=='\0') break;
      /* try to parse some keywords : */
      /* convert to lower case 'till a : is found or end of String */
      for (pc=header; (*pc!=':' && *pc) ; pc++) *pc=char(tolower(*pc));
      sscanf(header,"content-length: %d",&length);
      if (typebuf) sscanf(header,"content-type: %s",typebuf);
    }
    if (plength) *plength=length;
    close(fd);
  } else if (ret>=0) close(fd);
  return IsGood();
}

/*
 * Delete data on the server
 *
 * This function request a DELETE on the http data server.
 *
 * returns a negative error code or a positive code from the server
 *
 * limitations: filename is truncated to first 256 characters 
 */

bool Http::http_delete()
{  return http_query("DELETE",filename.c_str(),"",CLOSE, NULL, 0, NULL);
}

/* parses an url : setting the http_server and http_port global variables
 * and returning the filename to pass to http_get/put/...
 * returns a negative error code or 0 if sucessfully parsed.
 */
bool Http::http_parse_url(const char *url) 
{	if (strncasecmp("http://",url,7)) 
	{
#ifdef VERBOSE
    fprintf(stderr,"invalid url (must start with 'http://')\n");
#endif
		ret = ERRURLH;
		return IsGood();
	}
	// http://server:port/
	url+=7;
	http_server.erase();
	filename.erase();
	http_port=80;
	const char* domain=url;
	char* endOf=const_cast<char*>(strchr(domain,':'));
	if(endOf)
	{	http_server=String(domain,endOf-domain);
		endOf++;
		if (sscanf(endOf,"%d",&http_port)!=1) 
		{
#ifdef VERBOSE
			fprintf(stderr,"invalid port in url\n");
#endif
			ret = ERRURLP;
			return IsGood();
	}	}
	endOf= const_cast<char*>(strchr(domain,'/'));
	if(!http_server.size())
	{	if(endOf)
		{	http_server=String(domain,endOf-domain);
		}
		else
		{	http_server=String(domain);
			ret = OK0;
			return IsGood();
	}	}
	filename=String(endOf);
#ifdef VERBOSE
	fprintf(stderr,"host=%s, port=%d, filename=%s\n",
	http_server.c_str(),http_port,filename.c_str());
#endif
	ret = OK0;
	return IsGood();
}

int Http::read(int s,char*buf,int len)
{
#ifdef WIN32
	int size=recv(s,buf,len,0);
#if 0
	if(size==SOCKET_ERROR)
	{	const int err=WSAGetLastError();
	}
#endif
	if(size==16384)
	{	size+=recv(s,buf+16384,len,0);
	}
//	cout<<"Read bytes: "<<size<<endl;
#else
	const int size=::read(s,buf,len);
#endif
	return size;
}

int Http::write(int s,const char*buf,int len)
{
#ifdef WIN32
	return send(s,buf,len,0);
#else
	return ::write(s,buf,len);
#endif
}

void Http::close(int s)
{	
#ifdef WIN32
	closesocket(s);
#else
	::close(s);
#endif
}

#ifdef WIN32
int Http::GetSystemError() 
{	return WSAGetLastError();
}
#else
int Http::GetSystemError() 
{	return 0;
}
#endif

}

