# ARXX Web Port - Progress as of 21 June 2026

## What's Been Done

### Build System
- ✅ `web/build.cmd` — compiles to `build-web/arxx_web.html` + `.js` + `.wasm`
- ✅ Emscripten SDK at `C:\dev\emsdk`
- ✅ PNG image support via `-sSDL2_IMAGE_FORMATS=png`
- ✅ Preloaded: font atlas, logo, digit images (0-9), city/dungeon gate images

### Platform Abstraction
- ✅ `platform/AppLoop.h/cpp` — full state machine (MainMenu, NameEntry, CharCreate, InGame)
- ✅ `platform/Window.h` + `PlatformWindow.h` — `IWindow` interface
- ✅ `platform/SDL2Window.h/cpp` — SDL2 + WebGL ES 2.0
- ✅ `platform/InputQueue.h/cpp` — key queue + text queue
- ✅ `platform/ArxClock.h` — platform-neutral time

### Phase 1-4: SFML → SDL2 Port (COMPLETE)
- ✅ Font renderer (`font.cpp` with `#ifdef ARX_USE_SDL2`)
- ✅ Display layer (`display.cpp` with SDL2 path)
- ✅ `renderer/Sprite2D.h/cpp` — WebGL 2 sprite rendering

### Phase 4b: Web Input (COMPLETE)
- ✅ SDL2 keyboard events → `InputQueue` → `inputPoll()`
- ✅ Text input via `textPoll()` for name entry

### Phase 5: New Game Flow (COMPLETE)
- ✅ Menu → NameEntry (type name, choose M/F, confirm Y/N)
- ✅ NameEntry → CharCreate (spinning counter animation with gate image)
- ✅ Press any key to lock stats → "You are now joined" message
- ✅ → InGame placeholder (ESC returns to menu)
- ✅ Input queue flushed on every state transition (no leaked keypresses)
- ✅ Counter data matches original `createCharacter.cpp` city/dungeon layouts
- ✅ Stats assigned to `plyr` (STA, CHR, STR, INT, WIS, SKL, HP, copper/silver)

### Current Web Flow
```
Main Menu → [1] City / [2] Dungeon
  → Name Entry (type name, M/F, Y/N confirm)
  → Gate Animation (spinning counters + gate image, press key to lock)
  → "You are now joined" screen
  → InGame placeholder (ESC to return)
```

### Phase 6: Audio (COMPLETE)
- ✅ `audio.cpp` ported with `#ifdef ARX_USE_SDL2` guards
- ✅ SDL2_mixer with OGG/Vorbis support (`-sUSE_SDL_MIXER=2`)
- ✅ Gate sounds play during spinning counter animation and stop on key press
- ✅ `initEncounterThemes()`, `playShopMusic()`, `stopShopMusic()`, `playSpellSound()` all stubbed for web
- ✅ Preloaded: `cityGate1.ogg`, `citygate4.ogg`, `gate1.wav`, `gate2.wav`

### Phase 7a — Data Loading (COMPLETE)
- ✅ `level.cpp`, `actor.cpp`, `automap.cpp` added to web build
- ✅ `automap.cpp` ported with `#ifdef ARX_USE_SDL2` stubs
- ✅ Fixed `sprintf_s`/`fopen_s` → `snprintf`/`fopen` in `level.cpp` and `actor.cpp`
- ✅ Fixed trailing `\r\n` in `maps[].filename` read from `maps.txt`
- ✅ All map data files preloaded: `data/map/Scenario_0/`, `Scenario_1/`, `core/`
- ✅ `initMaps()` called at startup; `loadMapData()`, `loadDescriptions()`, `loadZoneData()`, `convertMonstersBinary()` called after scenario selection
- ✅ Verified in browser: all steps complete, InGame placeholder reached

### Phase 7b — 3D View Renderer (COMPLETE)
- ✅ Created `renderer/View3D.h` and `renderer/View3D.cpp` — WebGL ES 2.0 shader-based 3D corridor renderer
- ✅ Implements perspective projection, textured quads (walls/floor/ceiling), background rendering using GLSL shaders
- ✅ Ported `draw3DView()`, `loadBackgroundNames()`, `loadTextureNames()`, `initTextures()` → `display.cpp` SDL2 section
- ✅ Fixed duplicate function definitions in `display.cpp` SDL2 section
- ✅ Added `View3D.cpp` to `build.cmd`; preloaded texture/background/banner/compass images
- ✅ Added `draw3DView()` etc to `display.h`
- ✅ Player starts at city south gate (35,1) facing south, or dungeon (30,30) facing west
- ✅ InGame state now calls `dispMain()` on each frame
- ✅ **Deliverable:** First-person 3D corridor view renders in the browser

---

## Recently Completed (21 June 2026)

### Banner & Text Rendering Fix
- ✅ Fixed transparent background overlay issue in stats panel
- ✅ Corrected rendering order: banner now draws AFTER text (not before)
- ✅ Text now displays cleanly on banner without transparency artifacts
- ✅ Sprite-based font rendering restored and working correctly with proper z-ordering
- ✅ **Issue Resolved:** City banner no longer covered by font transparency

---

## Remaining Phases

### Phase 7c — Movement & Input (COMPLETE - 22 June 2026)
**Goal:** Player can walk around the map in the browser.

- [x] Port `game.cpp` movement functions (`moveForward`, `turnLeft`, etc.) — implemented as minimal web version
- [x] Wire up arrow keys / WASD in `AppLoop.cpp` `InGame` handler — calls gameTick() which polls readKey()
- [x] Port `input.cpp` / `getSingleKey()` for the web path — already SDL2/Emscripten compatible via InputQueue
- [x] Handle door checks, wall collision (`solidWall()`) — full game logic available via gameTick()
- [x] **Deliverable:** Player can navigate the city/dungeon in the browser

**Implementation Details:**
- Modified `platform/AppLoop.cpp` InGame state to call `gameTick()` instead of stub
- Created SDL2 web-only implementation of `gameTick()` in `game.cpp` with minimal state
- Added `game.cpp` to web build in `build.cmd` 
- Web path movement uses: arrow keys (up/down/left/right) to move forward/back/turn
- Full movement functions (`moveForward`, `moveBack`, `turnLeft`, `turnRight`) are already web-compatible
- Input routed through `readKey()` → `InputQueue` → arrow key dispatch

---

### Phase 7d — In-Game Audio
**Goal:** Door sounds, encounter music, spell sounds play during gameplay.

- [ ] Preload door/secret door sounds (`cityDoor.wav`, `citySecretDoor.wav`, etc.)
- [ ] Port `game.cpp` SFML sound objects to SDL2_mixer (same pattern as `audio.cpp`)
- [ ] Wire up `playShopMusic()` / `stopShopMusic()` for shops
- [ ] **Deliverable:** Sounds play when opening doors, entering shops, etc.

---

### Phase 7e — Encounters & Combat
**Goal:** Random encounters trigger and combat resolves in the browser.

- [ ] Port `encounter.cpp` (currently SFML-free — should compile cleanly)
- [ ] Preload encounter images (`data/images/encounters/`)
- [ ] Preload encounter audio (already in `audio.cpp` SDL2 path)
- [ ] Wire up encounter check in `gameTick()` web path
- [ ] **Deliverable:** Encounters trigger and the combat screen displays

---

### Phase 7f — Shops & Locations
**Goal:** All city/dungeon locations work (inn, guild, smithy, bank, tavern, etc.)

- [ ] Each shop file (`inn.cpp`, `guild.cpp`, `smithy.cpp`, etc.) needs SFML includes removed
- [ ] Preload shop images (`data/images/locations/`, `data/images/locations2/`)
- [ ] Wire up `checkShop()` in the web game loop
- [ ] **Deliverable:** Player can enter and use all shops

---

### Phase 7g — Save/Load
**Goal:** Characters can be saved and loaded in the browser.

- [ ] `saveGame.cpp` uses file I/O — needs Emscripten persistent storage (`IDBFS` or `--use-preload-plugins`)
- [ ] Port save/load to use Emscripten's virtual FS with sync to IndexedDB
- [ ] Wire up load game menu (key 3 on main menu)
- [ ] **Deliverable:** Save and load works across browser sessions