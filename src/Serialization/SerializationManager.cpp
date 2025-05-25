#include <string>
#include <fstream>

#include "SerializationManager.h"
#include "Events/Event.h"
#include "Scene/Entity.h"
#include "Log.h"
#include "Header Tool/ReflectionManager.h"
#include "Player.h"

using namespace std;

namespace RG
{
	SerializationManager* SerializationManager::s_instance;
	std::vector<Entity*>* SerializationManager::ents;

	void SerializationManager::Init()
	{
		EventManager::AddCallback<SerializationManager>(EventType::OnSceneInit, s_instance, &OnSceneInitCallback);
	}

	void SerializationManager::Save(std::string file)
	{
		//ofstream out;
		//out.open(file, ios::binary);
		//
		//int entitiesAmount = ents->GetLength();
		//out.write(reinterpret_cast<const char*>(&entitiesAmount), sizeof(entitiesAmount));
		//
		//for (int i = 0; i < ents->GetLength(); i++)
		//{
		//	Entity* cur = ents[0][i];
		//	int varByteSize = sizeof(*cur);
		//	int nameLength = cur->GetName().length();
		//	int componentAmount = cur->GetAmountOfComponents();
		//	//set the binary array to the correct size for the variable
		//	vector<unsigned char> binary;
		//	binary.resize(varByteSize);
		//
		//	std::memcpy(binary.data(), cur, varByteSize);
		//
		//	out.write(reinterpret_cast<const char*>(&varByteSize), sizeof(varByteSize));
		//	// 
		//	//out.write(reinterpret_cast<const char*>(&nameLength), 4);
		//	//out.write(cur->GetName().c_str(), nameLength);
		//	// 
		//	out.write(reinterpret_cast<const char*>(binary.data()), varByteSize);
		//	
		//	int curCompSize;
		//	Component* curComp;
		//	for (int j = 0; j < componentAmount; j++)
		//	{
		//		curComp = cur->GetComponent(j);
		//		curCompSize = curComp->GetSize();
		//
		//		std::string compName = ReflectionManager::GetTypeName(curComp->GetTypeID());
		//		int nameLength = compName.length();
		//		out.write((const char*)(&nameLength), 4);
		//		out.write(compName.c_str(), nameLength);
		//
		//		out.write((const char*)(&curCompSize), sizeof(curCompSize));
		//		out.write((const char*)(curComp), curCompSize);
		//	}
		//}
		//out.close();

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

			for (Component* curComp : *comps)
			{
				SaveString(ReflectionManager::GetTypeName(curComp->GetTypeID()), out);

				int compSize = curEnt->GetSize();
				out.write((char*)&compSize, 4);
				out.write((char*)curComp, compSize);
			}
		}
		//Entity* curEnt = 
	}

	void SerializationManager::Load(std::string file)
	{
		//ifstream in;
		//in.open(file, ios::binary | ios::ate);
		//streamsize s = in.tellg();
		//in.close();
		//
		//in.open(file, ios::binary);
		//
		//int entitiesAmount;
		//in.read((char*)(&entitiesAmount), sizeof(entitiesAmount));
		//
		//for (int i = 0; i < entitiesAmount; i++)
		//{
		//	int varByteSize = 0, nameLength = 0;
		//
		//	//binary array
		//	char* binary = nullptr;
		//	char* varByteSizeChar = new char[4], * nameLengthChar = new char[4];
		//	char* name;
		//
		//	in.read(varByteSizeChar, 4);
		//	memcpy(&varByteSize, varByteSizeChar, 4);
		//
		//	//in.read(nameLengthChar, 4);
		//	//nameLength = *(int*)(nameLengthChar);
		//	//name = new char[nameLength];
		//	//in.read(name, nameLength);
		//
		//	//RG_CORE_ERROR(name)
		//	//RG_CORE_ERROR(nameLength)
		//
		//	binary = new char[varByteSize];
		//
		//	in.read(binary, varByteSize);
		//
		//	Entity* var = (Entity*)binary;//(Entity*)((Entity*)ReflectionManager::GetType("Entity"))->GetCopy(binary);
		//
		//	int AmountOfComps = var->GetAmountOfComponents();
		//	int curCompSize;
		//	new (&var->m_components) Vec<Component*>();
		//	for (int j = 0; j < AmountOfComps; j++)
		//	{
		//		int compNameLength = 0;
		//		in.read((char*)(&compNameLength), 4);
		//		char* compName = new char[compNameLength + 1];
		//		in.read(compName, compNameLength);
		//		compName[compNameLength] = '\0';
		//
		//		//var->DeleteComponent(j);
		//		in.read((char*)(&curCompSize), sizeof(curCompSize));
		//		char* curCompBinary = new char[curCompSize];
		//
		//		in.read((char*)(curCompBinary), curCompSize);
		//		BaseSceneObject* curComp = ((Component*)ReflectionManager::GetType(compName))->GetCopy(curCompBinary);
		//		//ReflectionManager::
		//		//*curComp = *(Transform*)(curCompBinary);
		//
		//		//memcpy(curComp, curCompBinary, curComp->GetTypeI)
		//		var->AddComponent((Component*)curComp);
		//	}
		//	var->transform = (Transform*)var->GetComponent(0);
		//
		//	//string x = name;
		//	//var->SetName(x);
		//
		//	ents->push_back(var);
		//}
		//in.close();

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
			
			curEnt = (Entity*)ReflectionManager::GetType(type)->GetCopy((char*)curEnt); //Get entity* with correct vptr

			int amountOfComps = 0;
			in.read((char*)&amountOfComps, 4);

			for (int j = 0; j < amountOfComps; j++)
			{
				std::string compType = LoadStringCurSize(in);

				int compSize = 0;
				in.read((char*)&compSize, 4);

				char* compBinary = new char[compSize];
				in.read(compBinary, compSize);

				Component* curComp = (Component*)ReflectionManager::GetType(compType)->GetCopy(compBinary); //Get component* with correct vptr

				curEnt->AddComponent(curComp);
			}

			curEnt->transform = curEnt->GetComponent<Transform>();
			ents->push_back(curEnt);
		}
	}

	void SerializationManager::OnSceneInitCallback(Event* e)
	{
		ents = (std::vector<Entity*>*)((OnSceneInitEvent*)e)->vec;
	}

	/// <summary>
	/// Saves string size then string, don't forget to add /0 when loading
	/// </summary>
	/// <param name="str">string to save</param>
	/// <param name="file">refrence file to save to</param>
	inline void SerializationManager::SaveString(std::string str, std::ofstream& file)
	{
		int entTypenameSize = str.size();

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

		char* str = new char[size];
		file.read(str, size);
		str[size] = '\0';

		return str;
	}

}