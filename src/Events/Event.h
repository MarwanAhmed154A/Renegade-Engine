#pragma once

#include "Vec.h"
#include "Maths/Vec2.h"

namespace RG
{
	class BaseFuncPtr
	{
	public:
		//Call member func pointer and pass event with info as parameter
		virtual void DoFunc(class Event* e) = 0;
	};

	//Funcs have to be in .h because of template

	template<typename T>
	class FuncPtr : public BaseFuncPtr
	{
	public:
		void(T::* func)(class Event* e);
		T* object;

		FuncPtr() {}
		FuncPtr(T* obj, void(T::* func)(class Event* e))
		{
			object = obj;
			this->func = func;
		}

		virtual void DoFunc(class Event* e) override
		{
			(object->*func)(e);
		}
	};

	enum EventType
	{
		OnRender, OnBegin, OnRenderUI, OnWindowResize
	};

	struct Event
	{
	public:
		BaseFuncPtr*& operator[](int x) { return ptrs[x];}
		void Invoke(Event* e);
		int type;
	protected:
		friend class EventManager;
		//function pointer vector
		Vec<BaseFuncPtr*> ptrs;
	};

	struct OnRenderEvent : public Event
	{
	public:
		OnRenderEvent();
		
		Vec<class Entity*>* vec;
	};

	class OnWindowResizeEvent : public Event
	{
	public:
		OnWindowResizeEvent();

		Vec2 size;
	};

	class EventManager
	{
	public:
		static void Init();
		static void AddCallback(int EventType, BaseFuncPtr* func);
		static void Invoke(Event* e);
	private:
		static Vec<Event*> events;
	};
}
