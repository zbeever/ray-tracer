#pragma once

#include "../engine/Surface.h"

class Rotate : public Surface {
public:
	std::shared_ptr<Surface> ptr;
        double sin_theta;
        double cos_theta;
        bool hasbox;
        AABB bbox;
	
        Rotate(std::shared_ptr<Surface> p, double angle);

        virtual bool hit(const Ray& r, double t_min, double t_max, Record& rec, std::mt19937& rgen) const;
        virtual bool bounding_box(double t0, double t1, AABB& output_box) const;
	static std::shared_ptr<Rotate> applyTo(std::shared_ptr<Surface> p, double angle);
};

