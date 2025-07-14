#pragma once

#include "Vec.h"
#include "Asset.h"
#include <string>

namespace RG
{
	class Cubemap : public Asset
	{
		REFLECTABLE_CLASS(Cubemap, Asset)
	public:
		static Cubemap* Create(Vec<std::string> vec);
		virtual ~Cubemap() {}

		virtual void Load(Vec<std::string> vec) {}
		virtual void Draw() {}
		virtual void BindTex() {}
	protected:
		Vec<std::string> textures;
		unsigned int textureID;
		friend class SerializationManager;
	};
}