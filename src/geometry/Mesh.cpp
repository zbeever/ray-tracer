#include "Mesh.h"
#include "Triangle.h"

Mesh::Mesh(const std::string& model_name)
{
	std::ifstream object(model_name);	
	if (object.is_open())
	{
		std::string line;
		while (std::getline(object, line))
		{
			if (line.compare(std::string("v")) > 0)
			{
				std::stringstream ss;
				ss << line.substr(1, line.size());

				float p0, p1, p2;
				ss >> p0 >> p1 >> p2;

				vertex_array.push_back(Point3(p0, p1, p2));
			}
			else if (line.compare(std::string("f")) > 0)
			{
				std::stringstream ss;
				ss << line.substr(1, line.size());

				int i1, i2, i3;
				ss >> i1 >> i2 >> i3;

				index_array.push_back(i1 - 1);
				index_array.push_back(i2 - 1);
				index_array.push_back(i3 - 1);
			}
		}
		object.close();
	}
}

void Mesh::shift(const Vec3& offset)
{
	for (auto& p: vertex_array)
	{
		p += offset;
	}
	return;
}

void Mesh::scale(const float k, const Point3& o)
{
	for (Point3& p: vertex_array)
	{
		p = k*(p - o) + o;
	}
	return;
}

bool Mesh::hit(const Ray& r, HitRecord& hit_record, const float s_min, const float s_max) const
{
	hit_record.s = s_max;

	bool hit = false;

	for (int i = 0; i < index_array.size() / 3; ++i)
	{
		std::array<Point3, 3> vertex;
		vertex[0] = vertex_array[index_array[3*i]];
		vertex[1] = vertex_array[index_array[3*i + 1]];
		vertex[2] = vertex_array[index_array[3*i + 2]];

		if (triIntersect(r, hit_record, s_min, hit_record.s, vertex) == true)
		{
			hit = true;
		}
	}

	return hit;
}
