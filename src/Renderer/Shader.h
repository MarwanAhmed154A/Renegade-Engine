#pragma once

namespace RG
{
	class Shader
	{
	public:
		static Shader* CreateShader(const char* vertex_path, const char* fragment_path);

		virtual void Bind() = 0;
		virtual unsigned int GetUniformLoc(const char* name) = 0;

		virtual void SetVec3(unsigned int loc, const float* val) = 0;
		virtual void SetVec3(const    char* name, const float* val) = 0;
		virtual void SetVec4(unsigned int loc, const float* val) = 0;
		virtual void SetVec4(const    char* name, const float* val) = 0;
		virtual void SetMat4(unsigned int loc, float* val) = 0;
		virtual void SetMat4(const    char* name, void* mat) = 0;
		virtual void SetInt(unsigned int loc, const int val) = 0;
		virtual void SetInt(const     char* name, const int val) = 0;
		virtual void SetFloat(unsigned int loc, const float val) = 0;
		virtual void SetFloat(const     char* name, const float val) = 0;
	};
}