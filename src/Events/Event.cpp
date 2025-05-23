#include "Event.h"
#include "Log.h"

RG::Vec<RG::Event*> RG::EventManager::events;
namespace RG
{
	//Event
	void Event::Invoke(Event* e)
	{
		for (int i = 0; i < ptrs.GetLength(); i++)
			ptrs[i]->DoFunc(e);
	}
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//OnRenderEvent
	OnRenderEvent::OnRenderEvent()
	{
		type = EventType::OnRender;
	}
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//OnWindowResizeEvent
	OnWindowResizeEvent::OnWindowResizeEvent() : size(0, 0)
	{
		type = EventType::OnWindowResize;
	}
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//EventManager
	void EventManager::Init()
	{
		for (int i = 0; i < 4; i++)
			events.Push(new Event);
	}
	
	void EventManager::AddCallback(int EventType, BaseFuncPtr* func)
	{
		events[EventType]->ptrs.Push(func);
	}
	
	void EventManager::Invoke(Event* e)
	{
		for (int i = 0; i < events.GetLength(); i++)
			events[e->type]->Invoke(e);
	}
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
