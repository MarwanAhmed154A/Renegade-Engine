#pragma once

#include <string>
#include <memory>
#include <Log.h>

namespace RG
{
	class Texture
	{
	public:
		static std::shared_ptr<Texture> CreateTexture(const char* file);
		static std::shared_ptr<Texture> CreateTexture(unsigned char* data, int width, int height, int nrChannels);

		virtual void Bind() = 0;
		virtual void Unload() = 0;

		std::string GetPath() { return path; }
		void SetPath(std::string new_path) { path = new_path; }

		virtual unsigned int GetID() = 0;

		~Texture()
		{
			RG_CORE_WARN("UNLOADED!!!");
		}
	protected:
		friend class TextureComponent;
		unsigned int texture;
		std::string path;
	};
}