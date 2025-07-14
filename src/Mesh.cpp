#include "Mesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

using namespace std;

namespace RG
{
	Mesh::Mesh(Vec<Vertex> vertices, Vec<unsigned int> indices, Vec<Texture*> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}

	Mesh::Mesh(Vec<Vertex> vertices, Vec<unsigned int> indices, Material mat)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->mat = mat;

		setupMesh();
	}

	void Mesh::Draw()
	{
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		//for (unsigned int i = 0; i < textures.GetLength(); i++)
		//{
		//	glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		//	// retrieve texture number (the N in diffuse_textureN)
		//	string number;
		//	string name = textures[i].type;
		//	if (name == "texture_diffuse")
		//		number = std::to_string(diffuseNr++);
		//	else if (name == "texture_specular")
		//		number = std::to_string(specularNr++); // transfer unsigned int to string
		//	else if (name == "texture_normal")
		//		number = std::to_string(normalNr++); // transfer unsigned int to string
		//	else if (name == "texture_height")
		//		number = std::to_string(heightNr++); // transfer unsigned int to string
		//
		//	
		//	textures[i].Bind();
		//}

		glActiveTexture(GL_TEXTURE0);
		if(mat.diffuse)
			mat.diffuse->Bind();

		glActiveTexture(GL_TEXTURE1);
		if (mat.normal)
			mat.normal->Bind();

		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.GetLength()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// always good practice to set everything back to defaults once configured.
		glActiveTexture(GL_TEXTURE0);
	}

	void Mesh::Draw(Shader* shader)
	{
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		//for (unsigned int i = 0; i < textures.GetLength(); i++)
		//{
		//	glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		//	// retrieve texture number (the N in diffuse_textureN)
		//	string number;
		//	string name = textures[i].type;
		//	if (name == "texture_diffuse")
		//		number = std::to_string(diffuseNr++);
		//	else if (name == "texture_specular")
		//		number = std::to_string(specularNr++); // transfer unsigned int to string
		//	else if (name == "texture_normal")
		//		number = std::to_string(normalNr++); // transfer unsigned int to string
		//	else if (name == "texture_height")
		//		number = std::to_string(heightNr++); // transfer unsigned int to string
		//
		//	
		//	textures[i].Bind();
		//}

		glActiveTexture(GL_TEXTURE0);
		if (mat.diffuse)
		{
			mat.diffuse->Bind();
			shader->SetInt("hasBaseColorMap", true);
			shader->SetInt("baseColorMap", 0);
		}
		else
		{
			shader->SetInt("hasBaseColorMap", false);
			glm::vec4 solidBaseColor = glm::vec4(mat.baseColor.r, mat.baseColor.g, mat.baseColor.b, mat.baseColor.a);
			shader->SetVec4("solidBaseColor", &solidBaseColor.x);
		}


		glActiveTexture(GL_TEXTURE1);
		if (mat.normal)
		{
			mat.normal->Bind();
			shader->SetInt("hasNormalMap", true);
			shader->SetInt("normalMap", 1);
		}
		else
		{
			shader->SetInt("hasNormalMap", false);
		}

		glActiveTexture(GL_TEXTURE2);
		if (mat.metallicRoughness)
		{
			mat.metallicRoughness->Bind();
			shader->SetInt("hasMetallicRoughnessMap", true);
			shader->SetInt("metallicRoughnessMap", 2);
		}
		else
		{
			shader->SetInt("hasMetallicRoughnessMap", false);
			shader->SetFloat("metallicValue", mat.metallic);
			shader->SetFloat("roughnessValue", mat.roughness);
		}


		glActiveTexture(GL_TEXTURE3);
		if (mat.alphaMap)
		{
			mat.alphaMap->Bind();
			shader->SetInt("hasAlphaMap", true);
			shader->SetInt("alphaMap", 3);
		}
		else
		{
			shader->SetInt("hasAlphaMap", false);
			shader->SetFloat("alpha", mat.alpha);
		}

		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.GetLength()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//shader->SetInt("hasBaseColorMap", false);
		//shader->SetInt("hasMetallicRoughnessMap", false);
		//shader->SetInt("baseColorMap", 0);
		//shader->SetInt("normalMap", 0);
		//shader->SetInt("metallicRoughnessMap", 0);

		// always good practice to set everything back to defaults once configured.
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Mesh::setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.GetLength() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.GetLength() * sizeof(unsigned int),
			&indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// vertex Tangents
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// vertex BiTangents
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		glBindVertexArray(0);
	}
}