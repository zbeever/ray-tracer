#pragma once

#include <random>

#include "../geometry/Vec3.h"
#include "../geometry/Ray.h"
#include "Utils.h"

class Camera
{
private:
	double lens_radius;
	double time0, time1;

	Point3 origin;
	Point3 lower_left_corner;

	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;

public:
	Camera(const Point3& lookfrom, const Point3& lookat, const Vec3& vup, const double vfov, const double aspect_ratio, const double aperture, const double focus_dist, const double t0 = 0.0, const double t1 = 0.0);

	Ray get_ray(const double s, const double t, std::mt19937& rgen) const;
};
