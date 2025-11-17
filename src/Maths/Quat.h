#pragma once

#include "Vec3.h"

class Quat
{
public:
	Quat() : x(0), y(0), z(0), w(0) {}
	Quat(float x, float y, float z, float w) : x(z), y(y), z(z), w(w) {}
	float x, y, z, w;

	Vec3 operator*(Vec3 vec)
	{
		Vec3 qXYZ     = Vec3(x, y, z);
		Vec3 t        = Vec3::Cross(qXYZ, vec) * 2;

		return (vec + (t * w) + Vec3::Cross(qXYZ, t));
	}

	Quat operator*(const Quat& b) const {
		return {
			w * b.w - x * b.x - y * b.y - z * b.z,         // w
			w * b.x + x * b.w + y * b.z - z * b.y,         // x
			w * b.y - x * b.z + y * b.w + z * b.x,         // y
			w * b.z + x * b.y - y * b.x + z * b.w          // z
		};
	}

	Quat Normalize()
	{
		float len = sqrt(x * x + y * y + z * z + w * w);
		if (len < 1e-6f) return Quat(0, 0, 0, 1); // avoid divide by zero

		float invLen = 1 / len;
		x *= invLen;
		y *= invLen;
		z *= invLen;
		w *= invLen;

		return Quat(x, y, z, w);
	}
};