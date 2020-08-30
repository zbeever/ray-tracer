#pragma once

#include <random>
#include "Ray.h"
#include "ONB.h"

class PDF
{
public:
	ONB uvw;

	virtual ~PDF()
	{ }

	virtual double value(const Ray& incident, const Ray& exitant, std::mt19937& rgen) const = 0;
	virtual Vec3 generate(std::mt19937& rgen, const Vec3& wo, const Vec3& n) const = 0;
};
