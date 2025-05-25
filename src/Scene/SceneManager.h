#pragma once

#include "Scene.h"

namespace RG
{
	class SceneManager
	{
	public:
		static void Init();
		static void Load() {}
		static void Tick();
		static Scene* GetScene() { return m_scene; }
	private:
		static Scene* m_scene;
	};
}