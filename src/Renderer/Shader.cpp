#include "Shader.h"
#include "Platform/OpenGL/GLShader.h"

namespace RG
{
	Shader* Shader::CreateShader(const char* vertex_path, const char* fragment_path)
	{
		return new GLShader(vertex_path, fragment_path);
	}
}