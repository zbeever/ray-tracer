#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>

#include "geometry/Vec3.h"
#include "geometry/Ray.h"
#include "geometry/Sphere.h"
#include "geometry/PointRecord.h"
#include "geometry/Plane.h"

#include "scene/Camera.h"

int main()
{
	int width = 400;
	int height = 400;
	float v_fov = 60.0f;

	Camera cam = Camera(width, height, v_fov);
	Sphere ball = Sphere(Point3(0, 0, -3), 1);
	Plane sheet = Plane(Point3(-2, 0, 0), Vec3(1, 0, 0));

	std::ofstream myfile;
	myfile.open("example.ppm");
	
	myfile << "P3\n" << width << " " << height << "\n255\n";

	for (int j = height - 1; j >= 0; --j) {
		for (int i = 0; i < width; ++i) {
			Ray beam = cam.getRay(i, j);
			PointRecord hit_record(Point3(0, 0, 0), Vec3(0, 0, 0));

			if (sheet.getIntersection(beam, hit_record))
				myfile << 255 << " " << 255 << " " << 255 << "\n";
			else
				myfile << 0 << " " << 0 << " " << 0 << "\n";
		}
	}
	
	myfile.close();

	return 0;
}
