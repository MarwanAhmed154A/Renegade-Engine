#include "Texture.h"
#include "Platform/OpenGL/GLTexture.h"
#include "ResourceManager.h"

namespace RG
{
	Texture* Texture::CreateTexture(const char* file)
	{
		return ResourceManager::Create(file);
	}

	Texture* Texture::CreateTexture(unsigned char* data, int width, int height, int nrChannels)
	{
		return new GLTexture(data, width, height, nrChannels);
	}
}