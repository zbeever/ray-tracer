#pragma once

#include "Vec3.h"

Vec3 reflect(const Vec3& v, const Vec3& n);
Vec3 refract(const Vec3& v, const Vec3& n, double eta_i, double eta_o);

double schlick(double cos_theta_i, double eta_i_over_eta_o);

double fresnel_dielectric(double cos_theta_i, double eta_i, double eta_o);
double fresnel_conductor(double cos_theta_i, double eta_i, double eta_o, double k);
