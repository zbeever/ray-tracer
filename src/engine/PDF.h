#pragma once

#include <random>
#include "Vec3.h"
#include "ONB.h"

class PDF
{
public:
	ONB uvw;

	virtual ~PDF()
	{ }

	virtual double value(const Vec3& wi, const Vec3& wo, std::mt19937& rgen) const = 0;
	virtual Vec3 generate(std::mt19937& rgen, const Vec3& wo, const Vec3& n) const = 0;
};
