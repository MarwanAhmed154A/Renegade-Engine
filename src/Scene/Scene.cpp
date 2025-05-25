#include <vendor/Entt/Include/Entt.hpp>

#include "Scene.h"
#include "Entity.h"
#include "Renderer/Renderer.h"
#include "Log.h"
#include "Events/Event.h"

namespace RG
{
	Scene::Scene() 
	{
		event = new OnSceneInitEvent;
		event->vec = &m_entities;
		EventManager::Invoke(event);
		//event
		//cl.m_scene = this;
		
		//m_world->SetAutoClearForces(false);
	}

	Entity* Scene::AddEntity(Entity* ent)
	{
		return m_entities.Push(ent);
	}

	Entity* Scene::CreateNewEntity()
	{
		return m_entities.Push(new Entity);
	}

	Entity* Scene::CreateNewEntity(Entity* ent)
	{
		return m_entities.Push(new Entity(*ent));
	}

	Entity* Scene::CreateNewEntity(std::string name)
	{
		Entity* entity = new Entity;
		//entity->m_name = name;
		return m_entities.Push(entity);
	}

	void Scene::DeleteEntity(int index)
	{
		m_entities.Delete(index);
	}

	void Scene::DeleteEntity(std::string name)
	{
		for (int i = 0; i < m_entities.GetLength(); i++)
		{
			//if (m_entities[i]->m_name == name)
			//	m_entities.Delete(i);
		}
	}

	Entity* Scene::GetEntity(int index)
	{
		return m_entities[index];
	}

	Entity* Scene::GetEntity(std::string name)
	{
		for (int i = 0; i < m_entities.GetLength(); i++)
		{
			//if (m_entities[i]->m_name == name)
				//return m_entities[i];
		}
		return nullptr;
	}

	void Scene::OnRenderScene()
	{
		//EventManager::Invoke(event);
	}

	unsigned int Scene::GetAmountOfEntities()
	{
		return m_entities.GetLength();
	}

}