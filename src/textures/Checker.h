#pragma once

#include <memory>
#include "../engine/Texture.h"

class Checker: public Texture
{
public:
	std::shared_ptr<Texture> odd;
	std::shared_ptr<Texture> even;

	Checker();
	Checker(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1);

	virtual Color value(double u, double v, const Point3& p) const;
	static std::shared_ptr<Checker> make(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1);
};
