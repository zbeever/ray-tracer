#pragma once

#include "../geometry/Ray.h"

class Camera
{
public:
	int pixel_width;
	int pixel_height;

	float canvas_width;
	float canvas_height;

	float vertical_fov;
	float aperture = 1.0f;

	Camera(int pixel_width_v, int pixel_height_v, float vertical_fov_v, float aperture_v);

	Ray getRay(int x, int y);
};
