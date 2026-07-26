#pragma once

// Global platform window instance.
// All code that previously used the SFML global "App" should use g_window.
//
// Native:      set by appInit() via createPlatformWindow() (SDL2Window)
// Emscripten:  same
// Legacy SFML: display.cpp still owns sf::RenderWindow App for sprite drawing;
//              g_window wraps it via SFMLWindow shim until SFML is fully removed.

#include "PlatformWindow.h"

namespace arx {

// The one live window for this process.
extern IWindow* g_window;

// Set the window. Called once by appInit() before any rendering.
void setWindow(IWindow* window);

} // namespace arx
