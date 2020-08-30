#pragma once

#include "Vec3.h"

double D_beckmann(const Vec3& m, const Vec3& n, double alpha_b);
double G1_beckmann(const Vec3& v, const Vec3& m, const Vec3& n, double alpha_b);
double G_beckmann(const Vec3& i, const Vec3& o, const Vec3& m, const Vec3& n, double alpha_b);
