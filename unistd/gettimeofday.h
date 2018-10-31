// gettimeofday.h: Windows emulation of common time functions
// Libunistd Copyright Nov 10, 2002, Robin.Rowe@CinePaint.org
// License MIT (http://opensource.org/licenses/mit-license.php)

#ifndef gettimeofday_h
#define gettimeofday_h

#include "../portable/stub.h"

int gettimeofday(struct timeval* tv, struct timezone* tz);

inline
int settimeofday(const struct timeval *tv, const struct timezone *tz)
{	(void)tv;
	(void)tz;
	STUB_NEG(0);
}

#endif