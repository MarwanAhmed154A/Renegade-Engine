#pragma once

#include "Vec.h"
#include <string>

namespace RG
{
	class Entity;
	class Scene
	{
	public:
		Scene();
		//~Scene() { delete event; }

		Entity* AddEntity(Entity* ent);

		Entity* CreateNewEntity();
		Entity* CreateNewEntity(Entity* ent);
		Entity* CreateNewEntity(std::string name);

		template<typename T>
		void CreateNewEntity()
		{
			m_entities.Push(new T);
		}

		void DeleteEntity(int index);
		void DeleteEntity(std::string name);

		Entity* GetEntity(int index);
		Entity* GetEntity(std::string name);

		void OnRenderScene();

		unsigned int GetAmountOfEntities();
	private:
		friend class Rigidbody2D;
		//class OnRenderEvent* event;
		struct OnSceneInitEvent* event;
		Vec<Entity*> m_entities;
	};
}