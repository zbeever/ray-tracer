#pragma once

#include "Vec3.h"

class Ray
{
private:
	Point3 orig;
	Vec3 dir;
	int wavelength_bin;
	double tm;

public:
	Ray();
	Ray(const Point3& orig_, const Vec3& dir_, const double time_=0.0, const int wavelength_bin_=0);

	Point3 origin() const;
	Vec3 direction() const;
	double time() const;
	int bin() const;

	Point3 at(const double t) const;
};
