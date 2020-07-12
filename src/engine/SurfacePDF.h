#pragma once

#include <memory>

#include "PDF.h"
#include "../geometry/Surface.h"

class SurfacePDF: public PDF
{
public:
	Point3 o;
	std::shared_ptr<Surface> ptr;

	SurfacePDF(std::shared_ptr<Surface> p, const Point3& origin): ptr(p), o(origin)
	{ }

	virtual double value(const Vec3& direction, std::mt19937& rgen) const
	{
		return ptr->pdf_value(o, direction, rgen);
	}

	virtual Vec3 generate(std::mt19937& rgen) const
	{
		return ptr->random(o, rgen);
	}
};
