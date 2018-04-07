// libportable/Pump.cpp
// Created by Robin Rowe on 4/19/2015.
// License MIT Open Source
//

#include <chrono>
#include "Pump.h"

namespace portable 
{

void Pump::Run()
{   Init();
    while(isGo)
    {   Lock lock(mut);
		Wait(lock);
		if(!isWake)
		{	// spurious thread wake
			continue;
		}
		isWake=false;
        if(isGo)
        {   Action();
    }   }
}

}