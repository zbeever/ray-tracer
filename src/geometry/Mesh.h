#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include "Surface.h"

class Mesh: public Surface
{
public:
	std::vector<Point3> vertex_array;
	std::vector<int> index_array;

	Mesh(const std::string& model_name);

	void shift(const Vec3& offset);
	void scale(const float k, const Point3& o);

	bool hit(const Ray& r, HitRecord& hit_record, const float s_min, const float s_max) const;
};
