#include <glc2d.h>

#include <cstdio>

#include "GameApplication.h"

#if defined(_DEBUG)
    #if defined(_M_X64)
        #pragma comment(lib, "glc2d_x64_debug.lib")
    #elif defined(_M_IX86)
        #pragma comment(lib, "glc2d_win32_debug.lib")
    #endif
#else
    #if defined(_M_X64)
        #pragma comment(lib, "glc2d_x64_release.lib")
    #elif defined(_M_IX86)
        #pragma comment(lib, "glc2d_win32_release.lib")
    #endif
#endif

int main()
{
    GameApplication application;

    if (!application.Initialize())
    {
        return 1;
    }

    const int runResult = application.Run();
    application.Shutdown();

    if (runResult != 0)
    {
        std::fprintf(stderr, "glc2d game loop failed: %d\n", runResult);
        return 1;
    }

    return 0;
}
