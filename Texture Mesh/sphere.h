#pragma once
#include "object.h"
#include "math.h"
#include "vector3D.h"
#include "point3D.h"

class sphere : public object {
public:
	float speed;
	vector3D direction;

	sphere() {
		center = point3D(0.0, 0.0, 0.0);
		radius = 1.0f;
		speed = 1;
		direction = vector3D(0, 0, 0);
	}
	sphere(point3D cen, float r) : center(cen), radius(r) {
		speed = 1;
		direction = vector3D(0, 0, 0);
	};


	virtual bool hit(const ray& r, float tmin, float tmax, hit_record &rec, float time) const;
	virtual bool hit_shadow(const ray& r, float t_min, float t_max, float time) const;

	// Motion blur options
	point3D get_position(float time) const;
	
	void u_v_calculate(hit_record& rec, float time) const;

	virtual bool bounding_box(aabb& box) const;

protected:
	point3D center;
	float radius;
};

bool sphere::bounding_box(aabb& box) const {
	box = aabb(center - vector3D(radius, radius, radius), center + vector3D(radius, radius, radius));
	return true;
}

bool sphere::hit(const ray& ray, float t_min, float t_max, hit_record& rec, float time) const {
	point3D position_at_time = get_position(time);
	vector3D oc = ray.origin() - position_at_time;
	float a = dot(ray.direction(), ray.direction());
	float b = dot(oc, ray.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = ray.point_at_parameter(rec.t);
			rec.normal = (rec.p - position_at_time) / radius;
			u_v_calculate(rec, time);
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = ray.point_at_parameter(rec.t);
			rec.normal = (rec.p - position_at_time) / radius;
			u_v_calculate(rec, time);
			return true;
		}
	}
	return false;
}

point3D sphere::get_position(float time) const {
	float displacement = log(1-time);
	return center + direction * speed * displacement;
}

void sphere::u_v_calculate(hit_record& rec, float time) const {
	vector3D sphere_center = rec.p - get_position(time);

	float theta = acos(sphere_center.y / radius);
	float phi = atan2(sphere_center.x, sphere_center.z);
	if (phi < 0.0)
		phi += M_PI * 2;

	// next, map theta and phi to (u, v) in [0, 1] X [0, 1]
	rec.u = phi * (1 / (2 * M_PI));
	rec.v = theta * (1 / M_PI);
}

bool sphere::hit_shadow(const ray& ray, float t_min, float t_max, float time) const {
	point3D position_at_time = get_position(time);
	vector3D oc = ray.origin() - position_at_time;
	float a = dot(ray.direction(), ray.direction());
	float b = dot(oc, ray.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			return true;
		}
	}
	return false;
}
