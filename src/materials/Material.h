#include "../light/Radiance3.h"

#pragma once

#pragma once

class Material
{
public:
	Radiance3 rad;

	Material(const Radiance3& rad_v): rad(rad_v)
	{ }
};
