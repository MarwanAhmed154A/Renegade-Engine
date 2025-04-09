#pragma once

#pragma once

#include <vector>
#include <string>

namespace RG
{
	class Entity;

	class ReflectedProp
	{
	public:
		ReflectedProp() {}
		ReflectedProp(std::string name, int offset, class ReflectedTypeData* parent);

		std::string name;
		int offset;
	};

	class ReflectedTypeData
	{
	public:
		std::vector<ReflectedProp> vars;
		ReflectedTypeData(Entity* e);

		int AddedIndex;

		Entity* ent;
	};

	class ReflectionManager
	{
	public:
		static void Init();

		static int Add(ReflectedTypeData* a);

		static std::vector<Entity*>* GetTypes();

		static ReflectionManager& GetInstance();

		static std::vector<ReflectedProp> GetVarsFromType(int ID);

		template<typename E, typename T>
		static T* GetVarFromObject(std::string name, E* obj)
		{
			if (!obj || name.empty())
				return nullptr;

			int ID = ((Entity*)obj)->GetTypeID(); //Get the static type ID from the Entity based object (TO DO: change to support components) 
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

		static std::vector<Entity*>* s_types;
	private:
		static std::vector<ReflectedTypeData*>* s_reflectionDataList;
	};

}
using namespace RG;
#define ADD(x) extern ReflectedTypeData* x##_adder = new ReflectedTypeData(new x); int x##::s_TypeID = x##_adder->AddedIndex;
#define ADDVAR(x, v) x x##_for_##v##_Loc; int v##_offset_in_##x## = (char*)&(x##_for_##v##_Loc.##v##) - (char*)&(x##_for_##v##_Loc); ReflectedProp v_for_##v## = ReflectedProp(#v, v##_offset_in_##x##, x##_adder);