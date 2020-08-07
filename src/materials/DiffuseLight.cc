#include "DiffuseLight.h"

DiffuseLight::DiffuseLight(std::shared_ptr<Texture> a): emit(a)
{ }

bool DiffuseLight::scatter(const Ray& r_in, const Record& rec, Color& attenuation, Ray& scattered, std::mt19937& rgen) const
{
	return false;
}

Color DiffuseLight::emitted(const Ray& r_in, const Record& rec, double u, double v, const Point3& p) const
{
	if (rec.front_face) {
		return emit->value(u, v, p);
	} else {
		return Color(0.0, 0.0, 0.0);
	}

	return Color(0.0, 0.0, 0.0);
}

std::shared_ptr<DiffuseLight> DiffuseLight::make(std::shared_ptr<Texture> a)
{
	return std::make_shared<DiffuseLight>(a);
}
