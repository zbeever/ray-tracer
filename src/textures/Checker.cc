#include "Checker.h"

Checker::Checker()
{ }

Checker::Checker(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1): even(t0), odd(t1)
{ }

Color Checker::value(double u, double v, const Point3& p) const
{
	auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
	if (sines < 0)
	{
		return odd->value(u, v, p);
	}
	else
	{
		return even->value(u, v, p);
	}
}
