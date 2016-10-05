// libportable/TimerPump.cpp
// Created by Robin Rowe on 4/19/2015.
// License MIT Open Source
//

#include "TimerPump.h"

namespace portable
{
void TimerPump::Run()
{//   qDebug()<<"Starting pump "<<object;
    Init();
    while(isGo)
    {   Lock lock(pumpMutex);
        bool isTimeout=false;
        if(wakeDelay.count())
        {   isTimeout=std::cv_status::timeout==pumpCon.wait_for(lock,milliseconds(wakeDelay));
        }
        else
        {   pumpCon.wait(lock);
        }
        // qDebug()<<"Woke up";
        if(isGo)
        {   Action(isTimeout);
    }   }
//    qDebug()<<"Stopping pump";
}

}