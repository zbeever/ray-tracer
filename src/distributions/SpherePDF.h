#pragma once

#include "../engine/PDF.h"
#include "../engine/Utils.h"
#include "../engine/ONB.h"

class SpherePDF: public PDF
{
public:
	ONB uvw;

	SpherePDF(const Vec3& w)
	{
		uvw.build_from_w(w);
	}

	virtual double value(const Ray& incident, const Ray& exitant, std::mt19937& rgen) const
	{
		return 1 / (4 * PI);
	}

	virtual Vec3 generate(std::mt19937& rgen, const Vec3& wo, const Vec3& n) const
	{
		Vec3 possible = uvw.local(random_in_unit_sphere(rgen));
		return possible;
	}
};
