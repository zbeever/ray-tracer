#pragma once

#include <random>
#include "Vec3.h"

class PDF
{
public:
	virtual ~PDF()
	{ }

	virtual double value(const Vec3& direction, std::mt19937& rgen) const = 0;
	virtual Vec3 generate(std::mt19937& rgen) const = 0;
};
