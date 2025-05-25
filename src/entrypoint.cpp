#include "Application.h"
#include <Windows.h>

#ifdef NDEBUG
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	RG::Application* app = RG::CreateApp();
	app->Update();
}
#endif

#ifdef _DEBUG
int main()
{
	RG::Application* app = RG::CreateApp();
	app->Update();
}
#endif