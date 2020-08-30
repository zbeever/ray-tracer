#pragma once

#include "../engine/PDF.h"
#include "../engine/Utils.h"
#include "../engine/ONB.h"
#include "../engine/Microfacet.h"

class BeckmannPDF: public PDF
{
public:
	ONB uvw;
	double alpha_b;

	BeckmannPDF(const Vec3& w, const double alpha_b_): alpha_b(alpha_b_)
	{
		uvw.build_from_w(w);
	}

	virtual double value(const Ray& incoming, const Ray& outgoing, std::mt19937& rgen) const
	{
		Vec3 i = -normalize(incoming.direction());
		Vec3 o = normalize(outgoing.direction());
		Vec3 h = normalize(i + o);

		if ((dot(h, uvw.w()) < 1e-6) || (std::abs(dot(i, uvw.w())) < 1e-6)) {
			return 0.;
		}

		if ((std::abs(h.x()) < 1e-6) && (std::abs(h.y()) < 1e-6) && (std::abs(h.z()) < 1e-6)) {
			return 0.;
		}

		double cos_theta_h = std::abs(dot(h, uvw.w()));
		double D = D_beckmann(h, uvw.w(), alpha_b);

		return D * cos_theta_h;
	}

	virtual Vec3 generate(std::mt19937& rgen, const Vec3& i, const Vec3& n) const
	{
		Vec3 h = normalize(uvw.local(random_in_beckmann(rgen, alpha_b)));
		Vec3 i_aligned = (dot(i, uvw.w()) < 0.) ? i : -i; 
		Vec3 o = normalize(i_aligned - 2. * dot(i_aligned, h) * h);
		return o;
	}
};
