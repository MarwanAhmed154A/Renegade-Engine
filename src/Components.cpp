#include "Components.h"
#include "Scene/Entity.h"
#include "Scene/SceneManager.h"
#include "Renderer/Texture.h"
#include "Header Tool/ReflectionManager.h"
#include "Platform/OpenGL/GLTexture.h"

ADD(Component)
ADD(TextureComponent)
ADD(Transform)
ADD(Tilemap)

ADDVAR(InspectableType::Vec3, Transform, Position);
ADDVAR(InspectableType::Vec3, Transform, Rotation);
ADDVAR(InspectableType::Vec3, Transform, LocalScale);

namespace RG
{
	TextureComponent::TextureComponent() : m_texture(new GLTexture())
	{
		
	}

	TextureComponent::TextureComponent(const char* path)  : m_texture(Texture::CreateTexture(path))
	{
		
	}

	void TextureComponent::SetTexture(const char* path)
	{
		m_texture = Texture::CreateTexture(path);
	}

	void TextureComponent::SetTexture(Texture* texture)
	{
		*m_texture = *texture;
	}

	void TextureComponent::Bind()
	{
		if(m_texture)
			m_texture->Bind();
	}
}