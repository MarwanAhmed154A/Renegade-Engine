#include "Rigidbody.h"
#include "Solver.h"
#include "Events/Event.h"

REFLECT_REGISTER_TYPE(Rigidbody)
REFLECT_PROP(InspectableType::Float, Rigidbody, mass)
REFLECT_PROP(InspectableType::Bool,  Rigidbody, b_static)

namespace RG
{
	Rigidbody::Rigidbody() : mass(1), b_static(false)
	{
		mass = 1;
		static int first = 0;
		if (first < 2)
		{
			first++;
			return;
		}
		OnRigidbodyCreatedEvent event;
		event.body = this;
		EventManager::Invoke(&event);
	}

	Rigidbody::~Rigidbody()
	{
		Solver::RemoveRigidbody(index);
	}
}