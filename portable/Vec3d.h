// portable/Vec3d.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef Vec3d_h
#define Vec3d_h

namespace portable 
{

struct Vec3d
{	double x;
	double y;
	double z;
	int size() const
	{	return sizeof(*this) == 3*sizeof(double) ? sizeof(*this) : 0;
	}
	void Copy(const Vec3d& v)
	{	x=v.x;
		y=v.y;
		z=v.z;
	}
	void Copy(const double* v)
	{	x=v[0];
		y=v[1];
		z=v[2];
	}
	Vec3d()
	:	x(0.)
	,	y(0.)
	,	z(0.)
	{}
	Vec3d(const Vec3d& v)
	{	Copy(v);
	}
	Vec3d& operator=(const Vec3d& v)
	{	Copy(v);
	}
	bool operator!=(const Vec3d& v) const
	{	if(x!=v.x || y!=v.y || z!=v.z)
		{	return false;
		}
		return true;
	}
};

}
#endif
