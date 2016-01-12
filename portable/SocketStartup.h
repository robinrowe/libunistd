/* SocketStartup.h
// Copyright Nov 10, 2002, Robin.Rowe@MovieEditor.com
// License MIT (http://opensource.org/licenses/mit-license.php)
*/

#ifndef SocketStartup_h
#define SocketStartup_h

/* Usage:

int main()
{	SocketStartup socketStartup;
	.
	.
*/

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <windows.h>

class SocketStartup
{
public:
	SocketStartup()
	{	WORD version_requested=MAKEWORD(2,0);
		WSADATA data;
		WSAStartup(version_requested,&data);
	}
	~SocketStartup()
    {	WSACleanup();
	}
};
#else
class SocketStartup
{};
#endif

#endif
