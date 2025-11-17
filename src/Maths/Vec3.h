#pragma once

namespace RG
{
	class Vec3
	{
	public:
		Vec3();
		Vec3(float xyz);
		Vec3(float x, float y, float z);

		Vec3 operator+(Vec3 vec);
		Vec3 operator-(Vec3 vec);
		Vec3 operator*(Vec3 vec);
		Vec3 operator/(Vec3 vec);

		void operator+=(Vec3 vec);
		void operator-=(Vec3 vec);
		void operator*=(Vec3 vec);
		void operator/=(Vec3 vec);

		static float Dot(Vec3 LH, Vec3 RH)   { return LH.x * RH.x + LH.y * RH.y + LH.z * RH.z; }
		static Vec3  Cross(Vec3 LH, Vec3 RH) {
			return Vec3(
				LH.y * RH.z - LH.z * RH.y,
				LH.z * RH.x - LH.x * RH.z,
				LH.x * RH.y - LH.y * RH.x); 
		}
		float x, y, z;
	};
}