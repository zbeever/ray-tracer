#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Record.h"
#include "ScatterRecord.h"
#include "Texture.h"
#include "PDF.h"

#include "Spectrum.h"

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const;
	virtual double bxdf(const Ray& r_in, Ray& r_out, const Record& rec, const ScatterRecord& srec) const
	{
		return 0.0;
	}
	virtual double emitted(const Ray& r_in, const Record& rec, double u, double v, const Point3& p) const;
};
