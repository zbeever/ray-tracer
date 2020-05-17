#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include "geometry/Vec3.h"
#include "geometry/Ray.h"
#include "geometry/Sphere.h"
#include "geometry/Plane.h"
#include "geometry/Triangle.h"
#include "geometry/Mesh.h"

#include "engine/Camera.h"

void render(std::string file_name, Camera& cam, std::vector<std::shared_ptr<Surface>> scene)
{
	std::ofstream file;
	file.open(file_name);
	
	file << "P3\n" << cam.pixel_width << " " << cam.pixel_height << "\n255\n";

	for (int j = cam.pixel_height - 1; j >= 0; --j)
	{
		for (int i = 0; i < cam.pixel_width; ++i)
		{
			Ray beam = cam.getRay(i, j);
			HitRecord hit_record;

			bool hit_det = false;

			for (auto surf : scene)
			{
				if (surf->hit(beam, hit_record, 0.0f, 1e6f))
				{
					file << 255 << " " << 255 << " " << 255 << "\n";
				}
				else
				{
					file << 0 << " " << 0 << " " << 0 << "\n";
				}
			}
		}
	}
	
	file.close();
}

int main()
{
	int width = 100;
	int height = 100;
	float v_fov = 60.0f;
	float aperture = 1.0f;

	Camera cam = Camera(width, height, v_fov, aperture);

	std::vector<std::shared_ptr<Surface>> scene;
	// scene.push_back(std::make_shared<Sphere>(Point3(0, 0, -3), 1));
	// scene.push_back(std::make_shared<Plane>(Point3(0, -2, 0), Vec3(0, 1, 0)));
	// scene.push_back(std::make_shared<Triangle>(Point3(-1, 0, -1), Point3(1, 0, -1), Point3(0, 1, -1) ));

	auto teapot = std::make_shared<Mesh>("../models/teapot.obj");
	teapot->shift(Vec3(0, -1, -6));
	// scene.push_back(teapot);

	auto teddy = std::make_shared<Mesh>("../models/teddy.obj");
	teddy->shift(Vec3(0, 0, -55));
	scene.push_back(teddy);

	render(std::string("render.ppm"), cam, scene);

	return 0;
}
