#include "CosinePDF.h"

CosinePDF::CosinePDF(const Vec3& w)
{
	uvw.build_from_w(w);
}

double CosinePDF::value(const Vec3& direction, std::mt19937& rgen) const
{
	double cosine = dot(normalize(direction), uvw.w());
	return (cosine <= 0) ? 0 : cosine / pi;
}

Vec3 CosinePDF::generate(std::mt19937& rgen) const
{
	return uvw.local(random_cos_weighted_hemisphere(rgen));
}
