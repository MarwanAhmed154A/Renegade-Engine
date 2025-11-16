#include <string>
#include <fstream>

#include "SerializationManager.h"
#include "Events/Event.h"
#include "Scene/Entity.h"
#include "Log.h"
#include "Header Tool/ReflectionManager.h"
#include "Asset.h"
#include "ResourceManager.h"

using namespace std;

namespace RG
{
	std::vector<Entity*>* SerializationManager::ents;

	void SerializationManager::Init()
	{
		EventManager::AddCallback<SerializationManager>(EventType::OnSceneInit, nullptr, &OnSceneInitCallback);
	}

	void SerializationManager::Save(std::string file)
	{
		ofstream out;
		out.open(file, ios::binary);
		
		//save amount of entities to be loaded
		int amountOfEnts = ents->size();
		out.write((const char*)&amountOfEnts, 4);

		for (Entity* curEnt : *ents)
		{
			SaveString(ReflectionManager::GetTypeName(curEnt->GetTypeID()), out);
			SaveString(curEnt->GetName(), out);

			int entSize = curEnt->GetSize();
			out.write((char*)&entSize, 4);
			out.write((char*)curEnt, entSize);

			std::vector<Component*>* comps = (std::vector<Component*>*)&curEnt->m_components;
			//save amount of components in this entity to be loaded
			int amountOfComps = comps->size();
			out.write((const char*)&amountOfComps, 4);

			vector<ReflectedProp*>* props = (vector<ReflectedProp*>*)ReflectionManager::GetVarsFromType(curEnt->GetTypeID());
			for (ReflectedProp* prop : *props)
			{
				if (prop->type == InspectableType::Asset)
				{
					std::shared_ptr<RG::Asset>* asset = ReflectionManager::GetVarFromObject<std::shared_ptr<Asset>>(prop->name, curEnt);
					SaveAsset(prop, curEnt, out);
				}
			}

			for (Component* curComp : *comps)
			{
				SaveString(ReflectionManager::GetTypeName(curComp->GetTypeID()), out);

				int compSize = curEnt->GetSize();
				out.write((char*)&compSize, 4);
				out.write((char*)curComp, compSize);

				vector<ReflectedProp*>* props = (vector<ReflectedProp*>*)ReflectionManager::GetVarsFromType(curComp->GetTypeID());
				for (ReflectedProp* prop : *props)
				{
					if (prop->type == InspectableType::Asset)
					{
						std::shared_ptr<RG::Asset>* asset = ReflectionManager::GetVarFromObject<std::shared_ptr<Asset>>(prop->name, curComp);
						SaveAsset(prop, curComp, out);
					}
				}
			}
		}
	}

	void SerializationManager::Load(std::string file)
	{
		ifstream in;
		in.open(file, ios::binary);
		
		int amountOfEnts = 0;
		in.read((char*)&amountOfEnts, 4);

		for (int i = 0; i < amountOfEnts; ++i)
		{
			//load ent type and name
			std::string type = LoadStringCurSize(in);
			std::string name = LoadStringCurSize(in);

			int entSize = 0;
			in.read((char*)&entSize, 4);

			char* entBinary = new char[entSize];
			in.read(entBinary, entSize);

			Entity* curEnt = (Entity*)entBinary;

			//replace corrupted string with placement new
			std::string* namePtr = &curEnt->m_name;
			new (namePtr)(std::string);
			curEnt->m_name = name;

			//place new component array pointer over old corrupted data
			Vec<Component*>* comps = &(curEnt->m_components);
			new (comps)(Vec<Component*>);
			
			curEnt = (Entity*)ReflectionManager::GetType(type)->GetCopy(entBinary);
			delete[] entBinary;

			vector<ReflectedProp*>* props = (vector<ReflectedProp*>*)ReflectionManager::GetVarsFromType(curEnt->GetTypeID());
			for (ReflectedProp* prop : *props)
			{
				if (prop->type == InspectableType::Asset)
				{
					std::shared_ptr<RG::Asset>* assetPtr = ReflectionManager::GetVarFromObject<std::shared_ptr<Asset>>(prop->name, curEnt);
					new (assetPtr) std::shared_ptr<RG::Asset>;
					*assetPtr = LoadAsset(in);
				}
			}

			int amountOfComps = 0;
			in.read((char*)&amountOfComps, 4);

			for (int j = 0; j < amountOfComps; j++)
			{
				std::string compType = LoadStringCurSize(in);

				int compSize = 0;
				in.read((char*)&compSize, 4);

				char* compBinary = new char[compSize];
				in.read(compBinary, compSize);

				vector<ReflectedProp*>* props = (vector<ReflectedProp*>*)ReflectionManager::GetVarsFromType(ReflectionManager::GetType(compType)->GetTypeID());
				for (ReflectedProp* prop : *props)
				{
					if (prop->type == InspectableType::Asset)
					{
						std::shared_ptr<RG::Asset>* assetPtr = (std::shared_ptr<RG::Asset>*)((char*)compBinary + prop->offset);
						new (assetPtr) std::shared_ptr<RG::Asset>;
						*assetPtr = LoadAsset(in);
						int x = 0;
					}
				}

				Component* curComp = (Component*)ReflectionManager::GetType(compType)->GetCopy(compBinary); //Get component* with correct vptr
				curComp->SetParent(curEnt);

				curEnt->AddComponent(curComp);
			}

			curEnt->transform = curEnt->GetComponent<Transform>();
			ents->push_back(curEnt);
		}
	}

	void SerializationManager::OnSceneInitCallback(Event* e)
	{
		ents = (std::vector<Entity*>*)e->SafeCastTo<OnSceneInitEvent>()->vec;
	}

	/// <summary>
	///
	/// </summary>
	/// <param name="str">string to save</param>
	/// <param name="file">refrence file to save to</param>
	inline void SerializationManager::SaveString(std::string str, std::ofstream& file)
	{
		int entTypenameSize = (int)str.size();

		file.write((const char*)&entTypenameSize, 4);
		file.write(str.c_str(), entTypenameSize);
	}

	/// <summary>
	/// Loads string from file with the size of the cur int the loading stopped at
	/// </summary>
	/// <param name="file">file to load from</param>
	/// <returns>loaded string</returns>
	inline std::string SerializationManager::LoadStringCurSize(std::ifstream& file)
	{
		int size = 0;
		file.read((char*)&size, 4);

		char* c_str = new char[size];
		file.read(c_str, size);
		c_str[size] = '\0';

		std::string s = std::move(c_str);

		return s;
	}

	void SerializationManager::SaveAsset(ReflectedProp* prop, BaseSceneObject* obj, std::ofstream& file)
	{
		if (prop->type == InspectableType::Asset)
		{
			RG::Asset* asset = ReflectionManager::GetVarFromObject<std::shared_ptr<Asset>>(prop->name, obj)->get();

			//if the asset has different versions for different APIs save the parent (base) type name instead
			if (asset->GetParentTypeID() == Cubemap::s_GetTypeID())
			{
				SaveString(ReflectionManager::GetTypeName(asset->GetParentTypeID()), file);
				for (int i = 0; i < 6; i++)
					SaveString(((Cubemap*)asset)->textures[i], file);
				return;
			}
			else if (asset->GetParentTypeID() != Asset::s_GetTypeID())
			{
				SaveString(ReflectionManager::GetTypeName(asset->GetParentTypeID()), file);
				RG_CORE_INFO(asset->GetParentTypeID())
			}
			else
				SaveString(ReflectionManager::GetTypeName(asset->GetTypeID()),       file);

			SaveString(asset->GetPath(), file);
		}
	}

	shared_ptr<Asset> SerializationManager::LoadAsset(std::ifstream& file)
	{
		string type_name = LoadStringCurSize(file);
		string path      = LoadStringCurSize(file);

		std::shared_ptr<Asset> asset;

		if (type_name == "Texture")
			asset = ResourceManager::Create(path.c_str());
		else if (type_name == "Model")
			asset = ResourceManager::CreateModel(path.c_str());
		else if (type_name == "Cubemap")
		{
			Vec<std::string> paths;
			paths.Push(path);
			for (int i = 0; i < 5; i++)
				paths.Push(LoadStringCurSize(file));

			asset = std::shared_ptr<Asset>(Cubemap::Create(paths));
		}

		return asset;
	}

}