#include "Microfacet.h"
#include <cmath>

double D_beckmann(const Vec3& m, const Vec3& n, double alpha_b)
{
	if (dot(m, n) <= 0.) {
		return 0.;
	}

	double cos_theta_m = dot(m, n);
	double tan2_theta_m = (1. - pow(cos_theta_m, 2)) / pow(cos_theta_m, 2);

	return exp(-tan2_theta_m / pow(alpha_b, 2)) / (M_PI * pow(alpha_b, 2) * pow(cos_theta_m, 4));
}

double G1_beckmann(const Vec3& v, const Vec3& m, const Vec3& n, double alpha_b)
{
	if (dot(v, m) / dot(v, n) <= 0.) {
		return 0.;
	}

	double cos_theta_v = dot(v, n);
	double tan_theta_v = sqrt(1. - pow(cos_theta_v, 2)) / cos_theta_v;
	double a = 1. / (alpha_b * tan_theta_v);

	if (a < 1.6) {
		return (3.535 * a + 2.181 * pow(a, 2)) / (1 + 2.276 * a + 2.577 * pow(a, 2));
	} else {
		return 1.;
	}
}

double G_beckmann(const Vec3& i, const Vec3& o, const Vec3& m, const Vec3& n, double alpha_b)
{
	return G1_beckmann(i, m, n, alpha_b) * G1_beckmann(o, m, n, alpha_b);
}
