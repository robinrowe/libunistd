// gettimeofday.h: Windows emulation of common time functions
// Libunistd Copyright Nov 10, 2002, Robin.Rowe@CinePaint.org
// License MIT (http://opensource.org/licenses/mit-license.php)

#ifndef gettimeofday_h
#define gettimeofday_h

#include "../portable/stub.h"
#include "cfunc.h"

CFUNC int gettimeofday(struct timeval* tv, struct timezone* tz);
CFUNC int settimeofday(const struct timeval *tv, const struct timezone *tz);

#endif