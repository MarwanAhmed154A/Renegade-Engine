#include "GLShader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "Log.h"

namespace RG
{
	GLShader::GLShader(const char* vertex_path, const char* fragment_path)
	{
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		std::string vertexCode;
		std::string fragmentCode;

		try
		{
			// open files
			vShaderFile.open(vertex_path);
			fShaderFile.open(fragment_path);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			RG_CORE_ERROR("ERROR : SHADER FILE NOT SUCCESFULLY READ")
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		// print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			RG_CORE_ERROR("ERROR : VERTEX SHADER COMPILATION FAILED")
				RG_CORE_ERROR(infoLog)
		};

		// similiar for Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			RG_CORE_ERROR("ERROR : FRAGMENT SHADER COMPILATION FAILED")
				RG_CORE_ERROR(infoLog)
		};

		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		// print linking errors if any
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			RG_CORE_ERROR("ERROR : SHADER PROGRAM COMPILATION FAILED")
				RG_CORE_ERROR(infoLog)
		}

		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}


	void GLShader::Bind()
	{
		glUseProgram(ID);
	}


	unsigned int GLShader::GetUniformLoc(const char* name)
	{
		return glGetUniformLocation(ID, name);
	}



	void GLShader::SetVec3(unsigned int loc, const float* val)
	{
		glUniform3fv(loc, 1, val);
	}

	void GLShader::SetVec3(const char* name, const float* val)
	{
		glUniform3fv(glGetUniformLocation(ID, name), 1, val);
	}



	void GLShader::SetMat4(unsigned int loc, float* val)
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, val);
	}

	void GLShader::SetMat4(const char* name, void* mat)
	{
		glm::mat4* m = (glm::mat4*)mat;
		glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(*m));
	}



	void GLShader::SetInt(unsigned int loc, const int val)
	{
		glUniform1i(loc, val);
	}

	void GLShader::SetInt(const char* name, const int val)
	{
		glUniform1i(glGetUniformLocation(ID, name), val);
	}



	void GLShader::SetFloat(unsigned int loc, const float val)
	{
		glUniform1f(loc, val);
	}

	void GLShader::SetFloat(const char* name, const float val)
	{
		glUniform1f(glGetUniformLocation(ID, name), val);
	}
}