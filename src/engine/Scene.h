#pragma once

#include "../geometry/Surface.h"

#include <memory>
#include <vector>

class Scene: public Surface
{
public:
	std::vector<std::shared_ptr<Surface>> objects;

	Scene();
	Scene(std::shared_ptr<Surface> object);

	void clear();
	void add(std::shared_ptr<Surface> object);
	virtual bool hit(const Ray& ray, const double t_min, const double t_max, Record& rec, std::mt19937& rgen) const;
	virtual bool bounding_box(const double t0, const double t1, AABB& output_box) const;
	virtual double pdf_value(const Point3& o, const Vec3& v, std::mt19937& rgen) const;
	virtual Vec3 random(const Vec3& o, std::mt19937& rgen) const;
};
