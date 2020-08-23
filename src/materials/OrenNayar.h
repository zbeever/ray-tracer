#pragma once

#include "../engine/Material.h"
#include "../distributions/CosinePDF.h"
#include "../engine/ONB.h"


class OrenNayar: public Material
{
public:
	double A, B;
	std::shared_ptr<Texture> albedo;

	OrenNayar(std::shared_ptr<Texture> albedo_, const double sigma): albedo(albedo_)
	{
		A = 1.0 - 0.5 * pow(sigma, 2.0) / (pow(sigma, 2.0) + 0.33);
		B = 0.45 * pow(sigma, 2.0) / (pow(sigma, 2.0) + 0.09);
	}

	virtual bool scatter(const Ray& r_in, const Record& rec, ScatterRecord& srec, std::mt19937& rgen) const
	{
		srec.is_specular = false;
		srec.pdf_ptr = std::make_shared<CosinePDF>(rec.normal);
		return true;
	}

	virtual double bxdf(const Ray& r_in, Ray& r_out, const Record& rec, const ScatterRecord& srec) const
	{
		ONB local_basis = ONB();
		local_basis.build_from_w(rec.normal);

		Vec3 wi = normalize(r_in.direction());
		Vec3 wo = normalize(r_out.direction());

		double cosThetaI = dot(wi, local_basis.w());
		double cosThetaO = dot(wo, local_basis.w());
		double sinThetaI = sqrt(1.0 - pow(cosThetaI, 2.0));
		double sinThetaO = sqrt(1.0 - pow(cosThetaO, 2.0));
		double maxCos = 0.0;

		if (sinThetaI > EPS && sinThetaO > EPS) {
			double sinPhiI = dot(wi, local_basis.u());
			double sinPhiO = dot(wo, local_basis.u());
			double cosPhiI = sqrt(1.0 - pow(sinPhiI, 2.0));
			double cosPhiO = sqrt(1.0 - pow(sinPhiO, 2.0));
			double dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
			maxCos = std::max(0.0, dCos);
		}

		double sinAlpha, tanBeta;

		if (std::abs(cosThetaI) > std::abs(cosThetaO)) {
			sinAlpha = sinThetaO;
			tanBeta = sinThetaI / std::abs(cosThetaI);
		} else {
			sinAlpha = sinThetaI;
			tanBeta = sinThetaO / std::abs(cosThetaO);
		}

		double atten = albedo->value(r_in.bin(), rec.u, rec.v, rec.p);

		return (atten / PI) * (A + B * maxCos * sinAlpha * tanBeta);
	}


	static std::shared_ptr<OrenNayar> make(std::shared_ptr<Texture> albedo_, const double sigma)
	{
		return std::make_shared<OrenNayar>(albedo_, sigma);
	}
};
