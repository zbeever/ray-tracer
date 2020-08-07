#include "SolidColor.h"

SolidColor::SolidColor()
{ }

SolidColor::SolidColor(Color c): color_value(c)
{ }

SolidColor::SolidColor(double red, double green, double blue): SolidColor(Color(red, green, blue))
{ }

Color SolidColor::value(double u, double v, const Point3& p) const
{
	return color_value;
}

std::shared_ptr<SolidColor> SolidColor::make(Color c)
{
	return std::make_shared<SolidColor>(c);
}

std::shared_ptr<SolidColor> SolidColor::make(double red, double green, double blue)
{
	return std::make_shared<SolidColor>(red, green, blue);
}
