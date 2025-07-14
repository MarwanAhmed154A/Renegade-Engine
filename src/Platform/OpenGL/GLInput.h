#pragma once

#include "Input/Input.h"
//#include "Maths/Vec2.h"

class GLFWwindow;

namespace RG
{
	class GLInput : public Input
	{
	public:
		GLInput(RGWindow* window);

		virtual void UpdateImpl() override;
		virtual	bool GetKeyImpl(unsigned int keyCode) override;
		virtual bool GetKeyPressedImpl(unsigned int keyCode) override;
		virtual void SetMouseStateImpl(MouseState state) override;
		virtual int  GetAxisImpl(AxisCodes axisCode) override;
		virtual Vec2 GetCursorPosImpl() override;
	private:
		Key* glKeys;
		GLFWwindow* window_handle;
	};
}