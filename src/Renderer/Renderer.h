#pragma once

namespace RG 
{
	class Renderer
	{
	public:
		static void Init();
		static void Render();
		static void ViewportInit();
	private:
		virtual void InitImpl() = 0;
		virtual void ViewportInitImpl() = 0;
		virtual void RenderImpl() = 0;
		virtual void RenderEntitiesImpl(class Event*) = 0;
		virtual void OnWindowResizeCallback(class Event*) = 0;
		//API member pointer
		static Renderer* s_impl;
	};
}