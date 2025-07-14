#pragma once

#include <string>
#include "Header Tool/ReflectionManager.h"

namespace RG
{
	class Asset : public BaseSceneObject
	{
		REFLECTABLE_CLASS(Asset, BaseSceneObject)
	public:
		Asset() {}
		std::string GetPath() { return path; }

		void SetPath(std::string new_path) { path = new_path; }
	protected:
		std::string path;
	};
}


//REFLECT_REGISTER_TYPE(Asset)