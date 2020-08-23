#pragma once

#include "../engine/PDF.h"
#include "../engine/Utils.h"
#include "../engine/ONB.h"

class BeckmannFresnelPDF: public PDF
{
public:
	ONB uvw;
	double alpha;

	BeckmannFresnelPDF(const Vec3& w, const double alpha_): alpha(alpha_)
	{
		uvw.build_from_w(w);
	}

	virtual double value(const Vec3& wi, const Vec3& wo, std::mt19937& rgen) const
	{
		Vec3 incident = wi;
		if (dot(wi, uvw.w()) < 0.0) incident *= -1.0;

		Vec3 exitant = wo;
		if (dot(wo, uvw.w()) < 0.0) exitant *= -1.0;

		Vec3 wh = normalize(normalize(incident) + normalize(exitant));

		double cos_theta = dot(wh, uvw.w());

		if (cos_theta < 0.0) {
			return 0.0;
		}

		double tan2_theta = (1 - pow(cos_theta, 2.0)) / pow(cos_theta, 2.0);

		double coef = 1.0 / (M_PI * pow(alpha, 2.0) * pow(cos_theta, 4.0));
		double D = coef * exp(-tan2_theta / pow(alpha, 2));

		return D;
	}

	virtual Vec3 generate(std::mt19937& rgen, const Vec3& wi) const
	{
		Vec3 wh = uvw.local(random_in_beckmann(rgen, alpha));
		Vec3 incident = wi;
		if (dot(wi, uvw.w()) < 0.0) incident *= -1.0;
		Vec3 wo = 2.0 * wh - normalize(incident);
		return wo;
	}
};
