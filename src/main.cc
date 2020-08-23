#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <ratio>
#include <omp.h>
#include <math.h>
#include <algorithm>

#include "engine/Spectrum.h"

#include "spectra/Sellmeier.h"
#include "spectra/CIE.h"
#include "spectra/FlatSpectrum.h"
#include "spectra/PeakSpectrum.h"

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
// #include "materials/Isotropic.h"
#include "materials/TorranceSparrow.h"
#include "materials/Lambertian.h"
#include "materials/OrenNayar.h"
// #include "materials/Metal.h"

#include "primitives/AARect.h"
#include "primitives/Box.h"
// #include "primitives/ConstantMedium.h"
// #include "primitives/MovingSphere.h"
#include "primitives/Sphere.h"

#include "distributions/CosinePDF.h"
#include "distributions/MixturePDF.h"
#include "distributions/SurfacePDF.h"
#include "distributions/UniformPDF.h"
#include "distributions/BeckmannPDF.h"

// #include "textures/Checker.h"
// #include "textures/Image.h"
// #include "textures/Noise.h"
// #include "textures/Perlin.h"
#include "textures/SolidColor.h"

#include "transformations/FlipNormals.h"
#include "transformations/Rotate.h"
#include "transformations/Translate.h"

double radiance(const Ray& incident, const Spectrum& background, const Scene& world, std::shared_ptr<Surface> lights, const int depth, std::mt19937& rgen)
{
	Record rec;

	// If we hit nothing, return the background color.
	if (!world.hit(incident, EPS, INF, rec, rgen)) {
		return background.get(incident.bin());
	}

	// If we hit something that doesn't scatter light, return the object's emittance.
	ScatterRecord srec;
	double emitted = rec.mat_ptr->emitted(incident, rec, rec.u, rec.v, rec.p);
	if (!rec.mat_ptr->scatter(incident, rec, srec, rgen)) {
		return emitted;
	}

	// Set placeholder values for the BxDF, PDF value, Russian roulette probability, and exitant ray
	double bxdf = 1.0;
	double pdf_val = 1.0;
	double p = 1.0;
	Ray exitant;

	if (srec.is_specular) {
		// If the object we hit is specular, set the exitant ray and BxDF (a color divided by |wo . n| -- see notes)
		bxdf = rec.mat_ptr->bxdf(incident, srec.specular_ray, rec, srec);
		exitant = srec.specular_ray;
	} else {
		// If the object we hit is matte, importance sample the material and lights using a mixed PDF
		// This creates a PDF of the form (0.5 / n) * (f_1(x) + f_2(x) + ... + f_n(x)) + 0.5 * g(x)
		// where each f(x) is a PDF for a light source and g(x) is the PDF for the material
		auto light_ptr = std::make_shared<SurfacePDF>(lights, rec.p);
		MixturePDF pdf(light_ptr, srec.pdf_ptr);

		// Get a ray and its associated PDF value from the above distribution
		exitant = Ray(rec.p, pdf.generate(rgen, incident.direction()), incident.time(), incident.bin());
		pdf_val = pdf.value(incident.direction(), exitant.direction(), rgen);

		// Set the BxDF value
		bxdf = rec.mat_ptr->bxdf(incident, exitant, rec, srec);
	}

	// If the path depth is greater than 5, perform Russian roulette based of the BxDF value
	if (depth > 5) {
		p = bxdf;
		if (random_double(rgen) < 1 - p) {
			return 0.0;
		}
	}

	// The weakening factor in the rendering equation
	Vec3 wo = normalize(exitant.direction());
	Vec3 n = normalize(rec.normal);
	double cos_theta = std::abs(dot(wo, n));

	// A random sample of the rendering equation: L_o = L_e + (BxDF * L_i * cos_theta) / pdf_val
	// This is divided by the Russian roulette probability
	return emitted + bxdf * radiance(exitant, background, world, lights, depth + 1, rgen) * cos_theta / (pdf_val * p);
}

Scene cornell_box()
{
	Scene world;

	auto red   = OrenNayar::make( SolidColor::make( PeakSpectrum(700, 0.9, 60)), 0.2);
	auto white = OrenNayar::make( SolidColor::make( FlatSpectrum(0.9) ), 0.2);
	auto green = OrenNayar::make( SolidColor::make( PeakSpectrum(560, 0.9, 60) ), 0.2);
	auto plastic = std::make_shared<TorranceSparrow>( SolidColor::make( FlatSpectrum(0.9) ), 0.2);

	double A = 1.0;
	std::array<double, 3> B = {1.03961212, 0.231792344, 1.01046945};
	std::array<double, 3> C = {6.00069867e-3, 2.00179144e-2, 1.03560653e2};
	auto glass = Dielectric::make( SolidColor::make( FlatSpectrum(1.0) ), std::make_shared<Sellmeier>(A, B, C) );

	auto light = DiffuseLight::make( SolidColor::make( PeakSpectrum(450, 1.0, 150) ) );

	world.add( FlipNormals::applyTo( AARect::make('x', 0,   555, 0,   555, 555, green) ) );
	world.add( 			 AARect::make('x', 0,   555, 0,   555, 0,   red  )   );
	world.add( FlipNormals::applyTo( AARect::make('y', 151, 414, 151, 414, 554, light) ) );
	world.add( FlipNormals::applyTo( AARect::make('y', 0,   555, 0,   555, 555, white) ) );
	world.add( 			 AARect::make('y', 0,   555, 0,   555, 0,   white)   );
	world.add( FlipNormals::applyTo( AARect::make('z', 0,   555, 0,   555, 555, white) ) );

	world.add( Sphere::make( Point3(280, 380, 180), 85, glass ) );

	std::shared_ptr<Surface> box1 = Box::make(Point3(0, 0, 0), Point3(165, 330, 165), plastic);
	box1 = Rotate::applyTo(    box1, 15                );
	box1 = Translate::applyTo( box1, Vec3(265, 0, 295) );
	world.add(box1);

	std::shared_ptr<Surface> box2 = Box::make(Point3(0, 0, 0), Point3(165, 165, 165), white);
	box2 = Rotate::applyTo(    box2, -18              );
	box2 = Translate::applyTo( box2, Vec3(130, 0, 65) );
	world.add(box2);

	return world;
}

Color toRGB(const Color& pixel_color)
{
	double r = pixel_color.r();
	double g = pixel_color.g();
	double b = pixel_color.b();

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
	const Spectrum background = Spectrum();

	const auto aspect_ratio = 1.0; 
	const int image_width = 512;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int spp = 50;

	Scene world = cornell_box();
	std::shared_ptr<Scene> lights = std::make_shared<Scene>();
	lights->add(std::make_shared<AARect>('y', 213, 343, 227, 332, 554, std::shared_ptr<Material>()));
	lights->add(Sphere::make( Point3(280, 380, 180), 85, std::shared_ptr<Material>() ));

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

	CIE cie = CIE();

#pragma omp parallel 
	{
#pragma omp for schedule(dynamic)
		for (int j = image_height - 1; j >= 0; --j) {
			static thread_local std::mt19937 rgen((omp_get_thread_num() + 1));
			fprintf(stderr, "\rRendering %5.2f%%", 100.*(image_height - 1 - j) / (image_height - 1)); 
			for (int i = 0; i < image_width; ++i) {
				Spectrum pixel_color = Spectrum();
				int spps[73] = {0};
				for (int s = 0; s < spp; ++s) {
					auto u = double(i + random_double(rgen)) / (image_width - 1);
					auto v = double(j + random_double(rgen)) / (image_height - 1);
					Ray r = cam.get_ray(u, v, rgen);
					pixel_color[r.bin()] += radiance(r, background, world, lights, 0, rgen);
					spps[r.bin()] += 1;
				}
				for (int i = 0; i < 73; ++i) {
					if (spps[i] > 0) {
						pixel_color[i] /= spps[i];
					}
				}

				double radiance = pixel_color.sum();
				Color pixel_rgb(0., 0., 0.);
				pixel_rgb[0] = (pixel_color * cie.getX()).integrate();
				pixel_rgb[1] = (pixel_color * cie.getY()).integrate();
				pixel_rgb[2] = (pixel_color * cie.getZ()).integrate();

				double sum = pixel_rgb.r() + pixel_rgb.g() + pixel_rgb.b();
				if (sum > 1e-5) pixel_rgb /= sum;

				double xRed = 0.67;
				double yRed = 0.33;
				double xGreen = 0.21;
				double yGreen = 0.71;
				double xBlue = 0.14;
				double yBlue = 0.08;
				double xWhite = 0.3101;
				double yWhite = 0.3162;
				double gamma = 0.0;
				
				double xr = xRed;   double yr = yRed;   double zr = 1 - (xr + yr);
				double xg = xGreen; double yg = yGreen; double zg = 1 - (xg + yg);
				double xb = xBlue;  double yb = yBlue;  double zb = 1 - (xb + yb);
				double xw = xWhite; double yw = yWhite; double zw = 1 - (xw + yw);

				/* Compute the XYZ to RGB matrix. */
				double rx = (yg * zb) - (yb * zg);
				double ry = (xb * zg) - (xg * zb);
				double rz = (xg * yb) - (xb * yg);
				double gx = (yb * zr) - (yr * zb);
				double gy = (xr * zb) - (xb * zr);
				double gz = (xb * yr) - (xr * yb);
				double bx = (yr * zg) - (yg * zr);
				double by = (xg * zr) - (xr * zg);
				double bz = (xr * yg) - (xg * yr);

				/* Compute the RGB luminance scaling factor. */
				double rw = ((rx * xw) + (ry * yw) + (rz * zw)) / yw;
				double gw = ((gx * xw) + (gy * yw) + (gz * zw)) / yw;
				double bw = ((bx * xw) + (by * yw) + (bz * zw)) / yw;

				/* Scale the XYZ to RGB matrix to white. */
				rx = rx / rw;  ry = ry / rw;  rz = rz / rw;
				gx = gx / gw;  gy = gy / gw;  gz = gz / gw;
				bx = bx / bw;  by = by / bw;  bz = bz / bw;

				/* Calculate the desired RGB. */
				Color rgb = Color((rx * pixel_rgb.r()) + (ry * pixel_rgb.g()) + (rz * pixel_rgb.b()),
						(gx * pixel_rgb.r()) + (gy * pixel_rgb.g()) + (gz * pixel_rgb.b()),
						(bx * pixel_rgb.r()) + (by * pixel_rgb.g()) + (bz * pixel_rgb.b()));

				/* Constrain the RGB color within the RGB gamut. */
				double w = std::min(0.0, std::min(rgb.r(), std::min(rgb.g(), rgb.b()) ) );
				rgb = rgb - Color(w, w, w);
				
				pixel_array[i + (image_height - j - 1) * image_width] = toRGB(rgb * radiance);
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
