# ARX – Step 2: Native desktop build of the SDL2 version (CMake + vcpkg)

## Background

ARX is a C++ remake of *Alternate Reality*. The `arxweb` branch has moved the game to **SDL2 + SDL_mixer + an OpenGL ES 2.0-style renderer** (`renderer/View3D.cpp`, `renderer/Sprite2D.cpp`). It builds for the browser with Emscripten via `web/build.cmd`, and code specific to that path is switched on by `ARX_USE_SDL2`.

The old Visual Studio project `ARXX.vcxproj` does **not** define `ARX_USE_SDL2`, so it compiles the retired SFML code paths and fails with hundreds of errors (`deathMusic`, `App`, `SDL_mixer.h` not found, `gluErrorString`/`APIENTRY`, etc.). **Do not try to fix `ARXX.vcxproj`.**

Instead, create a new CMake build that compiles the **same SDL2 code the web build uses**, as a native Windows program. This build will also be the starting point for the Android build later, so keep it clean and portable.

## Ground rules

- Start from the `platform-layer` branch and create a new branch: `desktop-build`.
- Work in the stages below, **in order**. After each stage:
  1. Make sure it configures/compiles (or explain exactly what's blocking you).
  2. Commit with a message like `Stage 1: add CMake desktop build`.
  3. **Stop** and give Carl a short summary: files changed, anything you were unsure about, and what to try. Wait for his OK before continuing.
- **Do not change gameplay, timing, text or visuals.** This is a build task only.
- **Do not edit or delete the SFML code paths** (`#ifndef ARX_USE_SDL2` blocks), `ARXX.vcxproj`, `3Dview.cpp` or `3dviewnew.cpp`. They'll be removed in a later step.
- **Do not break the web build.** Leave `web/build.cmd`, `web/CMakeLists.txt`, `web/main_web.cpp` and `web/web_stubs.cpp` unchanged. Any fix you make to shared game files must also work under Emscripten.
- Never edit anything under `data/`.
- If you're unsure about something, ask rather than guess.

## Stage 1 – CMake project, presets and dependencies

1. **Shared source list.** Create `cmake/ArxSources.cmake` defining a variable `ARX_GAME_SOURCES` containing exactly the game and platform `.cpp` files that `web/CMakeLists.txt` compiles (everything except `web_stubs.cpp` and `main_web.cpp`). Compare it against `web/build.cmd` too; if the two lists differ, report the differences instead of guessing.

2. **Root `CMakeLists.txt`** (C++20) with a target `arxx`:
   - Sources: `${ARX_GAME_SOURCES}` plus `web/main_web.cpp` and `web/web_stubs.cpp`. `main_web.cpp` already has a native `while (appIsRunning())` loop for non-Emscripten builds, so reuse it; we'll rename it later.
   - `target_compile_definitions(arxx PRIVATE ARX_USE_SDL2=1)`.
   - Include the repo root as an include directory.
   - Link SDL2, SDL2main (Windows), SDL2_image and SDL2_mixer using `find_package(... CONFIG REQUIRED)` from vcpkg.
   - A post-build step that copies (or symlinks) the `data/` folder next to the executable, so the game finds its files when run from Visual Studio or Explorer.
   - Keep anything Windows-specific inside `if(WIN32)`.

3. **`CMakePresets.json`** with a `windows-debug` preset (and `windows-release`) that uses the vcpkg toolchain file at `${sourceDir}/vcpkg/scripts/buildsystems/vcpkg.cmake`, a binary dir of `build/<preset-name>`, and the x64 target. Visual Studio's "Open Folder" should pick this up directly.

4. **Dependencies.** Update `vcpkg.json` so it includes what this build needs: `sdl2`, `sdl2-image` (PNG support), and `sdl2-mixer` with **OGG Vorbis** playback (the music is `.ogg`; check which vcpkg feature provides it). Don't remove the existing entries (`sfml`, `bgfx`, `glew`, …) yet. Just list in your summary which ones the new build doesn't use.

5. Add `build/`, `vcpkg/` and `vcpkg_installed/` to `.gitignore` if they aren't already.

**Stop here** and tell Carl the exact commands to configure and build, and what the first run of vcpkg will download.

## Stage 2 – Get it compiling

Fix compile and link errors **for the `ARX_USE_SDL2` build only**, with the smallest possible changes:

- Files using `arx::sleep` / `arx::seconds` / `arx::Clock` must `#include "platform/ArxClock.h"` themselves rather than relying on another header to pull it in.
- Add missing standard headers such as `<vector>` where they're used.
- Emscripten-only calls (`emscripten_sleep`, `<emscripten/...>`) must stay inside `#ifdef __EMSCRIPTEN__`. Where a native build needs an equivalent (e.g. `emscripten_sleep(1000)` → `arx::sleep(arx::seconds(1))`), put it in the `#else` branch so the web behaviour is untouched.
- If a file includes `GL/glu.h` or `GL/glew.h` in code the SDL2 build compiles, report it before changing anything.
- `platform/platform.cpp` (the `plat::` layer from step 1) is not used by anything yet. Leave it out of this build and mention that in your summary.

Keep a running list of every file you touch and why. Include it in the stage summary.

## Stage 3 – OpenGL on the desktop

The renderer was written for WebGL/OpenGL ES (`SDL_opengles2.h`, `precision mediump float` in the shaders), but `platform/SDL2Window.cpp` currently requests a desktop **OpenGL 3.3 Core** context on native builds. Those shaders won't compile in a core profile, and the ES function names aren't loaded automatically on Windows.

Because phones use OpenGL ES, the desktop build should match them as closely as possible:

1. On native (non-Emscripten) builds, request an **OpenGL ES 3.0 context** through SDL (`SDL_GL_CONTEXT_PROFILE_ES`, major 3, minor 0). This matches WebGL2 in the browser and what Android will use.
2. Make sure the GL ES functions are loaded on Windows, for example with a GLES loader such as glad (generated for GLES 2.0/3.0, loaded with `SDL_GL_GetProcAddress`). Do this in one place, right after the context is created.
3. Check `renderer/Sprite2D.cpp`, which includes `GL/glew.h` for native. Make sure the SDL2 build uses the same loader and doesn't mix GLEW with the GLES headers.
4. **If creating an ES context fails on Carl's graphics card**, stop and explain the options (for example, using ANGLE from vcpkg, which provides OpenGL ES on top of DirectX). Don't switch approach without asking.

## Stage 4 – Run it and document it

1. Run the game and check it reaches the title screen, then character creation, then the City.
2. Add a "Building on Windows (desktop)" section to `README.md` with the exact steps: installing vcpkg into `vcpkg/`, the CMake preset commands, how to open the folder in Visual Studio, and where the `.exe` ends up.
3. Report anything that looks or sounds different from the web build.

## Out of scope (later steps)

- Removing SFML code, `ARXX.vcxproj` or the old 3D view.
- Touch controls, Android build, and the data/JSON format work.

## Test checklist for Carl

- The game starts and shows the title screen and logo.
- Character creation works, the city gate sounds play, and the stat pauses feel right.
- You can walk around the City, and the 3D view looks the same as the web version.
- Music plays in the Tavern and Guild, and stops when you leave.
- The Bank's one-second pauses are still there.
- An encounter starts and plays at its normal pace.
