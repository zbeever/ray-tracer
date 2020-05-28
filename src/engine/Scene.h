#pragma once

#include "../geometry/Surface.h"

#include <memory>
#include <vector>

class Scene: public Surface
{
public:
	std::vector<std::shared_ptr<Surface>> objects;

	Scene()
	{ }

	Scene(std::shared_ptr<Surface> object)
	{
		add(object);
	}

	void clear()
	{
		objects.clear();
	}

	void add(std::shared_ptr<Surface> object)
	{
		objects.push_back(object);
	}

	virtual bool hit(const Ray& ray, const double t_min, const double t_max, Record& rec) const;
};

bool Scene::hit(const Ray& ray, const double t_min, const double t_max, Record& rec) const
{
	Record temp;

	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects)
	{
		if (object->hit(ray, t_min, closest_so_far, temp))
		{
			hit_anything = true;
			closest_so_far = temp.t;
			rec = temp;
		}
	}

	return hit_anything;
}
