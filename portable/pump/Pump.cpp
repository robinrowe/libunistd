// libportable/Pump.cpp
// Created by Robin Rowe on 4/19/2015.
// License MIT Open Source
//

#include <chrono>
#include "Pump.h"

namespace portable 
{

void Pump::Run()
{   if(!Init())
	{	return;
	}
    while(isGo)
    {   if(Receive())
		{	Action();
    }   }
}

}