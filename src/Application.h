#pragma once

namespace RG
{
	class Application
	{
	public:
		Application() {}

		virtual void Update() = 0;
	};

	Application* CreateApp();
}