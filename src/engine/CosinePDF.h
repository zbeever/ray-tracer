#pragma once

#include "PDF.h"
#include "Utils.h"
#include "../geometry/ONB.h"

class CosinePDF: public PDF
{
public:
	ONB uvw;

	CosinePDF(const Vec3& w);

	virtual double value(const Vec3& direction, std::mt19937& rgen) const;
	virtual Vec3 generate(std::mt19937& rgen) const;
};
