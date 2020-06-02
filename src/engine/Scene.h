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
	virtual bool hit(const Ray& ray, const double t_min, const double t_max, Record& rec) const;
};
