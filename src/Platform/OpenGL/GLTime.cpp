#include "GLTime.h"

#include <GLFW/glfw3.h>

namespace RG
{
	void GLTime::OnUpdateBeginImpl()
	{
		beginTime = glfwGetTime();
	}

	void GLTime::OnUpdateEndImpl()
	{
		endTime = glfwGetTime();
		deltaTime = endTime - beginTime;
	}

	double GLTime::GetTimeImpl()
	{
		return glfwGetTime();
	}

	double GLTime::GetDeltaTimeImpl()
	{
		return deltaTime;
	}
}