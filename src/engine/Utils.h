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

inline double schlick(const double cosine, const double ri)
{
	auto r0 = (1 - ri) / (1 + ri);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow(1 - cosine, 5);
}

inline double fr_conductor(double cosThetaI, const double etai, const double etat, const double k) {
    cosThetaI = std::clamp(cosThetaI, -1.0, 1.0);
    double eta = etat / etai; // May need to swap
    double etak = k / etai; // Same as above

    double cosThetaI2 = cosThetaI * cosThetaI;
    double sinThetaI2 = 1.0 - cosThetaI2;
    double eta2 = eta * eta;
    double etak2 = etak * etak;

    double t0 = eta2 - etak2 - sinThetaI2;
    double a2plusb2 = sqrt(t0 * t0 + 4.0 * eta2 * etak2);
    double t1 = a2plusb2 + cosThetaI2;
    double a = sqrt(0.5 * (a2plusb2 + t0));
    double t2 = 2.0 * cosThetaI * a;
    double Rs = (t1 - t2) / (t1 + t2);

    double t3 = cosThetaI2 * a2plusb2 + sinThetaI2 * sinThetaI2;
    double t4 = t2 * sinThetaI2;
    double Rp = Rs * (t3 - t4) / (t3 + t4);

    return 0.5 * (Rp + Rs);
}
