#include "Cubemap.h"

#include "Platform/OpenGL/GLCubemap.h"

REFLECT_REGISTER_TYPE(Cubemap)

namespace RG
{
	Cubemap* Cubemap::Create(Vec<std::string> vec)
	{
		GLCubemap* map = new GLCubemap();
		map->Setup();
		map->Load(vec);
		return map;
	}
}