#pragma once


class GLFWwindow;
//#include "Maths/Vec2.h"

namespace RG
{
	class RGWindow;
	class Vec2;

	enum KeyCodes
	{
		W, A, S, D, UP, DOWN, LEFT, RIGHT, ESC, ENTER, LEFT_CTRL, R, X
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
		static MouseState GetMouseState();
		static void SetMouseState(MouseState state);
		static int  GetAxis(AxisCodes axisCode);
		static Vec2 GetCursorPos();
	protected:
		virtual void UpdateImpl() = 0;
		virtual	bool GetKeyImpl(unsigned int keyCode) = 0;
		virtual bool GetKeyPressedImpl(unsigned int keyCode) = 0;
		virtual void SetMouseStateImpl(MouseState state) = 0;
		virtual int  GetAxisImpl(AxisCodes axisCode) = 0;
		virtual Vec2 GetCursorPosImpl() = 0;
		static MouseState mouseState;
	private:
		static Input* s_handle;
	};
}