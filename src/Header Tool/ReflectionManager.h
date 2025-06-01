#pragma once

#include "Vec.h"
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
		ReflectedProp(std::string name, int offset, InspectableType type, int ID);

		InspectableType type;
		std::string name;
		int offset;
	};

	class ReflectedTypeData
	{
	public:
		Vec<ReflectedProp*> vars;
		ReflectedTypeData(std::string type_name);

		std::string type_name;
	};

	class ReflectionManager
	{
	public:
		static char AddType(std::string type_name, BaseSceneObject* e, BaseSceneObject* parent, int& typeID, int& parentTypeID);
		static char AddProp(std::string name, int offset, InspectableType type, int ID);

		static Vec<BaseSceneObject*>* GetTypes();
		static Vec<ReflectedProp*>* GetVarsFromType(int ID);

		static const std::string GetTypeName(int ID);
		static const std::string GetTypeName(BaseSceneObject*);

		static BaseSceneObject* GetType(std::string type_name);

		template<typename T>
		static T* GetVarFromObject(std::string name, BaseSceneObject* obj)
		{
			if (!obj || name.empty())
				return nullptr;

			int ID = ((BaseSceneObject*)obj)->GetTypeID(); //Get the static type ID from the BaseObject derived
			if (!&(*s_reflectionDataList)[ID])
				return nullptr;

			Vec<ReflectedProp*>* vars = &(*s_reflectionDataList)[ID]->vars; //Get the variables metadata list

			for (int i = 0; i < vars->GetLength(); i++)
			{
				if ((*vars)[i]->name == name) //iterate to find the variable with the same name
				{
					return (T*)((char*)obj + (*vars)[i]->offset); //add the offset to the object pointer to find the needed variable in memory
				}
			}

			return nullptr; //safely return nullptr if there are no variables with the requested name
		}

	private:
		static Vec<BaseSceneObject*>* s_types; //this keeps copies of the objects
		static Vec<ReflectedTypeData*>* s_reflectionDataList; //this stores per-type metadata
	};
}

using namespace RG;
#define REFLECTABLE_CLASS(x, parent)  protected: static int s_parentTypeID; protected: static int s_TypeID; static int s_Size;  static char x##_adder; public: typedef parent Super; virtual int GetTypeID() override {return s_TypeID;} virtual BaseSceneObject* GetCopy(char* binary) {return new x##(*(x##*)binary);} virtual int GetSize() override {return s_Size;} static int s_GetTypeID() {return s_TypeID;}
#define REFLECT_REGISTER_TYPE(x) int x##::s_parentTypeID = 0; int x##::s_TypeID = 0; char x::x##_adder = ReflectionManager::AddType(#x, new x, new x##::Super, s_TypeID, s_parentTypeID); int x##::s_Size = sizeof(x);
#define REFLECT_PROP(type, x, v) static char  v##_in_##x##      = ReflectionManager::AddProp(#v, offsetof(x, v), type, x##::s_GetTypeID());
#define REFLECTED_PRIV_DECL(type, x, v) static char v##_in_##x##;
#define REFLECT_PRIV_PROP(type, x, v) char x##::##v##_in_##x##  = ReflectionManager::AddProp(#v, offsetof(x, v), type, x##::s_GetTypeID());