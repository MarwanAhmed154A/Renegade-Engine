#include "SceneManager.h"
#include "Events/Event.h"


RG::Scene* RG::SceneManager::m_scene;

namespace RG
{
	void SceneManager::Init()
	{
		m_scene = new Scene;
	}

	void SceneManager::Tick()
	{
		
	}
}