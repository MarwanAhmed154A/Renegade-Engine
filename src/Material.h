#pragma once

#include "Renderer/Texture.h"
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace RG
{
	struct Material
	{
		std::shared_ptr<Texture> diffuse, normal, specualr, height, metallicRoughness, alphaMap;
		aiColor4D baseColor;
		float roughness, metallic, specular, alpha;
	};
}