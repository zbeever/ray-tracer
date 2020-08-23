#include "Material.h"

bool Material::scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const
{
	return false;
}

// Color Material::emitted(const Ray& r_in, const Record& rec, double u, double v, const Point3& p) const
double Material::emitted(const Ray& r_in, const Record& rec, double u, double v, const Point3& p) const
{
	return 0.0;
}
