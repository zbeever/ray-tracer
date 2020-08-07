#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <ratio>
#include <omp.h>
#include <math.h>
#include <algorithm>

#include "engine/AABB.h"
#include "engine/BVHNode.h"
#include "engine/Camera.h"
#include "engine/Material.h"
#include "engine/ONB.h"
#include "engine/PDF.h"
#include "engine/Ray.h"
#include "engine/Record.h"
#include "engine/ScatterRecord.h"
#include "engine/Scene.h"
#include "engine/Surface.h"
#include "engine/Texture.h"
#include "engine/Utils.h"
#include "engine/Vec3.h"

#include "materials/Dielectric.h"
#include "materials/DiffuseLight.h"
#include "materials/Isotropic.h"
#include "materials/Lambertian.h"
#include "materials/Metal.h"

#include "primitives/AARect.h"
#include "primitives/Box.h"
#include "primitives/ConstantMedium.h"
#include "primitives/MovingSphere.h"
#include "primitives/Sphere.h"

#include "samplers/CosinePDF.h"
#include "samplers/MixturePDF.h"
#include "samplers/SurfacePDF.h"

#include "textures/Checker.h"
#include "textures/Image.h"
#include "textures/Noise.h"
#include "textures/Perlin.h"
#include "textures/SolidColor.h"

#include "transformations/FlipNormals.h"
#include "transformations/Rotate.h"
#include "transformations/Translate.h"

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

	return emitted + srec.attenuation * srec.pdf_ptr->value(scattered.direction(), rgen) * ray_color(scattered, background, world, lights, depth + 1, rgen) / (pdf_val * p);
}

Scene cornell_box()
{
	Scene world;

	auto red   = Lambertian::make( SolidColor::make(0.65, 0.05, 0.05) );
	auto white = Lambertian::make( SolidColor::make(0.73, 0.73, 0.73) );
	auto green = Lambertian::make( SolidColor::make(0.12, 0.45, 0.15) );

	auto light = DiffuseLight::make( SolidColor::make(15.0, 15.0, 15.0) );

	world.add( FlipNormals::applyTo( AARect::make('x', 0,   555, 0,   555, 555, green) ) );
	world.add( 			 AARect::make('x', 0,   555, 0,   555, 0,   red  )   );
	world.add( FlipNormals::applyTo( AARect::make('y', 213, 343, 227, 332, 554, light) ) );
	world.add( FlipNormals::applyTo( AARect::make('y', 0,   555, 0,   555, 555, white) ) );
	world.add( 			 AARect::make('y', 0,   555, 0,   555, 0,   white)   );
	world.add( FlipNormals::applyTo( AARect::make('z', 0,   555, 0,   555, 555, white) ) );

	std::shared_ptr<Surface> box1 = Box::make(Point3(0, 0, 0), Point3(165, 330, 165), white);
	box1 = Rotate::applyTo(    box1, 15                );
	box1 = Translate::applyTo( box1, Vec3(265, 0, 295) );
	world.add(box1);

	std::shared_ptr<Surface> box2 = Box::make(Point3(0, 0, 0), Point3(165, 165, 165), white);
	box2 = Rotate::applyTo(    box2, -18              );
	box2 = Translate::applyTo( box2, Vec3(130, 0, 65) );
	world.add(box2);

	return world;
}

Color toRGB(const Color& pixel_color, const int spp)
{
	auto r = pixel_color.r();
	auto g = pixel_color.g();
	auto b = pixel_color.b();

	if (isnan(r) == true) r = 0.0;
	if (isnan(g) == true) g = 0.0;
	if (isnan(b) == true) b = 0.0;

	auto scale = 1. / spp;

	r *= scale;
	g *= scale;
	b *= scale;

	double m = std::max(std::max(r, g), std::max(b, 1.));
	r *= 1. / m;
	g *= 1. / m;
	b *= 1. / m;

	m = std::clamp((m - 1.) * .2, 0., 1.);
	r = m + r * (1. - m);
	g = m + g * (1. - m);
	b = m + b * (1. - m);

	auto inv_gamma = 1. / 2.2;
	r = pow(r, inv_gamma);
	g = pow(g, inv_gamma);
	b = pow(b, inv_gamma);

	return Color(static_cast<int>(256 * std::clamp(r, 0., .999)),
		     static_cast<int>(256 * std::clamp(g, 0., .999)),
		     static_cast<int>(256 * std::clamp(b, 0., .999)));
}

int main()
{
	const Color background(0.0, 0.0, 0.0);

	const auto aspect_ratio = 1.0; 
	const int image_width = 500;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int spp = 100;

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
