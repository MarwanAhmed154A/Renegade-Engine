#include "Vec2.h"

namespace RG
{
	Vec2::Vec2()
		: x(0), y(0)
	{
	}

	Vec2::Vec2(float x, float y)
		: x(x), y(y) {}

	Vec2 Vec2::operator+(Vec2 vec)
	{
		return Vec2(x + vec.x, y + vec.y);
	}

	Vec2 Vec2::operator-(Vec2 vec)
	{
		return Vec2(x - vec.x, y - vec.y);
	}

	Vec2 Vec2::operator*(Vec2 vec)
	{
		return Vec2(x * vec.x, y * vec.y);
	}

	Vec2 Vec2::operator/(Vec2 vec)
	{
		return Vec2(x / vec.x, y / vec.y);
	}

	void Vec2::operator+=(Vec2 vec)
	{
		x += vec.x;
		y += vec.y;
	}

	void Vec2::operator-=(Vec2 vec)
	{
		x -= vec.x;
		y -= vec.y;
	}

	void Vec2::operator*=(Vec2 vec)
	{
		x *= vec.x;
		y *= vec.y;
	}

	void Vec2::operator/=(Vec2 vec)
	{
		x /= vec.x;
		y /= vec.y;
	}
}