#pragma once

#include "Maths/Vec2.h"

namespace RG
{
	class Framebuffer
	{
	public:
		Framebuffer() = default;

		static Framebuffer* Create(Vec2 size);

		virtual void Bind() = 0;
		virtual void Resize(Vec2 newSize) = 0;

		unsigned int GetTextureID() { return tex_id; }
		unsigned int GetID() { return id; }
	protected:
		unsigned int id, tex_id;
		Vec2 m_size;
	};
}