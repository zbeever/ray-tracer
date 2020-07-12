#include "ONB.h"

ONB::ONB()
{ }

Vec3 ONB::u() const
{
	return axis[0];
}

Vec3 ONB::v() const
{
	return axis[1];
}

Vec3 ONB::w() const
{
	return axis[2];
}

Vec3 ONB::local(const double a, const double b, const double c) const
{
	return a * u() + b * v() + c * w();
}

Vec3 ONB::local(const Vec3& a) const
{
	return a.x() * u() + a.y() * v() + a.z() * w();
}

void ONB::build_from_w(const Vec3& n)
{
	axis[2] = normalize(n);
	Vec3 a = (std::abs(w().x()) > 0.9) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
	axis[1] = normalize(cross(w(), a));
	axis[0] = cross(w(), v());
}
