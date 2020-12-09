#pragma once
#include <windows.h>
#include <ppl.h>
#include <iostream>
#include <vector>
#include <ppl.h>
#include "SDL.h"
#include "vector3D.h"
#include "point3D.h"
#include "raster.h"
#include "camera.h"
#include "light.h"
#include "color.h"
#include "shader.h"
#include "bvh.h"
#include "instance.h"

// Se si usano mesh triangolari non usare il BVH
#define use_bvh		// commenta per non usare bvh
#define use_motion_blur		// commenta per non usare motion blur

using namespace std;
using namespace concurrency;

class scene {
public:
	int nx;
	int ny;
	int ns;

	vector<instance *> objs_vector;

	camera *cam;

	vector<light *> light_sources_vector;
	//light *light_source;

	bvh_node *bvh_scene;

	scene() {}

	void addObject(instance *new_object) {
		objs_vector.push_back(new_object);
	}

	void addLight(light *new_light) {
		light_sources_vector.push_back(new_light);
	}

	void clearLights() {
		light_sources_vector.clear();
	}

	void setCamera(point3D position, point3D lookat, vector3D up, float fov, int res_x, int res_y, int n_sampling) {
		nx = res_x;
		ny = res_y;
		ns = n_sampling;
		float ratio = float(res_x) / float(res_y);
		cam = new camera(position, lookat, up, fov, ratio);
	}

	void render();
	void parallel_render();

private:

	bool trace_ray(const ray& r, float t_min, float t_max, hit_record& rec, float time) const;
	bool trace_shadow_ray(const ray& r, const float closest_light, float time) const;
	color shot(ray &r, float time);
};

color scene::shot(ray &r, float time) {
	hit_record rec;

	color result(0, 0, 0);

	if (trace_ray(r, 0.0f, FLT_MAX, rec, time))
	{
		for (light* light_src : light_sources_vector) {
			ray shadow_ray(rec.p, normalize(light_src->position - rec.p));
			float closest_light = shadow_ray.t_at_point(light_src->position);

			if (trace_shadow_ray(shadow_ray, closest_light, time) || !light_src->in_light_range(rec.p))
				result += ambient_shading(*light_src, rec);
				//return phong_shading(*light_src, rec, *cam);
			else
				result += phong_shading(*light_src, rec, *cam);
		}
		return result;
	}
	else {
		vector3D unit_direction = normalize(r.direction());
		float t = 0.5f * (unit_direction.y + 1.0f);
		return lerp(color(1.0f, 1.0f, 1.0f), color(0.5f, 0.7f, 1.0f), t);
	}
}

void scene::render() {
	float ns_inv = 1 / float(ns);
#ifdef use_bvh
#ifndef use_motion_blur
	ns_inv = 0;
	bvh_scene = new bvh_node(objs_vector, 0, objs_vector.size() - 1);
#endif
#endif

	float cur_time;
	for (int j = 0; j < ny; j++) {
		for (int i = 0; i < nx; i++) {
			color col(0.0, 0.0, 0.0);
			cur_time = 0;
			for (int s = 0; s < ns; s++) {
				ray r = cam->get_ray(float((i + randZeroToOne()) / nx), float((j + randZeroToOne()) / ny));
				col += scene::shot(r, cur_time);
				cur_time += ns_inv;
			}

			col /= float(ns);
			col = color(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			setColor(col.r, col.g, col.b);
			setPixel(i, j);
		}
	}
	SDL_RenderPresent(renderer);
}

void scene::parallel_render(){
	float ns_inv = 1 / float(ns);
#ifdef use_bvh
#ifndef use_motion_blur
	ns_inv = 0;
	bvh_scene = new bvh_node(objs_vector, 0, objs_vector.size() - 1);
#endif
#endif

	vector<color> matrix(nx*ny);
	parallel_for(int(0), ny, [&](int j) {
		float cur_time;
		for (int i = 0; i < nx; i++)
		{
			color col(0.0, 0.0, 0.0);
			cur_time = 0;
			for (int s = 0; s < ns; s++) {
				ray r = cam->get_ray(float((i + randZeroToOne()) / nx), float((j + randZeroToOne()) / ny));
				col += scene::shot(r, cur_time);
				cur_time += ns_inv;
			}

			col /= float(ns);
			col = color(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			matrix[j * nx + i] = col;
		}
	});

	for (int j = 0; j < ny; j++)
	{
		for (int i = 0; i < nx; i++)
		{
			color col = matrix[j * nx + i];
			setColor(col.r, col.g, col.b);
			setPixel(i, j);
		}
	}
	SDL_RenderPresent(renderer);
}

bool scene::trace_ray(const ray& r, float t_min, float t_max, hit_record& rec, float time) const {
#ifdef use_bvh
#ifndef use_motion_blur
	return bvh_scene->hit(r, t_min, t_max, rec, time);
#endif
#endif

	hit_record temp_rec;
	bool hit_anything = false;
	float closest_so_far = t_max;

	for (int i = 0; i < objs_vector.size(); i++) {
		if (objs_vector[i]->hit(r, t_min, closest_so_far, temp_rec, time)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
}
	}
	return hit_anything;
}

bool scene::trace_shadow_ray(const ray& r, const float closest_light, float time) const {
#ifdef use_bvh
#ifndef use_motion_blur
	hit_record temp_rec;

	if (bvh_scene->hit(r, 0.01f, closest_light, temp_rec, 0))
	//if (bvh_scene->hit_shadow(r, t_min, closest_so_far))
		return true;

	return false;
#endif
#endif

	for (int i = 0; i < objs_vector.size(); i++)
		if (objs_vector[i]->hit_shadow(r, 0.01f, closest_light, time))
			return true;

	return false;
}
