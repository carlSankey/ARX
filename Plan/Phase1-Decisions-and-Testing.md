# Phase 1: Major Decisions & Automated Testing Strategy
**Document Version:** 1.0  
**Date:** 21 June 2026

---

## Major Decisions Made in Phase 1 Plan

### Decision 1: Complete Removal vs. Conditional Wrapping
**Question:** Should we maintain SFML code paths with `#ifndef ARX_USE_SDL2` guards, or delete SFML entirely?

**Decision:** ✅ **DELETE SFML ENTIRELY**

**Rationale:**
- Project is web-only (confirmed by user)
- No desktop/Windows platform required
- Dual-path maintenance = technical debt + confusion
- Simpler codebase = faster development + fewer bugs
- Reduces confusion: "Is this a SFML bug or SDL2 bug?"
- One rendering path to test instead of two

**Impact:**
- Smaller codebase (~20-30% reduction in display.cpp)
- Faster compilation
- Single code path to maintain and debug
- Cannot revert to SFML without significant refactoring

---

### Decision 2: Font System Redesign - Sprite-Based vs. TrueType
**Question:** Should we keep sprite-based character fonts (current system) or switch to TrueType?

**Decision:** ✅ **SWITCH TO TRUETYPE (SDL2_ttf)**

**Rationale:**
- TrueType fonts are web-native (browsers understand them natively)
- Eliminates complex sprite atlas management
- Scalable text (no pixelation at different sizes)
- Simpler code (render whole strings, not character-by-character)
- User already created AlternateReality.ttf font
- Reduces rendering complexity
- Better text quality

**Sprite-Based Approach Eliminated:**
- Character-by-character DrawChar() calls
- Texture coordinate calculations
- FontImage atlas management
- CharImage sprite manipulation
- All replaced with single TTF_RenderText_Solid() call

**Impact:**
- font.cpp reduced from ~300 lines to ~100 lines
- Text rendering 10x faster (no per-character overhead)
- Better maintainability
- Cannot use bitmap fonts anymore (not needed)

---

### Decision 3: Rendering Backend - Direct SDL2 vs. Abstraction Layer
**Question:** Should we use `arx::g_window` abstraction or call SDL2 directly?

**Decision:** ✅ **USE arx::g_window ABSTRACTION**

**Rationale:**
- Already exists in codebase (Window.h, PlatformWindow.h)
- Decouples game code from rendering library
- Easier to swap renderers later if needed
- Sprite2D already uses this pattern
- Reduces coupling to SDL2
- Maintains architectural consistency

**Benefits:**
- Game code doesn't know about SDL2_renderer
- Rendering library can be changed without touching game logic
- Tests can mock `arx::g_window` interface
- Cleaner separation of concerns

**Impact:**
- display.cpp routes through `arx::g_window` interface
- No direct SDL2 calls in game code
- Slightly more indirection (negligible performance impact)

---

### Decision 4: Texture Loading - Lazy vs. Preload
**Question:** Should textures be loaded on-demand or preloaded at startup?

**Decision:** ✅ **PRELOAD AT STARTUP**

**Rationale:**
- Web builds need files preloaded via Emscripten
- Preloaded files are in virtual FS (no network latency)
- Avoids mid-game loading stalls
- Simpler implementation (InitFont() loads everything)
- Web builds don't have traditional file I/O

**Current Approach:**
- All images preloaded in `web/build.cmd`: `--preload-file data/`
- All fonts preloaded: `--preload-file data/images/core/AlternateReality.ttf`
- LoadFromFile() reads from Emscripten virtual FS

**Impact:**
- Larger initial .wasm file (~100KB more for fonts)
- No loading delays during gameplay
- Cannot dynamically load new assets (not needed for this game)

---

### Decision 5: Rectangle Drawing - Sprite vs. SDL2 Primitives
**Question:** How should UI rectangles (borders, backgrounds) be drawn?

**Decision:** ✅ **IMPLEMENT SDL2 PRIMITIVE FUNCTION**

**Rationale:**
- SFML uses sf::RectangleShape (not available in SDL2)
- SDL2_gfx is not available in Emscripten by default
- Can implement simple rectangle drawing ourselves
- Only need filled rectangles (for backgrounds) and outlines (for borders)
- Minimal code, maximum compatibility

**Implementation Plan:**
```cpp
void drawRectangle(int x, int y, int w, int h, 
                   SDL_Color fill_color, SDL_Color outline_color, 
                   int outline_thickness) {
    // Use Sprite2D or SDL2_renderer to draw rectangle
    // Fill with fill_color, outline with outline_color
}
```

**Impact:**
- Adds ~20-30 lines of code
- Avoids SDL2_gfx dependency
- Fully portable to Emscripten

---

### Decision 6: Color Format - RGB vs. RGBA
**Question:** Should colors be RGB (3 components) or RGBA (4 components)?

**Decision:** ✅ **USE RGBA (4 components)**

**Rationale:**
- SDL_Color requires RGBA (Uint8 r, g, b, a)
- Alpha channel needed for transparency
- Consistent with existing code (sf::Color uses RGBA)
- Allows future UI polish (semi-transparent windows)
- No performance penalty

**Conversion:**
- SFML: `sf::Color(215, 215, 215, 255)` → SDL2: `{215, 215, 215, 255}`
- Same format, different syntax

**Impact:**
- SetFontColour() unchanged (takes r, g, b, a)
- SDL_Color declarations added
- One extra component per color (negligible memory)

---

### Decision 7: Font Size - Fixed vs. Scalable
**Question:** Should font size be fixed (16px) or scalable per UI section?

**Decision:** ✅ **FIXED AT 16px FOR NOW (with room for future scaling)**

**Rationale:**
- Original sprite font was fixed size
- Simpler initial implementation
- Can add TTF_OpenFont(size) variants later if needed
- Current layout assumes 16px font
- No UI redesign required

**Future Enhancement Option:**
```cpp
// Could support multiple font sizes:
static TTF_Font* g_font_16 = nullptr;
static TTF_Font* g_font_24 = nullptr;
static TTF_Font* g_font_32 = nullptr;
// Load all at init time
```

**Impact:**
- Single TTF_OpenFont(16) call
- All text rendered at same size
- Can scale with uiScale variable if needed

---

### Decision 8: Text Anti-Aliasing - Solid vs. Blended
**Question:** Should text use TTF_RenderText_Solid() or TTF_RenderText_Blended()?

**Decision:** ✅ **START WITH SOLID (can switch to BLENDED if needed)**

**Rationale:**
- TTF_RenderText_Solid() - faster, no anti-aliasing, no alpha blending
- TTF_RenderText_Blended() - slower, smooth text, alpha blending
- Solid is simpler and faster
- Can upgrade to Blended if text quality is poor
- Original sprite font had no anti-aliasing either

**If Text Quality Issues Occur:**
```cpp
// Switch to:
SDL_Surface* surface = TTF_RenderText_Blended(g_font, text.c_str(), g_currentColor);
```

**Impact:**
- Slightly faster text rendering
- Potential text quality trade-off (minimal expected)
- Easy to switch if needed

---

## Automated Testing Strategy

### Category 1: Build System Automated Tests

#### Test 1.1: Compilation Check (CI/CD)
**Type:** Automated build verification  
**Trigger:** On every commit to display.cpp, font.cpp, build.cmd

```bash
# web/build.cmd runs Emscripten compilation
# Exit code 0 = success, non-zero = failure
# Can integrate into CI/CD pipeline (GitHub Actions, etc.)

Command: cd c:\Source\ARXX\web && build.cmd
Expected: Exit code 0
Artifact Check: 
  - arxx_web.html exists and >10KB
  - arxx_web.js exists and >1MB
  - arxx_web.wasm exists and >1MB
Failure Action: Block PR, email maintainer
```

#### Test 1.2: No SFML References (Automated Linting)
**Type:** Code scanning for forbidden includes  
**Trigger:** On every commit

```bash
# Grep for SFML includes in critical files
grep -r "#include.*SFML" display.cpp font.cpp game.cpp audio.cpp
# If found: FAIL (exit 1)
# If not found: PASS (exit 0)

Forbidden Patterns:
  - #include <SFML/
  - #include "SFML
  - sf::
  - SFML::
```

**Tool:** Simple bash script or Git pre-commit hook

---

### Category 2: Runtime Automated Tests

#### Test 2.1: WebAssembly Loading Test
**Type:** Automated browser test (headless)  
**Trigger:** After successful build

```bash
# Use Puppeteer (Node.js) to:
# 1. Load arxx_web.html in headless Chrome
# 2. Wait for game to initialize (2 seconds)
# 3. Check browser console for critical errors
# 4. Take screenshot of main menu
# 5. Report pass/fail

Check Criteria:
  - No uncaught JavaScript exceptions
  - No "undefined function" errors
  - Canvas renders (width > 0, height > 0)
  - Game loads within 5 seconds
```

**Implementation:**
```javascript
// headless_test.js (Node.js + Puppeteer)
const browser = await puppeteer.launch();
const page = await browser.newPage();
page.on('console', msg => {
  if (msg.type() === 'error') { 
    console.error('Browser error:', msg.text()); 
    process.exit(1); 
  }
});
await page.goto('file:///path/to/arxx_web.html');
await page.waitForTimeout(2000);
await page.screenshot({ path: 'menu.png' });
await browser.close();
```

#### Test 2.2: Menu Display Test (Visual Regression)
**Type:** Automated visual screenshot comparison  
**Trigger:** After successful build + load test

```bash
# Compare current screenshot with baseline:
# 1. Take screenshot of main menu (from Test 2.1)
# 2. Compare with stored baseline (arxx_menu_baseline.png)
# 3. Calculate pixel difference percentage
# 4. Fail if difference > 5% (allows small rendering variations)

Tool: ImageMagick compare command
Command: compare -metric AE menu.png menu_baseline.png
Threshold: <5% different pixels = PASS
```

#### Test 2.3: Text Rendering Test (OCR Verification)
**Type:** Automated text detection  
**Trigger:** After visual regression passes

```bash
# Use Tesseract OCR to read text from screenshot:
# 1. Extract main menu region
# 2. Run OCR to extract text
# 3. Verify expected strings are present:
#    - "ALTERNATE REALITY"
#    - "1 Create Character"
#    - "2 Load Game"
#    - etc.

Tool: Tesseract OCR
Expected Text:
  - ALTERNATE REALITY
  - Select an option
  - 1 - Create Character
  - 2 - Enter City
  - (all 7 menu options)

Pass Criteria: >90% of expected text detected
```

**Limitation:** OCR may struggle with pixelated fonts. Alternative: hardcode known menu positions and check for non-zero pixels.

#### Test 2.4: Canvas Rendering Test (WebGL State Check)
**Type:** Automated WebGL validation  
**Trigger:** During headless browser test

```javascript
// Inject test code into page context
page.evaluateOnNewDocument(() => {
  window.TEST_RESULTS = {
    canvas_width: 0,
    canvas_height: 0,
    webgl_enabled: false,
    sprite_count: 0,
    error_log: []
  };
});

// After game loads:
const results = await page.evaluate(() => {
  const canvas = document.querySelector('canvas');
  return {
    canvas_width: canvas?.width || 0,
    canvas_height: canvas?.height || 0,
    webgl_enabled: !!canvas?.getContext('webgl2'),
    sprite_count: document.querySelectorAll('[data-sprite]').length
  };
});

console.assert(results.canvas_width > 0, 'Canvas not sized');
console.assert(results.webgl_enabled, 'WebGL not available');
```

---

### Category 3: Unit Tests (If Applicable)

#### Test 3.1: Font Initialization Test
**Type:** C++ unit test (using gtest or similar)  
**Trigger:** On font.cpp changes

```cpp
TEST(FontSystem, InitFontLoadsFile) {
    InitFont();
    // Mock TTF_OpenFont return value
    ASSERT_NE(g_font, nullptr) << "Font should be loaded";
}

TEST(FontSystem, SetFontColorUpdatesState) {
    SetFontColour(255, 0, 0, 255);  // Red
    ASSERT_EQ(g_currentColor.r, 255);
    ASSERT_EQ(g_currentColor.g, 0);
    ASSERT_EQ(g_currentColor.b, 0);
}
```

**Challenge:** Emscripten/WebAssembly environment makes traditional unit testing difficult. Consider:
- Mock TTF library in tests
- Test on native build first (if available)
- Focus on integration tests instead

---

### Category 4: Performance Automated Tests

#### Test 4.1: Frame Rate Monitoring
**Type:** Automated performance profiling  
**Trigger:** After visual tests pass

```javascript
// Inject performance monitoring
page.evaluateOnNewDocument(() => {
  window.FPS_LOG = [];
  let lastTime = performance.now();
  
  (function measureFPS() {
    const now = performance.now();
    const fps = 1000 / (now - lastTime);
    window.FPS_LOG.push(fps);
    lastTime = now;
    requestAnimationFrame(measureFPS);
  })();
});

// After 10 seconds of gameplay:
const fps_data = await page.evaluate(() => {
  const values = window.FPS_LOG.slice(-600); // last 10 sec at 60 FPS
  const avg = values.reduce((a, b) => a + b) / values.length;
  const min = Math.min(...values);
  return { average: avg, minimum: min };
});

console.assert(fps_data.average > 50, `Low FPS: ${fps_data.average}`);
console.assert(fps_data.minimum > 30, `Frame drops detected: ${fps_data.minimum}`);
```

#### Test 4.2: Memory Leak Detection
**Type:** Automated memory profiling  
**Trigger:** After performance test

```javascript
const memBefore = performance.memory.usedJSHeapSize;

// Simulate 30 seconds of menu interaction
await page.evaluate(() => {
  for (let i = 0; i < 1800; i++) {
    // Simulate key presses, rendering frames
    requestAnimationFrame(() => {});
  }
});

const memAfter = performance.memory.usedJSHeapSize;
const memGrowth = memAfter - memBefore;

console.assert(memGrowth < 10_000_000, `Memory leak: +${memGrowth} bytes`);
// Threshold: <10MB growth over 30 seconds = PASS
```

---

### Category 5: Accessibility & Regression Tests

#### Test 5.1: Font Color Contrast (WCAG Compliance)
**Type:** Automated accessibility check  
**Trigger:** After visual tests pass

```javascript
// Extract text and background colors from screenshots
// Calculate contrast ratio using WCAG formula
// Verify minimum 4.5:1 for normal text, 3:1 for large text

const contrastRatio = (L1, L2) => {
  const lighter = Math.max(L1, L2);
  const darker = Math.min(L1, L2);
  return (lighter + 0.05) / (darker + 0.05);
};

// Check each text element
Menu_Brown_Text_Contrast = contrastRatio(brownColor, blackBackground);
Menu_Purple_Stats_Contrast = contrastRatio(purpleColor, blackBackground);

console.assert(Menu_Brown_Text_Contrast > 4.5, 'Low contrast for name text');
console.assert(Menu_Purple_Stats_Contrast > 4.5, 'Low contrast for stats');
```

---

## Automated Testing Pipeline Summary

```
Commit to display.cpp / font.cpp
    ↓
[1.2] SFML Reference Check ← Fails? Block commit
    ↓
[1.1] Build Compilation ← Fails? Block commit
    ↓
[2.1] WebAssembly Load Test ← Fails? Notify + block
    ↓
[2.2] Visual Regression Test ← Fails? Manual review
    ↓
[2.3] Text Detection (OCR) ← Fails? Manual review
    ↓
[2.4] WebGL State Check ← Fails? Notify
    ↓
[4.1] Performance Monitor ← Fails? Warn (don't block)
    ↓
[4.2] Memory Leak Detection ← Fails? Warn (don't block)
    ↓
[5.1] Accessibility Check ← Fails? Warn (don't block)
    ↓
✅ All Tests Passed → Mark build as "Ready for Testing"
```

---

## Test Implementation Technology Stack

### Required Tools:
- **Emscripten** - WebAssembly compilation (already in use)
- **Node.js + Puppeteer** - Headless browser automation
- **Tesseract OCR** - Optional: text extraction from screenshots
- **ImageMagick** - Image comparison for visual regression
- **jq** - JSON parsing for test results
- **Bash** - Test script orchestration

### Optional (for CI/CD):
- **GitHub Actions** - Run tests on every commit
- **Jest** - JavaScript test framework (for Puppeteer tests)
- **gtest** - C++ unit testing (for native builds if applicable)

---

## Implementation Priority

### Tier 1 (Essential - Must Have):
- [1.1] Build Compilation Check
- [1.2] SFML Reference Scan
- [2.1] WebAssembly Load Test
- [2.4] WebGL State Check

### Tier 2 (Important - Should Have):
- [2.2] Visual Regression Test
- [4.1] Performance Monitor
- [4.2] Memory Leak Detection

### Tier 3 (Nice to Have):
- [2.3] OCR Text Detection
- [3.1] C++ Unit Tests
- [5.1] Accessibility Check

---

## Estimated Effort

| Test Category | Setup Time | Per-Run Time | Maintenance |
|---|---|---|---|
| Build Compilation | 0 hours | 2 min | 0 hours |
| SFML Reference Scan | 0.5 hours | 10 sec | 0 hours |
| WebAssembly Load | 2 hours | 10 sec | 1 hour/month |
| Visual Regression | 3 hours | 5 sec | 2 hours/week |
| Text Detection (OCR) | 2 hours | 10 sec | 1 hour/month |
| WebGL State Check | 1 hour | 5 sec | 0 hours |
| Performance Monitor | 2 hours | 10 sec | 1 hour/month |
| Memory Leak Detection | 1 hour | 10 sec | 0 hours |
| C++ Unit Tests | 4 hours | 30 sec | 2 hours/month |

**Total Setup:** ~15-16 hours  
**Per-Run Time:** ~2-3 minutes total  
**ROI:** Very high (catches regressions automatically)