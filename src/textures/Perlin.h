#pragma once

#include <random>
#include "../engine/Vec3.h"

class Perlin
{
private:
	static const int point_count = 256;
	Vec3* ranvec;
	int* perm_x;
	int* perm_y;
	int* perm_z;

	static int* perlin_generate_perm(std::mt19937& rgen)
	{
		auto p = new int[point_count];

		for (int i = 0; i < Perlin::point_count; ++i) {
			p[i] = i;
		}

		permute(p, point_count, rgen);

		return p;
	}

	static void permute(int* p, int n, std::mt19937& rgen)
	{
		for (int i = n - 1; i > 0; --i) {
			int target = random_int(0, i, rgen);
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

public:
	Perlin();
	~Perlin();

	double turb(const Point3& p, int depth=7) const;
	double noise(const Point3& p) const;
};

inline double perlin_interp(Vec3 c[2][2][2], double u, double v, double w)
{
	auto uu = u * u * (3 - 2 * u);
	auto vv = v * v * (3 - 2 * v);
	auto ww = w * w * (3 - 2 * w);
	auto accum = 0.0;

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				Vec3 weight_v(u - i, v - j, w - k);
				accum += (i * uu + (1 - i) * (1 - uu))
				       * (j * vv + (1 - j) * (1 - vv))
				       * (k * ww + (1 - k) * (1 - ww))
				       * dot(c[i][j][k], weight_v);
			}
		}
	}
	return accum;
}
