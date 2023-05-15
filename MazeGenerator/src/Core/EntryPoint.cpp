
// Ruan C. Keet (2022)
// EntryPoint.cpp

#include "Application.h"

#if defined(MG_RELEASE)
#include <Windows.h>

// Entry point to the program specifically for Windows platforms.
//
// @return The exit status of the program
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	maze::Application app;
	app.Run();
}
#elif defined(MG_DEBUG)

// Entry point to the program.
//
// @retun The exit status of the program.
int main()
{
	maze::Application app;
	app.Run();
}
#endif