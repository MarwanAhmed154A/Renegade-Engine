#pragma once

#include "Scene/Scene.h"
#include "Maths/Vec2.h"
#include "Maths/Vec3.h"
#include "BaseSceneObject.h"
#include "Header Tool/ReflectionManager.h"

#include "Platform/OpenGL/GLTexture.h"
//#include "Scene/Entity.h"

#define GEN(x, parent) friend class ReflectionManager; protected: static int s_parentTypeID; protected: static int s_TypeID; static int s_Size;  static char x##_adder; public: typedef parent Super; virtual int GetTypeID() override {return s_TypeID;} virtual BaseSceneObject* GetCopy(char* binary) {return new x##(*(x##*)binary);} virtual int GetSize() override {return s_Size;} static int s_GetTypeID() {return s_TypeID;}
#define ADDPRV(type, x, v) static int*;

namespace RG
{
	class Component : public BaseSceneObject
	{
	GEN(Component, BaseSceneObject)
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
		GEN(TextureComponent, Component)
		Texture* m_texture;

	public:
		TextureComponent();
		TextureComponent(const char* path);

		void SetTexture(const char* path);
		void SetTexture(Texture*);
		void Bind();
	};

	class Transform : public Component
	{
		GEN(Transform, Component)


		Transform() : Position(145, 643, 0) {}
	public:
		void Translate(Vec3 vec) { Position += vec; }
		void Rotate(Vec3 vec) { Rotation += vec; }
		void Scale(Vec3 vec) { LocalScale += vec; }

		Vec3 Position, Rotation, LocalScale;
	};

	class Tilemap : public Component
	{
		GEN(Tilemap, Component)
	public:
		Vec<Texture*> textures;
	private:
	};
}