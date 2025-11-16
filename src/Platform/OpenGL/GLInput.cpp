#include "GLInput.h"
#include "Time/Time.h"
#include "Renderer/RGWindow.h"

#include <GLFW/glfw3.h>

#define NUMBER_OF_KEYS 20

namespace RG
{
	GLInput::GLInput(RGWindow* window)
		: window_handle((GLFWwindow*)window->GetNativeWindow())
	{
		glKeys = new Key[NUMBER_OF_KEYS];
		glKeys[KeyCodes::W].keyCode         = GLFW_KEY_W;
		glKeys[KeyCodes::A].keyCode         = GLFW_KEY_A;
		glKeys[KeyCodes::S].keyCode         = GLFW_KEY_S;
		glKeys[KeyCodes::D].keyCode         = GLFW_KEY_D;
		glKeys[KeyCodes::UP].keyCode        = GLFW_KEY_UP;
		glKeys[KeyCodes::DOWN].keyCode      = GLFW_KEY_DOWN;
		glKeys[KeyCodes::LEFT].keyCode      = GLFW_KEY_LEFT;
		glKeys[KeyCodes::RIGHT].keyCode     = GLFW_KEY_RIGHT;
		glKeys[KeyCodes::ESC].keyCode       = GLFW_KEY_ESCAPE;
		glKeys[KeyCodes::ENTER].keyCode     = GLFW_KEY_ENTER;
		glKeys[KeyCodes::LEFT_CTRL].keyCode = GLFW_KEY_LEFT_CONTROL;
		glKeys[KeyCodes::R].keyCode         = GLFW_KEY_R;
		glKeys[KeyCodes::X].keyCode         = GLFW_KEY_X;
	}

	void GLInput::UpdateImpl()
	{
		for (int i = 0; i < NUMBER_OF_KEYS; i++)
		{
			if (glfwGetKey(window_handle, glKeys[i].keyCode))
				glKeys[i].state = KeyState::PRESSED;
			else
				glKeys[i].state = KeyState::RELEASED;
			//if (glfwGetKey(window_handle, glKeys[i].keyCode) && glKeys[i].state == KeyState::RELEASED)
			//{
			//	glKeys[i].state = KeyState::PRESSED;
			//	glKeys[i].pressTime = Time::GetTime();
			//}
			//else if (glfwGetKey(window_handle, glKeys[i].keyCode) && glKeys[i].state == KeyState::PRESSED && Time::GetTime() - glKeys[i].pressTime < .1)
			//	glKeys[i].state = KeyState::WAITING;
			//else if (glfwGetKey(window_handle, glKeys[i].keyCode) && glKeys[i].state == KeyState::WAITING && Time::GetTime() - glKeys[i].pressTime >= .1)
			//	glKeys[i].state = KeyState::REPEATED;
			//else if (!glfwGetKey(window_handle, glKeys[i].keyCode))
			//	glKeys[i].state = KeyState::RELEASED;
			//else
			//	continue;
		}
		
	}
	
	bool GLInput::GetKeyImpl(unsigned int keyCode)
	{
		return (glKeys[keyCode].state != KeyState::RELEASED && glKeys[keyCode].state != KeyState::WAITING);
	}

	bool GLInput::GetKeyPressedImpl(unsigned int keyCode)
	{
		return (glKeys[keyCode].state == KeyState::PRESSED);
	}

	void GLInput::SetMouseStateImpl(MouseState state)
	{
		if(state == MouseState::NORMAL)
			glfwSetInputMode(window_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	int GLInput::GetAxisImpl(AxisCodes axisCode)
	{
		int val = 0;
		if (axisCode == AxisCodes::Vertical)
		{
			if (GetKeyImpl(W))
				val = 1;
			else if (GetKeyImpl(S))
				val = -1;
		}
		else if (axisCode == AxisCodes::Horizontal)
		{
			if (GetKeyImpl(D))
				val = 1;
			else if (GetKeyImpl(A))
				val = -1;
		}
		return val;
	}

	Vec2 GLInput::GetCursorPosImpl()
	{
		double x, y;
		glfwGetCursorPos(window_handle, &x, &y);
		return Vec2((float)x, (float)y);
	}
}