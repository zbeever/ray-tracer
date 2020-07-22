#pragma once

#include "../geometry/Vec3.h"
#include "../engine/Record.h"
#include "../textures/Texture.h"
#include "../geometry/Ray.h"
#include "../engine/PDF.h"

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const;
	virtual Color emitted(const Ray& r_in, const Record& rec, double u, double v, const Point3& p) const;
};
