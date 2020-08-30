#include "Fresnel.h"
#include <cmath>

double schlick(double cos_theta_i, double eta_i_over_eta_o)
{
	double sin_theta_i = sqrt(1. - pow(cos_theta_i, 2));

	if (eta_i_over_eta_o * sin_theta_i > 1.) {
		return 1.;
	}

	auto r0 = pow((1. - eta_i_over_eta_o) / (1. + eta_i_over_eta_o), 2);
	return r0 + (1. - r0) * pow(1. - cos_theta_i, 5);
}

double fresnel_dielectric(double cos_theta_i, double eta_i, double eta_o)
{
	return 1.0;
}

double fresnel_conductor(double cos_theta_i, double eta_i, double eta_o, double k)
{
    double eta_o_over_eta_i = eta_o / eta_i;
    double k_over_eta_i = k / eta_i;

    double cos2_theta_i = pow(cos_theta_i, 2);
    double sin2_theta_i = 1. - cos2_theta_i;
    double eta2_t_over_eta2_i = pow(eta_o_over_eta_i, 2);
    double k2_over_eta2_i = pow(k_over_eta_i, 2);

    double t0 = eta2_t_over_eta2_i - k2_over_eta2_i - sin2_theta_i;
    double a2_plus_b2 = sqrt(pow(t0, 2) + 4. * eta2_t_over_eta2_i * k2_over_eta2_i);
    double t1 = a2_plus_b2 + cos2_theta_i;
    double a = sqrt(.5 * (a2_plus_b2 + t0));
    double t2 = 2. * cos_theta_i * a;
    double rs = (t1 - t2) / (t1 + t2);

    double t3 = cos2_theta_i * a2_plus_b2 + pow(sin2_theta_i, 2);
    double t4 = t2 * sin2_theta_i;
    double rp = rs * (t3 - t4) / (t3 + t4);

    return .5 * (rp + rs);
}

Vec3 reflect(const Vec3& v, const Vec3& n)
{
	return v - 2. * dot(v, n) * n;
}

Vec3 refract(const Vec3& v, const Vec3& n, double eta_i, double eta_o)
{
	double eta_i_over_eta_o = eta_i / eta_o;
	double cos_theta_v = dot(-v, n);
	Vec3 r_out_parallel = eta_i_over_eta_o * (v + cos_theta_v * n);
	Vec3 r_out_perp = -sqrt(1. - r_out_parallel.length_squared()) * n;
	return r_out_parallel + r_out_perp;
}
