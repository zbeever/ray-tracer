#include "Solid.h"

Solid::Solid()
{ }

Solid::Solid(Color c): color_value(c)
{ }

Solid::Solid(double red, double green, double blue): Solid(Color(red, green, blue))
{ }

Color Solid::value(double u, double v, const Point3& p) const
{
	return color_value;
}
