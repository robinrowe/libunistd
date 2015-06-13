// BsdSocket.h
//

#ifndef BsdSocket_h
#define BsdSocket_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unistd.h"
#include <sys/types.h> 
#include "sys/socket.h"
#include "netinet/in.h"

#define bzero(x,y) memset(x,y,0)

class BsdSocket
{	int sockfd;
	const char* error_msg;
public:
	BsdSocket()
	{	error_msg=0;
	}
	void Close()
	{	if(sockfd<0)
		{	return;
		}
		closesocket(sockfd);
		sockfd=-1;
	}
	bool Open(int portno)
	{	sockaddr_in serv_addr;
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0) 
        {	error_msg="ERROR opening socket";
			return false;
		}
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(portno);
		if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		{	error_msg="ERROR on binding";
			return false;
		}
		return true;
	}
	bool Listen(int qlimit)
	{	listen(sockfd,qlimit);
		return true;
	}
	int Accept()
	{	int newsockfd;
		sockaddr_in cli_addr; 
		socklen_t clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
		if(newsockfd < 0) 
		{	error_msg="ERROR on accept";
			return 0;
		}
		return newsockfd;
	}
};

#endif

/*     char buffer[256];
     int n;
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd);
*/

