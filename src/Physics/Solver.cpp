#include "Solver.h"
#include "Scene/Entity.h"
#include "Events/Event.h"
#include "Colliders.h"
#include "Maths/Vec3.h"
#include "Scene/SceneManager.h"
#include "Maths/Quat.h"

RG::Vec<RG::Rigidbody*> RG::Solver::rigidbodies;

int RG::Solver::velIterations = 20;
int RG::Solver::posIterations = 3;

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
		for(int i = 0; i < SceneManager::GetScene()->GetAmountOfEntities(); i++)
		{
			Entity* ent = SceneManager::GetScene()->GetEntity(i);
			if (ent->GetComponent<Rigidbody>())
				rbs.Push(ent->GetComponent<Rigidbody>());
		}

		for (Rigidbody* rb1 : rbs)
		{
			if (rb1 != rbs[0])
				break;
			BoxCollider* collider1 = rb1->m_parentPtr->GetComponent<BoxCollider>();
			if (!collider1) //skip if no collider attached
				continue;

			Transform* transform1 = rb1->m_parentPtr->GetComponent<Transform>();

			if (!rb1->b_static)
			{
				//rb1->force  += gravity * fixedDt;
				rb1->velocity += Vec3(0, -9.8, 0) * fixedDt;
			}

			for (Rigidbody* rb2 : rbs)
			{
				BoxCollider* collider2 = rb2->m_parentPtr->GetComponent<BoxCollider>();
				if (!collider2 || rb1 == rb2)
					continue;

				Transform* transform2 = rb2->m_parentPtr->GetComponent<Transform>();

				collisions.Push(DetectCollision(collider1, collider2));
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

	Collision Solver::DetectCollision(BoxCollider* collider1, BoxCollider* collider2)
	{
		Transform* transform1 = collider1->GetParent()->GetComponent<Transform>();
		Transform* transform2 = collider2->GetParent()->GetComponent<Transform>();


		float xDis = transform1->Position.x - transform2->Position.x;
		float yDis = transform1->Position.y - transform2->Position.y;
		float zDis = transform1->Position.z - transform2->Position.z;

		float overlapX = (collider1->size.x + collider2->size.x) - abs(xDis);
		float overlapY = (collider1->size.y + collider2->size.y) - abs(yDis);
		float overlapZ = (collider1->size.z + collider2->size.z) - abs(zDis);

		// Skip if there’s no overlap on any axis
		if (overlapX <= 0 || overlapY <= 0 || overlapZ <= 0)
			return Collision(nullptr, nullptr, Vec3(0), 0);

		float penetration = 0;
		Vec3 normal;
		if (overlapX < overlapY && overlapX < overlapZ)
		{
			penetration = overlapX;
			normal = (xDis > 0) ? Vec3(-1, 0, 0) : Vec3(1, 0, 0);
		}
		else if (overlapY < overlapZ)
		{
			penetration = overlapY;
			normal = (yDis > 0) ? Vec3(0, -1, 0) : Vec3(0, 1, 0);
		}
		else
		{
			penetration = overlapZ;
			normal = (zDis > 0) ? Vec3(0, 0, -1) : Vec3(0, 0, 1);
		}
	

		Entity* ent1 = collider1->GetParent();
		Entity* ent2 = collider2->GetParent();

		Rigidbody* rb1 = ent1->GetComponent<Rigidbody>();
		Rigidbody* rb2 = ent2->GetComponent<Rigidbody>();

		return Collision(rb1, rb2, normal, penetration);
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
				float velAlongNormal = Vec3::Dot(relativeVelocity, c.normal) + bias;

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
		float baumgarteFactor = 0.2f; // 20% of penetration correction per iteration

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
				RG_CORE_INFO(correction.y);
				Transform* tA = A->GetParent()->transform;
				Transform* tB = B->GetParent()->transform;

				if (!A->b_static)
					tA->Translate(correction * -invMassA);
				if (!B->b_static)
					tB->Translate(correction *  invMassB);
			}
		}
	}
}