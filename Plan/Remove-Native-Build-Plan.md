# Remove Native SFML Build Plan

## Goal
Eliminate the native SFML build entirely, keeping only the SDL2/web build.

## Current State
- All files have `#ifdef ARX_USE_SDL2` (SDL2) and `#else` (SFML) blocks
- Web build works with SDL2
- Native build uses SFML

## Steps

### Phase 1: Remove SFML Guards from Web Build Files
**Files to clean:**
- `audio.cpp` - Remove `#ifndef ARX_USE_SDL2` block, keep SDL2 code
- `automap.cpp` - Remove `#else` SFML block, keep SDL2 stubs
- `display.cpp` - Remove `#ifndef ARX_USE_SDL2` blocks
- `game.cpp` - Remove `#ifndef ARX_USE_SDL2` blocks
- `misc.cpp` - Remove `#ifndef ARX_USE_SDL2` includes and blocks

**For each file:**
1. Remove `#ifndef ARX_USE_SDL2` and `#endif` guards
2. Keep only the SDL2 implementation code
3. Remove SFML includes
4. Remove any SFML-dependent code in the `#else` sections

### Phase 2: Remove SFML Guards from Non-Web Build Files
**Files to clean (17 files):**
- tavern.cpp, guild.cpp, smithy.cpp, rathskeller.cpp
- createCharacter.cpp, lyrics.cpp
- ferry.cpp, healer.cpp, inn.cpp, retreat.cpp
- shop.cpp, undeadKing.cpp, lift.cpp, staircase.cpp
- damon.cpp, input.cpp, vaults.cpp

**For each file:**
1. Remove `#ifndef ARX_USE_SDL2` guards around SFML includes
2. Remove commented-out SFML code
3. Keep only the functional game logic (already SFML-free)

### Phase 3: Update Build System
**Files to update:**
- `ARXX.sln` - Remove native build configuration
- `ARXX.vcxproj` - Remove SFML references, keep only SDL2
- `web/build.cmd` - Keep as primary build script
- Remove or archive `archive_sfml/` directory

### Phase 4: Clean Up
- Remove SFML from vcpkg dependencies (if present)
- Update documentation
- Remove SFML-related comments
- Clean up any remaining SFML references

## Benefits
- Single codebase to maintain
- No conditional compilation complexity
- Cleaner code
- Easier debugging and development

## Risks
- Lose ability to build native Windows executable
- All development must target web/emscripten
- Requires Emscripten SDK for all builds

## Implementation Order
1. Start with `misc.cpp` (simplest, already mostly SDL2)
2. Then `game.cpp`, `display.cpp`, `audio.cpp`, `automap.cpp`
3. Then the 17 non-web files (mostly already clean)
4. Finally update build system files