#pragma once

#include "Vec.h"
#include <string>
#include "BaseSceneObject.h"

class Editor;

namespace RG
{
	enum class InspectableType
	{
		Int, Float, String, Vec3, Asset, Bool
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
		static char AddType(std::string type_name, BaseSceneObject* e);
		static char AddProp(std::string name, int offset, InspectableType type, int ID);

		static Vec<BaseSceneObject*>*& GetTypes();
		static Vec<ReflectedTypeData*>*& GetTypesData();
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
					T* t = (T*)((char*)obj + (*vars)[i]->offset); //add the offset to the object pointer to find the needed variable in memory
					return t;
				}
			}

			return nullptr; //safely return nullptr if there are no variables with the requested name
		}

	private:
		friend Editor;
		friend class Application;
		static Vec<BaseSceneObject*>*   s_types; //this keeps copies of the objects
		static Vec<ReflectedTypeData*>* s_reflectionDataList; //this stores per-type metadata
	};
}

using namespace RG;
#define REFLECTABLE_CLASS(thisClass, parent)  protected: static int s_TypeID; static int s_Size;  virtual void SetTypeID(int id) {s_TypeID = id;} public: typedef parent Super; virtual int GetTypeID() override {return s_TypeID;} virtual int GetParentTypeID() override {return Super##::s_GetTypeID();} virtual BaseSceneObject* GetCopy(char* binary) {return new thisClass##(*(thisClass##*)binary);} virtual int GetSize() override {return s_Size;} static int s_GetSize() {return s_Size;} static int s_GetTypeID() {return s_TypeID;} static int s_GetParentTypeID() {return Super##::s_GetTypeID();} virtual BaseSceneObject* GetDefaultCopy() {return new thisClass##;} friend class ReflectionManager;
#define REFLECT_REGISTER_TYPE(thisClass) int thisClass##::s_TypeID = 0; __declspec(selectany) volatile char thisClass##_adder = ReflectionManager::AddType(#thisClass, new thisClass); int thisClass##::s_Size = sizeof(thisClass);
#define REFLECT_PROP(type, thisClass, thisProp) static char  thisProp##_in_##thisClass##      = ReflectionManager::AddProp(#thisProp, offsetof(thisClass, thisProp), type, thisClass##::s_GetTypeID());
#define REFLECTED_PRIV_DECL(type, thisClass, thisProp) static char thisProp##_in_##thisClass;
#define REFLECT_PRIV_PROP(type, thisClass, thisProp) char thisClass##::thisProp##_in_##thisClass##  = ReflectionManager::AddProp(#thisProp, offsetof(thisClass, thisProp), type, thisClass##::s_GetTypeID());