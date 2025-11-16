#pragma once
#include "Header Tool/ReflectionManager.h"
#include "Components.h"

struct Collider : public Component
{
	REFLECTABLE_CLASS(Collider, Component)
};

struct BoxCollider : public Collider
{
	REFLECTABLE_CLASS(BoxCollider, Collider)

public:
	BoxCollider() { }

	Vec3 size;
};