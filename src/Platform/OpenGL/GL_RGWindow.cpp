#include "GL_RGWindow.h"
#include "Log.h"
#include "Events/Event.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

float R1, G1, B1;

namespace RG
{
	void WindowResizeCallback(GLFWwindow* window, int width, int height);

	GL_RGWindow::GL_RGWindow(int width, int height, const char* name)
	{
		window = glfwCreateWindow(width, height, name, NULL, NULL);
		if (window == NULL)
		{
			RG_CORE_ERROR("FAILED TO CREATE GLFW WINDOW");
			glfwTerminate();
		}

		glfwMakeContextCurrent(window);
		//Set callback for when window size changes
		glfwSetWindowSizeCallback(window, WindowResizeCallback);

		//save window size for later use
		size = Vec2(width, height);
	}

	void GL_RGWindow::Init()
	{
	}

	void GL_RGWindow::HandleEvents()
	{
		glfwPollEvents();
	}

	void GL_RGWindow::Clear(float R, float G, float B)
	{
		glClearColor(R, G, B, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	bool GL_RGWindow::ShouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void GL_RGWindow::SetColors(Event* e)
	{

	}

	void* GL_RGWindow::GetNativeWindow()
	{
		return window;
	}

	void GL_RGWindow::Update()
	{
		glfwSwapBuffers(window);
	}

	void WindowResizeCallback(GLFWwindow* window, int width, int height)
	{
	#ifdef _NDEBUG
		OnWindowResizeEvent event;
		event.size = Vec2(width, height);

		EventManager::Invoke(&event);
		OnWindowResizeEvent event;
		event.size = Vec2(width, height);

		EventManager::Invoke(&event);
	#endif // DEBUG
	}
}