#pragma once

#include "Components.h"
#include "Header Tool/ReflectionManager.h"

namespace RG
{
	class Rigidbody : public Component
	{
		REFLECTABLE_CLASS(Rigidbody, Component)
	public:
		//virtual void Tick() override;
		 Rigidbody();
		~Rigidbody();

		Vec3 velocity, force;
		float mass;
		bool b_static;
	private:
		unsigned int index;
		friend class Solver;
	};
}