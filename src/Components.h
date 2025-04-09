#pragma once

#include "Scene/Scene.h"
#include "Maths/Vec2.h"
#include "Maths/Vec3.h"

namespace RG
{
	class Component
	{
	protected:
	public:
		Component() : m_parentPtr(nullptr) {}
		virtual ~Component() {}

		virtual void Start() {}
		virtual void Tick() {}

		Entity* GetParent() { return m_parentPtr; }
	protected:
		friend class Entity;
		template<typename T>
		Component* GetComponent()
		{

		}

		Entity* m_parentPtr;
	};

	class Texture;
	class TextureComponent : public Component
	{
	public:
		TextureComponent(const char* path);

		void SetTexture(const char* path);
		void SetTexture(Texture*);
		void Bind();
	private:
		Texture* m_texture;
	};

	class Transform : public Component
	{
	public:
		void Translate(Vec3 vec) { Position += vec; }
		void Rotate(Vec3 vec) { Rotation += vec; }
		void Scale(Vec3 vec) { LocalScale += vec; }

		Vec3 Position, Rotation, LocalScale;
	};

	class Tilemap : public Component
	{
	public:

		Vec<Texture*> textures;
	private:

	};
}
