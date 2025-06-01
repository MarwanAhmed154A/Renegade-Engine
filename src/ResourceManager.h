#pragma once

#include <memory>

#include "Renderer/Texture.h"
#include "Vec.h"
namespace RG
{
	class ResourceManager
	{
	private:
		static Vec<std::weak_ptr<Texture>> m_textures;
	public:
		//static Texture* Create(const char* file_name);
		static std::shared_ptr<Texture> Create(const char* file_name);
	};
}