#pragma once

#include "../geometry/Vec3.h"
#include "../engine/Record.h"
#include "../geometry/Ray.h"

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const = 0;
};
