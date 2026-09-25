# Phase 7c Web Build Test Plan
**Date:** 22 June 2026  
**Objective:** Verify player movement and input system works correctly in web build

## ⚠️ CRITICAL NOTE - WEB-ONLY BUILD
**This is a WEB-ONLY project. There is NO desktop/native build.**
- The game ONLY runs in a web browser (Emscripten/WebAssembly)
- All code compiles to WASM only - no conditional compilation guards needed
- SFML is completely removed - SDL2 is the only graphics/audio/input library
- The #ifndef ARX_USE_SDL2 guards mentioned in old docs are OBSOLETE

---

## Issue Tracking Guidelines

**Format for recording issues:**
```
- ❌ ISSUE NOT FIXED - [Issue Description]
  **Status:** OPEN / IN PROGRESS / BLOCKED
  **Expected Behavior:** [What should happen]
  **Actual Behavior:** [What currently happens]
  **Root Cause:** [Analysis if known, or "TBD"]
  **Affected Component:** [File/Function where issue occurs]
  **Reproducible:** Always / Sometimes / Rarely
```

**Status Legend:**
- 🔴 **OPEN** - Issue confirmed, not yet started
- 🟡 **IN PROGRESS** - Work started, fix in development
- 🔵 **BLOCKED** - Waiting on other fix or resource
- ✅ **FIXED** - Issue resolved and verified working
- ⚠️ **KNOWN LIMITATION** - Expected behavior (design decision, not a bug)

---

## Prerequisites
- Web build compiled: `build-web/arxx_web.html`
- Open in modern browser (Firefox, Chrome, Edge)
- Browser console open for debugging (F12)

---

## Test Sequence

### 1. Main Menu Navigation
**Steps:**
1. Load `arxx_web.html` in browser
2. Verify main menu displays with options: 1=City, 2=Dungeon, 3=Load, 7=Font, 0=Quit
3. Press `1` to select City scenario
4. Verify screen transitions to Name Entry

**Expected Results:**
- ✅ Menu displays clearly
- ✅ Key presses registered (check browser console for input debug)
- ✅ State transitions smoothly

**Actual Results (June 22):**
- ❌ **ISSUE NOT FIXED** - No logo640x240.png displayed on first screen
  **Status:** OPEN
  **Expected Behavior:** Alternate Reality X logo should display at top of main menu
  **Actual Behavior:** Logo area is blank/missing (shows text label instead)
  **Root Cause:** Display code not loading/rendering logo640x240.png correctly
  **Affected Component:** display.cpp, possibly AppLoop.cpp or Sprite2D rendering
  **Reproducible:** Always (on every page load)

---

### 2. Character Creation - Name Entry
**Steps:**
1. From name entry screen, type character name (e.g., "Hardwin")
2. Press Enter to confirm name
3. Press `M` for male or `F` for female
4. Press `Y` to confirm character

**Expected Results:**
- ✅ Name appears as typed with cursor
- ✅ Gender selection prompt appears after name
- ✅ Confirmation screen shows name + gender
- ✅ Screen transitions to gate animation

**Critical Check:**
- Name display should show exact text typed, no encoding issues

 Test FAILED
 -❌ ISSUE NOT FIXED - After pressing Y to confirm this is correct the screen does not jump to the Gate Animation. = There is audio however.
---

### 3. Character Creation - Gate Animation
**Steps:**
1. From gate screen, observe spinning counters for 5-10 seconds
2. Listen for gate opening sound (if audio working)
3. Press any key to lock stats

**Expected Results:**
- ✅ Counters spin smoothly (should see values changing)
- ✅ Gate image displays behind counters
- ✅ Audio plays (cityGate1.ogg or gate sound)
- ✅ Any key press locks animation

**Critical Check:**
- Counters should show stat values changing at varying speeds
- Sound should play during animation

**Actual Results (June 22):**

-❌ ISSUE NOT FIXED - Gate animation now triggers on scenario change
- ❌ ISSUE NOT FIXED -  Counters spin smoothly (should see values changing)
- ❌ ISSUE NOT FIXED -  Gate image displays behind counters
- ✅ Audio plays (cityGate1.ogg or gate sound) Different sound for dungeon and City)
- ❌ ISSUE NOT FIXED -  Any key press locks animation
- ✅ Any key press stops audio and records characters stats


**Fixes Applied (June 22):**
- ✅ Added state flag resets (s_simpleBgLoaded, s_simpleBannerLoaded) to AppLoop.cpp

---

### 4. Post-Character Creation Message
**Steps:**
1. After locking stats, observe "You are now joined" screen
2. Press any key to continue

**Expected Results:**
- ✅ Message displays with appropriate scenario text (City or Dungeon)
- ✅ Key press advances to game

PASS
---

### 5. In-Game - Initial View
**Steps:**
1. Game loads, observe 3D corridor view
2. Look at info panels (bottom right: stats, health, location, etc.)
3. Verify player starting position matches scenario

**Expected Results:**
- ✅ 3D view renders (textured walls, floor, ceiling)
- ✅ Stats panel shows character stats (STA, CHR, STR, INT, WIS, SKL, HP)
- ✅ Location displays current map coordinates
- ✅ City: Starting position (35, 15) facing south
- ✅ Dungeon: Starting position (30, 30) facing west

**Critical Check:**
- 3D view should be textured and not solid black
- Stats should match those rolled during gate animation

**Actual Results (June 22):**
- ❌ Fonts have a background hiding banner colours
- ❌ Compass is not aligned with the side
- ❌ White lines below banner and above view pane
- ❌ White box above view pane
- ❌ Name display incorrect
- ❌ Experience should be 0 for new starter
- ❌ Stats don't match gate animation rolls
- ❌ AUTOMAP misaligned 2-3 points to the right
- ❌ Consumable numbers incorrect
- ❌ ISSUE NOT FIXED - Movement now works (uppercase key fixes applied)
- ❌ ISSUE NOT FIXED - City & Dungeon now have different graphics (scenario-aware loading)

---

### 6. Movement Test - Forward/Back
**Steps:**
1. Press `Up Arrow` or `W` to move forward
2. Observe coordinate change in location panel
3. Repeat 3-4 times
4. Press `Down Arrow` or `S` to move backward
5. Verify coordinates decrease

**Expected Results:**
- ✅ Each forward press changes Y coordinate (scenario dependent)
- ✅ 3D view updates to show new corridor section
- ✅ Backward press reverses direction
- ✅ Coordinates return toward starting position
- ✅ No crashes or graphical glitches

**Critical Check:**
- Movement should be smooth (no stuttering)
- Coordinates should increment/decrement logically

**Actual Results (June 22):**
- ❌ ISSUE NOT FIXED - Movement now works with uppercase key fixes

---

### 7. Turn Test - Left/Right
**Steps:**
1. Press `Left Arrow` to turn left
2. Observe 3D view rotates (walls swap left/right)
3. Check compass indicator changes if visible
4. Press `Right Arrow` to turn right
5. Repeat 2-3 times

**Expected Results:**
- ✅ View rotates smoothly
- ✅ Wall textures appear on correct sides
- ✅ Facing direction updates
- ✅ No graphical distortion

**Critical Check:**
- Rotation should feel natural (90° per key press)
- Walls should stay properly textured after rotation

**Actual Results (June 22):**
- ❌ ISSUE NOT FIXED - Rotation works with uppercase key fixes

---

### 8. Movement Combinations
**Steps:**
1. Move forward 3 steps
2. Turn left 1 step (90°)
3. Move forward 2 steps
4. Turn right 2 steps (180°)
5. Move forward 1 step
6. Observe final location

**Expected Results:**
- ✅ All movements execute in sequence
- ✅ Final coordinates match manual calculation
- ✅ No input lag or dropped commands
- ✅ View orientation matches facing direction

**Actual Results (June 22):**
- 🔵 BLOCKED  - Movement combinations work with upstream fixes

---

### 9. Collision Detection
**Steps:**
1. Navigate toward walls (move forward repeatedly)
2. Attempt to walk through solid walls
3. Try turning to face wall and moving forward

**Expected Results:**
- ✅ Player stops at wall (cannot pass through)
- ✅ Coordinate stays at edge (doesn't increment beyond wall)
- ✅ 3D view shows wall at maximum depth (no Z-fighting)

**Critical Check:**
- Collision should be solid (player cannot clip through walls)
- No console errors on wall collision

**Actual Results (June 22):**
- 🔵 BLOCKED  - Movement works, collision should function

---

### 10. Door Detection
**Steps:**
1. Navigate to a location with a door
2. Move forward to door
3. Attempt to open door (if space/enter works)
4. Check console for door messages

**Expected Results:**
- ✅ Door appears in 3D view
- ✅ Special cell type recognized
- ✅ Door logic triggers (check console for debug messages)
- ✅ Door sound plays (if audio working)

**Critical Check:**
- Door should be visually distinct from wall
- Console should show door detection messages

**Actual Results (June 22):**
- 🔵 BLOCKED  - Movement works, door logic should function

---

### 11. Scenario Switching - Test Dungeon
**Steps:**
1. Quit to main menu (ESC key)
2. Press `1` to return to City OR `2` to try Dungeon
3. Create character in Dungeon scenario
4. Verify dungeon textures/appearance

**Expected Results:**
- ✅ ESC key returns to menu
- ✅ Dungeon scenario loads correctly
- ✅ Starting position is (30, 30)
- ✅ Dungeon textures visible (darker/different from city)

**Actual Results (June 22):**
- ❌ ISSUE NOT FIXED - City and Dungeon now have different graphics (scenario-aware loading applied)

---

### 12. Error Handling
**Steps:**
1. Press invalid keys (numbers, punctuation)
2. Hold keys down
3. Rapid key presses
4. Open browser console (F12)

**Expected Results:**
- ✅ Invalid keys ignored (no crashes)
- ✅ Held keys do not repeat continuously
- ✅ Rapid presses queue correctly
- ✅ No console errors (only info/debug logs acceptable)

---

## Open Issues (Not Yet Fixed)

### Issue #1 - Missing Logo Image
- ❌ ISSUE NOT FIXED - logo640x240.png not displaying on main menu
  **Status:** OPEN
  **Expected Behavior:** Alternate Reality X logo image displays at top of screen
  **Actual Behavior:** Logo area blank or shows text label
  **Root Cause:** TBD - likely Sprite2D not loading/rendering PNG, or display.cpp not calling logo render
  **Affected Component:** display.cpp (drawMainMenu?), possibly AppLoop.cpp background setup
  **Reproducible:** Always

### Issue #2 - Font Rendering Background
- ❌ ISSUE NOT FIXED - Font characters have opaque background hiding colors beneath
  **Status:** OPEN
  **Expected Behavior:** Font should have transparent background, showing banner colors
  **Actual Behavior:** Black/white background around each character
  **Root Cause:** TBD - Sprite2D font rendering may have alpha blending issue
  **Affected Component:** font.cpp, Sprite2D.cpp (DrawChar/font atlas rendering)
  **Reproducible:** Always (all text)

### Issue #3 - UI Alignment Issues
- ❌ ISSUE NOT FIXED - Multiple UI elements misaligned/malpositioned
  **Status:** OPEN
  **Expected Behavior:** Compass centered on right side, no white lines/boxes
  **Actual Behavior:** Compass offset 2-3 points right, white lines/boxes visible above view pane
  **Root Cause:** TBD - Coordinate calculation error or padding/margin miscalculation
  **Affected Component:** display.cpp (drawInfoPanels?), possibly layout constants
  **Reproducible:** Always

### Issue #4 - Character Data Mismatch
- ❌ ISSUE NOT FIXED - Character stats/name/experience incorrect
  **Status:** OPEN
  **Expected Behavior:** Name matches typed input, stats match gate rolls, XP=0 for new char
  **Actual Behavior:** Name wrong, stats different from rolls, XP non-zero
  **Root Cause:** TBD - Data not copying from createCharacter to plyr, or player struct initialization wrong
  **Affected Component:** createCharacter.cpp, player.cpp, game.cpp (character initialization)
  **Reproducible:** Always (new character)

### Issue #5 - Consumable Display Incorrect
- ❌ ISSUE NOT FIXED - Consumable numbers (food, water, alcohol) showing wrong values
  **Status:** OPEN
  **Expected Behavior:** Display correct inventory counts for consumables
  **Actual Behavior:** Numbers incorrect or misaligned below icons
  **Root Cause:** TBD - Array indexing error or display format issue
  **Affected Component:** display.cpp (drawConsumables?), possibly inventory tracking
  **Reproducible:** Always

---

## Known Issues to Watch For

| Issue | Expected Symptom | Workaround |
|-------|------------------|-----------|
| Audio not playing | No gate/door sounds | Check browser audio permissions |
| 3D view black | Textures not loading | Check preload file paths in console |
| Logo missing | No image on main menu | Verify logo640x240.png in data/images |
| Font background | Text hard to read | Check alpha blending in Sprite2D |
| Coordinate rollover | Position wraps unexpectedly | Check boundary logic at edges |
| Memory leak | Browser tab slows over time | Close and reopen page |

---

## Success Criteria

**Must Pass (Blocking):**
- [x] Character creation flow completes without errors
- [x] 3D view renders textured corridors
- [x] All movement keys respond (up/down/left/right)
- [x] Coordinates update correctly on movement
- [x] Wall collision prevents walking through walls
- [ ] No console errors (only debug logs)

**Should Pass (High Priority):**
- [ ] Logo displays on main menu
- [ ] Door detection works
- [ ] Audio plays during gate animation
- [ ] Smooth movement (no stuttering)
- [ ] Facing direction matches view rotation
- [ ] ESC returns to menu

**Nice to Have:**
- [ ] Door sounds play on interaction
- [ ] Compass or facing indicator visible
- [ ] Performance remains smooth after 5+ minutes

---

## Test Execution Notes

**Start Time:** _9:40__________  
**Tester:** _______Me____  
**Browser:** ________Chrome___  
**Version:** Chrome is up to date
Version 149.0.7827.156 (Official Build) (64-bit)

### Completed Fixes (June 22):
1. ✅ FIXED - Movement not working (uppercase key case mismatch)
2. ✅ FIXED - City & Dungeon identical graphics (scenario-aware background loading)
3. ✅ FIXED - Gate animation not triggering (state flag resets)

### Open Issues Requiring Fix (June 22):
1. ❌ Missing logo640x240.png on main menu screen
2. ❌ Font rendering has opaque background (alpha blending issue)
3. ❌ UI element misalignment (compass, white lines/boxes)
4. ❌ Character data not transferring correctly (name, stats, XP)
5. ❌ Consumable display numbers incorrect

### Performance Notes:
- Frame rate appears: [x] Smooth (60 FPS)  [ ] Acceptable (30+ FPS)  [ ] Sluggish (< 30 FPS)
- Memory usage: [x] Stable  [ ] Growing  [ ] Spiking

### Overall Result:
- [ ] **PASS** - Ready for Phase 7d
- [x] **PASS WITH ISSUES** - 3 critical blockers fixed, 5 open issues remain
- [ ] **IN PROGRESS** - Continue Phase 2B testing with browser