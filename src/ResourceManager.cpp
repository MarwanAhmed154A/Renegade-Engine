#include "ResourceManager.h"
#include "Platform/OpenGL/GLTexture.h"
#include "Log.h"

namespace RG
{
	std::shared_ptr<Texture> ResourceManager::Create(const char* file_name)
	{
		for (int i = 0; i < m_textures.GetLength(); i++)
		{
			if (m_textures[i].lock() && std::strcmp(m_textures[i].lock().get()->GetPath().c_str(), file_name) == 0)
			{
				RG_CORE_INFO("TEXTURE ALREADY LOADED!")
					return m_textures[i].lock();
			}
			else if (!m_textures[i].lock())
			{
				m_textures.Delete(i);
				i--;
			}
		}
		
		RG_CORE_INFO("TEXTURE NOT LOADED, LOADING...")


		std::shared_ptr<GLTexture> temp(new GLTexture(file_name));
		m_textures.Push(temp);
		return temp;
	}

	std::shared_ptr<Model> ResourceManager::CreateModel(const char* file_name)
	{
		for (int i = 0; i < m_models.GetLength(); i++)
		{
			if (m_models[i].lock() && std::strcmp(m_models[i].lock().get()->GetPath().c_str(), file_name) == 0)
			{
				RG_CORE_INFO("Model ALREADY LOADED!")
					return m_models[i].lock();
			}
			else if (!m_models[i].lock())
			{
				m_models.Delete(i);
				i--;
			}
		}

		RG_CORE_INFO("MODEL NOT LOADED, LOADING...")

		std::shared_ptr<Model> temp(new Model(file_name));
		m_models.Push(temp);
		return temp;
	}
}