#include "Components.h"
#include "Scene/Entity.h"
#include "Scene/SceneManager.h"
#include "Renderer/Texture.h"
#include "Header Tool/ReflectionManager.h"
#include "Platform/OpenGL/GLTexture.h"
#include "Model.h"

REFLECT_REGISTER_TYPE(Component)
REFLECT_REGISTER_TYPE(Tilemap)


REFLECT_REGISTER_TYPE(TextureComponent)
REFLECT_PRIV_PROP(InspectableType::Asset, TextureComponent, m_texture)

REFLECT_REGISTER_TYPE(ModelComponent)
REFLECT_PRIV_PROP(InspectableType::Asset, ModelComponent, m_model)

REFLECT_REGISTER_TYPE(Transform)
REFLECT_PROP(InspectableType::Vec3, Transform, Position)
REFLECT_PROP(InspectableType::Vec3, Transform, Rotation)
REFLECT_PROP(InspectableType::Vec3, Transform, LocalScale)

REFLECT_REGISTER_TYPE(CubemapComponent)
REFLECT_PROP(InspectableType::Asset, CubemapComponent, map)

namespace RG
{
	TextureComponent::TextureComponent()
	{
		m_texture = std::make_shared<GLTexture>(GLTexture());
	}

	TextureComponent::TextureComponent(const char* path) : m_texture(Texture::CreateTexture(path))
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
		if (m_texture)
			m_texture->Bind();
	}

	ModelComponent::ModelComponent()
	{
		m_model = std::make_shared<Model>(Model());
	}

	ModelComponent::ModelComponent(const char* path) : m_model(std::make_shared<Model>(Model(path)))
	{

	}

	void ModelComponent::Bind()
	{
		if (m_model)
			m_model->Draw();
	}

	void ModelComponent::Draw(Shader* shader)
	{
		if (m_model)
			m_model->Draw(shader);
	}
}

