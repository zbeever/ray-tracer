#include "MixturePDF.h"

MixturePDF::MixturePDF(std::shared_ptr<PDF> p0, std::shared_ptr<PDF> p1)
{
	p[0] = p0;
	p[1] = p1;
}

double MixturePDF::value(const Ray& incident, const Ray& exitant, std::mt19937& rgen) const
{
	Vec3 wi = normalize(incident.direction());
	Vec3 wo = normalize(exitant.direction());

	return 0.5 * p[0]->value(incident, exitant, rgen) + 0.5 * p[1]->value(incident, exitant, rgen);
}

Vec3 MixturePDF::generate(std::mt19937& rgen, const Vec3& wo, const Vec3& n) const
{
	if (random_double(rgen) < 0.5) {
		return p[0]->generate(rgen, wo, n);
	} else {
		return p[1]->generate(rgen, wo, n);
	}
}
