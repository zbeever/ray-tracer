#pragma once

#include "../engine/Record.h"

class Surface
{
public:
	virtual bool hit(const Ray& r, const double t_min, const double t_max, Record& rec) const = 0;
};
