#include "Camera.h"
#include <random>
#include "../geometry/Ray.h"

Camera::Camera(const Point3& lookfrom, const Point3& lookat, const Vec3& vup, const double vfov, const double aspect_ratio, const double aperture, const double focus_dist)
{
	auto theta = degrees_to_radians(vfov);
	auto h = tan(theta * 0.5);
	auto viewport_height = 2.0 * h;
	auto viewport_width = aspect_ratio * viewport_height;

	w = normalize(lookfrom - lookat);
	u = normalize(cross(vup, w));
	v = cross(w, u);

	origin = lookfrom;
	horizontal = focus_dist * viewport_width * u;
	vertical = focus_dist * viewport_height * v;
	lower_left_corner = origin - 0.5 * horizontal - 0.5 * vertical - focus_dist * w;

	lens_radius = aperture * 0.5;
}

// u and v go from 0 to 1
Ray Camera::get_ray(const double s, const double t, std::mt19937& rgen) const
{
	Vec3 rd = lens_radius * random_in_unit_disc(rgen);
	Vec3 offset = u * rd.x() + v * rd.y();

	return Ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
}

