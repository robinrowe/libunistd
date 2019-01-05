// no_copy.h
// Copyright (c) 2019/1/5 Robin.Rowe@CinePaint.org
// License MIT open source

#ifndef no_copy
#define no_copy(T) T(const T&) = delete;void operator=(const T&) = delete
#endif