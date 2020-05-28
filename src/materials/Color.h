#pragma once

#include "../geometry/Vec3.h"

#include <fstream>
#include <algorithm>

Color toRGB(const Color& pixel_color, const int spp)
{
	auto r = pixel_color.r();
	auto g = pixel_color.g();
	auto b = pixel_color.b();

	auto scale = 1. / spp;

	r *= scale;
	g *= scale;
	b *= scale;

	double m = std::max(std::max(r, g), std::max(b, 1.));
	r *= 1. / m;
	g *= 1. / m;
	b *= 1. / m;

	m = std::clamp((m - 1.) * .2, 0., 1.);
	r = m + r * (1. - m);
	g = m + g * (1. - m);
	b = m + b * (1. - m);

	auto inv_gamma = 1. / 2.2;
	r = pow(r, inv_gamma);
	g = pow(g, inv_gamma);
	b = pow(b, inv_gamma);

	return Color(static_cast<int>(256 * std::clamp(r, 0., .999)),
		     static_cast<int>(256 * std::clamp(g, 0., .999)),
		     static_cast<int>(256 * std::clamp(b, 0., .999)));
}
