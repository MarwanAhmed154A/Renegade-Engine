#include "Time.h"

#include "Platform/OpenGL/GLTime.h"

RG::Time* RG::Time::s_handle;

namespace RG
{
	void Time::Init()
	{
		s_handle = new GLTime;
	}

	void Time::OnUpdateBegin()
	{
		s_handle->OnUpdateBeginImpl();
	}

	void Time::OnUpdateEnd()
	{
		s_handle->OnUpdateEndImpl();
	}

	double Time::GetTime()
	{
		return s_handle->GetTimeImpl();
	}

	double Time::GetDeltaTime()
	{
		return s_handle->GetDeltaTimeImpl();
	}
}