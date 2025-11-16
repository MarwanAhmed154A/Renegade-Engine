#pragma once

#include "Vec.h"
#include "Maths/Vec2.h"

#include <memory>

namespace RG
{
	class CallbackBase
	{
	public:
		//Call member func pointer and pass event with info as parameter
		virtual void DoFunc(class Event* e) = 0;
	};

	//Funcs have to be in .h because of template

	class StaticCallback : public CallbackBase
	{
	public:
		void(*func)(class Event* e);

		StaticCallback() {}
		StaticCallback(void(*func)(struct Event* e))
		{
			this->func = func;
		}

		virtual void DoFunc(struct Event* e) override
		{
			(*func)(e);
		}
	};

	template<typename T>
	class Callback : public CallbackBase
	{
	public:
		void(T::* func)(class Event* e);
		T* object;

		Callback() {}
		Callback(T* obj, void(T::* func)(struct Event* e))
		{
			object = obj;
			this->func = func;
		}

		virtual void DoFunc(struct Event* e) override
		{
			(object->*func)(e);
		}
	};

	enum EventType
	{
		OnRender, OnBegin, OnRenderUI, OnWindowResize, OnSceneInit, OnRigidbodyCreated
	};

	struct Event
	{
	public:
		void Invoke(Event* e);
		int type;

		/// <summary>
		/// Use to cast to specific needed event type to get event data safely
		/// </summary>
		/// <typeparam name="T">required Event based type to cast to</typeparam>
		/// <returns>safe pointer of the required type, nullptr if cast is unsafe</returns>
		template<typename T>
		T* SafeCastTo()
		{
			T t;

			if (((Event*)&t)->type == type)
				return (T*)this;

			return nullptr;
		}
	protected:
		friend class EventManager;
	};

	struct OnRenderEvent : public Event
	{
	public:
		OnRenderEvent();
		
		Vec<class Entity*>* vec;
	};

	struct OnWindowResizeEvent : public Event
	{
	public:
		OnWindowResizeEvent();

		Vec2 size;
	};

	struct OnSceneInitEvent : public Event
	{
		OnSceneInitEvent();

		Vec<class Entity*>* vec;
	};

	struct OnRigidbodyCreatedEvent : public Event
	{
		OnRigidbodyCreatedEvent();

		class Rigidbody* body;
	};

	class EventManager
	{
	public:
		static void Init();

		template<typename T>
		static void AddCallback(int EventType, T* obj, void(T::* func)(struct Event* e))
		{
			//instead of making the callback ptr manually, the function takes all the parameters and does it all at once
			(*callbacks)[EventType].PushMove(std::make_unique<Callback<T>>(obj, func));
		}

		static void AddStaticCallback(int EventType, void(*func)(struct Event* e))
		{
			//instead of making the callback ptr manually, the function takes all the parameters and does it all at once
			(*callbacks)[EventType].PushMove(std::make_unique<StaticCallback>(func));
		}

		static void Invoke(Event* e);

		static Vec<Vec<std::unique_ptr<CallbackBase>>>*& GetCallbacks() { return callbacks; }
	private:
		static Vec<Vec<std::unique_ptr<CallbackBase>>>* callbacks;
	};
}