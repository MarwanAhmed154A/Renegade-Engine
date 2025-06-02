#pragma once

#include <string>
#include <memory>
#include <Log.h>
#include "Asset.h"

namespace RG
{
	class Texture : public Asset
	{
	public:
		static std::shared_ptr<Texture> CreateTexture(const char* file);
		static std::shared_ptr<Texture> CreateTexture(unsigned char* data, int width, int height, int nrChannels);

		virtual void Bind() = 0;
		virtual void Unload() = 0;

		virtual unsigned int GetID() = 0;

	protected:
		friend class TextureComponent;
		unsigned int texture;
	};
}