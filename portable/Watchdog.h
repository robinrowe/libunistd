// Watchdog.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef Watchdog_h
#define Watchdog_h

#include <thread>
#include <mutex>
#include <condition_variable>

namespace portable
{

class Watchdog
{   typedef std::unique_lock<std::mutex> Lock;
    typedef std::chrono::milliseconds milliseconds;
    std::mutex m;
    std::condition_variable cv;
    milliseconds delay;
    bool isGo;
    void Run()
	{	while(isGo && delay.count())
		{   Lock lock(m);
			const bool isTimeout=std::cv_status::timeout==cv.wait_for(lock,delay);
			if(isGo)
			{   Action(isTimeout);
	}	}   }
    static void Main(Watchdog* self)
    {   self->Run();
    }
public:
    Watchdog()
	:	isGo(false)
	,	delay(0)
	{}
	void Start(unsigned dalay)
    {	isGo =true;
		this->delay = delay;
		std::thread worker(Main,this);
		worker.detach();
    }
    virtual ~Watchdog()
    {   isGo=false;
    }
	void Wake()
	{	cv.notify_one();
	}
    void Stop()
    {   isGo=false;
		Wake();
    }
	virtual void Action(bool isTimeout)=0;
};

}

#endif
