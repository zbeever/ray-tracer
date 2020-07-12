#pragma once

#include "PDF.h"

class MixturePDF: public PDF
{
public:
	std::shared_ptr<PDF> p[2];

	MixturePDF(std::shared_ptr<PDF> p0, std::shared_ptr<PDF> p1)
	{
		p[0] = p0;
		p[1] = p1;
	}

	virtual double value(const Vec3& direction, std::mt19937& rgen) const
	{
		return 0.5 * p[0]->value(direction, rgen) + 0.5 * p[1]->value(direction, rgen);
	}

	virtual Vec3 generate(std::mt19937& rgen) const
	{
		if (random_double(rgen) < 0.5)
			return p[0]->generate(rgen);
		else
			return p[1]->generate(rgen);
	}
};
