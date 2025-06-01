#include "Texture.h"
#include "Platform/OpenGL/GLTexture.h"
#include "ResourceManager.h"

namespace RG
{
	std::shared_ptr<Texture> Texture::CreateTexture(const char* file)
	{
		return ResourceManager::Create(file);
	}

	std::shared_ptr<Texture> Texture::CreateTexture(unsigned char* data, int width, int height, int nrChannels)
	{
		return  std::make_shared<GLTexture>(data, width, height, nrChannels);
	}
}