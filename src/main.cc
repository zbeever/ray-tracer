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

	double p = 1.00;
	if (depth > 5) {
		p = 0.75;
		if (random_double(rgen) < 1 - p) {
			return Color(0, 0, 0);
		}
	}
	if (!world.hit(r, eps, infinity, rec, rgen)) {
		return background / p;
	}

	ScatterRecord srec;
	Color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

	if (!rec.mat_ptr->scatter(r, rec, srec, rgen)) {
		return emitted / p;
	}

	if (srec.is_specular) {
		return srec.attenuation * ray_color(srec.specular_ray, background, world, lights, depth + 1, rgen) / p;
	}

	auto light_ptr = std::make_shared<SurfacePDF>(lights, rec.p);
	MixturePDF pdf(light_ptr, srec.pdf_ptr);

	Ray scattered = Ray(rec.p, pdf.generate(rgen), r.time());
	auto pdf_val = pdf.value(scattered.direction(), rgen);

	// return emitted + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered) * ray_color(scattered, background, world, lights, depth + 1, rgen) / (pdf_val * p);
	return emitted + srec.attenuation * srec.pdf_ptr->value(scattered.direction(), rgen) * ray_color(scattered, background, world, lights, depth + 1, rgen) / (pdf_val * p);
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
	const int spp = 40;

	Scene world = cornell_box();
	std::shared_ptr<Scene> lights = std::make_shared<Scene>();
	lights->add(std::make_shared<AARect>('y', 213, 343, 227, 332, 554, std::shared_ptr<Material>()));

	Point3 lookfrom(278, 278, -800);
	Point3 lookat(278, 278, 0);
	Vec3 vup(0, 1, 0);
	auto dist_to_focus = 10;
	auto aperture = 0.0;
	Camera cam(lookfrom, lookat, vup, 37, aspect_ratio, aperture, dist_to_focus, 0, 1);

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
