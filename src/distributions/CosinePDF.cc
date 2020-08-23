#include "CosinePDF.h"

CosinePDF::CosinePDF(const Vec3& w)
{
	uvw.build_from_w(w);
}

double CosinePDF::value(const Vec3& wi, const Vec3& wo, std::mt19937& rgen) const
{
	double cosine = dot(normalize(wo), uvw.w());
	return (cosine <= 0) ? 0 : cosine / M_PI;
}

Vec3 CosinePDF::generate(std::mt19937& rgen, const Vec3& wo) const
{
	return uvw.local(random_cos_weighted_hemisphere(rgen));
}
