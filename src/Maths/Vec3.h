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

		static float Dot(Vec3 LH, Vec3 RH) { return LH.x * RH.x + LH.y * RH.y + LH.z * RH.z; }

		float x, y, z;
	};
}