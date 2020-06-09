#include "Texture.h"

class SolidColor: public Texture
{
private:
	Color color_value;

public:
	SolidColor() { }
	SolidColor(Color c): color_value(c) { }

	SolidColor(double red, double green, double blue): SolidColor(Color(red, green, blue)) { }

	virtual color_value(double u, double v, const Point3& p) const
	{
		return color_value;
	}
};
