#include "Camera.h"
#include <random>
#include "../geometry/Ray.h"

Camera::Camera(const Point3& lookfrom, const Point3& lookat, const Vec3& vup, const double vfov, const double aspect_ratio, const double aperture, const double focus_dist, const double t0, const double t1)
{
	auto theta = degrees_to_radians(vfov);
	auto h = tan(theta * 0.5);
	auto viewport_height = 2.0 * h;
	auto viewport_width = aspect_ratio * viewport_height;

	// Setup a local coordinate system. w points to the scene, u is the new 'x axis' and v is the new 'y axis'
	w = normalize(lookfrom - lookat);
	u = normalize(cross(vup, w));
	v = cross(w, u);

	// The scaling value of focus_dist sets the horizontal and vertical vectors to cover the entire viewing plane
	origin = lookfrom;
	horizontal = focus_dist * viewport_width * u;
	vertical = focus_dist * viewport_height * v;
	lower_left_corner = origin - 0.5 * horizontal - 0.5 * vertical - focus_dist * w;

	lens_radius = aperture * 0.5;
	time0 = t0;
	time1 = t1;
}

Ray Camera::get_ray(const double s, const double t, std::mt19937& rgen) const
{
	Vec3 rd = lens_radius * random_in_unit_disc(rgen);
	Vec3 offset = u * rd.x() + v * rd.y();

	// Adding the offset to the ray origin and subtracting it from the direction vector ensures that rays sent to the same
	// (s, t) coordinates always coincide on the viewing plane
	return Ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset, random_double(time0, time1, rgen));
}

