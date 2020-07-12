#pragma once

#include "PDF.h"
#include "Utils.h"
#include "../geometry/ONB.h"

class CosinePDF: public PDF
{
public:
	ONB uvw;

	CosinePDF(const Vec3& w)
	{
		uvw.build_from_w(w);
	}

	virtual double value(const Vec3& direction, std::mt19937& rgen) const
	{
		auto cosine = dot(normalize(direction), uvw.w());
		return (cosine <= 0) ? 0 : cosine / pi;
	}

	virtual Vec3 generate(std::mt19937& rgen) const
	{
		return uvw.local(random_cos_weighted_hemisphere(rgen));
	}
};
