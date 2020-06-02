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

#include "geometry/Vec3.h"
#include "geometry/Ray.h"
#include "geometry/Sphere.h"
#include "geometry/Surface.h"

#include "materials/Dielectric.h"
#include "materials/Material.h"
#include "materials/Color.h"
#include "materials/Metal.h"
#include "materials/Lambertian.h"

Color ray_color(const Ray& r, const Scene& world, const int depth, std::mt19937& rgen)
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
					return attenuation * ray_color(scattered, world, depth + 1, rgen) / r;
				}
			} else {
				return attenuation * ray_color(scattered, world, depth + 1, rgen);
			}
		} else {
			return Color(0, 0, 0);
		}
	}

	Vec3 unit_direction = normalize(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0); 				// With a normalized direction vector, this can take on values between 1 and 1
	return (1.0 - t) * Color(1., 1., 1.) + t * Color(0.5, 0.7, 1.0); 	// Linear interpolation
}

int main()
{
	const auto aspect_ratio = 16.0 / 9.0; 									// Width / height
	const int image_width = 1024;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int spp = 75;

	Scene world;
	world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5, std::make_shared<Lambertian>(Color(0.1, 0.6, 0.1))));
	world.add(std::make_shared<Sphere>(Point3(1, 0, -1), 0.5, std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.0)));
	world.add(std::make_shared<Sphere>(Point3(-1, 0, -1), 0.5, std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0), 1.7)));
	world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100, std::make_shared<Lambertian>(Color(0.3, 0.3, 0.3))));

	Point3 lookfrom(1, 2, 3);
	Point3 lookat(0, 0, -1);
	Vec3 vup(0, 1, 0);
	auto dist_to_focus = 4.6;
	auto aperture = 1.0;
	Camera cam(lookfrom, lookat, vup, 40, aspect_ratio, aperture, dist_to_focus);

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
				pixel_color += ray_color(r, world, 0, rgen);
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
