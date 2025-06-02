#include "Components.h"
#include "Scene/Entity.h"
#include "Scene/SceneManager.h"
#include "Renderer/Texture.h"
#include "Header Tool/ReflectionManager.h"
#include "Platform/OpenGL/GLTexture.h"

REFLECT_REGISTER_TYPE(Component)
REFLECT_REGISTER_TYPE(TextureComponent)
REFLECT_REGISTER_TYPE(Transform)
REFLECT_REGISTER_TYPE(Tilemap)

REFLECT_PRIV_PROP(InspectableType::Asset, TextureComponent, m_texture)

REFLECT_PROP(InspectableType::Vec3, Transform, Position)
REFLECT_PROP(InspectableType::Vec3, Transform, Rotation)
REFLECT_PROP(InspectableType::Vec3, Transform, LocalScale)

//RG::Transform* RG::Transform::Transform_for_Position_Loc = new Transform;

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