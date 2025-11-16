#pragma once

#include "Vec.h"
#include "Maths/Vec3.h"
#include <string>

namespace RG
{
	struct KeyPosition
	{
		Vec3 pos;
		double timeStamp;
	};

	struct KeyRotation
	{
		Vec3 rot;
		double timeStamp;
	};

	struct KeyScale
	{
		Vec3 scale;
		double timeStamp;
	};

	struct NodeAnimation
	{
		std::string nodeName;
		Vec<KeyPosition> positions;
		Vec<KeyRotation> Rotations;
		Vec<KeyScale>    scales;
	};

	struct Animation
	{
		std::string name;
		double duration; //in ticks
		double ticksPerSec;
		Vec<NodeAnimation> channels;
	};
}