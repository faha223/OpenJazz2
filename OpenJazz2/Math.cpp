#include "Math.h"
#include <cmath>

float Math::Round(const float &f)
{
	float temp = f;
	temp -= std::floor(f);
	if (temp >= 0.5f)
		return (std::floor(f) + 1);
	return std::floor(f);
}

float Math::Floor(const float &f)
{
	return std::floor(f);
}

float Math::Sin(const float &f)
{
	return std::sin(f);
}

vec2::vec2() : x(0), y(0)
{ }
vec2::vec2(const float &x, const float &y) : x(x), y(y)
{ }
vec2::vec2(const vec2 &other) : x(other.x), y(other.y)
{ }

vec2 vec2::operator +=(const vec2 &other)
{
	x += other.x;
	y += other.y;
	return *this;
}

vec2 vec2::operator -=(const vec2 &other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

vec2 vec2::operator *=(const float &scale)
{
	x *= scale;
	y *= scale;
	return *this;
}

vec2 vec2::operator =(const vec2 &other)
{
	x = other.x;
	y = other.y;
	return *this;
}

float vec2::Length() const
{
	return sqrt((x*x) + (y*y));
}

vec2 operator +(const vec2 &a, const vec2 &b)
{
	return vec2(a.x + b.x, a.y + b.y);
}

vec2 operator -(const vec2 &a, const vec2 &b)
{
	return vec2(a.x - b.x, a.y - b.y);
}

vec2 operator *(const vec2 &a, const float &b)
{
	return vec2(a.x*b, a.y*b);
}

vec2 operator *(const float &a, const vec2 &b)
{
	return vec2(a * b.x, a * b.y);
}

float operator *(const vec2 &a, const vec2 &b)
{
	return (a.x * b.x) + (a.y * b.y);
}

float operator %(const vec2 &a, const vec2 &b)
{
	return (a.x * b.y) - (a.y * b.x);
}

Rect::Rect() : x(0), y(0), w(0), h(0) { }
Rect::Rect(const float &x, const float &y, const float &w, const float &h) : x(x), y(y), w(w), h(h) { }
Rect::Rect(const Rect &r) : x(r.x), y(r.y), w(r.w), h(r.h) { }
Rect Rect::operator =(const Rect &r)
{
	x = r.x;
	y = r.y;
	w = r.w;
	h = r.h;
	return *this;
}

mat4::mat4()
{
	for (auto i = 0; i < 4; i++)
		for(auto j = 0; j < 4; j++)
		{
			data[4 * i + j] = (i == j) ? 1 : 0;
		}
}

mat4::~mat4()
{

}

mat4 mat4::translate(const float &x, const float &y, const float &z)
{
	mat4 m;
	m.data[12] = x;
	m.data[13] = y;
	m.data[14] = z;
	return m;
}

mat4 mat4::ortho(const float &left, const float &right, const float &bottom, const float &top, const float &near, const float &far)
{
	float width = right - left;
	float height = bottom - top;
	mat4 m;
	m.data[0] = 2.0f / (right - left);
	m.data[12] = -(right + left) / (right - left);
	m.data[5] = 2.0f / (top - bottom);
	m.data[13] = -(top + bottom) / (top - bottom);
	m.data[10] = -2.0f / (far - near);
	m.data[14] = -(far + near) / (far - near);
	return m;
}

void mat4::getData(float *buffer) const
{
	for (auto i = 0; i < 16; i++)
		buffer[i] = data[i];
}