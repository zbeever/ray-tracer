#pragma once

#include "../engine/Texture.h"

class SolidColor: public Texture
{
private:
	Spectrum color_value;

public:
	SolidColor();
	SolidColor(const Spectrum& spectrum);

	virtual double value(int bin, double u, double v, const Point3& p) const;
	static std::shared_ptr<SolidColor> make(Color c);
	static std::shared_ptr<SolidColor> make(const Spectrum& spectrum);
};
