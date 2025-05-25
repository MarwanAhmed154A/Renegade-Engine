#include "Renderer.h"
#include "Platform/OpenGL/GLRenderer.h"

RG::Renderer * RG::Renderer::s_impl;

namespace RG
{
	void Renderer::Init()
	{
		s_impl = new GLRenderer;
		s_impl->InitImpl();
	}

	void Renderer::Render()
	{
		s_impl->RenderEntitiesImpl();
	}

	void Renderer::ViewportInit()
	{
		s_impl->ViewportInitImpl();
	}
}