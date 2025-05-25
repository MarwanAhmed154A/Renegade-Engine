#pragma once


class GLFWwindow;
//#include "Maths/Vec2.h"

namespace RG
{
	class RGWindow;

	enum KeyCodes
	{
		W, A, S, D, UP, DOWN, LEFT, RIGHT, ESC, ENTER
	};

	enum class KeyState
	{
		RELEASED, PRESSED, REPEATED, WAITING
	};

	enum class MouseState
	{
		NORMAL, HIDDEN
	};

	enum AxisCodes
	{
		Horizontal, Vertical
	};

	struct Key
	{
		public:
			//Key() : state(KeyState::RELEASED){}
			KeyState state;
			unsigned int keyCode;
			double pressTime;
	};

	class Input
	{
	public:
		static void Init(RGWindow* window);

		static void Update();
		static bool GetKey(unsigned int keyCode);
		static bool GetKeyPressed(unsigned int keyCode);
		static void SetMouseState(MouseState state);
		static int GetAxis(AxisCodes axisCode);
		//static Vec2 GetCursorPos();
	protected:
		virtual void UpdateImpl() = 0;
		virtual	bool GetKeyImpl(unsigned int keyCode) = 0;
		virtual bool GetKeyPressedImpl(unsigned int keyCode) = 0;
		virtual void SetMouseStateImpl(MouseState state) = 0;
		virtual int  GetAxisImpl(AxisCodes axisCode) = 0;
		//virtual Vec2 GetCursorPosImpl() = 0;
	private:
		static Input* s_handle;
		static MouseState mouseState;
	};
}