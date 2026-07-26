// ============================================================
// Web Stubs for Missing Game Functions
// ============================================================
// This file provides stub implementations for functions not yet
// ported to the web build. These allow linking to succeed.

#include <string>
#include <ctime>
#include <cstdlib>

// Build timestamp - set at compile time
std::string g_buildId = "";

void initBuildId()
{
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    g_buildId = buffer;
}

std::string getBuildId()
{
    return g_buildId;
}

// 3D View - delegates to the View3D shader-based corridor renderer
#include "renderer/View3D.h"

void draw3DView()
{
    arx::view3D_draw();
}

// Game flow stubs
// Note: scenarioEntrance, clearGuildDisplay, drawEncounterView, and itos
// are now implemented in their respective source files

// Integer to string conversion stub
// Note: itos is now implemented in saveGame.cpp
