#pragma once
#include "Renderer/Shader.h"

namespace RG
{
	class GLShader : public Shader
	{
	public:
		GLShader(const char* vertex_path, const char* fragment_path);

		virtual void Bind() override;
		virtual unsigned int GetUniformLoc(const char* name) override;

		virtual void SetVec3(unsigned int loc, const float* val) override;
		virtual void SetVec3(const    char* name, const float* val) override;
		virtual void SetMat4(unsigned int loc, float* val) override;
		virtual void SetMat4(const    char* name, void* mat) override;
		virtual void SetInt(unsigned int loc, const int val) override;
		virtual void SetInt(const     char* name, const int val) override;
		virtual void SetFloat(unsigned int loc, const float val) override;
		virtual void SetFloat(const     char* name, const float val) override;
	private:
		unsigned int ID;
	};
}