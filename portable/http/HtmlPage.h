// libportable/HtmlPage.h
// Created by Robin Rowe on 2015/6/9
// License MIT Open Source
//

#ifndef HtmlPage_h
#define HtmlPage_h

#include <iostream>
using namespace std;

class HtmlHeader
{public:
	HtmlHeader()
	{	cout<<"<header>/n";
	}
	~HtmlHeader()
	{	cout<<"</header>\n";
	}
};

class HtmlTitle
{public:
	HtmlTitle()
	{	cout<<"<title>\n";
	}
	~HtmlTitle()
	{	cout<<"</title>\n";
	}
};

class HtmlPage
{public:
	HtmlPage(const char* contentType=0)
	{	if(contentType)
		{	cout<<"Content-Type: "<<contentType<<"\n"
			"\n";
		}
		cout<<"<html>\n";
	}
	~HtmlPage()
	{	cout<<"<html>\n";
	}
};

class HtmlBody
{public:
	HtmlBody()
	{	cout<<"<body>\n";
	}
	~HtmlBody()
	{	cout<<"</body>\n";
	}
};

class HtmlP
{public:
	HtmlP()
	{	cout<<"\n<p>";
	}
	~HtmlP()
	{	cout<<"</p>\n";
	}
};

class HtmlTable
{public:
	HtmlTable(unsigned border=0)
	{	cout<<"<table border=\""<<border<<"\">\n";
	}
	~HtmlTable()
	{	cout<<"</table>\n";
	}
};

class HtmlTr
{public:
	HtmlTr()
	{	cout<<"<tr>\n";
	}
	~HtmlTr()
	{	cout<<"</tr>\n";
	}
};

class HtmlTd
{public:
	HtmlTd()
	{	cout<<"<td>";
	}
	~HtmlTd()
	{	cout<<"</td>";
	}
};

class HtmlList
{public:
	HtmlList()
	{	cout<<"<ol>\n";
	}
	~HtmlList()
	{	cout<<"</ol>\n";
	}
};

class HtmlListI
{public:
	HtmlListI()
	{	cout<<"<li>";
	}
};

class HtmlHeading
{	unsigned level;
public:
	HtmlHeading(unsigned level)
	:	level(level)
	{	cout<<"<h"<<level<<">";
	}
	~HtmlHeading()
	{	cout<<"</h"<<level<<">";
	}
};

class HtmlFormat
{	const char* format;
public:
	HtmlFormat(const char* format)
	:	format(format)
	{	cout<<"<"<<format<<">";
	}
	~HtmlFormat()
	{	cout<<"</"<<format<<">";
	}
};

class HtmlHref
{	
public:
	HtmlHref(const char* description,const char* link)
	{	cout<<"<a href=\""<<link<<"\">"<<description<<"</a>";
	}
};

#endif
