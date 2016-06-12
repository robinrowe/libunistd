// portable/Breakpoint.h
// Created by Robin Rowe on 6/30/2015.
// Copyright (c) 2015 Robin.Rowe@CinePaint.org. All rights reserved.
//

#if defined(_DEBUG) && defined(_WIN32)
#include <Windows.h>

inline 
void Breakpoint()
{	DebugBreak();
}
#else
inline 
void Breakpoint()
{}

/* How to breakpoint a cgi-bin:

#include "<portable/Breakpoint.h>

int main()
{	Breakpoint();//Launches VC++ and stops on bp here
	.
	. // your stuff...
	.
	return 0;
} 

*/

#endif