#include "Components.h"
#include "Scene/Entity.h"
#include "Scene/SceneManager.h"
#include "Renderer/Texture.h"

namespace RG
{
	TextureComponent::TextureComponent(const char* path)
	{
		m_texture = Texture::CreateTexture(path);
	}

	void TextureComponent::SetTexture(const char* path)
	{
		m_texture = Texture::CreateTexture(path);
	}

	void TextureComponent::SetTexture(Texture* texture)
	{
		m_texture = texture;
	}

	void TextureComponent::Bind()
	{
		if(m_texture)
			m_texture->Bind();
	}
}