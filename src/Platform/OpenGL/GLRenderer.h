#pragma once

#include "Renderer/Renderer.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

namespace RG
{
	class GLRenderer : public Renderer
	{
	public:
		GLRenderer();
		virtual void InitImpl() override;
		virtual void ViewportInitImpl() override;
		virtual void RenderImpl() override;
		virtual void OnWindowResizeCallback(class Event*) override;
		virtual void RenderEntitiesImpl() override;

		virtual void OnSceneInitCallback(class Event*) override;
	private:
		glm::mat4 projection, transform;
	};
}
