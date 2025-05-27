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
		//ReflectedProp(std::string name, int offset, InspectableType type, class ReflectedTypeData* parent);
		ReflectedProp(std::string name, int offset, InspectableType type, int ID);

		InspectableType type;
		std::string name;
		int offset;
	};

	class ReflectedTypeData
	{
	public:
		std::vector<ReflectedProp*> vars;
		ReflectedTypeData(std::string type_name, BaseSceneObject* e, BaseSceneObject* parent, void(*setter)(int, int));

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
		static std::vector<ReflectedProp*>* GetVarsFromType(int ID);
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

			std::vector<ReflectedProp*>* vars = &(*s_reflectionDataList)[ID]->vars; //Get the variables metadata list

			for (int i = 0; i < s_reflectionDataList->size(); i++)
			{
				if ((*vars)[i]->name == name) //iterate to find the variable with the same name
				{
					return (T*)((char*)obj + (*vars)[i]->offset); //add the offset to the object pointer to find the needed variable in memory
				}
			}

			return nullptr; //safely return nullptr if there are no variables with the requested name
		}

	private:
		inline static std::vector<BaseSceneObject*>* s_types;
		inline static std::vector<ReflectedTypeData*>* s_reflectionDataList;
		friend class  ReflectedProp;
	};
} 

using namespace RG;
#define ADD(x) int  x##::s_parentTypeID = 0; int x##::s_TypeID = 0; char x::x##_adder = *(char*)new ReflectedTypeData(#x, new x, new x##::Super, [](int ID, int pID){x##::s_TypeID = ID; x##::s_parentTypeID = pID;}); int x##::s_Size = sizeof(x);
#define ADDVAR(type, x, v) inline static char v_for_##v## = *(char*)new ReflectedProp(#v, offsetof(x, v), type, x##::s_GetTypeID());
#define ADDPRV(type, x, v) static int ;