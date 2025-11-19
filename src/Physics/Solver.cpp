#include "Solver.h"
#include "Scene/Entity.h"
#include "Events/Event.h"
#include "Colliders.h"
#include "Maths/Vec3.h"
#include "Scene/SceneManager.h"
#include "Maths/Quat.h"
#include <iostream>

RG::Vec<RG::Rigidbody*> RG::Solver::rigidbodies;

int RG::Solver::velIterations = 40;
int RG::Solver::posIterations = 10;

float RG::Solver::fixedDt = 1.0f / 200.0f;

namespace RG
{
	void Solver::Init()
	{
		EventManager::AddStaticCallback(EventType::OnRigidbodyCreated, &Solver::OnRigidbodyCreated);
	}

	void Solver::Update()
	{
		Vec3 gravity(0, -9.81, 0);

		Vec<Collision> collisions;

		Vec<Rigidbody*> rbs;

		//REMOVE THIS SHIT AFTER TEST
		for (int i = 0; i < SceneManager::GetScene()->GetAmountOfEntities(); i++)
		{
			Entity* ent = SceneManager::GetScene()->GetEntity(i);
			if (ent->GetComponent<Rigidbody>())
				rbs.Push(ent->GetComponent<Rigidbody>());
		}

		for (int i = 0; i < rbs.GetLength(); ++i)
		{
			Rigidbody* rb1 = rbs[i];

			BoxCollider* collider1 = rb1->m_parentPtr->GetComponent<BoxCollider>();
			if (!collider1) //skip if no collider attached
				continue;

			Transform* transform1 = rb1->m_parentPtr->GetComponent<Transform>();

			//intergrate velocity
			if (!rb1->b_static)
			{
				rb1->force    += gravity;
				rb1->velocity += rb1->force * fixedDt;
			}

			for (int j = i; j < rbs.GetLength(); ++j)
			{
				Rigidbody* rb2 = rbs[j];

				if (!rb2->b_static)
				{
					rb2->force += gravity;
					rb2->velocity += rb2->force * fixedDt;
				}

				BoxCollider* collider2 = rb2->m_parentPtr->GetComponent<BoxCollider>();
				if (!collider2 || rb1 == rb2)
					continue;

				Transform* transform2 = rb2->m_parentPtr->GetComponent<Transform>();

				Collision collision;
				if (DetectCollision(collider1, collider2, collision))
				{
					collisions.Push(collision);
				}
			}
		}

		IterateVelocity(collisions);
		IteratePositions(collisions);

		//integrate positions
		for (Rigidbody* rb : rbs)
		{
			if (rb->b_static)
				continue;

			Transform* t = rb->GetParent()->transform;
			//if(rb->velocity.LengthSquared() > (10.000f * 10.001f))
			t->Translate(rb->velocity * fixedDt);

			rb->force = Vec3(0);
		}

	}

	void Solver::OnRigidbodyCreated(Event* event)
	{
		Rigidbody* body = event->SafeCastTo<OnRigidbodyCreatedEvent>()->body;

		if (!body)
			return;

		rigidbodies.Push(body);
		body->index = rigidbodies.GetLength() - 1;
	}

	void Solver::RemoveRigidbody(unsigned int index)
	{
		rigidbodies.Delete(index);
	}

	bool Solver::DetectCollision(BoxCollider* collider1, BoxCollider* collider2, Collision& collisionOut)
	{
		Transform* t1 = collider1->GetParent()->GetComponent<Transform>();
		Transform* t2 = collider2->GetParent()->GetComponent<Transform>();
		if (!t1 || !t2) return false;

		// compute quaternions for each transform (from Euler XYZ in radians)
		auto quatFromEuler = [](const Vec3& e) {
			float hx = e.x * 0.5f, hy = e.y * 0.5f, hz = e.z * 0.5f;
			float sx = sin(hx), cx = cos(hx);
			float sy = sin(hy), cy = cos(hy);
			float sz = sin(hz), cz = cos(hz);

			Quat q;
			q.w = cx * cy * cz - sx * sy * sz;
			q.x = sx * cy * cz + cx * sy * sz;
			q.y = cx * sy * cz - sx * cy * sz;
			q.z = cx * cy * sz + sx * sy * cz;
			return q;
			};

		float deg2rad = 0.0174532925199432957f;

		Vec3 e = t1->Rotation * deg2rad;
		Quat q1 = quatFromEuler(e);
		q1.Normalize();

		e = t2->Rotation * deg2rad;
		Quat q2 = quatFromEuler(e);
		q2.Normalize();

		// local half extents
		Vec3 half1 = collider1->size;
		Vec3 half2 = collider2->size;

		// world corners (optional: for projection method below we can use centers+axes+extents)
		Vec3 corners1[8], corners2[8];
		int idx = 0;
		for (int xi = -1; xi <= 1; xi += 2)
			for (int yi = -1; yi <= 1; yi += 2)
				for (int zi = -1; zi <= 1; zi += 2)
				{
					Vec3 local = Vec3((float)xi, (float)yi, (float)zi) * half1;
					corners1[idx] = t1->Position + (q1 * local);

					Vec3 local2 = Vec3((float)xi, (float)yi, (float)zi) * half2;
					corners2[idx++] = t2->Position + (q2 * local2);
				}

		// get local axes (rotated basis) for both boxes
		Vec3 a[3] = { q1 * Vec3(1,0,0), q1 * Vec3(0,1,0), q1 * Vec3(0,0,1) };
		Vec3 b[3] = { q2 * Vec3(1,0,0), q2 * Vec3(0,1,0), q2 * Vec3(0,0,1) };

		// build axes array (first 6 are box axes, then 9 cross products)
		Vec3 axes[15];
		for (int i = 0; i < 3; ++i) axes[i] = a[i];
		for (int i = 0; i < 3; ++i) axes[3 + i] = b[i];
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				axes[6 + i * 3 + j] = Vec3::Cross(a[i], b[j]);

		// SAT: test all axes, keep minimum overlap
		const float EPS = 1e-8f;
		float minOverlap = FLT_MAX;
		Vec3 bestAxis(0, 0, 0);

		for (int ai = 0; ai < 15; ++ai)
		{
			Vec3 axis = axes[ai];

			// skip near-zero axes (parallel edges produce zero cross)
			float axisLen2 = Vec3::Dot(axis, axis);
			if (axisLen2 < EPS) continue;
			axis = axis / sqrt(axisLen2); // normalize

			float minA = FLT_MAX, maxA = -FLT_MAX;
			float minB = FLT_MAX, maxB = -FLT_MAX;

			for (int k = 0; k < 8; ++k)
			{
				float pA = Vec3::Dot(corners1[k], axis);
				minA = std::min(minA, pA);
				maxA = std::max(maxA, pA);

				float pB = Vec3::Dot(corners2[k], axis);
				minB = std::min(minB, pB);
				maxB = std::max(maxB, pB);
			}

			float overlap = std::min(maxA, maxB) - std::max(minA, minB);
			if (overlap <= 0.0f) // separating axis found
				return false;

			if (overlap < minOverlap)
			{
				minOverlap = overlap;
				bestAxis = axis;
			}
		}

		// Ensure normal points from box1 -> box2
		Vec3 centerDir = t2->Position - t1->Position;
		if (Vec3::Dot(centerDir, bestAxis) < 0.0f) bestAxis = bestAxis * -1;

		Vec3 faceNormal;
		Vec3 collisionPoint = CalcCollisionPoint(collider1, collider2, q1, q2, bestAxis, faceNormal);

		Rigidbody* rb1 = collider1->GetParent()->GetComponent<Rigidbody>();
		Rigidbody* rb2 = collider2->GetParent()->GetComponent<Rigidbody>();

		collisionOut = Collision(rb1, rb2, bestAxis, minOverlap, collisionPoint, faceNormal);
		return true;

		//float xDis = transform1->Position.x - transform2->Position.x;
		//float yDis = transform1->Position.y - transform2->Position.y;
		//float zDis = transform1->Position.z - transform2->Position.z;
		//
		//float overlapX = (collider1->size.x + collider2->size.x) - abs(xDis);
		//float overlapY = (collider1->size.y + collider2->size.y) - abs(yDis);
		//float overlapZ = (collider1->size.z + collider2->size.z) - abs(zDis);
		//
		//// Skip if there’s no overlap on any axis
		//if (overlapX <= 0 || overlapY <= 0 || overlapZ <= 0)
		//	return Collision(nullptr, nullptr, Vec3(0), 0);
		//
		//float penetration = 0;
		//Vec3 normal;
		//if (overlapX < overlapY && overlapX < overlapZ)
		//{
		//	penetration = overlapX;
		//	normal = (xDis > 0) ? Vec3(-1, 0, 0) : Vec3(1, 0, 0);
		//}
		//else if (overlapY < overlapZ)
		//{
		//	penetration = overlapY;
		//	normal = (yDis > 0) ? Vec3(0, -1, 0) : Vec3(0, 1, 0);
		//}
		//else
		//{
		//	penetration = overlapZ;
		//	normal = (zDis > 0) ? Vec3(0, 0, -1) : Vec3(0, 0, 1);
		//}
		//
		//
		//Entity* ent1 = collider1->GetParent();
		//Entity* ent2 = collider2->GetParent();
		//
		//Rigidbody* rb1 = ent1->GetComponent<Rigidbody>();
		//Rigidbody* rb2 = ent2->GetComponent<Rigidbody>();
		//
		//return Collision(rb1, rb2, normal, penetration);
	}

	Vec3 Solver::CalcCollisionPoint(BoxCollider* collider1, BoxCollider* collider2, Quat& q1, Quat& q2, Vec3& bestAxis, Vec3& faceNormalOut)
	{
		auto CalculatePrimaryAxisDot = [&bestAxis](Quat quat, Vec3 axis)
			{
				Vec3 worldSpaceAxis = quat * axis;
				float dot = Vec3::Dot(bestAxis, worldSpaceAxis);
				return abs(dot);
			};

		float xDot1 = CalculatePrimaryAxisDot(q1, Vec3(1, 0, 0));
		float yDot1 = CalculatePrimaryAxisDot(q1, Vec3(0, 1, 0));
		float zDot1 = CalculatePrimaryAxisDot(q1, Vec3(0, 0, 1));

		float xDot2 = CalculatePrimaryAxisDot(q2, Vec3(1, 0, 0));
		float yDot2 = CalculatePrimaryAxisDot(q2, Vec3(0, 1, 0));
		float zDot2 = CalculatePrimaryAxisDot(q2, Vec3(0, 0, 1));

		Vec3 alignment1, alignment2;
		float maxDot1, maxDot2;
		float halfExtentInAxis1, halfExtentInAxis2;
		Vec3 face1Axes, face2Axes;

		//get first collision axis
		if (xDot1 > yDot1 && xDot1 > zDot1)
		{
			alignment1 = q1 * Vec3(1, 0, 0);
			maxDot1 = xDot1;
			halfExtentInAxis1 = collider1->size.x;
			face1Axes = Vec3(0, 1, 1);
		}
		else if (yDot1 > zDot1)
		{
			alignment1 = q1 * Vec3(0, 1, 0);
			maxDot1 = yDot1;
			halfExtentInAxis1 = collider1->size.y;
			face1Axes = Vec3(1, 0, 1);
		}
		else
		{
			alignment1 = q1 * Vec3(0, 0, 1);
			maxDot1 = zDot1;
			halfExtentInAxis1 = collider1->size.z;
			face1Axes = Vec3(1, 1, 0);
		}

		//get sceond collision axis
		if (xDot2 > yDot2 && xDot2 > zDot2)
		{
			alignment2 = q2 * Vec3(1, 0, 0);
			maxDot2 = xDot2;
			halfExtentInAxis2 = collider1->size.x;
			face2Axes = Vec3(0, 1, 1);
		}
		else if (yDot2 > zDot2)
		{
			alignment2 = q2 * Vec3(0, 1, 0);
			maxDot2 = yDot2;
			halfExtentInAxis2 = collider1->size.y;
			face2Axes = Vec3(1, 0, 1);
		}
		else
		{
			alignment2 = q2 * Vec3(0, 0, 1);
			maxDot2 = zDot2;
			halfExtentInAxis2 = collider1->size.z;
			face2Axes = Vec3(1, 1, 0);
		}

		Vec3 faceNormal = maxDot1 > maxDot2 ? alignment1 : alignment2;
		Vec3 FaceCenter1 = collider1->GetParent()->transform->Position + faceNormal * halfExtentInAxis1;
		Vec3 FaceCenter2 = collider2->GetParent()->transform->Position - faceNormal * halfExtentInAxis2;

		Vec3 corners1[4], corners2[4];

		corners1[0] = ((maxDot1 > maxDot2) ? face1Axes : face2Axes) * collider1->size * Vec3(-1,  1,  1);//lazy signing makes sure that for any 2 axes there is always the right set of corners with the correct size, though order varies
		corners1[1] = ((maxDot1 > maxDot2) ? face1Axes : face2Axes) * collider1->size * Vec3( 1,  1, -1);//x y z
		corners1[2] = ((maxDot1 > maxDot2) ? face1Axes : face2Axes) * collider1->size * Vec3(-1, -1, -1);//- + +
		corners1[3] = ((maxDot1 > maxDot2) ? face1Axes : face2Axes) * collider1->size * Vec3( 1, -1,  1);//+ + -
																										 //- - -
		corners2[0] = ((maxDot1 > maxDot2) ? face1Axes : face2Axes) * collider2->size * Vec3(-1,  1,  1);//+ - +
		corners2[1] = ((maxDot1 > maxDot2) ? face1Axes : face2Axes) * collider2->size * Vec3( 1,  1, -1);//xy -+ ++ -- +-
		corners2[2] = ((maxDot1 > maxDot2) ? face1Axes : face2Axes) * collider2->size * Vec3(-1, -1, -1);//xz -+ +- -- ++
		corners2[3] = ((maxDot1 > maxDot2) ? face1Axes : face2Axes) * collider2->size * Vec3( 1, -1,  1);//yz ++ +- -- -+, all corners done

		for (int i = 0; i < 4; ++i)
		{
			corners1[i] = collider1->GetParent()->transform->Position + q1 * corners1[i];
			corners2[i] = collider2->GetParent()->transform->Position + q2 * corners2[i];
		}

		Vec<Vec3> curCorners;
		for (int i = 0; i < 4; ++i)
			curCorners.Push(corners2[i]);

		for (int i = 0; i < 4; ++i)
		{
			Vec<Vec3> tmpCorners;

			Vec3 edgeVector = corners1[(i + 1) % 4] - corners1[i];
			Vec3 planeNormal = Vec3::Cross(faceNormal, edgeVector);
			Vec3 planePoint = corners1[i];

			for (int j = 0; j < curCorners.GetLength(); ++j)
			{

				Vec3 firstPoint = curCorners[j];
				Vec3 secondPoint = curCorners[(j + 1) % curCorners.GetLength()];

				bool firstInside = Vec3::Dot(planeNormal, firstPoint - corners1[i]) <= 0;
				bool secondInside = Vec3::Dot(planeNormal, secondPoint - corners1[i]) <= 0;

				if (firstInside && secondInside)
				{
					//both inside, keep second
					tmpCorners.Push(secondPoint);
				}
				else if (firstInside && !secondInside)
				{
					float t = Vec3::Dot(planeNormal, planePoint - firstPoint) / Vec3::Dot(planeNormal, secondPoint - firstPoint);
					Vec3 intersectionPoint = firstPoint + (secondPoint - firstPoint) * t;

					tmpCorners.Push(intersectionPoint);
				}
				else if (!firstInside && secondInside)
				{
					//first outside, second inside, keep intersection point and second point
					float t = Vec3::Dot(planeNormal, planePoint - firstPoint) / Vec3::Dot(planeNormal, secondPoint - firstPoint);
					Vec3 intersectionPoint = firstPoint + (secondPoint - firstPoint) * t;

					tmpCorners.Push(intersectionPoint);
					tmpCorners.Push(secondPoint);
				}

			}
			curCorners = tmpCorners;
		}

		Vec3 collisionPoint;
		for (Vec3 corner : curCorners)
			collisionPoint += corner;

		if (curCorners.GetLength() > 0)
			collisionPoint /= curCorners.GetLength();

		faceNormalOut = faceNormal;

		return collisionPoint;
	}

	void Solver::IterateVelocity(Vec<Collision>& collisions)
	{
		for (int i = 0; i < velIterations; i++)
		{
			for (Collision& c : collisions)
			{
				Rigidbody* A = c.rb1;
				Rigidbody* B = c.rb2;
				if (!A || !B) continue;

				float invMassA = A->b_static ? 0.0f : 1.0f / A->mass;
				float invMassB = B->b_static ? 0.0f : 1.0f / B->mass;
				float totalInvMass = invMassA + invMassB;
				if (totalInvMass == 0) continue;

				Vec3 relativeVelocity = B->velocity - A->velocity;

				// add bias from penetration
				float bias = 0.2f * c.penetration / fixedDt;
				float velAlongNormal = Vec3::Dot(relativeVelocity, c.normal) - bias;

				// skip if separating too fast
				if (velAlongNormal > 0) continue;

				float restitution = 0.0f; // start with 0 for cubes
				float j = -(1 + restitution) * velAlongNormal / totalInvMass;

				Vec3 impulse = c.normal * j;

				if (!A->b_static) A->velocity -= impulse * invMassA;
				if (!B->b_static) B->velocity += impulse * invMassB;
			}
		}
	}

	void Solver::IteratePositions(Vec<Collision>& collisions)
	{
		float baumgarteFactor = 0.4f; // 20% of penetration correction per iteration

		for (int iter = 0; iter < posIterations; iter++)
		{
			for (Collision& c : collisions)
			{
				Rigidbody* A = c.rb1;
				Rigidbody* B = c.rb2;
				if (A == nullptr)
					continue;
				float invMassA = 1 / A->mass;
				float invMassB = 1 / B->mass;

				float totalInvMass = invMassA + invMassB;
				if (totalInvMass == 0) continue; // both static

				Vec3 correction = c.normal * ((c.penetration / totalInvMass) * baumgarteFactor);

				Transform* tA = A->GetParent()->transform;
				Transform* tB = B->GetParent()->transform;

				//if (!A->b_static)
					tA->Translate(correction * -invMassA);
				if (!B->b_static)
					tB->Translate(correction *  invMassB);
			}
		}
	}
}