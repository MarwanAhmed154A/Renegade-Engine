#pragma once

#include "Renderer/Texture.h"

namespace RG
{
	class GLTexture : public Texture
	{
	public:
		GLTexture(const char* file);
		GLTexture(unsigned char* data, int width, int height, int nrChannels);

		virtual void Bind() override;
		virtual void Unload() override;

		virtual unsigned int GetID() override;
	private:
		GLTexture() { path = ""; }
		friend class TextureComponent;
	};
}