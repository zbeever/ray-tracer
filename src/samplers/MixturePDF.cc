#include "MixturePDF.h"

MixturePDF::MixturePDF(std::shared_ptr<PDF> p0, std::shared_ptr<PDF> p1)
{
	p[0] = p0;
	p[1] = p1;
}

double MixturePDF::value(const Vec3& direction, std::mt19937& rgen) const
{
	return 0.5 * p[0]->value(direction, rgen) + 0.5 * p[1]->value(direction, rgen);
}

Vec3 MixturePDF::generate(std::mt19937& rgen) const
{
	if (random_double(rgen) < 0.5) {
		return p[0]->generate(rgen);
	} else {
		return p[1]->generate(rgen);
	}
}