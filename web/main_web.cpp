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
    // Silence the game's informational console output (std::cout). There are
    // hundreds of debug prints across the code, and console output is slow in
    // a browser. Errors (std::cerr) still show. Build with -DARX_LOGS=1 to
    // turn the full log back on when debugging.
#if !defined(ARX_LOGS) || !ARX_LOGS
    std::cout.setstate(std::ios_base::failbit);
#endif
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