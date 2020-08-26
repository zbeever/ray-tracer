#pragma once

#include <memory>
#include <random>
#include "../engine/PDF.h"

class MixturePDF: public PDF
{
public:
	std::shared_ptr<PDF> p[2];

	MixturePDF(std::shared_ptr<PDF> p0, std::shared_ptr<PDF> p1);
	virtual double value(const Vec3& wi, const Vec3& wo, std::mt19937& rgen) const;
	virtual Vec3 generate(std::mt19937& rgen, const Vec3& wo, const Vec3& n) const;
};
