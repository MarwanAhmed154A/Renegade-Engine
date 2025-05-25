#pragma once

#include "Vec.h"
#include "Framebuffer.h"

namespace RG 
{
	class Renderer
	{
	public:
		static void Init();
		static void Render();
		static void ViewportInit();

		static Framebuffer* GetNativeOutput() { return s_impl->m_nativeOutput; }
	protected:
		virtual void InitImpl() = 0;
		virtual void ViewportInitImpl() = 0;
		virtual void RenderImpl() = 0;
		virtual void RenderEntitiesImpl() = 0;

		virtual void OnWindowResizeCallback(class Event*) = 0;
		virtual void OnSceneInitCallback(class Event*) = 0;
		//API member pointer
		Vec<class Entity*>* ents;
		Framebuffer* m_nativeOutput;
	private:
		static Renderer* s_impl;
	};
}