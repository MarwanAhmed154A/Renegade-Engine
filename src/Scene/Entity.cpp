#include "Entity.h"
#include "Components.h"
#include "Header Tool/ReflectionManager.h"

REFLECT_REGISTER_TYPE(Entity)
REFLECT_PRIV_PROP(InspectableType::String, Entity, m_name)

namespace RG
{
	Entity::Entity() : m_parent(nullptr), m_name("Entity")
	{
		transform = (Transform*)(m_components.Push(new Transform));
	}

	Entity::Entity(std::string name) : m_parent(nullptr), m_name(name)
	{
		transform = (Transform*)(m_components.Push(new Transform));
	}

	void Entity::AddComponent(Component* component)
	{
		m_components.Push(component);
	}
}