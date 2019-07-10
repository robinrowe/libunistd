// mq_send.cpp
// Copyright 2016 Robin.Rowe@Cinepaint.org
// License open source MIT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <iostream>
using namespace std;

const char* QUEUE_NAME="/test_queue";
const unsigned MAX_SIZE=1024;
const char* MSG_STOP="quit";

int main()
{	char buffer[MAX_SIZE];
    mqd_t mq = mq_open(QUEUE_NAME, O_WRONLY, 0644);
	if(mq<0)
	{	perror("mq_open");
		return 1;
	}
	buffer[0]=0;
	cout << "Send to server (enter \"quit\" to stop it):\n";
	while(strncmp(buffer, MSG_STOP, strlen(MSG_STOP)))
	{	cout<< "> ";
		cin.getline(buffer,MAX_SIZE);
		int ok = mq_send(mq, buffer, strlen(buffer)+1, 0);
		if(ok<0)
		{	perror("mq_send");
			return 2;
	}	}
	mq_close(mq);
	return 0;
}