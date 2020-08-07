#pragma once

#include "../engine/PDF.h"
#include "../engine/Utils.h"
#include "../engine/ONB.h"

class CosinePDF: public PDF
{
public:
	ONB uvw;

	CosinePDF(const Vec3& w);

	virtual double value(const Vec3& direction, std::mt19937& rgen) const;
	virtual Vec3 generate(std::mt19937& rgen) const;
};