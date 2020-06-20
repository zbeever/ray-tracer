#pragma once

#include "Texture.h"
#include <memory>

class Checker: public Texture
{
public:
	std::shared_ptr<Texture> odd;
	std::shared_ptr<Texture> even;

	Checker();
	Checker(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1);

	virtual Color value(double u, double v, const Point3& p) const;
};
