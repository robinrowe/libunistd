// libportable/TimerPump.cpp
// Created by Robin Rowe on 4/19/2015.
// License MIT Open Source
//

#include <chrono>
#include "TimerPump.h"

namespace portable 
{


		if(!isWake)
		{	continue;
		}
		isWake=false;
		// qDebug()<<"Woke up";
        if(isGo)
        {   Action(isTimeout);
    }   }
//    qDebug()<<"Stopping pump";
}

}