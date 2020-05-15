#pragma once

#include "Surface.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class TriangleList: public Surface
{
public:
	std::vector<Point3> vertex_array;
	std::vector<int> index_array;

	TriangleList(const std::string& model_name)
	{
		std::ifstream object(model_name);	
		if (object.is_open()) {
			std::string line;
			while (std::getline(object, line)) {
				if (line.compare(std::string("v")) > 0) {
					std::stringstream ss;
					ss << line.substr(1, line.size());

					float p0, p1, p2;
					ss >> p0 >> p1 >> p2;

					vertex_array.push_back(Point3(p0, p1, p2));
				} else if (line.compare(std::string("f")) > 0) {
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

	void shift(const Vec3& offset)
	{
		for (auto& p: vertex_array) {
			p += offset;
		}

		return;
	}

	void scale(const float k)
	{
		for (auto& p: vertex_array) {
			p *= k;
		}

		return;
	}

	bool getIntersection(const Ray& r, PointRecord& hit_record) const
	{
		const float eps = 1e-6;

		float new_t = 1e6;
		Point3 new_pos(0, 0, 0);
		Vec3 new_normal(0, 0, 0);
		bool hit = false;

		for (int i = 0; i < index_array.size() / 3; ++i) {
			// std::cout << index_array[i] << "\n";
			// std::cout << index_array[3*i] << " " << index_array[3*i + 1] << " " << index_array[3*i + 2] << "\n";
			const Vec3 e_1 = vertex_array[index_array[3*i + 1]] - vertex_array[index_array[3*i]];
			const Vec3 e_2 = vertex_array[index_array[3*i + 2]] - vertex_array[index_array[3*i]];

			const Vec3 n = e_1.cross(e_2) / e_1.cross(e_2).length();

			const Vec3 q = r.dir.cross(e_2);
			const float a = dot(e_1, q);

			if ((dot(n, r.dir) >= 0) || (abs(a) <= eps)) continue;

			const Vec3 s = (r.origin - vertex_array[index_array[3*i]]) / a;
			const Vec3 ro = s.cross(e_1);

			float b[3];
			b[0] = dot(s, q);
			b[1] = dot(ro, r.dir);
			b[2] = 1.0f - b[0] - b[1];

			if ((b[0] < 0.0f) || (b[1] < 0.0f) || (b[2] < 0.0f)) continue;

			float t = dot(e_2, ro);

			if (t >= 0.0f && t < new_t) {
				new_t = t;
				new_pos = r.pointAt(t);
				new_normal = n;
				hit = true;
			}
		}
		
		if (hit == true) {
			hit_record.pos = new_pos;
			hit_record.normal = new_normal;
		}

		return hit;
	}
};
