// Http.h
// 2013/11/20

#ifndef Http_h
#define Http_h

#include <string>

namespace portable
{

class Http
{	typedef std::string String;
    enum http_retcode
	{	/* Client side errors */
		ERRHOST=-1, /* No such host */
		ERRSOCK=-2, /* Can't create socket */
		ERRCONN=-3, /* Can't connect to host */
		ERRWRHD=-4, /* Write error on socket while writing header */
		ERRWRDT=-5, /* Write error on socket while writing data */
		ERRRDHD=-6, /* Read error on socket while reading result */
		ERRPAHD=-7, /* Invalid answer from data server */
		ERRNULL=-8, /* Null data pointer */
		ERRNOLG=-9, /* No/Bad length in header */
		ERRMEM=-10, /* Can't allocate memory */
		ERRRDDT=-11,/* Read error while reading data */
		ERRURLH=-12,/* Invalid url - must start with 'http://' */
		ERRURLP=-13,/* Invalid port in url */
  		/* Return code by the server */
		ERR400=400, /* Invalid query */
		ERR403=403, /* Forbidden */
		ERR408=408, /* Request timeout */
		ERR500=500, /* Server error */
		ERR501=501, /* Not implemented */
		ERR503=503, /* Service overloaded */
		/* Succesful results */
		OK0 = 0,   /* successfull parse */
		OK201=201, /* Ressource succesfully created */
		OK200=200  /* Ressource succesfully read */
	};
	enum querymode
	{	CLOSE,  /* Close the socket after the query (for put) */
		KEEP_OPEN /* Keep it open */
	};
private:
	String http_server;
	int http_port;
	String http_proxy_server;
	int http_proxy_port;
	String filename;
	const char* http_user_agent;
	http_retcode ret;
private:
	bool IsGood() const
	{	return ret>=0;
	}
	int http_read_line (int fd,char *buffer, int max);
	int http_read_buffer (int fd,char *buffer, int max);
	bool http_put(char *data, int length, int overwrite, char *type) ;
	bool http_get(char *data,int *plength, char *typebuf);
	bool http_parse_url(const char *url);
	bool http_delete();
	bool http_head(int *plength, char *typebuf);
	bool http_query(const char *command, const char *url,const char *additional_header, querymode mode, char* data, int length, int *pfd);
	int read(int s,char*buf,int len);
	int write(int s,const char*buf,int len);
	void close(int s);
	int GetSystemError(); 
public:
	Http(const char* http_user_agent=0)
	:	http_port(0),
		http_proxy_port(0),
		http_user_agent(http_user_agent),
		ret(OK0)
	{	if(!http_user_agent)
		{	http_user_agent="Http lib/1.0";
	}	}
	int Get(const String& url,char* data,int length)
	{	const bool ok=http_parse_url(url.c_str());
		if(!ok)
		{	return -1;
		}
		char typebuf[70];
		if(!http_get(data,&length,typebuf))
		{	return -1;
		}
		return length;
	}
};

}

#endif
