#include "ResourceManager.h"
#include "Platform/OpenGL/GLTexture.h"
#include "Log.h"

RG::Vec<RG::Texture*> RG::ResourceManager::m_textures;

namespace RG
{
	Texture* RG::ResourceManager::Create(const char* file_name)
	{
		for (int i = 0; i < m_textures.GetLength(); i++)
			if (m_textures[i]->GetPath() == file_name)
			{
				RG_CORE_INFO("TEXTURE LOADED")
				return m_textures[i];
			}
		
		RG_CORE_INFO("TEXTURE NOT LOADED, LOADING...")
		m_textures.Push(new GLTexture(file_name));
		return m_textures[m_textures.GetLength() - 1];
	}
}