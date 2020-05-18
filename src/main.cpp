#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <algorithm>

#include "geometry/Vec3.h"
#include "geometry/Ray.h"
#include "geometry/Sphere.h"
#include "geometry/Plane.h"
#include "geometry/Triangle.h"
#include "geometry/Mesh.h"

#include "engine/Camera.h"
#include "engine/Object.h"

#include "materials/Material.h"

#include "light/Radiance3.h"
#include "light/Color.h"

Color3 radToRGB(Radiance3& L, const float k, const float gamma)
{
	L *= 1.0f / k;

	float m = std::max(std::max(L.r, L.g), std::max(L.b, 1.0f));

	L *= 1.0f / m;

	m = std::clamp((m - 1.0f) * 0.2f, 0.0f, 1.0f);
	L = L * (1.0 - m) + Radiance3(m, m, m);

	int r = int(std::pow(L.r, 1.0f / gamma));
	int g = int(std::pow(L.g, 1.0f / gamma));
	int b = int(std::pow(L.b, 1.0f / gamma));

	return Color3(r, g, b);
}


void render(std::string file_name, Camera& cam, std::vector<std::shared_ptr<Object>> scene) // std::vector<std::shared_ptr<Surface>> scene)
{
	float s_min = 0.0f;
	float s_max = 1e6f;

	std::vector<Point3> lights;
	lights.push_back(Point3(-1, 3, 3));

	std::ofstream file;
	file.open(file_name);
	
	file << "P3\n" << cam.pixel_width << " " << cam.pixel_height << "\n255\n";

	for (int j = cam.pixel_height - 1; j >= 0; --j)
	{
		for (int i = 0; i < cam.pixel_width; ++i)
		{
			Ray beam = cam.getRay(i, j);
			HitRecord hit_record;
			hit_record.s = s_max;

			bool hit = false;

			for (auto surface : scene)
			{
				HitRecord temp_rec;

				if (!surface->hit(beam, temp_rec, s_min, s_max)) continue;
				if (hit == false) hit = true;
				if (temp_rec.s < hit_record.s) hit_record = temp_rec;
			}

			if (hit)
			{
				// Radiance3 rad = hit_record.mat->rad;

				Radiance3 rad(0.0, 0.0, 0.0);

				for (auto light : lights)
				{
					float dist = (light - hit_record.pos).length();
					Vec3 dir = (light - hit_record.pos) / dist;
					Ray to_light(dir, hit_record.pos + 1e-2 * dir);

					HitRecord temp;
					bool light_blocked = false;

					for (auto surface : scene)
					{
						if (surface->hit(to_light, temp, s_min, s_max))
						{
							light_blocked = true;
							break;
						}
					}
					
					if (light_blocked) continue;

					float phi = 2000.0f;
					float rho = 1.0;
					float beta = phi / (4 * M_PI * dist*dist);	

					rad += hit_record.mat->rad * beta * (rho / M_PI) * abs(dot(beam.dir, hit_record.normal));
				}

				file << int(255*rad.r) << " " << int(255*rad.g) << " " << int(255*rad.b) << std::endl;
			}
			else
			{
				file << 0 << " " << 0 << " " << 0 << "\n";
			}
		}
	}
	
	file.close();
}

int main()
{
	int width = 200;
	int height = 200;
	float v_fov = 60.0f;
	float aperture = 1.0f;

	Camera cam = Camera(width, height, v_fov, aperture);

	std::vector<std::shared_ptr<Surface>> scene;
	scene.push_back(std::make_shared<Sphere>(Point3(0, 0, -3), 1));
	scene.push_back(std::make_shared<Plane>(Point3(0, -2, 0), Vec3(0, 1, 0)));

	std::vector<std::shared_ptr<Object>> world;
	world.push_back(std::make_shared<Object>( std::make_shared<Sphere>(Point3(2, 0, -6), 2), std::make_shared<Material>(Radiance3(0.0, 0.5, 1.0)) ));
	world.push_back(std::make_shared<Object>( std::make_shared<Sphere>(Point3(0, 0, -2), 1), std::make_shared<Material>(Radiance3(1.0, 0.5, 1.0)) ));
	world.push_back(std::make_shared<Object>( std::make_shared<Plane>(Point3(0, -2, 0), Vec3(0, 1, 0)), std::make_shared<Material>(Radiance3(1.0, 1.0, 0.8)) ));

	// scene.push_back(std::make_shared<Triangle>(Point3(-1, 0, -1), Point3(1, 0, -1), Point3(0, 1, -1) ));

	auto teapot = std::make_shared<Mesh>("../models/teapot.obj");
	teapot->shift(Vec3(0, -1, -6));
	// scene.push_back(teapot);

	auto teddy = std::make_shared<Mesh>("../models/teddy.obj");
	teddy->shift(Vec3(0, 0, -55));
	// scene.push_back(teddy);

	render(std::string("render.ppm"), cam, world); // scene);

	return 0;
}
