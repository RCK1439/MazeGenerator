/*
* Ruan C. Keet (2022)
* EntryPoint.cpp
*/

#include "Application.h"
#include "Platform.h"

#if defined(MG_RELEASE) && defined(MG_PLATFORM_WINDOWS)
#include <Windows.h>

/**
* \brief Entry point to the application for 64-bit Windows platforms.
* 
* \return The exit status of the program.
*/
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	maze::Application app;
	app.Run();
}
#elif defined(MG_DEBUG) || defined(MG_PLATFORM_LINUX)

/**
* \brief Entry point to the application.
* 
* \return The exit status of the program.
*/
int main()
{
	maze::Application app;
	app.Run();
}
#endif	// Configuration