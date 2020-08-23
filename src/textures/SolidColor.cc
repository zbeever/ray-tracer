#include "SolidColor.h"

SolidColor::SolidColor()
{ }

SolidColor::SolidColor(const Spectrum& spectrum): color_value(spectrum)
{
}

double SolidColor::value(int bin, double u, double v, const Point3& p) const
{
	return color_value.get(bin);
}

std::shared_ptr<SolidColor> SolidColor::make(Color c)
{
	return std::make_shared<SolidColor>();
}

std::shared_ptr<SolidColor> SolidColor::make(const Spectrum& spectrum)
{
	return std::make_shared<SolidColor>(spectrum);
}
