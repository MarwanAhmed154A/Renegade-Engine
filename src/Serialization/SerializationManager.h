#pragma once

#include <string>
#include "Vec.h"

namespace RG
{
	class Entity;
	class SerializationManager
	{
	public:
		static void Init();
		static void Save(std::string file);
		static void Load(std::string file);
	private:
		void OnSceneInitCallback(struct Event*);

		static inline void SaveString(std::string str, std::ofstream& file);
		static inline std::string LoadStringCurSize(std::ifstream& file);

		static std::vector<Entity*>* ents;
	};
}