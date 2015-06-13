// fix_vcast.h
// Copyright 2005 Robin.Rowe@MovieEditor.com
// License: BSD
// Purpose: Correct warnings, make 64-bit safe pointer conversions
// Note this is the C++ version of fix_vast.h, not C++.

#ifndef FIX_VCAST_H
#define FIX_VCAST_H

#ifdef __cplusplus

inline
int ptr2int(void* p)
{	union P_I
	{	void* p;
		int i;
	};
	P_I p_i;
	p_i.p=p;
	return p_i.i;
}
inline
void* int2ptr(int i)
{	union P_I
	{	void* p;
		int i;
	};
	P_I p_i;
	p_i.p=0;
	p_i.i=i;
	return p_i.p;
}

#else

__inline
int ptr2int(void* p)
{	typedef union P_I
	{	void* p;
		int i;
	} P_I;
	P_I p_i;
	p_i.p=p;
	return p_i.i;
}
__inline
void* int2ptr(int i)
{	typedef union P_I
	{	void* p;
		int i;
	} P_I;
	P_I p_i;
	p_i.p=0;
	p_i.i=i;
	return p_i.p;
}

#endif

#endif