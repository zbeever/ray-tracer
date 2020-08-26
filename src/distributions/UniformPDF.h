#pragma once

#include "../engine/PDF.h"
#include "../engine/Utils.h"
#include "../engine/ONB.h"

class UniformPDF: public PDF
{
public:
	ONB uvw;

	UniformPDF(const Vec3& w)
	{
		uvw.build_from_w(w);
	}

	virtual double value(const Vec3& wi, const Vec3& wo, std::mt19937& rgen) const
	{
		return 1 / (2 * PI);
	}

	virtual Vec3 generate(std::mt19937& rgen, const Vec3& wo, const Vec3& n) const
	{
		// std::cout << dot(uvw.w(), n) << std::endl;
		Vec3 possible = uvw.local(random_in_unit_sphere(rgen));
		if (dot(possible, uvw.w()) < 0.0) {
			possible *= -1.0;
		}
		return possible;
	}
};
