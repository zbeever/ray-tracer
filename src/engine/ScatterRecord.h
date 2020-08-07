#pragma once

#include <memory>

#include "Vec3.h"
#include "Ray.h"
#include "PDF.h"

struct ScatterRecord
{
	Ray specular_ray;
	bool is_specular;
	Color attenuation;
	std::shared_ptr<PDF> pdf_ptr;
};
