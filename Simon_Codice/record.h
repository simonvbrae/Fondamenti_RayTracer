#pragma once
#include "vector3D.h"
#include "point3D.h"
#include "material.h"

class material;

struct hit_record
{
	float u;
	float v;
	//point2D texcoord;  prossimamente al posto di u e v
	float t;
	point3D p;
	vector3D normal;
	material *m;
};
