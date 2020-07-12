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
#include "engine/BVHNode.h"

#include "engine/PDF.h"
#include "engine/CosinePDF.h"
#include "engine/SurfacePDF.h"
#include "engine/MixturePDF.h"

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
#include "geometry/ConstantMedium.h"
#include "geometry/ONB.h"

#include "materials/Dielectric.h"
#include "materials/Material.h"
#include "materials/Color.h"
#include "materials/Metal.h"
#include "materials/Lambertian.h"
#include "materials/DiffuseLight.h"
#include "materials/Isotropic.h"

#include "textures/Solid.h"
#include "textures/Checker.h"
#include "textures/Texture.h"
#include "textures/Perlin.h"
#include "textures/Noise.h"
#include "textures/Image.h"

Color ray_color(const Ray& r, const Color& background, const Scene& world, std::shared_ptr<Surface> lights, const int depth, std::mt19937& rgen)
{
	Record rec;

	double p = 0.75;
	if (depth > 5) {
		if (random_double(rgen) < 1 - p) {
			return Color(0, 0, 0);
		}
	}

	if (!world.hit(r, eps, infinity, rec, rgen)) {
		if (depth > 5) {
			return background / p;
		} else {
			return background;	
		}
	}

	Ray scattered;
	Color attenuation;
	Color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
	double pdf_val;
	Color albedo;

	if (!rec.mat_ptr->scatter(r, rec, albedo, scattered, pdf_val, rgen)) {
		if (depth > 5) {
			return emitted / p;
		} else {
			return emitted;
		}
	}

	std::shared_ptr<Surface> light_shape = std::make_shared<AARect>('y', 213, 343, 227, 332, 554, std::shared_ptr<Material>());
	auto p0 = std::make_shared<SurfacePDF>(light_shape, rec.p);
	auto p1 = std::make_shared<CosinePDF>(rec.normal);
	MixturePDF pdf(p0, p1);

	scattered = Ray(rec.p, pdf.generate(rgen), r.time());
	pdf_val = pdf.value(scattered.direction(), rgen);

	if (depth > 5) {
		return emitted + albedo * rec.mat_ptr->scattering_pdf(r, rec, scattered) * ray_color(scattered, background, world, lights, depth + 1, rgen) / (pdf_val * p);
	} else {
		return emitted + albedo * rec.mat_ptr->scattering_pdf(r, rec, scattered) * ray_color(scattered, background, world, lights, depth + 1, rgen) / pdf_val;
	}
}

Scene final_scene()
{
	std::mt19937 rgen;

	Scene objects;

	Scene boxes1;
	auto light = std::make_shared<DiffuseLight>(std::make_shared<Solid>(7, 7, 7));
	auto ground = std::make_shared<Lambertian>(std::make_shared<Solid>(0.48, 0.83, 0.53));

	const int boxes_per_side = 10;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = 0.0 + i*w;
			auto z0 = -500.0 + j*w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_double(1, 101, rgen);
			auto z1 = z0 + w;

			boxes1.add(std::make_shared<Box>(Point3(x0,y0,z0), Point3(x1,y1,z1), ground));
		}
	}


	objects.add(std::make_shared<BVHNode>(boxes1, 0, 1, rgen));
	objects.add(std::make_shared<AARect>('y', 123, 423, 147, 412, 554, light));

	auto center1 = Point3(400, 400, 200);
	auto center2 = center1 + Vec3(30,0,0);
	auto moving_sphere_material = std::make_shared<Lambertian>(std::make_shared<Solid>(0.7, 0.3, 0.1));

	objects.add(std::make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_material));

	// Glass sphere
	objects.add(std::make_shared<Sphere>(Point3(260, 150, 45), 50, std::make_shared<Dielectric>(std::make_shared<Solid>(1.0, 1.0, 1.0), 1.5)));

	// Metal sphere
	objects.add(std::make_shared<Sphere>(Point3(0, 150, 145), 50, std::make_shared<Metal>(std::make_shared<Solid>(0.8, 0.8, 0.9), 10.0)));

	// Foggy glass sphere
	auto boundary = std::make_shared<Sphere>(Point3(360,150,145), 70, std::make_shared<Dielectric>(std::make_shared<Solid>(1.0, 1.0, 1.0), 1.5));
	objects.add(boundary);
	objects.add(std::make_shared<ConstantMedium>(boundary, 0.2, std::make_shared<Solid>(0.2, 0.4, 0.9)));
	boundary = std::make_shared<Sphere>(Point3(0, 0, 0), 5000, std::make_shared<Dielectric>(std::make_shared<Solid>(1.0, 1.0, 1.0), 1.5));
	objects.add(std::make_shared<ConstantMedium>(boundary, .0001, std::make_shared<Solid>(1,1,1)));

	// Earth sphere
	auto emat = std::make_shared<Lambertian>(std::make_shared<Image>("../images/earthmap.jpg"));
	objects.add(std::make_shared<Sphere>(Point3(400,200,400), 100, emat));

	// Marble sphere
	auto pertext = std::make_shared<Noise>(0.1);
	objects.add(std::make_shared<Sphere>(Point3(220,280,300), 80, std::make_shared<Lambertian>(pertext)));

	/*
	Scene boxes2;
	auto white = std::make_shared<Lambertian>(std::make_shared<Solid>(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.add(std::make_shared<Sphere>(Point3(random_double(0, 165, rgen), random_double(0, 165, rgen), random_double(0, 165, rgen)), 10, white));
	}

	objects.add(std::make_shared<Translate>(
				std::make_shared<Rotate>(
					std::make_shared<BVHNode>(boxes2, 0.0, 1.0, rgen), 15),
				Vec3(-100,270,395)));
	*/

	return objects;
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
	world.add(std::make_shared<FlipNormals>(std::make_shared<AARect>('y', 213, 343, 227, 332, 554, light)));
	world.add(std::make_shared<FlipNormals>(std::make_shared<AARect>('y', 0, 555, 0, 555, 555, white)));
	world.add(std::make_shared<AARect>('y', 0, 555, 0, 555, 0, white));
	world.add(std::make_shared<FlipNormals>(std::make_shared<AARect>('z', 0, 555, 0, 555, 555, white)));

	std::shared_ptr<Surface> box1 = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
	box1 = std::make_shared<Rotate>(box1, 15);
	box1 = std::make_shared<Translate>(box1, Vec3(265, 0, 295));
	world.add(box1);

	std::shared_ptr<Surface> box2 = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
	box2 = std::make_shared<Rotate>(box2, -18);
	box2 = std::make_shared<Translate>(box2, Vec3(130, 0, 65));
	world.add(box2);

	return world;
}

int main()
{
	const Color background(0.0, 0.0, 0.0);

	const auto aspect_ratio = 1.0; 
	const int image_width = 500;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int spp = 100;

	Scene world = cornell_box();
	std::shared_ptr<Surface> lights;

	Point3 lookfrom(278, 278, -800);
	Point3 lookat(278, 278, 0);
	Vec3 vup(0, 1, 0);
	auto dist_to_focus = 10;
	auto aperture = 0.0;
	Camera cam(lookfrom, lookat, vup, 40, aspect_ratio, aperture, dist_to_focus, 0, 1);

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
					pixel_color += ray_color(r, background, world, lights, 0, rgen);
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
