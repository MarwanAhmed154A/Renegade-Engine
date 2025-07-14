#pragma once

#include "Renderer/Cubemap.h"

namespace RG
{
	class GLCubemap : public Cubemap
	{
		REFLECTABLE_CLASS(GLCubemap, Cubemap)
	public:
		GLCubemap();
		~GLCubemap();

		void Setup();
		virtual void Load(Vec<std::string> vec) override;
		virtual void Draw() override;
		virtual void BindTex() override;
	private:
		unsigned int skyboxVAO, skyboxVBO;
	};
}