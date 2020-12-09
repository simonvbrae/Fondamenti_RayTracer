#pragma once
#include <cmath>
#include "SDL.h"
#include "SDL_image.h"
#include "point3D.h"
#include "raster.h"

using namespace std;

class texture {
public:
	virtual color value(float u, float v, const point3D& p) const = 0;
};

class constant_texture : public texture {
public:
	constant_texture() { }

	constant_texture(color c) : color(c) { }

	virtual color value(float u, float v, const point3D& p) const {
		return color;
	}

	color color;
};

class checker_texture : public texture {
public:
	checker_texture() { }

	checker_texture(texture *t0, texture *t1) : even(t0), odd(t1) { }

	virtual color value(float u, float v, const point3D& p) const {
		float sines = sin(2 * p.x)*sin(2 * p.y)*sin(2 * p.z);
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}

	texture *odd;
	texture *even;
};


class image_texture : public texture {
public:
	image_texture(const char* filename) {
		surface = loadTexture(filename, imageWidth, imageHeight);
	}

	virtual color value(float u, float v, const point3D& p) const;

	SDL_Surface* surface;
	int imageWidth, imageHeight;
};

color image_texture::value(float u, float v, const point3D& p) const {
	int i = (u) * float(imageWidth);
	int j = (1.0f - v) * imageHeight - 0.001f;
	// Clamp
	if (i < 0) i = 0;
	if (j < 0) j = 0;
	if (i > imageWidth - 1) i = imageWidth - 1;
	if (j > imageHeight - 1) j = imageHeight - 1;

	Uint32 value = getpixel(surface, i, j);

	float red = float((value >> 16) & 0xff) / 255.0f;
	float green = float((value >> 8) & 0xff) / 255.0f;
	float blue = float(value & 0xff) / 255.0f;

	return color(blue, green, red);
}


class image_texture_cylindrical : public texture {
public:
	image_texture_cylindrical(const char* filename) {
		surface = loadTexture(filename, imageWidth, imageHeight);
	}

	virtual color value(float u, float v, const point3D& p) const;

	SDL_Surface* surface;
	int imageWidth, imageHeight;
};


color image_texture_cylindrical::value(float u, float v, const point3D& p) const {
	int i = (u) * float(imageWidth);
	int j = (1.0f - v) * imageHeight - 0.001f;

	// Clamp
	if (i < 0) i = 0;
	if (j < 0) j = 0;
	if (i > imageWidth - 1) i = imageWidth - 1;
	if (j > imageHeight - 1) j = imageHeight - 1;

	Uint32 value = getpixel(surface, i, j);

	float red = float((value >> 16) & 0xff) / 255.0f;
	float green = float((value >> 8) & 0xff) / 255.0f;
	float blue = float(value & 0xff) / 255.0f;

	return color(blue, green, red);
}

class image_texture_spherical: public texture {
public:
	image_texture_spherical(const char* filename) {
		surface = loadTexture(filename, imageWidth, imageHeight);
	}

	virtual color value(float u, float v, const point3D& p) const;

	SDL_Surface* surface;
	int imageWidth, imageHeight;
};

color image_texture_spherical::value(float u, float v, const point3D& p) const {
	int i = (int)((imageWidth - 1) * u);   	// column is across
	int j = (int)((imageHeight - 1) * v);    	// row is up

	//int i = (u_new) * float(imageWidth);
	//int j = (1.0f - v_new) * imageHeight - 0.001f;

	// Clamp
	if (i < 0) i = 0;
	if (j < 0) j = 0;
	if (i > imageWidth - 1) i = imageWidth - 1;
	if (j > imageHeight - 1) j = imageHeight - 1;

	Uint32 value = getpixel(surface, i, j);

	float red = float((value >> 16) & 0xff) / 255.0f;
	float green = float((value >> 8) & 0xff) / 255.0f;
	float blue = float(value & 0xff) / 255.0f;

	return color(blue, green, red);
}
