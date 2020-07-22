#include "Scene.h"
#include "../geometry/AABB.h"

Scene::Scene()
{ }

Scene::Scene(std::shared_ptr<Surface> object)
{
	add(object);
}

void Scene::clear()
{
	objects.clear();
}

void Scene::add(std::shared_ptr<Surface> object)
{
	objects.push_back(object);
}

bool Scene::hit(const Ray& ray, const double t_min, const double t_max, Record& rec, std::mt19937& rgen) const
{
	Record temp;

	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects) {
		if (object->hit(ray, t_min, closest_so_far, temp, rgen)) {
			hit_anything = true;
			closest_so_far = temp.t;
			rec = temp;
		}
	}

	return hit_anything;
}

bool Scene::bounding_box(const double t0, const double t1, AABB& output_box) const
{
	if (objects.empty()) {
		return false;
	}

	AABB temp_box;
	bool first_box = true;

	for (const auto& object : objects) {
		if (!object->bounding_box(t0, t1, temp_box)) return false;
		output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
		first_box = false;
	}

	return true;
}

double Scene::pdf_value(const Point3& o, const Vec3& v, std::mt19937& rgen) const
{
	auto weight = 1.0 / objects.size();
	auto sum = 0.0;

	for (const auto& object : objects) {
		sum += weight * object->pdf_value(o, v, rgen);
	}

	return sum;
}

Vec3 Scene::random(const Vec3& o, std::mt19937& rgen) const
{
	auto int_size = static_cast<int>(objects.size());
	return objects[random_int(0, int_size - 1, rgen)]->random(o, rgen);
}
