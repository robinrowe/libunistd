// test/test_logger/main.cpp: 
// robin.rowe@cinepaint.org 2019/10/16

#include <iostream>
#include <portable/SystemLog.h>
using namespace std;
using namespace portable;

#define P(name) cout << #name << ":" << endl

int main()
{	P(SysLogMsg);
	SysLogMsg("msg","function");
	P(SysLogError);
	SysLogError("msg","function");
	P(LogMsg);
	LogMsg("msg");
	P(LogError);
	LogError("msg");
	P(SYSLOG);
	SYSLOG("msg");
	P(SYSERR);
	SYSERR("msg");
	P(SystemLog);
	SystemLog(__FILE__,__LINE__,"msg");
	P(StatusMsg);
	StatusMsg("msg");
	P(tty_msg);
	tty_msg("tag","msg");
	P(status_msg);
	status_msg("msg");
	P(warning_msg);
	warning_msg("msg");
	P(trace_msg);
	trace_msg("msg");
	P(error_msg);
	error_msg("msg");
	P(return_msg);
	enum { RETURN_NAME };
	return_msg(RETURN_NAME);
	return 0;
}

