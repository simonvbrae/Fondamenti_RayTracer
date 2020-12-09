#include <iostream>
#include <thread>
#include <time.h>
#include <stdio.h>
#include "SDL.h" 
#include "SDL_image.h"
#include "float.h"
#include "vector3D.h"
#include "point3D.h"
#include "color.h"
#include "ray.h"
#include "geometry.h"
#include "cylinder.h"
#include "sphere.h"
#include "instance.h"
#include "mesh.h"
#include "raster.h"
#include "object.h"
#include "camera.h"
#include "color.h"
#include "scene.h"
#include "texture.h"

int const nx = 800;		//x
int const ny = 400;		//y
int const ns = 16;		//number of samples

using namespace std;

int init() {
	/* // Initialize SDL2. */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	/* Create the window where we will draw. */
	window = SDL_CreateWindow("Mesh Poligonale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nx, ny, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	//Initialize PNG,JPG,TIF loading
	int flags = IMG_INIT_JPG;
	int initted = IMG_Init(flags);
	if ((initted&flags) != flags) {
		cout << "SDL_image could not initialize! SDL_image Error:" << IMG_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	/* We must call SDL_CreateRenderer in order for draw calls to affect this window. */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		SDL_DestroyWindow(window);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		IMG_Quit();
		SDL_Quit();
		return 1;
	}
	return 0;
}

void close() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}


void setup_mapping_cylindrical(scene* world) {
	// X-axis points
	for (int i = -10; i <= 10; i++) {
		object* c = new sphere();
		material* m = new material();
		m->texture = new constant_texture(red);
		m->ka = lightgray;
		m->ks = lightgray;

		instance* obj_inst = (new instance(c, m))->scale(.10, .10, .10)->translate(i, 0, 0);
		world->addObject(obj_inst);
	}

	// Texture Mapping preliminare
	point3D light_position(2.0f, 10.0f, -4.0f);
	light* light_src = new point_light(light_position, black, color(0.62f, 0.54f, 0.44f), black);
	world->addLight(light_src);

	float deg = 180;
	point3D lookfrom(10 * sin(deg / 180 * M_PI), 1, 10 * cos(deg / 180 * M_PI));
	point3D lookat(0, 0, 0);
	vector3D up(0, 1, 0);
	world->setCamera(lookfrom, lookat, up, 20, nx, ny, ns);

	object* c = new cylinder();
	material* m = new material();
	m->texture = new image_texture_cylindrical("../models/barrel_texture.jpg");
	m->ka = lightgray;
	m->ks = lightgray;

	instance* cylinder_instance = (new instance(c, m))->scale(1.0, 1.0, 1.0);
	cylinder_instance->getMaterial()->reflective = 0.8;
	world->addObject(cylinder_instance);
}
void setup_mapping_sphere(scene* world) {
	// X-axis points
	for (int i = -10; i <= 10; i++) {
		object* c = new sphere();
		material* m = new material();
		m->texture = new constant_texture(red);
		m->ka = lightgray;
		m->ks = lightgray;

		instance* obj_inst = (new instance(c, m))->scale(.10, .10, .10)->translate(i, 0, 0);
		world->addObject(obj_inst);
	}

	// Light source
	point3D light_position(0.0f, 5.0f, -4.0f);
	point_light* light = new point_light(light_position, black, lightgray, black);
	world->addLight(light);

	// Camera
	float deg = 180;
	point3D lookfrom(10 * sin(deg / 180 * M_PI), 1, 10 * cos(deg / 180 * M_PI));
	point3D lookat(0, 0, 0);
	vector3D up(0, 1, 0);
	world->setCamera(lookfrom, lookat, up, 20, nx, ny, ns);

	// Background sphere
	object* sphere_model = new sphere();
	material* x = new material();
	x->texture = new constant_texture(color(2.5f, 2.5f, 2.5f));
	x->ka = lightgray;
	x->ks = lightgray;
	instance* sphere_ptr = (new instance(sphere_model, x))->scale(1000.0, 1000.0, 1000.0)->translate(0, -1001.5f, 0);
	world->addObject(sphere_ptr);

	// Textured sphere
	object* c = new sphere();
	material* m = new material();
	m->texture = new image_texture_spherical("../models/earth.jpg");
	m->ka = lightgray;
	m->ks = lightgray;

	instance* obj_inst = (new instance(c, m))->scale(1.0, 1.0, 1.0)->rotate_y(M_PI);
	obj_inst->getMaterial()->reflective = 0.8;
	world->addObject(obj_inst);
}
void setup_spotlight(scene* world) {
	// Add spotlight aiming directly down with an opening of 10 degrees
	point3D light_position(-1.0f, 2.0f, -3.0f);
	point3D light_lookat(0.5f, 0.5f, 0.5f);
	vector3D light_direction = normalize(light_lookat - light_position);
	light* light = new spotlight(light_position, light_direction, M_PI / 9, black, lightgray, black);
	world->addLight(light);

	point3D lookfrom(-3, 20, 5);
	point3D lookat(1, 0, 1);
	vector3D up(0, 1, 0);
	world->setCamera(lookfrom, lookat, up, 20, nx, ny, ns);

	// background sphere
	object* sphere_model = new sphere();
	material* x = new material();
	x->texture = new constant_texture(green);
	x->ka = red;
	x->ks = blu;
	instance* sphere_ptr = new instance(sphere_model, x);
	sphere_ptr->scale(1000.0, 1000.0, 1000.0);
	sphere_ptr->translate(0, -1001.5f, 0);
	world->addObject(sphere_ptr);

	// smaller sphere
	object* small_sphere_model = new sphere();
	material* m2 = new material();
	m2->texture = new constant_texture(red);
	m2->ka = green;
	m2->ks = blu;
	instance* small_sphere_ptr = new instance(small_sphere_model, m2);
	small_sphere_ptr->scale(.5f, .5f, .5f);
	small_sphere_ptr->translate(0, 1.0f, 0);
	world->addObject(small_sphere_ptr);

	// second smaller sphere
	material* m3 = new material();
	m3->texture = new constant_texture(red);
	m3->ka = green;
	m3->ks = blu;
	instance* small_sphere_ptr_2 = new instance(new sphere(), m3);
	small_sphere_ptr_2->scale(0.5f, 0.5f, 0.5f);
	small_sphere_ptr_2->translate(2.0f, 1.0f, 2.0f);
	world->addObject(small_sphere_ptr_2);

	std::cout << "setup_done" << std::endl;
}
void setup_multiple_lights(scene* world) {
	// Add spotlight aiming directly down with an opening of 10 degrees
	point3D light_position(-1.0f, 3.0f, -3.0f);
	point3D light_lookat(0.5f, 0.5f, 0.5f);
	vector3D light_direction = normalize(light_lookat - light_position);
	light* light = new spotlight(light_position, light_direction, M_PI / 9, darkgray, white, white);
	world->addLight(light);

	light_direction = normalize(point3D(0.5f, 0.5f, 0.5f) - point3D(-3.0f, 3.0f, -1.0f));
	world->addLight(new spotlight(point3D(-3.0f, 3.0f, -1.0f), light_direction, M_PI / 9, darkgray, white, white));

	point3D lookfrom(1, 30, 1);
	point3D lookat(2, 0, 2);
	vector3D up(0.5f, 0, 0.5f);
	world->setCamera(lookfrom, lookat, up, 20, nx, ny, ns);

	// background sphere
	object* sphere_model = new sphere();
	material* x = new material();
	x->texture = new constant_texture(green);
	x->ka = red;
	x->ks = blu;
	instance* sphere_ptr = new instance(sphere_model, x);
	sphere_ptr->scale(1000.0, 1000.0, 1000.0);
	sphere_ptr->translate(0, -1001.5f, 0);
	world->addObject(sphere_ptr);

	// smaller sphere
	object* small_sphere_model = new sphere();
	material* m2 = new material();
	m2->texture = new constant_texture(black);
	m2->ka = green;
	m2->ks = blu;
	instance* small_sphere_ptr = new instance(small_sphere_model, m2);
	small_sphere_ptr->scale(.5f, .5f, .5f);
	small_sphere_ptr->translate(0, 1.0f, 0);
	world->addObject(small_sphere_ptr);

	std::cout << "setup_done" << std::endl;
}
void setup_motion_blur(scene* world, float speed) {
	// Lighting setup
	world->addLight(new point_light(point3D(-10.0f, 10.0f, 4.0f), black, color(0.62f, 0.54f, 0.44f), black));
	point3D light_position(0.0f, 10.0f, 0.0f);
	point3D light_lookat(0.0f, 0.0f, 0.0f);
	vector3D light_direction = normalize(light_lookat - light_position);
	light* light = new spotlight(light_position, light_direction, M_PI / 9, darkgray, color(0.62f, 0.54f, 0.44f), white);
	world->addLight(light);
	light_position = point3D(-4.0f, 3.0f, 0.0f);
	world->addLight(new spotlight(light_position, normalize(point3D(0.0f, 0.0f, 0.0f) - light_position), M_PI / 9, darkgray, color(0.62f, 0.54f, 0.44f), white));

	point3D lookfrom(-4.0f, 4.0f, 0.0f);
	point3D lookat(0, 0.5f, 0);
	vector3D up(0, 1, 0);
	world->setCamera(lookfrom, lookat, up, 50, nx, ny, ns);

	// background sphere
	object* sphere_model = new sphere();
	material* x = new material();
	x->texture = new constant_texture(darkgreen);
	x->ka = darkgreen;
	x->ks = white;
	instance* sphere_ptr = new instance(sphere_model, x);
	sphere_ptr->scale(1000.0, 1000.0, 1000.0);
	sphere_ptr->translate(0, -1000, 0);
	world->addObject(sphere_ptr);

	// smaller sphere
	object* small_sphere_model = new sphere();
	material* m2 = new material();
	m2->texture = new image_texture_spherical("../models/8ball.jpg");
	m2->ka = black;
	m2->ks = white;
	instance* small_sphere_ptr = new instance(small_sphere_model, m2);
	small_sphere_ptr->scale(.5f, .5f, .5f);
	small_sphere_ptr->translate(0, .5f, 1);
	world->addObject(small_sphere_ptr);

	// small blurred sphere
	sphere* small_sphere_model_2 = new sphere();
	small_sphere_model_2->speed = speed;
	small_sphere_model_2->direction = vector3D(0,0,1);
	material* m3 = new material();
	m3->texture = new image_texture_spherical("../models/3ball.jpg");
	m3->ka = red;
	m3->ks = lightgray;
	instance* small_sphere_ptr_2 = new instance(small_sphere_model_2, m3);
	small_sphere_ptr_2->scale(.5f, .5f, .5f);
	small_sphere_ptr_2->translate(0, .5f, -1);
	world->addObject(small_sphere_ptr_2);

	std::cout << "setup_done" << std::endl; // TODO
}
void setup_animation(scene* world) {
	// Light
	// Directional
	point3D light_position(-2.0f, 0.0f, 3.0f);
	light* light_src = new point_light(light_position, black, color(0.29f, 0.35f, 0.41f), black);
	world->addLight(light_src);

	// Spot
	light_position = point3D(0.0f, 0.0f, -4.0f);
	point3D light_lookat(0.0f, 0.0f, 0.0f);
	vector3D light_direction = normalize(light_lookat - light_position);
	light* light = new spotlight(light_position, light_direction, M_PI / 12, black, color(0.62f, 0.54f, 0.44f), black);
	world->addLight(light);


	// Camera
	float deg = -90;
	point3D lookfrom(6 * cos(deg / 180 * M_PI), 0, 6 * sin(deg / 180 * M_PI));
	point3D lookat(0, 0, 0);
	vector3D up(0, 1, 0);
	world->setCamera(lookfrom, lookat, up, 20, nx, ny, ns);

	// Scene
	// Cylinders
	// Barrel 1
	object* c = new cylinder();
	material* m = new material();
	m->texture = new image_texture_cylindrical("../models/barrel_texture.jpg");
	m->ka = lightgray;
	m->ks = lightgray;
	instance* cylinder_instance = (new instance(c, m))->scale(0.2f, 0.2f, 0.2f);
	cylinder_instance->getMaterial()->reflective = 0.8;
	world->addObject(cylinder_instance);

	// Barrel 2
	m = new material();
	m->texture = new image_texture_cylindrical("../models/barrel_texture_2.jpg");
	m->ka = lightgray;
	m->ks = lightgray;
	instance* cylinder_instance_2 = (new instance(c, m))->scale(0.2f, 0.2f, 0.2f)->translate(-1.0f, 0, 0);
	cylinder_instance_2->getMaterial()->reflective = 0.8;
	world->addObject(cylinder_instance_2);

	// Barrel 3
	m = new material();
	m->texture = new image_texture_cylindrical("../models/barrel_texture_3.jpg");
	m->ka = lightgray;
	m->ks = lightgray;
	instance* cylinder_instance_3 = (new instance(c, m))->scale(0.2f, 0.2f, 0.2f)->translate(1.0f, 0, 0);
	cylinder_instance_3->getMaterial()->reflective = 0.8;
	world->addObject(cylinder_instance_3);

	// Spheres
	// Ball 1
	c = new sphere();
	m = new material();
	m->texture = new image_texture_spherical("../models/earth.jpg");
	m->ka = lightgray;
	m->ks = lightgray;
	instance* sphere_instance_1 = (new instance(c, m))->scale(0.2f, 0.2f, 0.2f)->translate(0.0f, 0.4f, 0.0f);
	sphere_instance_1->getMaterial()->reflective = 0.8;
	world->addObject(sphere_instance_1);

	// Ball 2
	c = new sphere();
	m = new material();
	m->texture = new image_texture_spherical("../models/planet.jpg");
	m->ka = lightgray;
	m->ks = lightgray;
	instance* sphere_instance_2 = (new instance(c, m))->scale(0.2f, 0.2f, 0.2f)->translate(-1.0f, 0.4f, 0.0f);
	sphere_instance_2->getMaterial()->reflective = 0.8;
	world->addObject(sphere_instance_2);

	// Ball 3
	c = new sphere();
	m = new material();
	m->texture = new image_texture_spherical("../models/planet2.jpg");
	m->ka = lightgray;
	m->ks = lightgray;
	instance* sphere_instance_3 = (new instance(c, m))->scale(0.2f, 0.2f, 0.2f)->translate(1.0f, 0.4f, 0.0f);
	sphere_instance_3->getMaterial()->reflective = 0.8;
	world->addObject(sphere_instance_3);

	// Ground
	object* sphere_model = new sphere();
	material* x = new material();
	x->texture = new constant_texture(color(2.5f, 2.5f, 2.5f));
	x->ka = lightgray;
	x->ks = lightgray;
	instance* sphere_ptr = (new instance(sphere_model, x))->scale(1000.0, 1000.0, 1000.0)->translate(0, -1000.2f, 0);
	world->addObject(sphere_ptr);

	// Sheep model
	object* model3d = new mesh("../models/85-cartoon-sheep/sheep.obj", "../models/");
	m = new material();
	m->texture = new constant_texture(lightgray);
	m->ka = lightgray;
	m->ks = lightgray;
	instance* mesh_ptr = new instance(model3d, m);
	mesh_ptr->scale(0.2f, 0.2f, 0.2f)->translate(0.0f,-0.2f,-1.0f);
	world->addObject(mesh_ptr);
}
void run_animation_1(scene* world) {
	int i = 0;
	char filename[200];
	for (int cam_angle = 270; cam_angle >= -90; cam_angle -= 10) {
		// Camera movement
		point3D lookfrom(6 * cos(cam_angle / float(180) * M_PI), 0, 6 * sin(cam_angle / float(180) * M_PI));
		std::cout << "Camera angle: " << cam_angle << std::endl;
		point3D lookat(0, 0, 0);
		vector3D up(0, 1, 0);
		world->setCamera(lookfrom, lookat, up, 20, nx, ny, ns);

		// Spotlight movement
		point3D light_position = point3D(0.0f, 0.0f, -4.0f);
		point3D light_lookat(0.5f * cos(cam_angle / float(180) * M_PI), 0.5f * sin(cam_angle / float(180) * M_PI), 0);
		vector3D light_direction = normalize(light_lookat - light_position);
		light* light = new spotlight(light_position, light_direction, M_PI / 12, black, color(0.62f, 0.54f, 0.44f), black);
		world->light_sources_vector[1] = light;

		world->render();

		sprintf_s(filename, "screenshot%d.bmp", i);
		saveScreenshotBMP(filename);
		cout << filename << " - Screenshot saved!" << endl;

		std::this_thread::sleep_for(chrono::seconds(1));

		i++;
	}
	std::cout << "Animation Completed" << std::endl;
}
void run_animation_2(scene* world) {
	int i = 0;
	char filename[200];
	for (int cam_angle = 270; cam_angle >= -90; cam_angle -= 10) {
		std::cout << "Camera angle: " << cam_angle << std::endl;

		// Spotlight movement
		point3D light_position = point3D(0.0f, 0.0f, -4.0f);
		point3D light_lookat(0.5f * cos(cam_angle / float(180) * M_PI), 0.5f * sin(cam_angle / float(180) * M_PI), 0);
		vector3D light_direction = normalize(light_lookat - light_position);
		light* light = new spotlight(light_position, light_direction, M_PI / 12, black, color(0.62f, 0.54f, 0.44f), black);
		world->light_sources_vector[1] = light;

		world->render();

		sprintf_s(filename, "screenshot%d.bmp", i);
		saveScreenshotBMP(filename);
		cout << filename << " - Screenshot saved!" << endl;

		std::this_thread::sleep_for(chrono::seconds(1));

		i++;
	}
	std::cout << "Animation Completed" << std::endl;
}
void run_animation_3(scene* world) {
	char filename[200];
	int cam_angle = -90;
	int n = 0;
	for (float i = 0; i <= 3; i+=.1) {
		// Camera movement
		point3D lookfrom(6 * cos(cam_angle / float(180) * M_PI), i, 6 * sin(cam_angle / float(180) * M_PI));
		std::cout << "Camera angle: " << cam_angle << std::endl;
		point3D lookat(0, 0, 0);
		vector3D up(0, 1, 0);
		world->setCamera(lookfrom, lookat, up, 20, nx, ny, ns);

		world->render();

		sprintf_s(filename, "screenshot%d.bmp", n);
		saveScreenshotBMP(filename);
		cout << filename << " - Screenshot saved!" << endl;

		std::this_thread::sleep_for(chrono::seconds(1));
		n++;
	}

	for (float i = 3; i >= 0; i -= .1) {
		// Camera movement
		point3D lookfrom(6 * cos(cam_angle / float(180) * M_PI), i, 6 * sin(cam_angle / float(180) * M_PI));
		std::cout << "Camera angle: " << cam_angle << std::endl;
		point3D lookat(0, 0, 0);
		vector3D up(0, 1, 0);
		world->setCamera(lookfrom, lookat, up, 20, nx, ny, ns);

		world->render();

		sprintf_s(filename, "screenshot%d.bmp", n);
		saveScreenshotBMP(filename);
		cout << filename << " - Screenshot saved!" << endl;

		std::this_thread::sleep_for(chrono::seconds(1));
		n++;
	}

	std::cout << "Animation Completed" << std::endl;
}

int main(int argc, char* argv[])
{
	if (init() == 1) {
		cout << "App Error! " << std::endl;
		return 1;
	}

	scene world;

	//setup_mapping_cylindrical(&world);
	//setup_mapping_sphere(&world);
	//setup_spotlight(&world);
	setup_motion_blur(&world, 1);

	//setup_multiple_lights(&world);

	//setup_animation(&world);
	//run_animation_1(&world);
	//run_animation_2(&world);
	//run_animation_3(&world);
	
	time_t start, end;
	time(&start);

	world.parallel_render();
	//world.render();

	time(&end);
	double dif = difftime(end, start);
	cout << "Rendering time: " << dif << " seconds." << endl;

	SDL_Event event;
	bool quit = false;

	/* Poll for events */
	int deg = 90;
	while (SDL_PollEvent(&event) || (!quit)) {

		switch (event.type) {

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;
				// cases for other keypresses

			case SDLK_s:
				saveScreenshotBMP("screenshot.bmp");
				cout << "Screenshot saved!" << endl;
				break;

			}
		}
	}

	close();
	return 0;
}
