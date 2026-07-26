#include "Window.h"

namespace arx {

IWindow* g_window = nullptr;

void setWindow(IWindow* window)
{
    g_window = window;
}

} // namespace arx
