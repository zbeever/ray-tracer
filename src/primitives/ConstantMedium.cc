#include "ConstantMedium.h"

ConstantMedium::ConstantMedium(std::shared_ptr<Surface> b, double d, std::shared_ptr<Texture> a): boundary(b), neg_inv_density(-1.0 / d)
{
	phase_function = std::make_shared<Isotropic>(a);
}

bool ConstantMedium::hit(const Ray& r, double t_min, double t_max, Record& rec, std::mt19937& rgen) const
{
	const bool enableDebug = false;
	const bool debugging = enableDebug && random_double(rgen) < 1e-5;

	Record rec1, rec2;

	// Does the ray pierce the medium (even at negative t)?
	// If it does, record the entry point (r.at(rec1.t))
	if (!boundary->hit(r, -infinity, infinity, rec1, rgen)) {
		return false;
	}

	// If it does, is it a glancing blow? We won't count these.
	// If it spends an appreciable distance inside the minimum, record the exit point. (r.at(rec2.t))
	if (!boundary->hit(r, rec1.t + 1e-4, infinity, rec2, rgen)) {
		return false;
	}

	if (debugging) {
		std::cerr << "\nt0=" << rec1.t << ", t1=" << rec2.t << "\n";
	}

	
	// Since t_min is always 0+, this constrains the entry location to be in the future of the ray.
	if (rec1.t < t_min) {
		rec1.t = t_min;
	}

	// This just makes sure t_max isn't infinitely far away
	if (rec2.t > t_max) {
		rec2.t = t_max;
	}

	// If t_min > rec2.t, the object lies in the past of the ray
	if (rec1.t >= rec2.t) {
		return false;
	}

	// Seems redundant
	if (rec1.t < 0) {
		rec1.t = 0;
	}

	const auto ray_length = r.direction().length(); // We have not normalized direction vectors, so we need this code
	const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length; // Exactly what it says
	const auto hit_distance = neg_inv_density * log(random_double(rgen)); // -log(rand) is the scattering PDF (it integrates to 1 over the [0, 1] interval)

	// Clearly there is no intersection if the hit distance is outside of the medium
	if (hit_distance > distance_inside_boundary) {
		return false;
	}

	rec.t = rec1.t + hit_distance / ray_length; // Again, this division stems from our decision to not normalize direction vectors
	rec.p = r.at(rec.t); // The point of intersection

	if (debugging) {
		std::cerr << "hit_distance = " << hit_distance << "\n"
			  << "rec.t = " << rec.t << "\n"
			  << "rec.p = " << rec.p << "\n";
	}

	rec.normal = Vec3(1, 0, 0); // Arbitrary
	rec.front_face = true; // Arbitrary
	rec.mat_ptr = phase_function; // How should we scatter the ray?

	return true;
}

bool ConstantMedium::bounding_box(double t0, double t1, AABB& output_box) const
{
	return boundary->bounding_box(t0, t1, output_box);
}
