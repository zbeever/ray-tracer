#pragma once

#include "../engine/Texture.h"

class SolidColor: public Texture
{
private:
	Color color_value;

public:
	SolidColor();
	SolidColor(Color c);
	SolidColor(double red, double green, double blue);

	virtual Color value(double u, double v, const Point3& p) const;
	static std::shared_ptr<SolidColor> make(Color c);
	static std::shared_ptr<SolidColor> make(double red, double green, double blue);
};
