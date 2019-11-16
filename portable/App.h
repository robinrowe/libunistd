// App.h 
// Created by Robin Rowe 2019-07-10
// License Copyright 2019 Robin.Rowe@HeroicRobots.com ***Proprietary***

#ifndef App_h
#define App_h

#include <iostream>
#include <signal.h>
#include "NamerPump.h"
#include "ReminderPump.h"

class App
{	App(const App&) = delete;
	void operator=(const App&) = delete;
	NamerPump& namerPump;
	static App* app;
	static void Stop(int signal)
	{	(void) signal;
		if(!app)
		{	return;
		}
		app->namerPump.Stop();
	}
public:
	std::ostream& Print(std::ostream& os) const
	{	return os << "App";
	} 
	App()
	:	namerPump(namerPump)
	{	if(0!=app)
		{	puts("ERROR: App twice");
			return;
		}
		app = this;
		signal(SIGINT,Stop);
		signal(SIGTERM,Stop);
	}
	~App()
	{}
	bool operator!() const
	{	return true;
	}
};

inline
std::ostream& operator<<(std::ostream& os,const App& app)
{	return app.Print(os);
}

#endif
