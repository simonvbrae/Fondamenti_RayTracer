#pragma once

struct color {
	float		r, g, b;

	color() = default;

	color(float rc, float gc, float bc) { r = rc; g = gc; b = bc; }

	float& operator [](int i)
	{
		return ((&r)[i]);
	}

	const float& operator [](int i) const
	{
		return ((&r)[i]);
	}

	color& operator *=(float s)
	{
		r *= s;
		g *= s;
		b *= s;
		return (*this);
	}

	color& operator /=(float s)
	{
		s = 1.0F / s;
		r *= s;
		g *= s;
		b *= s;
		return (*this);
	}

	color& operator +=(const color c)
	{
		r += c.r;
		g += c.g;
		b += c.b;
		return (*this);
	}

	color& operator -=(const color& c)
	{
		r -= c.r;
		g -= c.g;
		b -= c.b;
		return (*this);
	}
};

inline color operator *(const color& c, float s)
{
	return (color(c.r * s, c.g * s, c.b * s));
}

inline color operator *(float s, const color& c)
{
	return (color(c.r * s, c.g * s, c.b * s));
}

inline color operator /(const color& c, float s)
{
	s = 1.0f / s;
	return (color(c.r * s, c.g * s, c.b * s));
}

inline color operator -(const color& c)
{
	return (color(-c.r, -c.g, -c.b));
}

inline float magnitude(const color& c)
{
	return (sqrt(c.r * c.r + c.g * c.g + c.b * c.b));
}

inline color normalize(const color& c)
{
	return (c / magnitude(c));
}

inline color operator +(const color& a, const color& b)
{
	return (color(a.r + b.r, a.g + b.g, a.b + b.b));
}

inline color operator -(const color& a, const color& b)
{
	return (color(a.r - b.r, a.g - b.g, a.b - b.b));
}

inline color operator *(const color& a, const color& b)
{
	return (color(a.r * b.r, a.g * b.g, a.b * b.b));
}

inline color clamp(color& c) {
	c.r = (c.r > 1.0f) ? 1.0f : ((c.r < 0.0f) ? 0.0f : c.r);
	c.g = (c.g > 1.0f) ? 1.0f : ((c.g < 0.0f) ? 0.0f : c.g);
	c.b = (c.b > 1.0f) ? 1.0f : ((c.b < 0.0f) ? 0.0f : c.b);
	return c;
}


color red			= color(1.0f, 0.0f, 0.0f);
color green			= color(0.0f, 1.0f, 0.0f);
color darkgreen		= color(0.0f, 0.4f, 0.0f);
color darkergreen	= color(0.0f, 0.2f, 0.0f);
color blu			= color(0.0f, 0.0f, 1.0f);
color gray			= color(0.5f, 0.5f, 0.5f);
color white			= color(1.0f, 1.0f, 1.0f);
color black			= color(0.0f, 0.0f, 0.0f);
color darkgray		= color(0.25f, 0.25f, 0.25f);
color lightgray		= color(0.75f, 0.75f, 0.75f);
color slategray		= color(0.4f, 0.5f, 0.56f);
color magenta		= color(1.0f, 0.0f, 1.0f);
color cyan			= color(0.0f, 1.0f, 1.0f);
color brown			= color(0.60f, 0.40f, 0.12f);
