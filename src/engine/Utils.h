#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <random>
#include <cmath>

const double INF = std::numeric_limits<double>::infinity();
const double EPS = 1e-3;
const double PI = M_PI;
const double INV_PI = M_1_PI;
const double PI_OVER_180 = M_PI / 180.0;

inline double degrees_to_radians(const double degrees)
{
	return degrees * PI_OVER_180;
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

