#pragma once

#include <cmath>
#include <cstdlib>
#include <random>

const double infinity = std::numeric_limits<double>::infinity();
const double eps = 1e-3;
const double pi = 3.1415926535897932385;
const double inv_pi = 1.0 / pi;
const double pi_over_180 = pi / 180.0;

inline double degrees_to_radians(const double degrees)
{
	return degrees * pi_over_180;
}

inline double random_double(std::mt19937& rgen)
{
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	return dist(rgen);
}

inline double random_double(double min, double max, std::mt19937& rgen)
{
	std::uniform_real_distribution<double> dist(min, max);
	return dist(rgen);
}

inline int random_int(int min, int max, std::mt19937& rgen)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rgen);
}
