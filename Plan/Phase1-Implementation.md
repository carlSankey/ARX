 # Phase 1 Implementation Plan - Core Rendering & Font
**Status:** Starting  
**Priority:** CRITICAL (blocks all web rendering)

This document details the implementation, testing, and validation for Phase 1.

---

## Overview

**Objective:** Remove all SFML code from display.cpp and font.cpp, implement SDL2 equivalents, and verify the web build renders text and UI correctly.

**Deliverable:** Web build that displays:
- ✅ Main menu with text
- ✅ Stats panel with banner and colored text
- ✅ 3D view (already working)
- ✅ All UI elements (boxes, borders)
- ✅ Gate images and animations
- ✅ Compass
- ✅ Automap (optional, can be Phase 3)

---

## Phase 1a: display.cpp Replacement

### Implementation Tasks

#### Task 1a-1: Audit and Document SFML Usage
- [ ] List all SFML variables in display.cpp (sf::Texture, sf::Sprite, sf::RenderWindow, etc.)
- [ ] List all SFML function calls (App.draw(), App.clear(), App.display())
- [ ] Document the SFML-only code sections (inside `#ifndef ARX_USE_SDL2`)
- [ ] Create mapping table: SFML code → SDL2 equivalent

#### Task 1a-2: Remove Conditional Compilation Guards
- [ ] Remove all `#ifndef ARX_USE_SDL2` guards
- [ ] Keep SDL2 implementations (delete SFML-only branches)
- [ ] Remove `#include "platform/SFMLWindow.h"`
- [ ] Remove SFML includes: `#include <SFML/Graphics.hpp>`, etc.

#### Task 1a-3: Replace Rendering System
- [ ] `sf::RenderWindow App` → Use `arx::g_window` interface
- [ ] `App.draw()` calls → Use Sprite2D.draw() or SDL2 equivalents
- [ ] `App.clear()` → Route through `arx::g_window->clear()`
- [ ] `App.display()` → Route through `arx::g_window->display()`

#### Task 1a-4: Replace Texture/Sprite System
- [ ] `sf::Texture` declarations → Remove (use Sprite2D)
- [ ] `.loadFromFile()` → Use Sprite2D.load()
- [ ] `sf::Sprite` declarations → Remove (use Sprite2D)
- [ ] All sprite operations → Use Sprite2D methods

#### Task 1a-5: Replace Vector/Color/Shape Types
- [ ] `sf::Vector2f()` → Use SDL_FRect or arx types
- [ ] `sf::IntRect()` → Use SDL_Rect
- [ ] `sf::Color()` → Use SDL_Color or equivalent
- [ ] `sf::RectangleShape` → Implement SDL2 rectangle drawing function

#### Task 1a-6: Implement SDL2 Rectangle Drawing
- [ ] Create helper function: `void drawRectangle(int x, int y, int w, int h, SDL_Color color, bool filled)`
- [ ] Use SDL2_renderer to draw rectangles instead of sf::RectangleShape
- [ ] Apply to all UI borders and backgrounds (console background, stats panel border, compass background, etc.)

### Testing for Task 1a

#### Test 1a-1: Build Verification
```
Command: cd c:\Source\ARXX\web && build.cmd
Expected: No SFML-related compile errors
Check: Verify no sf:: namespace references remain in display.cpp
```

#### Test 1a-2: Visual Regression - Main Menu
```
Expected Output: Main menu displays with text and logo
- Logo image visible
- Menu text readable (option 1-7, 0)
- Text colors appear correct
Success Criteria: Menu is identical to previous build
```

#### Test 1a-3: Visual Regression - Stats Panel
```
Expected Output: Stats panel displays correctly
- Banner image visible
- Player name, level, stats visible
- Colors are correct (brown for name, purple for stats, blue for XP)
- Banner does NOT have transparency overlay artifacts
Success Criteria: Stats panel layout and colors match previous build
```

#### Test 1a-4: Visual Regression - 3D View
```
Expected Output: 3D corridor view renders
- Walls, floor, ceiling visible
- Background renders correctly
- Player can see first-person view
Success Criteria: 3D view unchanged from Phase 7b
```

#### Test 1a-5: Browser Console Check
```
Open browser DevTools (F12) → Console tab
Expected: No WebGL errors related to display
Expected: No "undefined function" errors for Sprite2D
```

---

## Phase 1b: font.cpp Redesign with SDL2_ttf

### Pre-Implementation: Build System Setup

#### Task 1b-0a: Update web/build.cmd
- [ ] Add SDL2_ttf flag: `-sUSE_SDL_TTF=2`
- [ ] Ensure SDL2_mixer still present: `-sUSE_SDL_MIXER=2`
- [ ] Verify AlternateReality.ttf is preloaded in `--preload-file data/images/core/AlternateReality.ttf`
- [ ] Check CMakeLists.txt includes SDL2_ttf linking

#### Task 1b-0b: Verify Font File
- [ ] Confirm `data/images/core/AlternateReality.ttf` exists and is readable
- [ ] File size reasonable (~50KB - 1MB)
- [ ] Font supports ASCII characters (a-z, A-Z, 0-9, special chars)

### Implementation Tasks

#### Task 1b-1: Remove Sprite-Based Font System
- [ ] Remove `sf::Texture FontImage` declaration
- [ ] Remove `std::optional<sf::Sprite> CharImage` declaration
- [ ] Remove sprite-based `DrawChar()` function entirely
- [ ] Remove `SetFontColour()` that uses sprite color
- [ ] Remove all character texture rect calculations
- [ ] Remove SFML includes: `#include <SFML/Graphics.hpp>`

#### Task 1b-2: Implement SDL2_ttf Font Loading
```cpp
// New header section in font.cpp
#include <SDL2/SDL_ttf.h>

static TTF_Font* g_font = nullptr;
static SDL_Color g_currentColor = {215, 215, 215, 255};  // Light gray default

void InitFont() {
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        return;
    }
    
    g_font = TTF_OpenFont("data/images/core/AlternateReality.ttf", 16);
    if (!g_font) {
        std::cerr << "Failed to load AlternateReality.ttf: " << TTF_GetError() << std::endl;
    }
}

void releaseFontResources() {
    if (g_font) {
        TTF_CloseFont(g_font);
        g_font = nullptr;
    }
    TTF_Quit();
}
```

#### Task 1b-3: Implement SetFontColour for SDL2_ttf
```cpp
void SetFontColour(int r, int g, int b, int a) {
    g_currentColor = {(Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a};
}
```

#### Task 1b-4: Implement Text Rendering Functions
Replace character-by-character rendering with direct text rendering:

```cpp
// New rendering function - render text string directly
void renderTextString(int x, int y, const std::string& text) {
    if (!g_font || text.empty()) return;
    
    SDL_Surface* surface = TTF_RenderText_Solid(g_font, text.c_str(), g_currentColor);
    if (!surface) {
        std::cerr << "Text render failed: " << TTF_GetError() << std::endl;
        return;
    }
    
    // Create texture from surface and render using Sprite2D or SDL2
    // (implementation depends on existing rendering backend)
    
    SDL_FreeSurface(surface);
}
```

#### Task 1b-5: Update Text Functions
- [ ] Rewrite `drawText(int x, int y, std::string)` to use `renderTextString()`
- [ ] Rewrite `bText(int x, int y, std::string)` to use `renderTextString()`
- [ ] Rewrite `cyText(int y, std::string)` to use `renderTextString()`
- [ ] Update `drawText(int x, int y, int number)` to convert number to string, then render
- [ ] Update `bText(int x, int y, int number)` similarly
- [ ] Update `cText()` and `text()` functions

#### Task 1b-6: Remove DrawChar Function
- [ ] Delete entire `DrawChar()` function
- [ ] Update all callers to use string-based text functions instead

### Testing for Task 1b

#### Test 1b-1: Build Verification
```
Command: cd c:\Source\ARXX\web && build.cmd
Expected: No SDL_ttf-related compile errors
Expected: SDL2_ttf library linked successfully
Check: Verify no sprite-based font code remains
```

#### Test 1b-2: Font Load Verification
```
Browser DevTools → Console
Expected: "Font loaded successfully!" message (if you add logging)
Expected: No TTF_OpenFont() errors
```

#### Test 1b-3: Text Rendering - Menu
```
Expected Output: Main menu text renders using new font
- All menu options visible (1-7, 0)
- Text readable and properly positioned
- Text is crisp (no pixelation)
Success Criteria: Text appearance same or better than sprite-based
```

#### Test 1b-4: Text Rendering - Stats Panel
```
Expected Output: Stats panel text renders correctly
- Player name visible
- Stats labels visible: "Stats:", "STA", "CHR", "STR", "INT", "WIS", "SKL"
- Experience label and value visible
- "Hit Points:" label and value visible
- All colors correct (brown name, purple stats, blue XP)
Success Criteria: Text layout unchanged, no transparency artifacts
```

#### Test 1b-5: Text Rendering - Varied Colors
```
Test Function: displayMainMenu() -> setScreenValues() path
Expected: Each text section renders in correct color
- Brown text for player name
- Purple text for stats
- Blue text for experience
- Green text for location descriptions
Success Criteria: Color changes work correctly with new font
```

#### Test 1b-6: Character Creation Gate Sounds
```
Scenario: Start new character, proceed to gate animation
Expected: Text displays during name entry and gate screen
- "Enter name:" prompt visible
- "Male/Female?" visible
- Gate animation display works
- After pressing key: "You are now joined" displays
Success Criteria: Text renders throughout character creation
```

---

## Integration Testing (After 1a + 1b Complete)

### Test Phase1-Full-1: Complete Web Build
```
Command: cd c:\Source\ARXX\web && build.cmd
Expected: Build completes without errors
Artifact: build-web/arxx_web.html, .js, .wasm
```

### Test Phase1-Full-2: Browser Launch
```
Command: Open build-web/arxx_web.html in Chrome/Firefox
Expected: Game loads without errors
Browser Console: No critical errors
```

### Test Phase1-Full-3: Main Menu Walkthrough
```
Steps:
1. Wait for game to load
2. Verify main menu displays with text
3. Verify logo image visible
4. Select option 1 (Create City character)
5. Enter character name (type "TestChar")
6. Select M/F (press 1 for Male)
7. Confirm Y/N (press Y)
8. Verify character creation/gate animation
9. Verify gate sounds play (if audio working)
10. Press key to continue
11. Verify "You are now joined" displays
12. Verify stats panel displays
13. Press ESC to return to menu
14. Press 0 to quit

Expected: All text displays correctly, no graphics glitches
Success: Game flow works, rendering system functional
```

### Test Phase1-Full-4: Visual Comparison
```
Compare with previous builds:
- Text readability (same or better)
- Layout (identical)
- Colors (identical)
- Spacing (identical)
- No transparency artifacts
```

### Test Phase1-Full-5: Performance Check
```
Browser DevTools → Performance tab
Monitor during 10 seconds of gameplay:
- Frame rate (should be ~60 FPS)
- GPU memory usage
- Memory leaks (check for increasing memory)
Success: No performance regression
```

---

## Rollback Plan (If Testing Fails)

### If Build Fails
1. Revert changes: `git checkout display.cpp font.cpp`
2. Identify error from compiler output
3. Fix specific issue, rebuild
4. Re-run Test Phase1-Full-1

### If Menu Doesn't Display
1. Check browser console for errors
2. Verify Sprite2D is being called (add logging if needed)
3. Check if SDL2 rendering is initialized
4. Verify preloaded images are accessible

### If Text Doesn't Display
1. Verify TTF_Init() succeeded
2. Check TTF_OpenFont() error message
3. Verify AlternateReality.ttf is preloaded
4. Check font size parameter (currently 16)
5. Verify SDL2_ttf library is linked

### If Colors Are Wrong
1. Verify SetFontColour() is being called
2. Check g_currentColor values
3. Verify SDL_Color format (RGBA vs BGRA)

### If Text Is Pixelated
1. Increase font size in TTF_OpenFont (try 24, 32)
2. Check if anti-aliasing is enabled in SDL2_ttf
3. Consider using TTF_RenderText_Blended instead of _Solid

---

## Success Criteria (Phase 1 Complete)

- [ ] display.cpp compiles without SFML code
- [ ] font.cpp compiles with SDL2_ttf
- [ ] Web build generates .html, .js, .wasm files
- [ ] Browser loads game without console errors
- [ ] Main menu displays with text and logo
- [ ] Text renders in correct colors
- [ ] Stats panel displays correctly
- [ ] 3D view still renders
- [ ] No transparency artifacts
- [ ] Character creation flow works
- [ ] Game feels responsive (60 FPS)
- [ ] All text is crisp and readable

**Once Phase 1 passes all tests, proceed to Phase 2 (Audio).**