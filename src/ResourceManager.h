#pragma once

#include <memory>

#include "Renderer/Texture.h"
#include "Vec.h"
#include "Model.h"

namespace RG
{
	class ResourceManager
	{
	private:
		inline static Vec<std::weak_ptr<Texture>> m_textures;
		inline static Vec<std::weak_ptr<Model>>   m_models;
	public:
		//static Texture* Create(const char* file_name);
		static std::shared_ptr<Texture> Create(const char* file_name);
		static std::shared_ptr<Model>   CreateModel(const char* file_name);
	};
}