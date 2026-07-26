#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

#include "../platform/AppLoop.h"
#include "../platform/InputQueue.h"
#include "../display.h"

#ifdef ARX_USE_SDL2
#include "../renderer/View3D.h"
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

// ============================================================
// Web Entry Point
// ============================================================

void browserFrame()
{
    if (!appTick()) {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        appShutdown();
    }
}

int main()
{
    std::cout << "ARXX web scaffold booted." << std::endl;

    if (!appInit()) {
        return 1;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(browserFrame, 0, 1);
#else
    while (appIsRunning()) {
        if (!appTick()) {
            break;
        }
    }
    appShutdown();
#endif

    return 0;
}