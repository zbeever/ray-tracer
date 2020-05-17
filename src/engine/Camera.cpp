#include "Camera.h"

Camera::Camera(int pixel_width_v, int pixel_height_v, float vertical_fov_v, float aperture_v): pixel_width(pixel_width_v), pixel_height(pixel_height_v), vertical_fov(vertical_fov_v), aperture(aperture_v)
{
	canvas_height = 2.0f * std::tan(vertical_fov * 0.5 * M_PI / 180.0f) * abs(aperture);
	canvas_width = float(pixel_width) / float(pixel_height) * canvas_height;
}

Ray Camera::getRay(int x, int y)
{
	float canvas_x = canvas_width * float(x) / float(pixel_width) - canvas_width*0.5;
	float canvas_y = canvas_height * float(y) / float(pixel_height) - canvas_height*0.5;

	Point3 origin = Point3(0.0f, 0.0f, aperture);
	Vec3 dir = Vec3(canvas_x, canvas_y, -aperture);

	return Ray(dir, origin);
}
