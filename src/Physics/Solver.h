#pragma once

#include "Rigidbody.h"
#include "Colliders.h"
#include "Maths/Quat.h"
namespace RG
{
	struct Collision
	{
		float penetration;
		Vec3 normal, collisionPoint, faceNormal;
		Rigidbody *rb1, *rb2;

		Collision() : rb1(nullptr), rb2(nullptr),
			normal(Vec3(0)), penetration(0) {}

		Collision(Rigidbody * rb1, Rigidbody * rb2, Vec3 normal, float penetration, Vec3 collisionPoint, Vec3 faceNormal) : rb1(rb1), rb2(rb2),
			normal(normal), penetration(penetration), collisionPoint(collisionPoint), faceNormal(faceNormal)  {}
	};

	class Solver
	{
	public:
		static void Init();
		static void Update();
		void t(class Event* e) {}

		static void  SetVelocityIterations(int newVelIterations) { velIterations = newVelIterations; }
		static int   GetVelocityIterations()                     { return velIterations; }
		static void  SetPositionIteratios(int newPosIterations)  { posIterations = newPosIterations; }
		static int   GetPositionIteratios()                      { return posIterations; }

		static void  SetFixedDeltaTime(float newFixedDt)         { fixedDt = newFixedDt; }
		static float GetFixedDeltaTime()                         { return fixedDt; }
	private:
		static void OnRigidbodyCreated(class Event* e);
		static void RemoveRigidbody(unsigned int index);
		static void IterateVelocity(Vec<Collision>&);
		static void IteratePositions(Vec<Collision>&);
		static bool DetectCollision(BoxCollider* collider1, BoxCollider* collider2, Collision& collisionOut);
		static Vec3 CalcCollisionPoint(BoxCollider* collider1, BoxCollider* collider2, Quat& q1, Quat& q2, Vec3& bestAxis, Vec3& faceNormalOut);
		static  Vec<Rigidbody*> rigidbodies;

		friend class Rigidbody; 
		friend class EngineInterface;

		static int   velIterations, posIterations;
		static float fixedDt;
	};
}