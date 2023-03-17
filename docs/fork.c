#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

enum {ERROR = -1,CHILD = 0};

pid_t pid = fork();
if(pid==ERROR)
{	return FALSE;
}
if(pid!=CHILD)
{	return TRUE;
}
// execute child code:
char* args = {"1","2"};
execv("hello.exe",args)
return TRUE;