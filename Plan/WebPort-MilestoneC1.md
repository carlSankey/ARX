# ARXX Browser Port - Milestone C1 (frame-driven game loop)

## Added

- `gameInit()` — start a play session (`Running`, timers, smithy sound state).
- `gameTick()` — one in-game frame (movement, render, encounters, input).
- `gameIsRunning()` — query `Running`.
- `gameLoop()` — thin wrapper: `gameInit()` + `while (gameTick())` (unchanged call sites if any).

## App loop wiring

- `enterGame()` calls `gameInit()` only (no blocking `gameLoop()`).
- `appTick()` in `InGame` calls `gameTick()` once per frame; on `false`, returns to `MainMenu`.

## Native behavior

Desktop still runs `while (appIsRunning()) { appTick(); }` — one menu frame or one game frame per tick. Submenus (`optionsMenu`, `playerDies`, shops, etc.) can still block inside a single `gameTick()` until a later milestone.

## Web

Browser build still uses the Emscripten stub in `AppLoop.cpp` (no `game.cpp` linked). Next: link game + platform layers or continue stub until render/input/FS are ready.

## Verify (native)

1. Rebuild Debug | Win32.
2. New game / load — explore, ESC options, quit to menu.
3. Die — death screen, return to menu.
4. Confirm main menu works after session ends.
5. Close game window from menu or in-game (no crash on exit).

## C1 bugfixes (post-smoke-test)

- **Load game:** removed erroneous `g_state = MainMenu` after `enterGame()`; only enter play when `newloadCharacter` succeeds.
- **Session end:** `gameEndSession()` stops smithy/death audio and clears `encounterRunning` when returning to menu.
- **Window close:** `App.isOpen()` checks + `QUIT` handling in `optionsMenu`/`quitMenu`/`readKey` to avoid polling a closed window.
