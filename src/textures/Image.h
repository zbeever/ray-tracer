#pragma once

#include <algorithm>
#include "../engine/Vec3.h"
#include "../engine/Texture.h"

class Image: public Texture {
private:
	unsigned char *data;
	int width, height;
	int bytes_per_scanline;

public:
	const static int bytes_per_pixel = 3;
	
	Image();
	Image(const char* filename);
	~Image();

	virtual Color value(double u, double v, const Point3& p) const;
	static std::shared_ptr<Image> make(const char* filename);
};
