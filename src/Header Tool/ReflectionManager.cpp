#include "ReflectionManager.h"

#include <string>

namespace RG
{
	ReflectedProp::ReflectedProp(std::string name, int offset, InspectableType type, ReflectedTypeData* parent) : name(name), offset(offset), type(type)
	{
		parent->vars.push_back(*this);
	}

	std::vector<BaseSceneObject*>* ReflectionManager::s_types;
	std::vector<ReflectedTypeData*>* ReflectionManager::s_reflectionDataList;

	ReflectedTypeData::ReflectedTypeData(std::string type_name, BaseSceneObject* e, BaseSceneObject* parent) : type_name(type_name)
	{
		ent = e;
		AddedIndex = ReflectionManager::Add(this) - 1;
		parentIndex = parent->GetTypeID();
	}

	void ReflectionManager::Init()
	{
		//types.resize(1);
	}

	int ReflectionManager::Add(ReflectedTypeData* a)
	{
		//initialize lists as int, safety from unkown static-init order and UB, on static defintion lines to avoid multiple assignments of value
		static std::vector<BaseSceneObject*>*   types              = s_types              = new std::vector<BaseSceneObject*>();
		static std::vector<ReflectedTypeData*>* reflectionDataList = s_reflectionDataList = new std::vector<ReflectedTypeData*>();

		s_types->push_back(a->ent);
		s_reflectionDataList->push_back(a);

		return s_types->size();
	}

	std::vector<BaseSceneObject*>* ReflectionManager::GetTypes()
	{
		return s_types;
	}

	std::vector<ReflectedProp>* ReflectionManager::GetVarsFromType(int ID)
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
		for (int i = 0; i < s_reflectionDataList->size(); i++)
		{
			if ((*s_reflectionDataList)[i]->type_name == type_name)
			{
				return (*s_types)[i];
			}
		}
	}
}