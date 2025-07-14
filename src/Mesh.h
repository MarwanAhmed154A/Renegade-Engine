#pragma once

#include <glm/glm.hpp>

#include "Vec.h"
#include "Renderer/Texture.h"
#include "Renderer/Shader.h"
#include "Material.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

namespace RG
{
	class Mesh {
	public:
		// mesh data
		Vec<Vertex>       vertices;
		Vec<unsigned int> indices;
		Vec<Texture*>     textures;
		Material mat;

		Mesh(Vec<Vertex> vertices, Vec<unsigned int> indices, Vec<Texture*> textures);
		Mesh(Vec<Vertex> vertices, Vec<unsigned int> indices, Material mat);
		void Draw();
		void Draw(Shader*);
	private:
		//  render data
		unsigned int VAO, VBO, EBO;

	void setupMesh();
	};
}