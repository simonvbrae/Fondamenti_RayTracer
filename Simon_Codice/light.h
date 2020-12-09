#pragma once
#include "vector3D.h"
#include "color.h"

class light {
public:
	virtual bool in_light_range(point3D p) { return true; };

	point3D position;

	color ambient;
	color diffuse;
	color specular;

	float att_a;
	float att_b;
	float att_c;
};

class point_light : public light {
public:
	point_light() {}
	point_light(point3D p, color a, color d, color s) {
		position = p;
		ambient = a;
		diffuse = d;
		specular = s;
	}
};


class spotlight : public light {
public:
	bool in_light_range(point3D p);
	vector3D direction;
	float angle;
	float cos_angle;

	spotlight() {}
	spotlight(point3D p, vector3D v, float ag, color a, color d, color s) {
		position = p;
		direction = v;
		angle = ag;
		cos_angle = cos(angle);
		ambient = a;
		diffuse = d;
		specular = s;
	}
};

bool spotlight::in_light_range(point3D p) {
	vector3D ray = normalize(p-position);
	float cos_a = dot(direction, ray);

	return cos_a >= cos_angle;
}
