#include "Application.h"

#if defined(MG_RELEASE)
#include <Windows.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	maze::Application app;
	app.Run();
}
#elif defined(MG_DEBUG)

int main()
{
	maze::Application app;
	app.Run();
}
#endif