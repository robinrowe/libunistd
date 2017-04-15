// mq_receive.cpp
// Copyright 2016 Robin.Rowe@Cinepaint.org
// License open source MIT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <iostream>
using namespace std;

const char* QUEUE_NAME="/test_queue";
const unsigned MAX_SIZE=1024;
const char* MSG_STOP="quit";

int main()
{	char buffer[MAX_SIZE + 1];
    bool isGo=true;
	mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;
	cout << "opening message queue "<< QUEUE_NAME << endl;
    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
	if(mq<0)
	{	perror("mq_open");
		return 1;
	}
    while(isGo)
	{	ssize_t bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
		if(bytes_read < 0)
		{	perror("mq_read");
			return 2;
		}
		buffer[bytes_read-1] = 0;
		cout << "received: " << buffer << endl;
		if (!strcmp(buffer, MSG_STOP))
		{	isGo=false;
	}	}
	mq_close(mq);
	mq_unlink(QUEUE_NAME);
	return 0;
}