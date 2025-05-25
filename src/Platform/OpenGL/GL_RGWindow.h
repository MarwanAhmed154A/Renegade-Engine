#pragma once

#include "Renderer/RGWindow.h"

class GLFWwindow;

namespace RG
{
	class GL_RGWindow : public RGWindow
	{
	public:
		GL_RGWindow(int width, int height, const char* name);

		virtual void Init() override;
		virtual void HandleEvents() override;
		virtual void Update() override;
		virtual void Clear(float R, float G, float B) override;
		virtual bool ShouldClose() override;
		virtual void* GetNativeWindow() override;

		void SetColors(class Event* e);
	private: 
		GLFWwindow* window;

	};
}