# Black Background Issue - Investigation Log

**Date:** 2025-06-29 (late night session)
**Issue:** Web build shows black background instead of city sky/3D walls
**Status:** UNRESOLVED - needs further investigation

---

## What Was Implemented

### 1. Full 3D Corridor Rendering (renderer/View3D.cpp)
- Implemented complete wall/ceiling rendering loop in `view3D_draw()`
- Added `processCell()` and `drawCellWalls()` functions
- Iterates over 13 depth x 25 columns grid
- Renders left/right/front walls with proper texture mapping
- **Floor rendering DISABLED** (commented out) due to rendering issues with pixel-art textures
- Ceiling rendering enabled for interior zones

### 2. Texture Loading Fix
- Changed from `textures_alternate/` to `textures_original/` directory
- Uses `textures.txt` names (Atari 8-bit style: "citywall", "wall", etc.)
- Converts all textures to RGBA32 for WebGL compatibility
- Loads 70+ wall textures successfully

### 3. Background System
- **City outdoor (zone 99):** Shows direction-appropriate sky background
  - EAST → cityBackE (bgIdx=4)
  - SOUTH → cityBackS (bgIdx=5)
  - NORTH → cityBackN (bgIdx=6)
  - WEST → cityBackW (bgIdx=7)
- **Interior zones (zone != 99):** Draws solid black background
- Background shader uses full-screen textured quad
- Added extensive debug logging to track background decisions

### 4. Spawn Position Changes
- **Original:** (35, 15) - southern city area
- **Changed to:** (1, 1) - near corner of map
- **Reason:** User requested spawn near center to see walls

### 5. Zone Assignment Fix
- Moved `plyr.zone = 99` to AFTER `setCurrentZone()` call
- **Reason:** `setCurrentZone()` was overwriting zone=99 with interior zone

---

## Current Behavior

From console logs:
```
ARXX web: starting at (1,1) facing=2 zoneSet=17 timeOfDay=0
View3D: Drawing city background (zone 99)
View3D: city bgIdx=6 tex=18 name=cityBackN
View3D: City background drawn
```

**What this tells us:**
1. ✅ Player spawns at (1,1) as intended
2. ✅ zone=99 is correctly set (outdoor city)
3. ✅ Background IS being drawn (cityBackN texture loaded)
4. ❌ **But screen is still black!**

---

## What Was Tried

1. **Changed spawn position** from (35,15) → (1,1)
   - Thought maybe (35,15) was in an interior zone
   - Didn't fix the issue

2. **Added debug logging** to view3D_drawBackground()
   - Confirms background IS being drawn
   - Confirms correct texture is selected (cityBackN)

3. **Fixed zone assignment order**
   - Moved `plyr.zone = 99` after `setCurrentZone()`
   - Confirmed zone=99 in logs

4. **Verified background shader compiles**
   - Shader program compiles successfully
   - Texture loads (tex=18 is non-zero)

---

## Hypotheses for Tomorrow

### Hypothesis 1: Background is drawn but immediately overwritten
- The background is drawn first in `view3D_draw()`
- Then walls are rendered with depth test enabled
- **Maybe the wall rendering is clearing the background?**
- **Test:** Comment out wall rendering temporarily, see if background shows

### Hypothesis 2: Background quad is wrong size/position
- Background uses vertices: (-1,-1) to (1,1) in NDC
- Should cover full screen
- **Maybe the viewport is wrong?**
- **Test:** Log viewport values, check if they match window size

### Hypothesis 3: Background texture is actually black/empty
- Texture ID 18 loads successfully
- But maybe the image file is black or corrupted
- **Test:** Try drawing a solid color fallback (blue sky) instead of texture

### Hypothesis 4: Depth test is clearing the background
- Background disables depth test
- But then walls enable it and clear depth buffer
- **Maybe glClear() is clearing color buffer too?**
- **Test:** Check if glClear() is called after background draw

### Hypothesis 5: Player is facing a wall immediately
- At (1,1) facing NORTH, maybe frontwall is solid
- Wall texture might be black/dark
- **Test:** Log wall types being rendered, check texture IDs

---

## Key Code Locations

### Background Drawing (renderer/View3D.cpp:309-439)
```cpp
void view3D_drawBackground()
{
    // City outdoor zone 99 check
    if ((plyr.scenario == 0) && (plyr.zone != 99)) {
        // Draws solid black - THIS IS THE PROBLEM?
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        return;
    }
    
    // City background selection
    if (plyr.scenario == 0) {
        switch (plyr.facing) {
            case NORTH: bgIdx = 6; break; // cityBackN
        }
    }
}
```

### Main Draw Loop (renderer/View3D.cpp:578-637)
```cpp
void view3D_draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Draw background for zone 99
    if ((plyr.scenario == 0) && (plyr.zone == 99)) {
        view3D_drawBackground();  // Should draw sky
    }
    
    // Then draw walls...
    for (int d = 0; d < k_depth; d++) {
        for (int c = 0; c < midcol; c++) {
            processCell(mvp, c, d);  // Renders walls
        }
    }
}
```

### Spawn Position (platform/AppLoop.cpp)
```cpp
if (plyr.scenario == 0) {
    plyr.x = 1; plyr.y = 1;  // Changed from (35,15)
    plyr.facing = NORTH;
}
plyr.zone = 99;  // Set AFTER setCurrentZone()
```

---

## Next Steps for Tomorrow

1. **Add more logging:**
   - Log when walls are drawn and what textures they use
   - Log viewport values
   - Log clear() calls

2. **Test Hypothesis 1:** Comment out wall rendering, see if background shows

3. **Test Hypothesis 3:** Replace texture with solid blue color

4. **Check if issue is in view3D_drawBackground() early return:**
   - The function has an early return for `zone != 99`
   - But we confirmed zone=99, so this shouldn't trigger
   - **Unless there's a logic error in the condition?**

5. **Verify the background is actually being called:**
   - Add logging BEFORE the if statement
   - Add logging AFTER each branch

6. **Check if glClear() in view3D_draw() is clearing after background:**
   - Background is drawn first
   - Then walls are rendered
   - But glClear() is called BEFORE background
   - **This should be fine... unless something else clears after**

---

## Files Modified

1. `renderer/View3D.cpp` - 3D rendering, background system, debug logging
2. `platform/AppLoop.cpp` - Spawn position, zone assignment order
3. `web/build.cmd` - No changes (build script unchanged)

## Build Status
- ✅ Builds successfully (warnings only, no errors)
- ✅ WASM loads in browser
- ✅ Game initializes and enters InGame state
- ❌ Screen remains black despite background being drawn

---

## Questions to Answer

1. Is the background actually appearing but being covered by black walls?
2. Is the background texture actually loaded and valid?
3. Is there a second glClear() call somewhere that's wiping the screen?
4. Are the walls rendering as black instead of textured?
5. Is the viewport correct for the background quad?