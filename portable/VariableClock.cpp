// VariableClock.cpp
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#include "VariableClock.h"

namespace portable
{

double VariableClock::speed;
timeval VariableClock::dayStart;
timespec VariableClock::clockStart;

}
