#include "Input.h"
#include "Renderer/RGWindow.h"
#include "Maths/Vec2.h"
#include "Platform/OpenGL/GLInput.h"

RG::Input* RG::Input::s_handle;
RG::MouseState RG::Input::mouseState;

namespace RG
{
	void Input::Init(RGWindow* window)
	{
		s_handle = new GLInput(window);
	}

	void Input::Update()
	{
		s_handle->UpdateImpl();
	}

	bool Input::GetKey(unsigned int keyCode)
	{
		return s_handle->GetKeyImpl(keyCode);
	}

	bool Input::GetKeyPressed(unsigned int keyCode)
	{
		return s_handle->GetKeyPressedImpl(keyCode);
	}

	MouseState Input::GetMouseState()
	{
		return mouseState;
	}

	void Input::SetMouseState(MouseState state)
	{
		mouseState = state;
		s_handle->SetMouseStateImpl(state);
	}

	int Input::GetAxis(AxisCodes axisCode)
	{
		return s_handle->GetAxisImpl(axisCode);
	}

	Vec2 Input::GetCursorPos()
	{
		return s_handle->GetCursorPosImpl();
	}
}