#include "Box.h"
#include "FlipNormals.h"

Box::Box()
{ }

Box::Box(const Point3& p0, const Point3& p1, std::shared_ptr<Material> ptr)
{
	box_min = p0;
	box_max = p1;

	sides.add(			    std::make_shared<AARect>('z', p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
	sides.add(std::make_shared<FlipNormals>(std::make_shared<AARect>('z', p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr)));

	sides.add(			    std::make_shared<AARect>('y', p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
	sides.add(std::make_shared<FlipNormals>(std::make_shared<AARect>('y', p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr)));

	sides.add(			    std::make_shared<AARect>('x', p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
	sides.add(std::make_shared<FlipNormals>(std::make_shared<AARect>('x', p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr)));
}

bool Box::hit(const Ray& r, double t0, double t1, Record& rec) const
{
	return sides.hit(r, t0, t1, rec);	
}

bool Box::bounding_box(double t0, double t1, AABB& output_box) const
{
	output_box = AABB(box_min, box_max);
	return true;
}
