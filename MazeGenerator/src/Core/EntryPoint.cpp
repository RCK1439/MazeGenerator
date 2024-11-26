#include "Application.h"
#include "Platform.h"

#if defined(MG_RELEASE) && defined(MG_PLATFORM_WINDOWS)
#include <minwindef.h>

static int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    maze::Application app;
    app.Run();
}
#elif defined(MG_DEBUG) || defined(MG_PLATFORM_LINUX)

int main()
{
    maze::Application app;
    app.Run();
}
#endif	// Configuration