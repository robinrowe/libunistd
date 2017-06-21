// StackTrank.h
// 2017/6/16

#ifndef StackTrack_h
#define StackTrack_h

#include <stdio.h>
#if 0
template<typename T>
class StackTrack
{	int count;
	const char* name;
	const int size;
	const T* t;
public:
	StackTrack(const StackTrack&) = delete;
	StackTrack& operator=(const StackTrack&) = delete;
	StackTrack(int size,const char* name)
	:	name(name)
	,	size(size)
	,	t(nullptr)
	{	count++;
		Print();
	}
	~StackTrack()
	{	count--;
		Print();
	}
	void Print()
	{	printf("StackTrack(%s): %i (%u bytes)\n",name,count,count*size);
	}
};

#define STACK_TRACK(t) stackTrack(sizeof(t),#t)
#else
#define STACK_TRACK(t) {\
static int count; \
printf("StackTrack(%s): %i (%i bytes)\n", #t, ++count, int(count*sizeof(t)));\
}

#endif
#endif
