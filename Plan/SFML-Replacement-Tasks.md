# SFML → SDL2 Replacement Audit
**Generated:** 21 June 2026  
**Updated:** 21 June 2026 - Web-only architecture confirmed, TrueType font integration planned

This document lists all areas where SFML needs to be **completely replaced** with SDL2/web equivalents. This is a web-only project with no desktop build requirement.

---

## Strategy Change: Direct Replacement (Not Wrapping)

**Previous approach:** Maintain dual code paths with `#ifndef ARX_USE_SDL2` guards (for hypothetical desktop builds)

**New approach:** Direct replacement - delete SFML code entirely and implement SDL2 equivalents only. Simplified codebase, no technical debt, faster development.

---

## Critical Path (Must Fix for Web Build)

### 1. Display Layer (`display.cpp`)
**Status:** ❌ BLOCKS WEB BUILD  
**Priority:** CRITICAL  
**Action Type:** REPLACE (not wrap)

**SFML Code to Remove:**
- `#ifndef ARX_USE_SDL2` / `#endif` guard blocks (remove guard, keep SDL2 code only)
- `sf::Texture` declarations (img0-img9, imgc0-c9, BannerImageCity, etc.)
- `sf::Sprite` declarations and usage (Banner, BannerStrip, counterImage, etc.)
- `sf::RenderWindow App` - replace with SDL2_renderer
- `sf::RectangleShape` - replace with SDL2 rectangle drawing
- `App.draw()`, `App.clear()`, `App.display()` calls
- `sf::Vector2f`, `sf::IntRect` - use SDL2 equivalents (SDL_FRect, SDL_Rect)
- `sf::Color`, `sf::BlendAlpha`
- SFMLWindow includes: `#include "platform/SFMLWindow.h"`

**SDL2 Already Available:**
- `arx::Sprite2D` system in `renderer/Sprite2D.h/cpp` for texture/sprite operations
- `arx::g_window` interface for platform abstraction
- Web path exists with `Sprite2D.draw()`, rectangle drawing stubs

**Action Items:**
- [ ] Remove all `#ifndef ARX_USE_SDL2` / `#endif` guards
- [ ] Delete SFML-only code branches (keep SDL2 implementations)
- [ ] Replace `sf::RenderWindow App` with SDL2 renderer calls
- [ ] Implement SDL2 rectangle drawing for UI borders/backgrounds
- [ ] Update all texture loading to use Sprite2D
- [ ] Replace `App.draw()` with Sprite2D or SDL2 equivalents
- [ ] Remove SFML includes completely

---

### 2. Font System (`font.cpp`)
**Status:** ❌ BLOCKS TEXT RENDERING  
**Priority:** CRITICAL  
**Action Type:** COMPLETE REDESIGN (use TrueType font)

**SFML Code to Remove:**
- Sprite-based font rendering (character textures, texture rects)
- `sf::Texture FontImage` and `sf::Sprite CharImage`
- `DrawChar()` function with sprite logic
- Character-by-character rendering loop in text functions
- `SetFontColour()` color management
- All SFML font-related includes

**New Approach - SDL2_ttf + TrueType Font:**
- Load `data/images/core/AlternateReality.ttf` at startup using SDL2_ttf
- Replace `DrawChar()` with direct text rendering using SDL2_ttf
- Render entire strings/lines at once (not character by character)
- Font scaling via font size parameter (no sprite scaling)
- Color management via SDL2_ttf color parameters

**Benefits:**
- Much simpler code (no sprite atlas management)
- Native font rendering (crisp text at any size)
- Web-native (fonts work perfectly in browser)
- Maintainable single-path implementation

**Action Items:**
- [ ] Add SDL2_ttf integration to build system (`web/build.cmd`)
- [ ] Preload AlternateReality.ttf in Emscripten VFS
- [ ] Rewrite font initialization to load TTF file
- [ ] Replace `DrawChar()` with SDL2_ttf text rendering
- [ ] Update text rendering functions (drawText, bText, cyText) to use TTF
- [ ] Remove all sprite-based font code
- [ ] Remove all SFML font includes

**Implementation Note:**
```cpp
// Pseudocode for new approach
void InitFont() {
  g_font = TTF_OpenFont("data/images/core/AlternateReality.ttf", 16);
}

void drawText(int x, int y, std::string text) {
  SDL_Surface* surface = TTF_RenderText_Solid(g_font, text.c_str(), g_currentColor);
  // Render surface using Sprite2D or SDL2 equivalent
}
```

---

### 3. Audio System (`audio.cpp`, `game.cpp`, shop files)
**Status:** ⚠️ PARTIALLY PORTED  
**Priority:** HIGH  
**Action Type:** REPLACE with SDL2_mixer

**SFML Code to Remove:**
- `sf::Sound` declarations and usage
- `sf::SoundBuffer` declarations
- `sf::Music` declarations and usage
- `sf::SoundSource::Status` checking
- `.getStatus()` calls on sounds
- SFML audio includes

**Files Affected:**
- `audio.cpp` - primary audio management
- `game.cpp` - door/secret door sounds, smithy sound
- `createCharacter.cpp` - gate sounds (cityGate2Sound, cityGate3Sound)
- `guild.cpp` - guild music
- `smithy.cpp` - smithy music
- `tavern.cpp` - tavern music
- `rathskeller.cpp` - rathskeller music
- `inn.cpp`, `retreat.cpp`, `shop.cpp`, `healer.cpp`, `damon.cpp`, `scenarioEntrance.cpp` - music includes

**SDL2_mixer Already Available:**
- SDL2_mixer configured in `web/build.cmd` with `-sUSE_SDL_MIXER=2`
- OGG/Vorbis support enabled
- Audio files preloaded

**Action Items:**
- [ ] Remove all SFML audio includes
- [ ] Replace `sf::SoundBuffer` with SDL2_mixer `Mix_Chunk*`
- [ ] Replace `sf::Sound` with SDL2_mixer channel management
- [ ] Replace `sf::Music` with SDL2_mixer `Mix_Music*`
- [ ] Replace `.getStatus()` checks with SDL2_mixer equivalents (`Mix_Playing()`)
- [ ] Complete audio.cpp SDL2_mixer implementation
- [ ] Port game.cpp door/sound effects to SDL2_mixer
- [ ] Port shop music to SDL2_mixer
- [ ] Remove music extern declarations from shop files

---

## Secondary Path (UI/Input)

### 4. Automap System (`automap.cpp`)
**Status:** ⚠️ PARTIAL  
**Priority:** MEDIUM  
**Action Type:** REPLACE

**SFML Code to Remove:**
- `sf::Texture` mapImage, legendImage
- `sf::Sprite` cellImage, mapLegend
- `sf::IntRect` for texture rects
- `sf::Vector2f` positioning
- `sf::RectangleShape` for borders
- `sf::Color` constructors

**SDL2 Equivalent:**
- Use Sprite2D system for all sprite/texture operations
- Use SDL2 rectangle drawing for borders

**Action Items:**
- [ ] Remove `#ifndef ARX_USE_SDL2` guards
- [ ] Delete SFML-only code paths
- [ ] Convert texture management to Sprite2D
- [ ] Implement SDL2 rectangle drawing for map borders
- [ ] Remove SFML includes

---

### 5. Lyrics/Lyric Display (`lyrics.cpp`)
**Status:** ⚠️ NATIVE ONLY  
**Priority:** MEDIUM  
**Action Type:** REPLACE with SDL2_ttf

**SFML Code to Remove:**
- `sf::Texture lyricFontImage` - sprite-based font
- `sf::Sprite lyricCharImage`
- `sf::RenderTexture lyricstexture` - offscreen rendering
- Character-by-character rendering logic
- SFML color/vector types

**SDL2 Equivalent:**
- Use SDL2_ttf for lyric text rendering (same as main font system)
- Use SDL2 texture/surface for offscreen rendering if needed
- Use Sprite2D for display

**Action Items:**
- [ ] Replace sprite-based lyric font with SDL2_ttf
- [ ] Use AlternateReality.ttf for consistency
- [ ] Implement SDL2 offscreen rendering if needed
- [ ] Simplify character rendering to use TTF
- [ ] Remove SFML rendering code

---

### 6. Input System (`misc.cpp`, `input.cpp`)
**Status:** ✅ MOSTLY COMPLETE  
**Priority:** LOW (already has SDL2 path)

**Current State:**
- Already wrapped in `#ifndef ARX_USE_SDL2` with complete SDL2 equivalents
- SDL2 event handling implemented
- Key mapping complete

**Action Items:**
- [ ] Verify SDL2 event handling is complete
- [ ] Test all key mappings in web build
- [ ] Optionally remove SFML guard blocks if SDL2 path is verified working

---

## Non-Critical (Cleanup)

### 7. Shop/Location Files (inn.cpp, guild.cpp, tavern.cpp, etc.)
**Status:** ⚠️ HEADERS ONLY  
**Priority:** LOW

**SFML Usage:**
- `#include <SFML/Graphics.hpp>` - unused includes
- `#include <SFML/Audio.hpp>` - for music externs (handled by audio.cpp)
- Extern declarations for music objects

**Action Items:**
- [ ] Remove SFML graphics includes (unused)
- [ ] Keep audio includes only if externs are needed
- [ ] Clean up extern declarations (consolidate in audio.h)
- [ ] Verify shop logic doesn't depend on SFML

---

### 8. Game Logic (`game.cpp`)
**Status:** ⚠️ MIXED  
**Priority:** MEDIUM

**SFML Code to Remove:**
- `sf::SoundBuffer`, `sf::Sound` for door/sound effects
- `sf::Music` for death music
- `sf::Texture` for teleport effects (teleBlack, teleGold)
- `sf::Sprite` for teleport background
- `sf::Clock` usage (in commented code)

**Action Items:**
- [ ] Port sound effects to SDL2_mixer (coordinate with audio.cpp)
- [ ] Port teleport textures/sprites to Sprite2D
- [ ] Remove SFML includes
- [ ] Remove commented SFML code

---

## Archive/Deprecated (Safe to Ignore)

### 9. SFML-Only Files
- `archive_sfml/3Dview.cpp` - superseded by `renderer/View3D.cpp`
- `archive_sfml/3dviewnew.cpp` - alternate SFML version
- `archive_sfml/SFMLWindow.h` - replaced by SDL2Window

**Action:** Leave as-is for historical reference.

---

## Summary Table

| Component | File(s) | SFML Type | Severity | Action |
|-----------|---------|-----------|----------|--------|
| **Rendering** | display.cpp | Texture/Sprite/RenderWindow | CRITICAL | Replace (remove guards) |
| **Font** | font.cpp | Sprite font | CRITICAL | Redesign with SDL2_ttf |
| **Audio** | audio.cpp, game.cpp, shops | Sound/Music/SoundBuffer | HIGH | Replace with SDL2_mixer |
| **Automap** | automap.cpp | Texture/Sprite/Rect | MEDIUM | Replace with Sprite2D |
| **Lyrics** | lyrics.cpp | Sprite font, RenderTexture | MEDIUM | Replace with SDL2_ttf |
| **Input** | misc.cpp | Event/Keyboard | LOW | Verify SDL2 path |
| **Shops** | *.cpp | Includes/externs | LOW | Cleanup includes |
| **Game** | game.cpp | Sound/Texture/Clock | MEDIUM | Replace sounds/sprites |

---

## Recommended Work Order

### Phase 1 - Core Rendering & Font (CRITICAL)
- [ ] **display.cpp** - Remove SFML code, keep SDL2 paths, wrap in conditional if needed
  - [ ] Replace `sf::RenderWindow App` with SDL2_renderer
  - [ ] Verify Sprite2D usage for all textures/sprites
  - [ ] Implement SDL2 rectangle drawing
  - [ ] Remove all `#ifndef ARX_USE_SDL2` guards

- [ ] **font.cpp** - Redesign with SDL2_ttf
  - [ ] Add SDL2_ttf to build system
  - [ ] Implement `InitFont()` to load AlternateReality.ttf
  - [ ] Rewrite text rendering functions using TTF
  - [ ] Remove all sprite-based font code
  - [ ] Test text rendering in web build

### Phase 2 - Audio (HIGH)
- [ ] **audio.cpp** - Complete SDL2_mixer implementation
  - [ ] Verify SDL2_mixer is initialized
  - [ ] Implement sound/music playback
  - [ ] Implement status checking (Mix_Playing equivalent)
  - [ ] Test all audio in web build

- [ ] **game.cpp** - Port sounds to SDL2_mixer
  - [ ] Replace door/secret door sound effects
  - [ ] Replace smithy sound
  - [ ] Test sound effects during gameplay

- [ ] **createCharacter.cpp** - Ensure gate sounds work
  - [ ] Port cityGate2Sound, cityGate3Sound to SDL2_mixer
  - [ ] Test sounds during character creation

### Phase 3 - UI & Remaining (MEDIUM)
- [ ] **automap.cpp** - Replace with Sprite2D + SDL2 rectangles
- [ ] **lyrics.cpp** - Replace with SDL2_ttf
- [ ] **Shop files** - Remove unused SFML includes
- [ ] **tavern.cpp** - Handle sf::Clock for tavern timer

### Phase 4 - Cleanup & Testing (LOW)
- [ ] Remove all SFML includes that aren't in guards
- [ ] Verify input system (misc.cpp) is working
- [ ] Full web build test
- [ ] Performance testing

---

## Build System Updates Needed

### web/build.cmd
- Ensure SDL2_ttf is available: `-sUSE_SDL_TTF=2`
- Already has SDL2_mixer: `-sUSE_SDL_MIXER=2`
- Preload AlternateReality.ttf in Emscripten data

### CMakeLists.txt / Emscripten Config
- Add SDL2_ttf library link
- Configure font file preloading