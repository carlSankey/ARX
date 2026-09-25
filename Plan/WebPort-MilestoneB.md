# ARXX Browser Port - Milestone B (loop abstraction)

## Added

- `platform/AppLoop.h` — `appInit`, `appTick`, `appShutdown`, `AppState` enum.
- `platform/AppLoop.cpp` — native menu/load/game entry; Emscripten stub.
- `ARX.cpp` — thin `main()` calling the app loop API.
- `web/main_web.cpp` — uses the same `appTick()` via `emscripten_set_main_loop`.

## States (outer flow only)

| State | Meaning |
|-------|---------|
| `Boot` | Before init completes |
| `MainMenu` | Title menu (one frame per `appTick`) |
| `LoadGameMenu` | Load slot picker (one frame per `appTick`) |
| `InGame` | One `gameTick()` per `appTick()` (see Milestone C1) |
| `Shutdown` | Quit |

Inner gameplay/location menus (`tavern`, `goblins`, etc.) are unchanged inside `gameLoop()`.

## Build

- Native: rebuild `ARXX.sln` (includes `platform\AppLoop.cpp`).
- Web: `web\build.cmd` (links `AppLoop.cpp` with `-I..`).

## Web console (expected)

- `ARXX web: appInit (stub — render/input not ported yet)`
- `ARXX web: appTick frame=0 state=MainMenu` (every ~300 frames)
