#include "BVHNode.h"
#include <algorithm>

BVHNode::BVHNode(Scene& list, double time0, double time1, std::mt19937& rgen): BVHNode(list.objects, 0, list.objects.size(), time0, time1, rgen)
{ }

BVHNode::BVHNode(std::vector<std::shared_ptr<Surface>>& objects, size_t start, size_t end, double time0, double time1, std::mt19937& rgen)
{
	int axis = random_int(0, 2, rgen);
	auto comparator = (axis == 0) ? box_x_compare
			: (axis == 1) ? box_y_compare
				      : box_z_compare;

	size_t object_span = end - start;

	if (object_span == 1) {
		left = right = objects[start];
	} else if (object_span == 2) {
		if (comparator(objects[start], objects[start + 1])) {
			left = objects[start];
			right = objects[start + 1];
		} else {
			left = objects[start + 1];
			right = objects[start];
		}
	} else {
		std::sort(objects.begin() + start, objects.begin() + end, comparator);

		auto mid = start + object_span / 2;
		left = std::make_shared<BVHNode>(objects, start, mid, time0, time1, rgen);
		right = std::make_shared<BVHNode>(objects, mid, end, time0, time1, rgen);
	}

	AABB box_left, box_right;

	if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right)) {
		std::cerr << "No bounding box in BVHNode constructor.\n";
	}

	box = surrounding_box(box_left, box_right);
}

bool BVHNode::hit(const Ray& ray, const double t_min, const double t_max, Record& rec, std::mt19937& rgen) const
{
	if (!box.hit(ray, t_min, t_max, rec, rgen)) {
		return false;
	}

	bool hit_left = left->hit(ray, t_min, t_max, rec, rgen);
	bool hit_right = right->hit(ray, t_min, hit_left ? rec.t : t_max, rec, rgen);

	return hit_left || hit_right;
}

bool BVHNode::bounding_box(double t0, double t1, AABB& output_box) const
{
	output_box = box;
	return true;
}
