#include "RGWindow.h"
#include "Platform/OpenGL/GL_RGWindow.h"

namespace RG
{
	RGWindow* RGWindow::Create(int width, int height, const char* name)
	{
		return new GL_RGWindow(width, height, name);
	}
	Vec2 RGWindow::GetSize()
	{
		return size;
	}
}