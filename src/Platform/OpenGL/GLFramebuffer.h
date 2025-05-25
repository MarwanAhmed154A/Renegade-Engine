#pragma once
#include "Renderer/Framebuffer.h"

namespace RG
{
	class GLFramebuffer : public Framebuffer
	{
	public:
		GLFramebuffer(class Vec2);

		virtual void Bind() override;
		virtual void Resize(Vec2 newSize) override;
	};
}