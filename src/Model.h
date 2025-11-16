#pragma once

#include "Mesh.h"

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Components.h"
#include "Header Tool/ReflectionManager.h"
#include "Animation/Animation.h"

using namespace std;

namespace RG
{
	class Model : public Asset
	{
		REFLECTABLE_CLASS(Model, Asset)
	private:

		// model data
		Vec<Mesh> meshes;
		string directory;

		void loadModel(string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		shared_ptr<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
			string typeName);
		aiColor4D loadMaterialSolidColor(aiMaterial* mat);
		float loadMaterialKeyValue(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int idx);
	public:
		Animation anim;
		~Model();

		Model()
		{

		}

		Model(string path)
		{
			this->path = path;
			loadModel(path);
		}

		void Load(string path)
		{
			this->path = path;
			loadModel(path);
		}

		void Draw();
		void Draw(class Shader*);
	};
}