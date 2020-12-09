#pragma once
#include "color.h"
#include "light.h"
#include "math.h"
#include "record.h"
#include "texture.h"

struct hit_record;

struct material {

	color ka, kd, ks;
	float alpha;
		
	material() {
		random_material();
	}

	material(color ambient, color diffuse, color specular, float a) : ka(ambient), kd(diffuse), ks(specular), alpha(a) {};

public:
	void random_material() {
		ka = color(randZeroToOne(), randZeroToOne(), randZeroToOne());
		kd = color(randZeroToOne(), randZeroToOne(), randZeroToOne());
		ks = color(randZeroToOne(), randZeroToOne(), randZeroToOne());
		
		alpha = 20 + randZeroToOne() * 200;
	}
	
	texture* texture;
	
	float reflective = 0.0;
	float refractive = 0.0;
};



	
