#pragma once

#include "../engine/PDF.h"
#include "../engine/Utils.h"
#include "../engine/ONB.h"

class BeckmannFresnelPDF: public PDF
{
public:
	ONB uvw;
	double m;
	double etai_over_etat;

	BeckmannFresnelPDF(const Vec3& w, const double m_, const double etai_over_etat_): m(m_), etai_over_etat(etai_over_etat_)
	{
		uvw.build_from_w(w);
	}

	virtual double value(const Vec3& incident, const Vec3& exitant, std::mt19937& rgen) const
	{
		Vec3 wi = normalize(incident);
		Vec3 wo = normalize(exitant);
		Vec3 wh;


		if (dot(wi, wo) < 0.0) {
			wh = (wo - wi) / 2.0;
		} else {
			wh = normalize(wi - etai_over_etat * wo);
		}

		double cos_theta = std::abs(dot(wh, uvw.w()));
		double F = schlick(cos_theta, etai_over_etat);

		double tan2_theta = (1 - pow(cos_theta, 2.0)) / pow(cos_theta, 2.0);

		double coef = 1.0 / (M_PI * pow(m, 2.0) * pow(cos_theta, 4.0));
		double D = coef * exp(-tan2_theta / pow(m, 2));

		double val;

		if (dot(wi, wo) < 0.0) {
			val = D * F;
		} else {
			val = D * (1.0 - F);
		}

		return val;
	}

	virtual Vec3 generate(std::mt19937& rgen, const Vec3& wi, const Vec3& n) const
	{
		Vec3 wh = uvw.local(random_in_beckmann(rgen, m));
		Vec3 incident = wi;

		double cosine = std::abs(dot(normalize(wh), normalize(incident)));
		double F = schlick(cosine, etai_over_etat);

		Vec3 wo;
		if (random_double(rgen) < F) {
			wo = 2.0 * wh + normalize(incident);
		} else {
			wo = normalize(wi - wh);
		}
		return wo;
	}
};
