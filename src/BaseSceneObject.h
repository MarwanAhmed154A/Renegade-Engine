#pragma once

namespace RG
{
	class BaseSceneObject
	{
	public:
		virtual int GetTypeID()
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

		virtual BaseSceneObject* GetCopy(char* binary) {
			return new BaseSceneObject(*(BaseSceneObject*)binary);
		}

		static int s_TypeID;
	};
}