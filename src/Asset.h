#pragma once

#include <string>
#include "Header Tool/ReflectionManager.h"

namespace RG
{
	class Asset : public BaseSceneObject
	{
	public:
		std::string GetPath() { return path; }

		void SetPath(std::string new_path) { path = new_path; }
	protected:
		std::string path;
	};
}