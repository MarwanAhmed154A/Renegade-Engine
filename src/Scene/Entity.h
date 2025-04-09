#pragma once

#include "Scene.h"
#include "Components.h"

#include <string>

namespace RG
{
	class Entity
	{
	public:
		Entity();
		Entity(std::string name);

		virtual void Start() {}
		virtual void Tick() {}

		virtual void OnCollisionEnter(Entity* other) {}
		virtual void OnCollisionExit(Entity* other) {}


		void AddComponent(Component* component);

		int GetAmountOfComponents()
		{
			return m_components.GetLength();
		}

		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			m_components.Push(new T(std::forward<Args>(args)...));
			m_components[m_components.GetLength() - 1]->m_parentPtr = this;
		}

		Component* GetComponent(int index) { return m_components[index]; }

		template<typename T>
		T* GetComponent()
		{
			for (int i = 0; i < m_components.GetLength(); i++)
				if(typeid(T) == typeid(*m_components[i]))
					return (T*)m_components[i];
			return nullptr;
		}

		void DeleteComponent(int i)
		{
			delete m_components[i];
			m_components.Delete(i);
		}

		template<typename T>
		void DeleteComponent()
		{
			for (int i = 0; i < m_components.GetLength(); i++)
				if (typeid(T) == typeid(*m_components[i]))
					m_components.Delete(i);
		}

		//Transform transfrom;

		void SetName(std::string p_name) { m_name = p_name; }
		std::string GetName() { return m_name; }
		Entity* m_parent;
		class Transform* transform;
	protected:
		std::string m_name;
		Vec<Component*> m_components;
		friend class Scene;
	};
}