# SFML to SDL2 Conversion Plan - Web Build Files

## Overview
Convert remaining SFML code in web build files to SDL2. The web build compiles these files:
- `display.cpp` - Core rendering (text, sprites, images)
- `game.cpp` - Window management, main loop
- `misc.cpp` - Graphics utilities
- `automap.cpp` - Map rendering (can stub - nice to have)
- `audio.cpp` - ✅ Already converted to SDL2_mixer

## Conversion Strategy

### Phase 1: Core Rendering (display.cpp)
**Priority: CRITICAL** - Required for basic gameplay

**SFML → SDL2 replacements needed:**
- `sf::RenderWindow` → Use SDL2Window from platform/
- `sf::Texture` → `SDL_Texture*`
- `sf::Sprite` → `SDL_Rect` (destination) + texture
- `sf::Color` → `SDL_Color`
- `sf::IntRect` → `SDL_Rect`
- `sf::Vector2f` → manual x/y coordinates
- `sf::Text` → `SDL_Color` + manual text rendering
- `sf::Font` → Already using custom font system
- `sf::Clock` → `arx::Clock` (already done)
- `sf::Time` → `arx::Time` (already done)
- `sf::sleep()` → `arx::sleep()` (already done)

**Key functions to convert:**
- `drawImage()` - Sprite/texture rendering
- `drawText()` - Text rendering
- `clearDisplay()` - Screen clearing
- `updateDisplay()` - Present to screen
- Color/alpha blending functions

### Phase 2: Window & Game Loop (game.cpp)
**Priority: HIGH** - Required for window management

**SFML → SDL2 replacements needed:**
- `sf::RenderWindow` creation → `SDL2Window::create()`
- `sf::Event` → `SDL_Event`
- Event polling → `SDL_PollEvent()`
- Window close events → `SDL_QUIT`
- Keyboard input → `SDL_KEYDOWN`/`SDL_KEYUP`
- Mouse input → `SDL_MOUSEBUTTONDOWN` etc.

**Key functions to convert:**
- Window initialization
- Event loop
- Input handling
- Window state management

### Phase 3: Graphics Utilities (misc.cpp)
**Priority: MEDIUM** - Helper functions

**SFML → SDL2 replacements needed:**
- `sf::Color` conversions → `SDL_Color`
- Any remaining SFML graphics calls
- Image loading utilities

### Phase 4: Automap (automap.cpp)
**Priority: LOW** - Nice to have feature

**Approach: Create SDL2 stubs** (like audio.cpp has for unimplemented features)
- Keep `#ifdef ARX_USE_SDL2` block with stubs
- Stub functions: `automap()`, `drawAutomap()`, `drawFullAutomap()`
- Keep `setAutoMapFlag()` functional (just sets flags, no rendering)
- Move SFML code to `#else` block (never compiled in web build)

## Implementation Order
1. **display.cpp** - Start here, most critical for rendering
2. **game.cpp** - Window/event handling
3. **misc.cpp** - Utility functions
4. **automap.cpp** - Stub implementation

## Notes
- All `arx::Clock`, `arx::Time`, `arx::sleep()` already converted
- SDL2 renderer infrastructure exists in `platform/SDL2Window.cpp`
- Font rendering already uses custom system (not SFML)
- Audio already uses SDL2_mixer

## Success Criteria
- Web build compiles without SFML dependencies
- All `#ifndef ARX_USE_SDL2` blocks removed from web build files
- Only `#ifdef ARX_USE_SDL2` blocks remain (SDL2 implementation)
- Game renders correctly in browser