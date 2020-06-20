#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <ratio>
#include <omp.h>

#include "engine/Utils.h"
#include "engine/Camera.h"
#include "engine/Record.h"
#include "engine/Scene.h"

#include "geometry/MovingSphere.h"
#include "geometry/Ray.h"
#include "geometry/Sphere.h"
#include "geometry/Surface.h"
#include "geometry/Vec3.h"
#include "geometry/AARect.h"
#include "geometry/FlipNormals.h"
#include "geometry/Box.h"
#include "geometry/Translate.h"
#include "geometry/Rotate.h"

#include "materials/Dielectric.h"
#include "materials/Material.h"
#include "materials/Color.h"
#include "materials/Metal.h"
#include "materials/Lambertian.h"
#include "materials/DiffuseLight.h"

#include "textures/Solid.h"
#include "textures/Checker.h"
#include "textures/Texture.h"
#include "textures/Perlin.h"
#include "textures/Noise.h"
#include "textures/Image.h"

Color ray_color(const Ray& r, const Color& background, const Scene& world, const int depth, std::mt19937& rgen)
{
	Record rec;
	if (world.hit(r, eps, infinity, rec)) {
		Ray scattered;
		Color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered, rgen)) {
			double r = 0.75;
			if (depth > 5) {
				if (random_double(rgen) < 1 - r) {
					return Color(0, 0, 0);
				} else {
					return rec.mat_ptr->emitted(rec.u, rec.v, rec.p) + attenuation * ray_color(scattered, background, world, depth + 1, rgen) / r;
				}
			} else {
				return rec.mat_ptr->emitted(rec.u, rec.v, rec.p) + attenuation * ray_color(scattered, background, world, depth + 1, rgen);
			}
		} else {
			return rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		}
	}

	return background;
}

Scene cornell_box()
{
	Scene world;

	auto red   = std::make_shared<Lambertian>(  std::make_shared<Solid>(0.65, 0.05, 0.05));
	auto white = std::make_shared<Lambertian>(  std::make_shared<Solid>(0.73, 0.73, 0.73));
	auto green = std::make_shared<Lambertian>(  std::make_shared<Solid>(0.12, 0.45, 0.15));
	auto light = std::make_shared<DiffuseLight>(std::make_shared<Solid>(15.0, 15.0, 15.0));

	world.add(std::make_shared<FlipNormals>(std::make_shared<AARect>('x', 0, 555, 0, 555, 555, green)));
	world.add(std::make_shared<AARect>('x', 0, 555, 0, 555, 0, red));
	world.add(std::make_shared<AARect>('y', 213, 343, 227, 332, 554, light));
	world.add(std::make_shared<FlipNormals>(std::make_shared<AARect>('y', 0, 555, 0, 555, 555, white)));
	world.add(std::make_shared<AARect>('y', 0, 555, 0, 555, 0, white));
	world.add(std::make_shared<FlipNormals>(std::make_shared<AARect>('z', 0, 555, 0, 555, 555, white)));

	std::shared_ptr<Surface> box1 = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
	// box1 = std::make_shared<Rotate>(box1, 'y', 15);
	box1 = std::make_shared<Translate>(box1, Vec3(265, 0, 295));
	world.add(box1);

	std::shared_ptr<Surface> box2 = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
	// box2 = std::make_shared<Rotate>(box2, 'y', -18);
	box2 = std::make_shared<Translate>(box2, Vec3(130, 0, 65));
	world.add(box2);

	return world;
}

int main()
{
	const Color background(0.0, 0.0, 0.0);

	const auto aspect_ratio = 16.0 / 9.0; 									// Width / height
	const int image_width = 800;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int spp = 75;

	Scene world = cornell_box();
	/*
	world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5, std::make_shared<Lambertian>(std::make_shared<Solid>(0.1, 0.6, 0.1))));
	world.add(std::make_shared<Sphere>(Point3(1, 0, -1), 0.5, std::make_shared<Metal>(std::make_shared<Solid>(0.8, 0.8, 0.8), 0.0)));
	world.add(std::make_shared<Sphere>(Point3(-1, 0, -1), 0.5, std::make_shared<Dielectric>(std::make_shared<Solid>(1.0, 1.0, 1.0), 1.7)));
	world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100, std::make_shared<Lambertian>(std::make_shared<Noise>(10))));
	*/

	Point3 lookfrom(277, 277, -300);
	Point3 lookat(277, 277, 555);
	Vec3 vup(0, 1, 0);
	auto dist_to_focus = 8;
	auto aperture = 0.1;
	Camera cam(lookfrom, lookat, vup, 70, aspect_ratio, aperture, dist_to_focus);

	// Open a file, make sure it's loaded, and create the header
	std::ofstream file;
	file.open("image.ppm");
	if (!file.is_open()) exit(EXIT_FAILURE);
	file << "P3\n" << image_width << " " << image_height << "\n255\n";

	std::vector<Point3> pixel_array(image_width * image_height); 

	std::chrono::time_point<std::chrono::system_clock> start, end; 
	start = std::chrono::system_clock::now(); 

#pragma omp parallel 
{
#pragma omp for schedule(dynamic)
	for (int j = image_height - 1; j >= 0; --j) {
		static thread_local std::mt19937 rgen((omp_get_thread_num() + 1));
		fprintf(stderr, "\rRendering %5.2f%%", 100.*(image_height - 1 - j) / (image_height - 1)); 
		for (int i = 0; i < image_width; ++i) {
			Color pixel_color(0, 0, 0);
			for (int s = 0; s < spp; ++s) {
				auto u = double(i + random_double(rgen)) / (image_width - 1);
				auto v = double(j + random_double(rgen)) / (image_height - 1);
				Ray r = cam.get_ray(u, v, rgen);
				pixel_color += ray_color(r, background, world, 0, rgen);
			}
			pixel_array[i + (image_height - j - 1) * image_width] = toRGB(pixel_color, spp);
		}
	}
}
	end = std::chrono::system_clock::now(); 
	std::chrono::duration<double> elapsed_seconds = end - start; 

	for (auto color : pixel_array) {
		file << color.r() << " " << color.g() << " " << color.b() << std::endl;
	}

	std::cerr << "\nDone in " << elapsed_seconds.count() << "s\n";
	file.close();
}
