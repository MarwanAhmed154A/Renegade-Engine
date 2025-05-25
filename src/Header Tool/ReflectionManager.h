#pragma once

#pragma once

#include <vector>
#include <string>

#include "BaseSceneObject.h"

namespace RG
{
	enum class InspectableType
	{
		Int, Float, String, Vec3
	};

	class ReflectedProp
	{
	public:
		ReflectedProp() = default;
		ReflectedProp(std::string name, int offset, InspectableType type, class ReflectedTypeData* parent);

		InspectableType type;
		std::string name;
		int offset;
	};

	class ReflectedTypeData
	{
	public:
		std::vector<ReflectedProp> vars;
		ReflectedTypeData(std::string type_name, BaseSceneObject* e, BaseSceneObject* parent);

		int AddedIndex, parentIndex;

		std::string type_name;
		BaseSceneObject* ent;
	};

	class ReflectionManager
	{
	public:
		static void Init();

		static int Add(ReflectedTypeData* a);
		static std::vector<BaseSceneObject*>* GetTypes();
		static std::vector<ReflectedProp>* GetVarsFromType(int ID);
		static const std::string GetTypeName(int ID);
		static const std::string GetTypeName(BaseSceneObject*);
		static BaseSceneObject* GetType(std::string type_name);

		template<typename T>
		static T* GetVarFromObject(std::string name, BaseSceneObject* obj)
		{
			if (!obj || name.empty())
				return nullptr;

			int ID = ((BaseSceneObject*)obj)->GetTypeID(); //Get the static type ID from the Entity based object (TO DO: change to support components) 
			if (!&(*s_reflectionDataList)[ID])
				return nullptr;

			std::vector<ReflectedProp>* vars = &(*s_reflectionDataList)[ID]->vars; //Get the variables metadata list

			for (int i = 0; i < s_reflectionDataList->size(); i++)
			{
				if ((*vars)[i].name == name) //iterate to find the variable with the same name
				{
					return (T*)((char*)obj + (*vars)[i].offset); //add the offset to the object pointer to find the needed variable in memory
				}
			}

			return nullptr; //safely return nullptr if there are no variables with the requested name
		}

	private:
		static std::vector<BaseSceneObject*>* s_types;
		static std::vector<ReflectedTypeData*>* s_reflectionDataList;
	};
} 

using namespace RG;
#define ADD(x) extern ReflectedTypeData* x##_adder = new ReflectedTypeData(#x, new x, new x##::Super); int x##::s_TypeID = x##_adder->AddedIndex; int x##::s_parentTypeID = x##_adder->parentIndex; int x##::s_Size = sizeof(x);
#define ADDVAR(type, x, v) x x##_for_##v##_Loc; int v##_offset_in_##x## = (char*)&(x##_for_##v##_Loc.##v##) - (char*)&(x##_for_##v##_Loc); ReflectedProp v_for_##v## = ReflectedProp(#v, v##_offset_in_##x##, type, x##_adder);