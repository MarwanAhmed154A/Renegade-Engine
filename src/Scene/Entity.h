#pragma once

#include "Scene.h"
#include "Components.h"
#include "BaseSceneObject.h"

#include <string>
#include <memory>

//#define GEN(x, parent) protected: static int s_parentTypeID; static int s_TypeID; static int s_Size; static ReflectedTypeData* x##_adder; public: typedef parent Super; virtual int GetTypeID() override {return s_TypeID;} virtual BaseSceneObject* GetCopy(char* binary) {return new x##(*(x##*)binary);} virtual int GetSize() override {return s_Size;} static int s_GetTypeID() {return s_TypeID;}

namespace RG
{
	class Entity : public BaseSceneObject
	{
		REFLECTABLE_CLASS(Entity, BaseSceneObject)

	public:
		Entity();
		Entity(std::string name);

		virtual void Start() {}
		virtual void Tick() {}

		virtual void OnCollisionEnter(Entity* other) {}
		virtual void OnCollisionExit(Entity* other) {}


		void AddComponent(Component* component);

		Component* GetComponent(int index) { return m_components[index]; }


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


		void SetName(std::string p_name) { m_name = p_name; }
		std::string GetName() { return m_name; }
		Entity* m_parent;
		class Transform* transform;
	protected:
		std::string m_name;
		Vec<Component*> m_components;
		friend class Scene;
		friend class SerializationManager;
	};
}