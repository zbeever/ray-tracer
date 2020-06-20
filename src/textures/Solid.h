#include "Texture.h"

class Solid: public Texture
{
private:
	Color color_value;

public:
	Solid();
	Solid(Color c);
	Solid(double red, double green, double blue);

	virtual Color value(double u, double v, const Point3& p) const;
};
