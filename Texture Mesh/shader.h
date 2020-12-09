#pragma once
#include <iostream>
#include <time.h>
#include <algorithm>
#include "camera.h"
#include "light.h"
#include "material.h"
#include "object.h"
#include "color.h"
#include "point3D.h"
#include "vector3D.h"
#include "math.h"
#include "record.h"

color phong_shading(light &light_src, hit_record &hr, camera &cam) {
	color ambient(0.0f, 0.0f, 0.0f);
	color diffuse(0.0f, 0.0f, 0.0f);
	color specular(0.0f, 0.0f, 0.0f);

	ambient = hr.m->ka * light_src.ambient; //ka * La

	vector3D L = normalize(light_src.position - hr.p);
	float LDotN = max(dot(L, hr.normal), 0.0f);

	if (LDotN > 0) {
		diffuse = hr.m->texture->value(hr.u, hr.v, hr.p) * light_src.diffuse * LDotN; // kd*Ld*max(l.n, 0)
		vector3D V = normalize(cam.origin - hr.p);
		vector3D R = reflect(L, hr.normal);
		float VDotR = std::pow(max(dot(V, R), 0.0f), hr.m->alpha); // max(r.v,0)**a

		specular = hr.m->ks * light_src.specular * VDotR; // ks*ls*max(r.v,0)**a

		return ambient + diffuse + specular;
	}
	else
		return ambient;
}

color ambient_shading(light &light_src, hit_record &hr) {
	color ambient(0.0f, 0.0f, 0.0f);
	
	ambient = hr.m->ka * light_src.ambient;

	return ambient;
}
