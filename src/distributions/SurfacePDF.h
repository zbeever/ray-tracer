#pragma once

#include <memory>
#include "../engine/PDF.h"
#include "../engine/Surface.h"

class SurfacePDF: public PDF
{
public:
	Point3 o;
	std::shared_ptr<Surface> ptr;

	SurfacePDF(std::shared_ptr<Surface> p, const Point3& origin);

	virtual double value(const Ray& incident, const Ray& exitant, std::mt19937& rgen) const;
	virtual Vec3 generate(std::mt19937& rgen, const Vec3& wo, const Vec3& n) const;
};
