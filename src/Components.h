#pragma once

#include <memory>

#include "Scene/Scene.h"
#include "Maths/Vec2.h"
#include "Maths/Vec3.h"
#include "BaseSceneObject.h"
#include "Header Tool/ReflectionManager.h"
#include "Platform/OpenGL/GLTexture.h"
#include "Renderer/Cubemap.h"

//#include "Scene/Entity.h"

namespace RG
{
	class Component : public BaseSceneObject
	{
	REFLECTABLE_CLASS(Component, BaseSceneObject)
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
	private:
		REFLECTABLE_CLASS(TextureComponent, Component)

		std::shared_ptr<Texture> m_texture;
		REFLECTED_PRIV_DECL(InspectableType::Asset, TextureComponent, m_texture)
	public:
		TextureComponent();
		TextureComponent(const char* path);

		void SetTexture(const char* path);
		void SetTexture(Texture*);
		void Bind();
	};

	class Model;
	class ModelComponent : public Component
	{
	private:
		REFLECTABLE_CLASS(ModelComponent, Component)

		std::shared_ptr<Model> m_model;
		REFLECTED_PRIV_DECL(InspectableType::Asset, ModelComponent, m_model)
	public:
		ModelComponent();
		ModelComponent(const char* path);

		void SetModel(const char* path) {}
		void Bind();
		void Draw(class Shader*);
	};

	class Transform : public Component
	{
		REFLECTABLE_CLASS(Transform, Component)


		Transform() : Position(0, 0, 0), LocalScale(100, 100, 100) {}
	public:
		void Translate(Vec3 vec) { Position   += vec; }
		void Rotate(Vec3 vec)    { Rotation   += vec; }
		void Scale(Vec3 vec)     { LocalScale += vec; }

		Vec3 Position, Rotation, LocalScale;
	};

	class Tilemap : public Component
	{
		REFLECTABLE_CLASS(Tilemap, Component)
	public:
		Vec<Texture*> textures;
	private:
	};

	class CubemapComponent : public Component
	{
		REFLECTABLE_CLASS(CubemapComponent, Component)
	public:
		Cubemap* map;
	};
}