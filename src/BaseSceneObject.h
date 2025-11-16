#pragma once

class Editor;

namespace RG
{
	class BaseSceneObject
	{
	protected:
		friend Editor;
		virtual void SetTypeID(int id)
		{ 
			s_TypeID = id;
		}
	public:
		virtual int GetTypeID()
		{
			return s_TypeID;
		}

		virtual int GetParentTypeID()
		{
			return s_TypeID;
		}

		virtual int GetSize()
		{
			return 0;
		}

		static int s_GetTypeID()
		{
			return s_TypeID;
		}

		static int s_GetParentTypeID()
		{
			return s_TypeID;
		}

		virtual BaseSceneObject* GetCopy(char* binary) {
			return new BaseSceneObject(*(BaseSceneObject*)binary);
		}

		virtual BaseSceneObject* GetDefaultCopy()
		{
			return new BaseSceneObject;
		}

		
		static int s_TypeID;
		friend class ReflectionManager;
	};
}