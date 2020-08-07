#include "Perlin.h"

Perlin::Perlin()
{
	std::mt19937 rgen;

	ranvec = new Vec3[point_count];
	for (int i = 0; i < point_count; ++i) {
		ranvec[i] = random_in_unit_sphere(rgen);
	}

	perm_x = perlin_generate_perm(rgen);
	perm_y = perlin_generate_perm(rgen);
	perm_z = perlin_generate_perm(rgen);
}

Perlin::~Perlin()
{
	delete[] ranvec;
	delete[] perm_x;
	delete[] perm_y;
	delete[] perm_z;
}

double Perlin::turb(const Point3& p, int depth) const
{
	auto accum = 0.0;
	auto temp_p = p;
	auto weight = 1.0;

	for (int i = 0; i < depth; ++i) {
		accum += weight * noise(temp_p);
		weight *= 0.5;
		temp_p *= 2;
	}

	return std::abs(accum);
}

double Perlin::noise(const Point3& p) const
{
	auto u = p.x() - floor(p.x());
	auto v = p.y() - floor(p.y());
	auto w = p.z() - floor(p.z());

	int i = floor(p.x());
	int j = floor(p.y());
	int k = floor(p.z());

	Vec3 c[2][2][2];

	for (int di = 0; di < 2; ++di) {
		for (int dj = 0; dj < 2; ++dj) {
			for (int dk = 0; dk < 2; ++dk) {
				c[di][dj][dk] = ranvec[
						     perm_x[(i + di) & 255]
						   ^ perm_y[(j + dj) & 255]
						   ^ perm_z[(k + dk) & 255]
						];
			}
		}
	}

	return perlin_interp(c, u, v, w);
}
