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
#include "materials/MicrofacetReflection.h"
#include "materials/Lambertian.h"
#include "materials/OrenNayar.h"
// #include "materials/Metal.h"
#include "materials/MicrofacetTransmission.h"

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
	// std::cout << "MADE IT" << std::endl;
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
		// std::cout << "Getting BXDF...\n" << std::flush;
		bxdf = rec.mat_ptr->bxdf(incident, srec.specular_ray, rec, srec);


		// std::cout << "Setting exitant ray...\n" << std::flush;
		exitant = srec.specular_ray;
	} else {
		// If the object we hit is matte, importance sample the material and lights using a mixed PDF
		// This creates a PDF of the form (0.5 / n) * (f_1(x) + f_2(x) + ... + f_n(x)) + 0.5 * g(x)
		// where each f(x) is a PDF for a light source and g(x) is the PDF for the material
		auto light_ptr = std::make_shared<SurfacePDF>(lights, rec.p);
		MixturePDF pdf(light_ptr, srec.pdf_ptr);

		// Get a ray and its associated PDF value from the above distribution
		exitant = Ray(rec.p, pdf.generate(rgen, incident.direction(), rec.normal), incident.time(), incident.bin());
		pdf_val = pdf.value(incident, exitant, rgen);

		// Set the BxDF value
		bxdf = rec.mat_ptr->bxdf(incident, exitant, rec, srec);
	}

	// If the path depth is greater than 5, perform Russian roulette based of the BxDF value
	if (depth > 5) {
		// Keep an eye on this! If this is consistently too high we get a segmentation fault.
		if (srec.is_specular) {
			p = 0.75;
		} else {
			p = bxdf;
		}
		if (random_double(rgen) < 1 - p) {
			return 0.0;
		}
	}
	

	// The weakening factor in the rendering equation
	// if (srec.is_specular) std::cout << "Getting the Lambertian term...\n" << std::flush;
	Vec3 wo = normalize(exitant.direction());
	Vec3 n = normalize(rec.normal);
	double cos_theta = std::abs(dot(wo, n));

	if ((pdf_val < 1e-6) || (bxdf < 1e-6)) {
		return 0.0;
	}

	/*
	if (srec.is_specular) std::cout << "Recursing...\n" << std::flush;
	if (srec.is_specular && srec.reflected) {
		std::cout << "Recursion record:" << 
			     "\nemitted: " << emitted <<
			     "\nbxdf: " << bxdf <<
			     "\nexitant (ori): " << exitant.origin() <<
			     "\nexitant (dir): " << exitant.direction() <<
			     "\ndepth + 1: " << depth + 1 <<
			     "\ncos_theta: " << cos_theta <<
			     "\npdf_val: " << pdf_val <<
			     "\np: " << p <<
			     "\n" << std::flush;
	}
	*/
	// A random sample of the rendering equation: L_o = L_e + (BxDF * L_i * cos_theta) / pdf_val
	// This is divided by the Russian roulette probability
	return emitted + bxdf * radiance(exitant, background, world, lights, depth + 1, rgen) * cos_theta / (pdf_val * p);
}

Scene cornell_box()
{
	Scene world;

	auto red   = OrenNayar::make( SolidColor::make( PeakSpectrum(700, 0.9, 60)), 0.2);
	auto white = OrenNayar::make( SolidColor::make( FlatSpectrum(0.9) ), 0.2);
	auto other = OrenNayar::make( SolidColor::make( PeakSpectrum(400, 0.9, 180) ), 0.2);
	auto green = OrenNayar::make( SolidColor::make( PeakSpectrum(560, 0.9, 60) ), 0.2);

	std::array<double, 73> gold_n_arr = {1.46220588, 1.46220126, 1.46534591, 1.46849057, 1.46836478,
       1.46522013, 1.46207547, 1.45901163, 1.45610465, 1.45319767,
       1.4502907 , 1.43455882, 1.41740196, 1.4002451 , 1.38308824,
       1.366     , 1.34892683, 1.33185366, 1.31478049, 1.27032653,
       1.21522449, 1.16012245, 1.10502041, 1.04991837, 0.97112   ,
       0.88712   , 0.80312   , 0.71912   , 0.63512   , 0.59187726,
       0.55758123, 0.5232852 , 0.48898917, 0.45469314, 0.42414925,
       0.40325373, 0.38235821, 0.36146269, 0.34056716, 0.31967164,
       0.29877612, 0.28331412, 0.27178674, 0.26025937, 0.24873199,
       0.23720461, 0.22567723, 0.21414986, 0.2047541 , 0.19655738,
       0.18836066, 0.18016393, 0.17196721, 0.16377049, 0.15557377,
       0.14737705, 0.13988889, 0.13877778, 0.13766667, 0.13655556,
       0.13544444, 0.13433333, 0.13322222, 0.13211111, 0.131     ,
       0.13009709, 0.13106796, 0.13203883, 0.13300971, 0.13398058,
       0.13495146, 0.13592233, 0.1368932};

	std::array<double, 73> gold_k_arr = {1.92880882, 1.93718239, 1.94315723, 1.94913208, 1.95298113,
       1.95486792, 1.95675472, 1.95701163, 1.95410465, 1.95119767,
       1.9482907 , 1.9405    , 1.93216667, 1.92383333, 1.9155    ,
       1.901     , 1.88514634, 1.86929268, 1.85343902, 1.84664898,
       1.84338367, 1.84011837, 1.83685306, 1.83358776, 1.873672  ,
       1.923272  , 1.972872  , 2.022472  , 2.072072  , 2.1363574 ,
       2.20386643, 2.27137545, 2.33888448, 2.4063935 , 2.47205075,
       2.53294627, 2.59384179, 2.65473731, 2.71563284, 2.77652836,
       2.83742388, 2.89718156, 2.95611527, 3.01504899, 3.07398271,
       3.13291643, 3.19185014, 3.25078386, 3.30385012, 3.35361593,
       3.40338173, 3.45314754, 3.50291335, 3.55267916, 3.60244496,
       3.65221077, 3.70151111, 3.74662222, 3.79173333, 3.83684444,
       3.88195556, 3.92706667, 3.97217778, 4.01728889, 4.0624    ,
       4.10726214, 4.1498835 , 4.19250485, 4.23512621, 4.27774757,
       4.32036893, 4.36299029, 4.40561165
	};

	auto gold_n = std::make_shared<Spectrum>(gold_n_arr);
	auto gold_k = std::make_shared<Spectrum>(gold_k_arr);

	auto plastic = std::make_shared<MicrofacetReflection>(gold_n, gold_k, 0.2);

	double A = 1.0;
	std::array<double, 3> B = {1.03961212, 0.231792344, 1.01046945};
	std::array<double, 3> C = {6.00069867e-3, 2.00179144e-2, 1.03560653e2};
	auto glass = Dielectric::make( SolidColor::make( FlatSpectrum(1.0) ), std::make_shared<Sellmeier>(A, B, C) );
	auto fglass = std::make_shared<MicrofacetTransmission>(SolidColor::make( FlatSpectrum(1.0) ), 0.05, std::make_shared<Sellmeier>(A, B, C));

	auto light = DiffuseLight::make( SolidColor::make( FlatSpectrum(1.0)) );

	world.add( FlipNormals::applyTo( AARect::make('x', 0,   500, -500,   1250, 0,   green) ) );
	world.add( 			 AARect::make('x', 0,   500, -500,   1250, 500, red  )   );
	world.add( FlipNormals::applyTo( AARect::make('y', 170, 330,  -30,   130,  499, light) ) );
	world.add( FlipNormals::applyTo( AARect::make('y', 0,   500, -500,   1250, 0,   white) ) );
	world.add( 			 AARect::make('y', 0,   500, -500,   1250, 500, white)   );
	world.add( FlipNormals::applyTo( AARect::make('z', 0,   500,  0,     500, -500, white) ) );

	world.add( Sphere::make( Point3(200, 85, 200), 85, fglass) );

	std::shared_ptr<Surface> box1 = Box::make(Point3(0, 0, 0), Point3(150, 300, 150), white);
	box1 = Rotate::applyTo(    box1, 15                );
	box1 = Translate::applyTo( box1, Vec3(50, 0, -300) );
	world.add(box1);

	std::shared_ptr<Surface> box2 = Box::make(Point3(0, 0, 0), Point3(150, 150, 150), plastic);
	box2 = Rotate::applyTo(    box2, -18              );
	box2 = Translate::applyTo( box2, Vec3(300, 0, -150) );
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
	const Spectrum background = FlatSpectrum(0.0);

	const auto aspect_ratio = 1.0; 
	const int image_width = 512;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int spp = 20;

	Scene world = cornell_box();
	std::shared_ptr<Scene> lights = std::make_shared<Scene>();
	lights->add(AARect::make('y', 170, 330,  -30,   130,  499, std::shared_ptr<Material>()));

	Point3 lookfrom(250, 250, 1250);
	Point3 lookat(250, 250, 0);
	Vec3 vup(0, 1, 0);
	auto dist_to_focus = 1250 - 200 - 45;
	auto aperture = 20.0;
	Camera cam(lookfrom, lookat, vup, 34, aspect_ratio, aperture, dist_to_focus, 0, 1);

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
