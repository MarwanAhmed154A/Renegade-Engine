#include "ReflectionManager.h"

#include <string>

namespace RG
{
	Vec<BaseSceneObject*>* ReflectionManager::s_types;
	Vec<ReflectedTypeData*>* ReflectionManager::s_reflectionDataList;

	ReflectedProp::ReflectedProp(std::string name, int offset, InspectableType type, int ID) : name(name), offset(offset), type(type) {}

	ReflectedTypeData::ReflectedTypeData(std::string type_name) : type_name(type_name) {}

	char ReflectionManager::AddType(std::string type_name, BaseSceneObject* e, BaseSceneObject* parent, int& typeID, int& parentTypeID)
	{
		//initialize lists as ptrs, safety from unkown static-init order and UB, on static defintion lines to avoid multiple assignments of value
		static Vec<BaseSceneObject*>* types = s_types = new Vec<BaseSceneObject*>();
		static Vec<ReflectedTypeData*>* reflectionDataList = s_reflectionDataList = new Vec<ReflectedTypeData*>();

		ReflectedTypeData* data = new ReflectedTypeData(type_name);

		s_types->Push(e);
		s_reflectionDataList->Push(data);

		typeID = (int)s_types->GetLength() - 1;
		parentTypeID = parent->s_TypeID;

		return '0';
	}

	char ReflectionManager::AddProp(std::string name, int offset, InspectableType type, int ID)
	{
		(*s_reflectionDataList)[ID]->vars.Push(new ReflectedProp(name, offset, type, ID)); //add this prop's metadata to it's owning class's data
		return '0';
	}

	Vec<BaseSceneObject*>* ReflectionManager::GetTypes()
	{
		return s_types;
	}

	Vec<ReflectedProp*>* ReflectionManager::GetVarsFromType(int ID)
	{
		return &(*s_reflectionDataList)[ID]->vars;
	}

	const std::string ReflectionManager::GetTypeName(int ID)
	{
		return (*s_reflectionDataList)[ID]->type_name;
	}

	const std::string ReflectionManager::GetTypeName(BaseSceneObject* obj)
	{
		return (*s_reflectionDataList)[obj->GetTypeID()]->type_name;
	}

	BaseSceneObject* ReflectionManager::GetType(std::string type_name)
	{
		for (int i = 0; i < s_reflectionDataList->GetLength(); i++)
		{
			if ((*s_reflectionDataList)[i]->type_name == type_name)
			{
				return (*s_types)[i];
			}
		}
		return nullptr;
	}
}

int BaseSceneObject::s_TypeID = 999999999; //for safety