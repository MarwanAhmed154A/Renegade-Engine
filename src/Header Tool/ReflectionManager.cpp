#include "ReflectionManager.h"

#include <string>

namespace RG
{
	ReflectedProp::ReflectedProp(std::string name, int offset, ReflectedTypeData* parent) : name(name), offset(offset)
	{
		parent->vars.push_back(*this);
	}

	std::vector<Entity*>* ReflectionManager::s_types;
	std::vector<ReflectedTypeData*>* ReflectionManager::s_reflectionDataList;

	ReflectedTypeData::ReflectedTypeData(Entity* e)
	{
		ent = e;
		AddedIndex = ReflectionManager::Add(this) - 1;
	}

	void ReflectionManager::Init()
	{
		//types.resize(1);
	}

	int ReflectionManager::Add(ReflectedTypeData* a)
	{
		static std::vector<Entity*> types;
		static std::vector<ReflectedTypeData*> reflectionDataList;

		s_types = &types;
		s_reflectionDataList = &reflectionDataList;

		s_types->push_back(a->ent);
		s_reflectionDataList->push_back(a);

		return s_types->size();
	}

	std::vector<Entity*>* ReflectionManager::GetTypes()
	{
		return s_types;
	}

	ReflectionManager& ReflectionManager::GetInstance()
	{
		static ReflectionManager instance; // Ensures initialization before use
		return instance;
	}

	std::vector<ReflectedProp> ReflectionManager::GetVarsFromType(int ID)
	{
		return (*s_reflectionDataList)[ID]->vars;
	}
}