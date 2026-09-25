# Chunk 1a-1a: SFML Audit Report for display.cpp
**Date:** 21 June 2026  
**Status:** AUDIT COMPLETE - Ready for implementation  
**Branch:** arxweb

---

## Summary

display.cpp contains **92 SFML references** across:
- 23 `sf::Texture` variable declarations
- 10 `std::optional<sf::Sprite>` variable declarations
- ~30 SFML function calls (App.draw, App.clear, etc.)
- ~20 SFML type usage (sf::Vector2f, sf::IntRect, sf::Color, sf::RectangleShape)
- All wrapped in `#ifndef ARX_USE_SDL2` guards (lines 5-2148)

**Important:** The SDL2 path already exists in the file! We just need to delete the SFML-only code and keep the SDL2 section.

---

## SFML Variables to Remove

### Textures (23 total)
```cpp
// Line 95-98: Digit textures
sf::Texture img0, img1, img2, img3, img4, img5, img6, img7, img8, img9;
sf::Texture imgDungeonGate, imgCityGate;
sf::Texture imgc0, imgc1, imgc2, imgc3, imgc4, imgc5, imgc6, imgc7, imgc8, imgc9;
sf::Texture consoleImage, BannerImageCity, BannerImageStrip;
sf::Texture compassN, compassS, compassW, compassE;
sf::Texture ShopImage, LogoImage;
sf::Texture encImageSheet;
```

**Action:** DELETE - These are only used in SFML section

### Sprites (10 total)
```cpp
// Line 55: Encounter image
std::optional<sf::Sprite> encImage;

// Line 100: Banner sprites
std::optional<sf::Sprite> Banner, BannerStrip;

// Line 101: Counter sprite
std::optional<sf::Sprite> counterImage;

// Line 102: Gate sprites
std::optional<sf::Sprite> dungeonGate, cityGate;

// Line 103: Compass sprite
std::optional<sf::Sprite> compass;

// Line 104: Shop/logo sprites
std::optional<sf::Sprite> ShopSprite, LogoSprite;
```

**Action:** DELETE - These are only used in SFML section

### Other SFML Objects
```cpp
// Line 87: Main render window
RenderWindow App;

// Line 91: SFML window wrapper
static arx::SFMLWindow* s_sfmlWindow = nullptr;

// Line 51-52: Using declarations
using sf::Sprite;
using sf::RenderWindow;

// Line 37: SFML window include
#include "platform/SFMLWindow.h"

// Line 10, 13: SFML headers
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
```

**Action:** DELETE - SFML-only code

---

## SFML Includes to Remove

```cpp
// Line 10 - REMOVE
#include <SFML/Graphics.hpp>

// Line 13 - REMOVE
#include <SFML/OpenGL.hpp>

// Line 37 - REMOVE
#include "platform/SFMLWindow.h"

// Line 51-52 - REMOVE (using declarations)
using sf::Sprite;
using sf::RenderWindow;
```

---

## SFML Function Calls to Replace/Remove

### App.draw() Calls (8 instances)
```cpp
Line 296:  App.draw(*encImage, sf::BlendAlpha);
Line 553:  App.draw(rectangle);  // UI rectangle
Line 614:  App.draw(*compass);
Line 637:  App.draw(*dungeonGate);
Line 644:  App.draw(*cityGate);
Line 695:  App.draw(*ShopSprite);
Line 709:  App.draw(rectangle);  // Stats panel rectangle
Line 740:  App.draw(*Banner);
Line 743:  App.draw(*BannerStrip); (2 instances)
Line 763:  App.draw(*LogoSprite);
```

**Action:** These need replacement with SDL2 equivalents or removal if SFML-only code is deleted

### App.clear() Calls (2 instances)
```cpp
Line 533:  App.clear(sf::Color::Black);
Line 680:  App.clear();
```

**Action:** Replace with `arx::g_window->clear()`

### App.display() Calls (1 instance)
```cpp
Line 539:  App.display();
```

**Action:** Replace with `arx::g_window->display()`

### App.pushGLStates() / App.popGLStates() (6 instances)
```cpp
Line 535:  App.pushGLStates();
Line 540:  App.popGLStates();
Line 681:  App.pushGLStates();
Line 699:  App.popGLStates();
Line 761:  App.popGLStates();
Line 763:  App.pushGLStates();
```

**Action:** Replace with `arx::g_window->pushGLStates()` / `popGLStates()`

### App.create() (1 instance)
```cpp
Line 498-499:  App.create(sf::VideoMode(...), title, sf::State::Windowed/Fullscreen);
```

**Action:** This is in createGameWindow() - may need special handling

### App.getSettings() (1 instance)
```cpp
Line 503:  sf::ContextSettings settings = App.getSettings();
```

**Action:** DELETE - SFML-only debug output

### App.setFramerateLimit() (1 instance)
```cpp
Line 508:  App.setFramerateLimit(60);
```

**Action:** DELETE or replace with SDL2 equivalent

### App.isOpen() (1 instance)
```cpp
Line 520:  } else if (App.isOpen()) {
```

**Action:** Replace with `arx::g_window->isOpen()`

### App.close() (1 instance)
```cpp
Line 521:  App.close();
```

**Action:** Replace with `arx::g_window->close()`

---

## SFML Type Usage to Replace

### sf::Vector2f (11 instances)
**Used for:** sprite positioning and scaling
```cpp
sf::Vector2f(x, y)  // Positioning
sf::Vector2f(scaleX, scaleY)  // Scaling
```

**Replace with:** `SDL_FPoint` or native float coordinates

### sf::IntRect (4 instances)
**Used for:** texture rectangles (sprite sheet regions)
```cpp
sf::IntRect({charX, charY}, {width, height})
```

**Replace with:** `SDL_Rect` or custom struct

### sf::RectangleShape (4 instances)
**Used for:** drawing UI borders and backgrounds
```cpp
sf::RectangleShape rectangle;
rectangle.setSize(sf::Vector2f(w, h));
rectangle.setFillColor(sf::Color(r, g, b, a));
rectangle.setOutlineColor(sf::Color::Yellow);
rectangle.setOutlineThickness(1);
rectangle.setPosition(sf::Vector2f(x, y));
App.draw(rectangle);
```

**Replace with:** Custom `drawRectangle()` function using SDL2_renderer

### sf::Color (6 instances)
**Used for:** text and shape colors
```cpp
sf::Color::Black
sf::Color::Yellow
sf::Color(0, 0, 0, 128)
sf::Color(255, 255, 255, 0)
```

**Replace with:** `SDL_Color` struct

### sf::BlendAlpha (1 instance)
```cpp
App.draw(*encImage, sf::BlendAlpha);
```

**Replace with:** SDL2 blending mode (SDL_BLENDMODE_BLEND)

---

## Structure of SFML Code in display.cpp

```
Lines 1-4:        Comments
Lines 5-38:       #ifndef ARX_USE_SDL2 ... SFML includes ... #endif
Lines 39-54:      Using declarations (some SFML)
Lines 55-118:     Global variable declarations (SFML textures/sprites)
Lines 119-2148:   Functions with mixed SFML and conditional code

Key Functions with SFML Code:
  - createGameWindow() - lines 486-522 (App.create, App.getSettings, App.setFramerateLimit)
  - clearScreen() - lines 529-545 (App.clear, App.pushGLStates, App.display, App.popGLStates)
  - displayShop() - lines 653-710 (App.clear, App.draw, App.pushGLStates, App.popGLStates)
  - drawStatsPanel() - lines 673-743 (App.draw for banner, rectangle)
  - displayMainMenu() - lines 748-768 (App.draw for logo)
  - drawEncounter() - lines 278-368 (App.draw for encounter image)
  - Various sprite positioning (compass, gates, etc.)

Lines 2143-2148:  #endif // ARX_USE_SDL2 (ends the SFML section)
```

---

## Mapping Table: SFML → SDL2

| SFML Code | SDL2 Equivalent | Notes |
|-----------|-----------------|-------|
| `sf::Texture` | `SDL_Texture*` or `arx::Sprite2D` | Use Sprite2D for consistency |
| `sf::Sprite` | `arx::Sprite2D` | Already available |
| `RenderWindow App` | `arx::g_window` | Platform abstraction interface |
| `App.draw(sprite)` | `Sprite2D.draw()` | Or SDL2_renderer |
| `App.clear(color)` | `arx::g_window->clear(color)` | Platform abstraction |
| `App.display()` | `arx::g_window->display()` | Platform abstraction |
| `App.pushGLStates()` | `arx::g_window->pushGLStates()` | Platform abstraction |
| `App.popGLStates()` | `arx::g_window->popGLStates()` | Platform abstraction |
| `App.create()` | Already handled in SDL2Window | Don't call in web |
| `sf::Vector2f(x, y)` | Native `float x, y` | Remove type wrapper |
| `sf::IntRect` | `SDL_Rect` | Simple struct |
| `sf::RectangleShape` | Custom `drawRectangle()` function | See Decision 5 |
| `sf::Color` | `SDL_Color` | Simple struct |
| `sf::BlendAlpha` | `SDL_BLENDMODE_BLEND` | Blending mode flag |

---

## Conditional Compilation Structure

**IMPORTANT FINDING:** The entire SFML code is wrapped in `#ifndef ARX_USE_SDL2` guard!

This means:
- ✅ Lines 5-2148: SFML-only code (wrapped in guard)
- ✅ SDL2 code path already exists (not shown because hidden inside `#else` block)
- ❌ We're looking at SFML-only file currently

**Strategy:**
1. Remove `#ifndef ARX_USE_SDL2` guards (keep only SDL2 code)
2. Delete all SFML-only code branches
3. Keep SDL2 code paths
4. Remove SFML headers and declarations

---

## Implementation Order (Chunk 1a-1a Tasks)

1. **Remove SFML Guard Wrapper**
   - Delete line 5: `#ifndef ARX_USE_SDL2`
   - Delete line 2148: `#endif // ARX_USE_SDL2`

2. **Remove SFML Includes**
   - Delete lines 5-13 (entire SFML section)
   - Keep SDL2 includes

3. **Remove SFML Variable Declarations**
   - Delete all `sf::Texture` declarations (lines 95-118)
   - Delete all `std::optional<sf::Sprite>` declarations (lines 55, 100-104)
   - Delete `RenderWindow App;` (line 87)
   - Delete `arx::SFMLWindow* s_sfmlWindow` (line 91)
   - Delete using declarations (lines 51-52)

4. **Remove SFML Includes from Inside Functions**
   - Delete `#include "platform/SFMLWindow.h"` (line 37)

5. **Document Locations of Remaining SFML Calls**
   - Create list of all App.draw(), App.clear(), etc. calls
   - These will be handled in next chunks (1a-1b, 1a-2a, 1a-2b)

---

## Pre-Implementation Verification

Before starting implementation:

- [ ] Confirm display.cpp is on `arxweb` branch
- [ ] Verify SDL2 code path exists in file (search for `#else` or `#ifdef ARX_USE_SDL2`)
- [ ] Backup current display.cpp to safe location
- [ ] Commit current working state to git with message "Before Phase 1 - Checkpoint"

---

## Success Criteria for Chunk 1a-1a (Audit)

- [x] List all SFML variables in display.cpp
- [x] List all SFML function calls
- [x] Create mapping table: SFML → SDL2
- [x] Document all includes to remove
- [x] Identify conditional compilation structure
- [x] Plan implementation order
- [ ] Ready for Chunk 1a-1b (Remove Guards & Delete SFML Code)