# ARX – Step 3: Android-safe file loading and saving

## Background

ARX now builds natively on Windows with CMake (`CMakeLists.txt`, `cmake/ArxSources.cmake`) using the SDL2 code path (`ARX_USE_SDL2`), and also builds for the web.

The next target is Android. On Android, the `data/` folder is packed **inside the APK**, which is read-only and invisible to normal C++ file access. `std::ifstream` and `fopen` cannot open `data/...` there. SDL's own file functions (`SDL_RWFromFile`) *can*, and SDL_image/SDL_mixer already use them, so images and sounds are fine.

This task routes **every other file read and write** through one small helper, so the same code works on Windows, the web, and Android. It is all done and tested on the desktop.

**This is a refactor. The game must play exactly the same afterwards.**

## Ground rules

- Create a new branch `file-access` from `desktop-build`.
- Work in the stages below, **in order**. After each stage: make sure the desktop build compiles and runs, commit (`Stage N: ...`), then **stop** and give Carl a short summary (files changed, anything unsure, what to test). Wait for his OK.
- Only change files in `${ARX_GAME_SOURCES}` (plus the new helper files). Don't touch `3Dview.cpp`, `3dviewnew.cpp`, `ARXX.vcxproj` or SFML-only code paths.
- Don't change file formats, file names or anything under `data/`.
- Don't break the web build. Everything must also compile under Emscripten.
- If an existing piece of code looks buggy, **report it; don't silently fix it.** The exception is where preserving the bug is impossible with the new helper; then explain what you did.

## Stage 0 – Tidy up the previous step

The pushed `desktop-build` branch only contains the Stage 1 commit, and `platform/SDL2Window.cpp` still requests an OpenGL 3.3 **Core** context. Carl's local build works, so:

1. Check `git status` for uncommitted work from step 2 (compile fixes, OpenGL ES context, GL loader, README). Commit it to `desktop-build` with a clear message and tell Carl to push it.
2. If the OpenGL ES context change from step 2 was never made, **tell Carl and stop.** Don't do it as part of this task.

## Stage 1 – Add the file helper

Create `platform/FileSystem.h` and `platform/FileSystem.cpp` (add the `.cpp` to `cmake/ArxSources.cmake`) in `namespace arx::fs`:

```cpp
namespace arx::fs {

// ---- Read-only game data (data/...) ----
// Uses SDL_RWFromFile, so it works on desktop, web and inside an Android APK.
// 'path' is the same relative path the game uses today, e.g. "data/map/core/items.txt".
bool readText(const std::string& path, std::string& out);
bool readBinary(const std::string& path, std::vector<uint8_t>& out);

// Convenience for existing line-by-line parsing code: returns a stream over
// the file contents. ok is set to false if the file couldn't be read.
std::istringstream openText(const std::string& path, bool& ok);

// ---- Writable user files (saves, settings) ----
// Returns an absolute path inside SDL_GetPrefPath("ARXProject", "ARX"),
// creating any sub-folders, e.g. userPath("saves/save1.txt").
// Windows: %APPDATA%\ARXProject\ARX\...   Android: the app's private storage.
std::string userPath(const std::string& relative);

// Write a whole file safely: write to "<file>.tmp", then rename over the
// original, so a phone being killed mid-save can't corrupt it.
bool writeTextAtomic(const std::string& userRelative, const std::string& content);

} // namespace arx::fs
```

Notes:
- Call `SDL_GetPrefPath` once and cache it. Free the returned string with `SDL_free`.
- Keep the text helpers byte-for-byte faithful: no newline conversion that would change parsing. Existing files may have Windows line endings, so check that current parsers still behave the same (e.g. a trailing `\r`).
- Log a clear message via `std::cerr` when a file can't be opened, including the path.

## Stage 2 – Route all **read-only data** through the helper

Convert every read of files under `data/` that uses `std::ifstream` or `fopen`. From a scan of the code, these include (verify, and find any others):

| File | What it reads |
|---|---|
| `renderer/View3D.cpp` | `data/map/core/textures.txt`, `backgrounds.txt` |
| `actor.cpp` | `encountersNew.txt`, `encounters.txt`, `monsters.bin`, other `data/map/core/` files |
| `spells.cpp`, `items.cpp` | `data/map/core/` spell and item files |
| `damon.cpp` | `data/map/` files and `DamonItems.bin` |
| `smithy.cpp`, `dwarvenSmithy.cpp` | `smithyItems.bin`, `DwarvenItems.bin` |
| `level.cpp` | `maps.txt`, each scenario's `Descriptions.txt`, `Messages.txt`, `Zones.txt`, map files |
| `lyrics.cpp` | lyrics files in `data/audio/` and `data/audio/B/` |
| `config.cpp` | the settings file (see Stage 3; it's read *and* written) |

Guidelines:
- For line-based parsing, the smallest change is usually replacing `std::ifstream instream; instream.open(path);` with `bool ok; auto instream = arx::fs::openText(path, ok);` and replacing `is_open()` checks with `ok`. Keep the parsing code itself unchanged.
- For `.bin` files read with `fopen`/`fread`, use `readBinary` and copy from the buffer with the same layout and sizes.
- **Known suspicious code:** in `smithy.cpp` (~line 337) and `dwarvenSmithy.cpp` (~line 531), the result of `fopen` is compared to `0` and never assigned to `fp`. Work out what this code actually does today, keep the behaviour Carl sees in-game, and explain it in your summary.

**Done when:** in the files listed in `${ARX_GAME_SOURCES}`, a search for `ifstream`, `fopen` and `"data/` finds **no** reads that bypass `arx::fs` or SDL (SDL_image/SDL_mixer loads are fine). Put that search output in your summary.

## Stage 3 – Move **writable files** to the user folder

The APK is read-only, so anything the game writes must go to `arx::fs::userPath(...)`.

1. **Save games** (`saveGame.cpp`: `saveGames.txt` and `save<N>.txt`):
   - Write to `userPath("saves/...")`, using `writeTextAtomic` where the whole file is written at once.
   - Read from `userPath("saves/...")` first. If the file isn't there, fall back to the bundled `data/saves/...` copy through `arx::fs`, so a fresh install still sees the default slot list.
2. **Settings** (`config.cpp`): same pattern. Read the user copy if it exists, otherwise the bundled default, and write to the user copy.
3. **Other writes** found in `actor.cpp`, `level.cpp` (~line 612 writes into `data/images/...`) and `misc.cpp`: work out what each one is for. If it's a debug or developer export, leave it writing where it does today on desktop, but wrap it in `#ifndef __ANDROID__`, and list it in your summary. If it's something the player needs, move it to `userPath`.

Don't migrate or delete Carl's existing save files. Just tell him in your summary where the new saves live, so he can copy old ones across if he wants.

## Stage 4 – Test on desktop and document

1. Run the full test checklist below.
2. Temporarily **rename** the `data/saves` folder next to the `.exe` and confirm the game still starts, then creates and loads saves from the AppData folder. Rename it back afterwards.
3. Add a short "Where files live" note to `README.md`: game data vs user saves/settings, with the Windows and Android locations.

## Out of scope

- The Android project itself (next step).
- Changing file formats, moving to JSON, or cloud saves.
- Persisting saves in the web build between browser sessions. Note how it behaves now, but don't change it.

## Test checklist for Carl

- The game starts, and the title screen, city, textures and sky all look normal.
- Create a character, save, quit, restart and load. The character is intact.
- Check the save appears in `%APPDATA%\ARXProject\ARX\saves`.
- Change a setting, restart, and check it stuck.
- Shops, the Smithy, the Dwarven Smithy and Damon show their usual items.
- Tavern lyrics (F1) still show and stay in time.
- Walk into the Dungeon: its map, messages and encounters load.
- Monsters and encounters appear as normal.
