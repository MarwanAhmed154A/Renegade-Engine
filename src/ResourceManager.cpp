#include "ResourceManager.h"
#include "Platform/OpenGL/GLTexture.h"
#include "Log.h"

RG::Vec<std::weak_ptr<RG::Texture>> RG::ResourceManager::m_textures;

namespace RG
{
	std::shared_ptr<Texture> ResourceManager::Create(const char* file_name)
	{
		for (int i = 0; i < m_textures.GetLength(); i++)
			if (m_textures[i].lock().get()->GetPath() == file_name)
			{
				RG_CORE_INFO("TEXTURE LOADED")
				return m_textures[i].lock();
			}
		
		RG_CORE_INFO("TEXTURE NOT LOADED, LOADING...")

		std::shared_ptr temp = std::make_shared<GLTexture>(GLTexture(file_name));
		m_textures.Push(temp);
		return temp;
	}
}