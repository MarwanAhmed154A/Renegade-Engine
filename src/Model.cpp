#include "Model.h"
#include "ResourceManager.h"

#include <iostream>

REFLECT_REGISTER_TYPE(Model)

namespace RG
{
	Model::~Model()
	{

	}
	void Model::Draw()
	{
		for (unsigned int i = 0; i < meshes.GetLength(); i++)
			meshes[i].Draw();
	}

	void Model::Draw(Shader* shader)
	{
		for (unsigned int i = 0; i < meshes.GetLength(); i++)
			meshes[i].Draw(shader);
	}

	void Model::loadModel(string path)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// retrieve the directory path of the filepath
		directory = path.substr(0, path.find_last_of('/'));

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.Push(processMesh(mesh, scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// data to fill
		Vec<Vertex> vertices;
		Vec<unsigned int> indices;
		Vec<Texture*> textures;

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
			}
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				//std::cout << "x: " << vec.x << " y: " << vec.y << '\n';
				vertex.TexCoords = vec;
				// tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;
				// bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vertices.Push(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.Push(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN
		
		// 1. diffuse maps
		Material mat;
		mat.diffuse  = loadMaterialTextures(material, aiTextureType::aiTextureType_BASE_COLOR, "texture_base_color");
		if (!mat.diffuse)
		{
			mat.baseColor = loadMaterialSolidColor(material);
		}

		mat.normal   = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");

		mat.metallicRoughness = loadMaterialTextures(material, aiTextureType_UNKNOWN, "texture_metallic_roughness");

		if (!mat.metallicRoughness)
		{
			mat.metallic  = loadMaterialKeyValue(material, AI_MATKEY_METALLIC_FACTOR);
			mat.roughness = loadMaterialKeyValue(material, AI_MATKEY_ROUGHNESS_FACTOR);
		}

		mat.alphaMap = loadMaterialTextures(material, aiTextureType_OPACITY, "texture_opacity");
		if (!mat.alphaMap)
		{
			mat.alpha = loadMaterialKeyValue(material, AI_MATKEY_OPACITY);
		}
		//mat.specualr = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_diffuse");
		//mat.height = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_diffuse");
		//// 2. specular maps
		//vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//// 3. normal maps
		//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		//// 4. height maps
		//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, mat);
	}

	shared_ptr<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		shared_ptr<Texture> tex = nullptr;

		//vector<Texture*> textures;
		//for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		
		aiString str;
		if (mat->GetTexture(type, 0, &str) == aiReturn_SUCCESS)
		{
			tex = ResourceManager::Create(str.C_Str());
			std::cout << "TEX: " << str.C_Str() << "\n";
		}
		return tex;
		
	}

	aiColor4D Model::loadMaterialSolidColor(aiMaterial* mat)
	{
		aiColor4D baseColor;
		if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, baseColor) == AI_SUCCESS)
		{
			return baseColor;
		}
	}

	float Model::loadMaterialKeyValue(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int idx)
	{
		float val;
		if (mat->Get(pKey, type, idx, val) == AI_SUCCESS)
		{
			return val;
		}
	}
}