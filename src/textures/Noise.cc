#include "Noise.h"

Noise::Noise(double scale_): scale(scale_)
{ }

Color Noise::value(double u, double v, const Point3& p) const
{
	return Color(1.0, 1.0, 1.0) * 0.5 * (1.0 + sin(scale * p.z() + 10 * noise.turb(p)));
}

std::shared_ptr<Noise> Noise::make(double scale_)
{
	return std::make_shared<Noise>(scale_);
}
