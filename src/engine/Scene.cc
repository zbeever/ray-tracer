#include "Scene.h"

Scene::Scene() { }

Scene::Scene(std::shared_ptr<Surface> object) {
	add(object);
}

void Scene::clear() {
	objects.clear();
}

void Scene::add(std::shared_ptr<Surface> object) {
	objects.push_back(object);
}

bool Scene::hit(const Ray& ray, const double t_min, const double t_max, Record& rec) const {
	Record temp;

	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects) {
		if (object->hit(ray, t_min, closest_so_far, temp)) {
			hit_anything = true;
			closest_so_far = temp.t;
			rec = temp;
		}
	}

	return hit_anything;
}
