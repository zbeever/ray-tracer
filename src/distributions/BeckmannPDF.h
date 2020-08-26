#pragma once

#include "../engine/PDF.h"
#include "../engine/Utils.h"
#include "../engine/ONB.h"

class BeckmannPDF: public PDF
{
public:
	ONB uvw;
	double alpha;

	BeckmannPDF(const Vec3& w, const double alpha_): alpha(alpha_)
	{
		uvw.build_from_w(w);
	}

	virtual double value(const Vec3& wi, const Vec3& wo, std::mt19937& rgen) const
	{
		Vec3 wi_aligned = (dot(wi, uvw.w()) > 0.0) ? wi : -wi;
		Vec3 wh = normalize(normalize(wi_aligned) + normalize(wo));

		if (dot(wh, uvw.w()) < 0.0) {
			return 0.0;
		}

		if ((dot(wh, uvw.w()) < 1e-6) || (std::abs(dot(wi_aligned, uvw.w())) < 1e-6)) {
			return 0.0;
		}

		if ((std::abs(wh.x()) < 1e-6) && (std::abs(wh.y()) < 1e-6) && (std::abs(wh.z()) < 1e-6)) {
			return 0.0;
		}

		double cos_theta = std::abs(dot(wh, uvw.w()));
		double tan2_theta = (1.0 - pow(cos_theta, 2.0)) / pow(cos_theta, 2.0);

		double coef = 1.0 / (M_PI * pow(alpha, 2.0) * pow(cos_theta, 4.0));
		double D = coef * exp(-tan2_theta / pow(alpha, 2));

		return D * cos_theta;
	}

	virtual Vec3 generate(std::mt19937& rgen, const Vec3& wi, const Vec3& n) const
	{
		Vec3 wh = normalize(uvw.local(random_in_beckmann(rgen, alpha)));
		Vec3 wi_aligned = (dot(wi, uvw.w()) < 0.0) ? wi : -wi; 
		Vec3 wo = normalize(wi_aligned - 2.0 * dot(wi_aligned, wh) * wh);
		return wo;
	}
};
