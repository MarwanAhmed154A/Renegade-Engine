#pragma once

#include "Renderer/Texture.h"
#include "Vec.h"

namespace RG
{
	class ResourceManager
	{
	private:
		static Vec<Texture*> m_textures;
	public:
		static Texture* Create(const char* file_name);
	};
}