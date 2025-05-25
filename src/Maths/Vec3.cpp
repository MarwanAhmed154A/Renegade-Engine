#include "Vec3.h"

namespace RG
{
	Vec3::Vec3()
		: x(0), y(0), z(0)
	{
	}

	Vec3::Vec3(float xyz) 
		: x(xyz), y(xyz), z(xyz) {}

	Vec3::Vec3(float x, float y, float z)
		: x(x), y(y), z(z) {}

	Vec3 Vec3::operator+(Vec3 vec)
	{
		return Vec3(x + vec.x, y + vec.y, z + vec.z);
	}

	Vec3 Vec3::operator-(Vec3 vec)
	{
		return Vec3(x - vec.x, y - vec.y, z - vec.z);
	}

	Vec3 Vec3::operator*(Vec3 vec)
	{
		return Vec3(x * vec.x, y * vec.y, z * vec.z);
	}

	Vec3 Vec3::operator/(Vec3 vec)
	{
		return Vec3(x / vec.x, y / vec.y, z / vec.z);
	}

	void Vec3::operator+=(Vec3 vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	void Vec3::operator-=(Vec3 vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	void Vec3::operator*=(Vec3 vec)
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
	}

	void Vec3::operator/=(Vec3 vec)
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
	}
}