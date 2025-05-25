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

		float x, y, z;
	};
}