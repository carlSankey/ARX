#pragma once
// View3D.h - WebGL ES 2.0 / OpenGL ES 3.0 3D corridor renderer
// Replaces 3Dview.cpp for the web/SDL2 build.
// Uses shader-based rendering (no fixed-function pipeline).

#ifdef ARX_USE_SDL2

#include <string>

namespace arx {

// Load background image names from data/map/core/backgroundsUpdated.txt
// and upload them as GL textures.
void view3D_loadBackgrounds();

// Load wall/floor/ceiling texture names from data/map/core/texturesUpdated.txt
// and upload them as GL textures.
void view3D_loadTextures();

// Draw the full 3D corridor view for the current player position/facing.
// Reads plyr.x, plyr.y, plyr.facing, plyr.zoneSet, plyr.scenario etc.
void view3D_draw();

// Draw the background image (sky/dungeon backdrop) for the current zone/time.
void view3D_drawBackground();

// Release all GL resources.
void view3D_shutdown();

} // namespace arx

#endif // ARX_USE_SDL2
