#ifndef _MATH_H_
#define _MATH_H_

template<typename T, typename TX>
T min(const T &a, const TX &b)
{	return (a<b) ? a : b;	}

template<typename T, typename TX>
T max(const T &a, const TX &b)
{	return (a>b) ? a : b;	}

class Math
{
public:
	static float Round(const float &f);
	static float Floor(const float &f);
	static float Sin(const float &f);
};

class vec2
{
public:
	vec2();
	vec2(const float &x, const float &y);
	vec2(const vec2 &other);
	float x;
	float y;
	vec2 operator +=(const vec2 &other);
	vec2 operator -=(const vec2 &other);
	vec2 operator *=(const float &scale);
	vec2 operator =(const vec2 &other);
	float Length() const;
};

vec2 operator +(const vec2 &a, const vec2 &b);
vec2 operator -(const vec2 &a, const vec2 &b);
vec2 operator *(const vec2 &a, const float &b);
vec2 operator *(const float &a, const vec2 &b);
float operator *(const vec2 &a, const vec2 &b);
float operator %(const vec2 &a, const vec2 &b);

class Rect
{
public:
	float x;
	float y;
	float w;
	float h;
	Rect();
	Rect(const float &x, const float &y, const float &w, const float &h);
	Rect(const Rect &r);
	Rect operator =(const Rect &r);
};

#endif
