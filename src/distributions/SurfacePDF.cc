#include "SurfacePDF.h"

SurfacePDF::SurfacePDF(std::shared_ptr<Surface> p, const Point3& origin): ptr(p), o(origin)
{ }

double SurfacePDF::value(const Vec3& wi, const Vec3& wo, std::mt19937& rgen) const
{
	return ptr->pdf_value(o, wo, rgen);
}

Vec3 SurfacePDF::generate(std::mt19937& rgen, const Vec3& wo) const
{
	return ptr->random(o, rgen);
}
