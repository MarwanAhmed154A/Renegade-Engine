#pragma once

#include "Vec.h"

//Just a convenience layer for working on the game, you can skip and deal with the core itself if needed

namespace std
{
	class string;
}

namespace RG
{
	class Entity;

	class EngineInterface
	{
	public:
		//World
		static Vec<Entity*> FindEntitiesWithTag();
		static Entity* FindEntityWithName();

		//Serialization
		static void Save(std::string file_name);
		static void Load(std::string file_name);
	};
}