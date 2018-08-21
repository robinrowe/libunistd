// HttpDecoder.h
// Copyright Robin.Rowe@Cinepaint.org 2015/6/14
// License MIT Open Source
//
#ifndef HttpDecoder_h
#define HttpDecoder_h

#ifdef ENABLE_FASTCGI
#include "fcgi_stdio.h"
#else
#include <stdio.h>
#endif
#include <stdlib.h>
//#include <stdbool.h>
#include <qdecoder/src/qdecoder.h>

#ifdef ENABLE_FASTCGI
#	define FASTCGI_START while(FCGI_Accept() >= 0) {
#	define FASTCGI_STOP }
#else
#	define FASTCGI_START	
#	define FASTCGI_STOP
#endif

class HttpDecoder
{	qentry_t* req;
public:
	~HttpDecoder()
	{	Close();
	}
	HttpDecoder()
	{	req = 0;
	}
	void Close()
	{	if(!req)
		{	return;
		}
		req->free(req);
		req=0;
	}
	void Open()
	{	req = qcgireq_parse(NULL,Q_CGI_ALL);
	}
	const char* GetValue(const char* key) const
	{	const char* value=(const char *)req->getstr(req,key,false);
		return value ? value:"";
	}
	const char* operator[](const char* key) const
	{	return GetValue(key);
	}
	void SetContentType(const char* contentType)
	{	qcgires_setcontenttype(req, contentType);
	}
};

#endif
