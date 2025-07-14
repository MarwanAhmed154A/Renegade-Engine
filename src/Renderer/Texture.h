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
		REFLECTABLE_CLASS(Texture, Asset)

		Texture() {}
		static std::shared_ptr<Texture> CreateTexture(const char* file);
		static std::shared_ptr<Texture> CreateTexture(unsigned char* data, int width, int height, int nrChannels);

		virtual void Bind() {}
		virtual void Unload() {}

		virtual unsigned int GetID() { return texture; }

	protected:
		friend class TextureComponent;
		unsigned int texture;
	};
}