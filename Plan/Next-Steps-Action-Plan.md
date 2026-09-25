# Next Steps - Action Plan

## Current Status
- SFML includes are still present in the codebase (not removed)
- Test plan shows 5 critical issues blocking progress
- Browser-testable components identified but blocked by graphics

## Immediate Next Steps (Priority Order)

### Step 1: Decide on Graphics Library Strategy
**Decision Required:** Which graphics library to use?

Options:
A. **SDL2** (Already exists: `platform/SDL2Window.cpp`)
   - Pros: Already started, multiplatform, C++ friendly
   - Cons: Need to complete migration

B. **Web Canvas API** (For Emscripten/WebAssembly)
   - Pros: Native web support, lightweight
   - Cons: Different from native build

C. **Hybrid** (SDL2 for native, Canvas for web)
   - Pros: Both platforms supported
   - Cons: More work, dual implementation

**Recommendation:** Given this is a WEB-ONLY project (per test plan), focus on **Canvas API for Emscripten** first.

---

### Step 2: Create SFML Replacement Stubs
Instead of removing SFML entirely, create stub/wrapper functions that map SFML calls to the new graphics library.

**Files to Modify (in order):**
1. `audio.cpp` - Remove SFML audio, create no-op audio stubs (you said audio can wait)
2. `misc.cpp` - Replace SFML event handling with web/SDL2 input
3. `game.cpp` - Remove SFML textures/sprites, use Canvas/SDL2
4. `display.cpp` - Core graphics - replace RenderWindow calls
5. `automap.cpp` - Map rendering system
6. `lyrics.cpp` - Text rendering

**Approach:** Create stub implementations that don't crash, then add real implementations.

---

### Step 3: Audio System (Deferred, Low Priority)
Current status: ✅ Audio IS playing (per test plan line 109: "✅ Audio plays")

**Action:** Skip audio implementation for now. Just remove SFML/Audio includes and create no-op stubs.

**Stub Examples:**
```cpp
// audio.cpp stubs
class SoundBuffer { };
class Sound { 
  public:
    void play() { }
    int getStatus() { return 0; } // Stopped
};
class Music {
  public:
    void play() { }
    void stop() { }
};
```

---

### Step 4: Input System (Critical for Testing)
**Current Issue:** misc.cpp still using sf::Event, sf::Keyboard::Key

**Solution:** Create input abstraction:
```cpp
// In misc.cpp, replace sf::Event with:
struct InputEvent {
    int type; // key pressed, text entered, quit
    std::string key;
    char unicode;
};

// Instead of sf::Keyboard::Key::Left, use string "left"
```

**Browser Implementation:** Use JavaScript keyboard events from Emscripten

---

### Step 5: Graphics - Start Simple
**Phase 1 (Minimal):** Text-only interface
- Remove all sprite/texture rendering
- Keep text output working
- Test character creation flow (name entry, gender, confirmation)

**Phase 2 (Medium):** Basic 2D
- Implement Canvas rectangle drawing
- Draw simple panels (info boxes)
- Display compass as ASCII or simple shapes

**Phase 3 (Advanced):** 3D Corridor
- Implement textured 3D view
- Door detection
- Collision system

---

## Specific Test Plan Issues to Address

### Issue #1: Missing Logo (Line 307)
**Current:** Logo not displaying on main menu
**Fix:** 
- Remove `sf::Sprite` rendering of logo
- Create Canvas-based logo display OR
- Skip logo rendering entirely for now

### Issue #2: Font Rendering Background (Line 316)
**Current:** Black/white background hiding colors
**Fix:** 
- Use Canvas text rendering instead of sprite-based fonts
- Or implement proper alpha blending in sprite system

### Issue #3: UI Alignment (Line 325)
**Current:** Compass offset, white lines/boxes
**Fix:**
- Recalculate coordinate offsets for Canvas-based layout
- Remove hardcoded SFML-specific positioning

### Issue #4: Character Data Mismatch (Line 334)
**Current:** Name/stats/XP incorrect
**Fix:**
- Not graphics-related, likely data transfer issue
- Debug: Check createCharacter.cpp → game.cpp data flow
- May just need printf debugging to trace values

### Issue #5: Consumable Display (Line 343)
**Current:** Wrong numbers displayed
**Fix:**
- Check inventory array indexing
- Verify display coordinates
- Not graphics-critical, likely logic bug

---

## Recommended Execution Order

### Week 1 (Graphics Library Setup):
1. Choose graphics library (Canvas for web)
2. Create initial stubs for SFML functions
3. Get code compiling without SFML dependency errors

### Week 2 (Audio & Input):
1. Create audio no-op stubs
2. Replace SFML event system with simple key handlers
3. Test character name entry (text input only)

### Week 3 (Basic Graphics):
1. Implement Canvas text rendering
2. Draw simple panels/borders
3. Display stats panel (text + boxes)

### Week 4 (3D & Polish):
1. Implement 3D corridor rendering
2. Door/collision detection
3. Visual feedback for compass/facing

---

## Testing Milestones

**Milestone 1 - Text Only:**
- ✅ Character name entry works
- ✅ Gender selection works
- ✅ Stats display as text
- ✅ No crashes or SFML errors

**Milestone 2 - Basic Graphics:**
- ✅ Panel borders drawn
- ✅ Logo displays (Canvas version)
- ✅ Info panels positioned correctly
- ✅ Compass displayed

**Milestone 3 - 3D Rendering:**
- ✅ Corridor displays textured
- ✅ Movement updates view
- ✅ Door detection works
- ✅ Collision prevents wall clipping

**Milestone 4 - Full Game:**
- ✅ All test plan tests pass
- ✅ City & Dungeon both playable
- ✅ Audio working
- ✅ Smooth performance (60 FPS)

---

## Decision Point

**What should we do first?**

Option A: Implement text-only interface (fastest path to testable game)
Option B: Set up graphics library properly (slower but cleaner)
Option C: Focus on fixing data bugs (character name/stats issue)

**Recommendation:** Option A (Text-Only) → Option B (Graphics) → Option C (Polish)

This gives testable progress quickly while building toward full graphics implementation.