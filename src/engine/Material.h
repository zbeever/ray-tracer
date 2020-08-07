#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Record.h"
#include "ScatterRecord.h"
#include "Texture.h"
#include "PDF.h"

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const;
	virtual Color emitted(const Ray& r_in, const Record& rec, double u, double v, const Point3& p) const;
};
