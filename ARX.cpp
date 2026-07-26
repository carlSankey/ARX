#include <cstdlib>

#include "platform/AppLoop.h"

int main()
{
    if (!appInit()) {
        return EXIT_FAILURE;
    }

    while (appIsRunning()) {
        if (!appTick()) {
            break;
        }
    }

    appShutdown();
    return 0;
}
