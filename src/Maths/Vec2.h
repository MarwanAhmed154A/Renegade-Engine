#pragma once

namespace RG
{
	class Vec2
	{
	public:
		Vec2();
		Vec2(float x, float y);

		Vec2 operator+(Vec2 vec);
		Vec2 operator-(Vec2 vec);
		Vec2 operator*(Vec2 vec);
		Vec2 operator/(Vec2 vec);

		void operator+=(Vec2 vec);
		void operator-=(Vec2 vec);
		void operator*=(Vec2 vec);
		void operator/=(Vec2 vec);

		float x, y;
	};
}