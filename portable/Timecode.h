// Timecode.h
// 2016/06/17

#ifndef Timecode_h
#define Timecode_h

union Timecode // char[4] h:m:s:f, range 0-255 each
{	struct at
	{	unsigned char reel;
		unsigned char minutes;
		unsigned char seconds;
		unsigned char frame;
	};
	unsigned int intVal;
	Timecode()
	:	intVal(0)
	{}
};

#endif