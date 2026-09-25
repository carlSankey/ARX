# ARX Web Build Test Plan
**Date**: 2026-06-22  
**Version**: 1.0  
**Status**: Ready for Testing

---

## Overview
This test plan validates the SFML-to-Web conversion and verifies web-specific functionality for the ARX game engine running on Emscripten/WebAssembly.

---

## Phase 1: Build Verification

### Test 1.1: Web Build Compilation
**Objective**: Verify the web build compiles without errors

**Steps**:
1. Navigate to `c:\Source\ARXX\web\`
2. Execute `build.cmd`
3. Observe compilation output

**Expected Results**:
- ✅ All source files compile successfully
- ✅ No SFML linker errors
- ✅ WebAssembly (.wasm) file generated in `build-web/`
- ✅ HTML wrapper file created (`arxx_web.html`)
- ✅ JavaScript glue code generated

**Pass Criteria**: Build completes with exit code 0

PASS
---

### Test 1.2: Build Artifacts Verification
**Objective**: Verify all required build outputs are present

**Steps**:
1. Check `build-web/` directory contents
2. Verify file existence and sizes

**Expected Artifacts**:
- `arxx_web.wasm` (>1MB - compiled game code)
- `arxx_web.js` (JavaScript runtime wrapper)
- `arxx_web.html` (Browser entry point)
- `arxx_web.data` (Asset file package)

**Pass Criteria**: All files present and non-zero size

PASS
---

## Phase 2: Web Launch & Initialization

### Test 2.1: Browser Loading
**Objective**: Verify web build loads in browser

**Steps**:
1. Open browser (Chrome/Firefox/Safari)
2. Navigate to `file:///c:/Source/ARXX/build-web/arxx_web.html`
3. Wait 5 seconds for Emscripten initialization
4. Check browser console for errors (F12)

**Expected Results**:
- ✅ Page loads without 404 or CORS errors
- ✅ WebAssembly module initializes
- ✅ No JavaScript errors in console
- ✅ Game window renders (even if blank)

**Pass Criteria**: Page loads and no critical console errors

**Note**: Local file:// URLs may have limitations; use a local HTTP server if needed:
```bash
python -m http.server 8000
# Then navigate to http://localhost:8000/build-web/arxx_web.html
```
** ACtual Results **
- ✅ Page loads without 404 or CORS errors
- ✅ WebAssembly module initializes
- failed No JavaScript errors in console 1 java erro
- ✅ Game window renders (even if blank)

f12 log
arxx_web.html:1 ARXX web scaffold booted.
arxx_web.html:1 ARXX web: appInit (SDL2 window + input path)
arxx_web.js:1 emscripten_set_main_loop_timing: Cannot set timing mode for main loop since a main loop does not exist! Call emscripten_set_main_loop first to set one up.
_emscripten_set_main_loop_timing @ arxx_web.js:1
arxx_web.html:1 Current Working Directory: /
arxx_web.html:1 Sprite2D: loaded image data/images/core/arfont.png (256x256)
arxx_web.html:1 Sprite2D: loaded texture 2 (256x256)
arxx_web.html:1 ARXX web: display initialized (640x400)
arxx_web.html:1 ARXX web: loading maps index...
arxx_web.html:1 ARXX web: maps index loaded OK
arxx_web.html:1 ARXX web: loading 3D resources...
arxx_web.html:1 ARXX web: 3D resources loaded
arxx_web.js:1 emscripten_set_main_loop_timing: Cannot set timing mode for main loop since a main loop does not exist! Call emscripten_set_main_loop first to set one up.
_emscripten_set_main_loop_timing @ arxx_web.js:1

---

### Test 2.2: Build Timestamp Display
**Objective**: Verify build ID system is working

**Steps**:
1. Open browser developer console (F12)
2. Look for build timestamp in initialization logs
3. Check if timestamp appears on main menu (when implemented)

**Expected Results**:
- ✅ Console shows build timestamp on startup
- ✅ Timestamp format: YYYY-MM-DD HH:MM:SS
- ✅ Example: "Build ID: 2026-06-22 12:49:00"

**Pass Criteria**: Timestamp correctly generated and logged

**Actual results**
- Fail - Console shows build timestamp on startup 
- Fail -  Timestamp format: YYYY-MM-DD HH:MM:SS
- Fail - Example: "Build ID: 2026-06-22 12:49:00"
---

## Phase 3: SDL2/WebGL Rendering

### Test 3.1: Graphics Context Initialization
**Objective**: Verify SDL2/WebGL context creation

**Steps**:
1. With game loaded in browser
2. Check WebGL context status in console
3. Verify no WebGL errors

**Expected Results**:
- ✅ WebGL 2.0 context created successfully
- ✅ SDL2 initialization messages in log
- ✅ Canvas element properly sized

**Pass Criteria**: WebGL context active, no errors

**actual results **
- PASS WebGL 2.0 context created successfully - PAss ARXX web: appInit (SDL2 window + input path)
- PASS SDL2 initialization messages in log - ARXX web: appInit (SDL2 window + input path)
- PASS Canvas element properly sized - P
---

### Test 3.2: Asset Loading
**Objective**: Verify game assets load from prepackaged data

**Steps**:
1. Monitor Network tab in browser DevTools
2. Check for `.data` file loading
3. Verify images/audio files accessible

**Expected Results**:
- ✅ `arxx_web.data` loads successfully
- ✅ No 404 errors for game assets
- ✅ Asset file size reasonable (>5MB for full game data)

**Pass Criteria**: All assets load without errors

**Actual results**+- ✅ `arxx_web.data` loads successfully
-PASS No 404 errors for game assets
-PASS Asset file size reasonable (>5MB for full game data) size 0.1kb
---

## Phase 4: Input System

### Test 4.1: Keyboard Input Detection
**Objective**: Verify keyboard input capture in web build

**Steps**:
1. With game loaded, press various keys
2. Check browser console for input events
3. Try arrow keys, WASD, number keys, ESC

**Expected Results**:
- ✅ Keyboard events detected
- ✅ Console logs key presses
- ✅ No browser shortcuts interfering

**Pass Criteria**: Keyboard input captured correctly


**Actual results**
+- ✅ Keyboard events detected - in menu screen only
- ✅ Console logs key presses - in menu screen only
- ✅ No browser shortcuts interfering 
---

### Test 4.2: Game Loop Responsiveness
**Objective**: Verify game loop runs at acceptable frame rate

**Steps**:
1. Open browser Performance tab
2. Record for 10 seconds with game running
3. Check frame rate and CPU usage

**Expected Results**:
- ✅ Frame rate ≥ 30 FPS
- ✅ Smooth rendering without stuttering
- ✅ CPU usage reasonable (<50% on modern hardware)

**Pass Criteria**: Maintains >30 FPS consistently

---

## Phase 5: Function Stubs Verification

### Test 5.1: Stub Function Calls
**Objective**: Verify stub functions execute without crashing

**Steps**:
1. Console logs when stubs are called
2. Monitor for "Uncaught ReferenceError" messages
3. Verify game doesn't crash on stub calls

**Expected Results**:
- ✅ No undefined symbol errors
- ✅ Stubs return gracefully
- ✅ Game continues running

**Pass Criteria**: All stub functions callable without errors

**Stub Functions to Verify**:
- `scenarioEntrance(0)` - City scenario
- `draw3DView()` - 3D viewport rendering
- `checkForItemsHere()` - Item detection
- `selectItem(0)` - Item selection

---

### Test 5.2: Build ID Accessibility
**Objective**: Verify build ID can be retrieved at runtime

**Steps**:
1. In browser console, run: `Module.getBuildId()`
2. Verify output format and value
3. Record timestamp value

**Expected Results**:
- ✅ Function returns string
- ✅ Format: "YYYY-MM-DD HH:MM:SS"
- ✅ Timestamp matches build time (within 1 minute)

**Pass Criteria**: Build ID correctly accessible and formatted

---

## Phase 6: Performance Profiling

### Test 6.1: Memory Usage
**Objective**: Check WASM memory footprint

**Steps**:
1. Open DevTools Memory tab
2. Take heap snapshot on startup
3. Monitor memory growth over 1 minute

**Expected Results**:
- ✅ Initial WASM memory: 16-32 MB
- ✅ Memory growth: <5 MB per minute during idle
- ✅ No memory leaks detected

**Pass Criteria**: Memory usage within expected bounds

---

### Test 6.2: Compilation Overhead
**Objective**: Measure build time

**Steps**:
1. Clean build directory
2. Run build.cmd with timer
3. Record total time

**Expected Results**:
- ✅ Build time: 30-120 seconds (depends on hardware)
- ✅ Reproducible build times
- ✅ No memory exhaustion during build

**Pass Criteria**: Build completes in reasonable time

---

## Phase 7: Error Handling

### Test 7.1: Missing Dependencies
**Objective**: Verify graceful handling of missing stubs

**Steps**:
1. Intentionally comment out a stub function
2. Attempt rebuild
3. Observe linker error message

**Expected Results**:
- ✅ Clear error message with symbol name
- ✅ No silent failures
- ✅ Build stops with appropriate exit code

**Pass Criteria**: Errors reported clearly

---

### Test 7.2: Browser Console Errors
**Objective**: Verify no spurious console errors

**Steps**:
1. Load game in browser
2. Check console for warnings/errors
3. Filter for WASM-related issues

**Expected Results**:
- ✅ No "Uncaught" exceptions
- ✅ No "undefined is not a function" errors
- ✅ Only expected stub/feature warnings

**Pass Criteria**: Console clean of critical errors

---

## Test Result Summary Template

| Test | Result | Notes | Pass/Fail |
|------|--------|-------|-----------|
| 1.1 - Compilation | | | |
| 1.2 - Artifacts | | | |
| 2.1 - Browser Load | | | |
| 2.2 - Build ID | | | |
| 3.1 - WebGL Init | | | |
| 3.2 - Assets Load | | | |
| 4.1 - Keyboard | | | |
| 4.2 - Frame Rate | | | |
| 5.1 - Stubs | | | |
| 5.2 - Build ID API | | | |
| 6.1 - Memory | | | |
| 6.2 - Build Time | | | |
| 7.1 - Error Handling | | | |
| 7.2 - Console Clean | | | |

**Overall Result**: [ ] PASS [ ] FAIL

---

## Known Limitations & Notes

⚠️ **Current Known Issues**:
1. Game loop not fully implemented - stubs return early
2. Main menu may not fully render
3. 3D viewport rendering stubbed (displays blank)
4. Linker may report undefined symbols for unimplemented game functions

✅ **Working Features**:
- Build compilation and WASM generation
- WebGL2 context creation
- Asset preloading
- Keyboard input capture
- Build timestamp generation

---

## Next Steps After Testing

1. **If all tests pass**: 
   - Proceed to game loop implementation
   - Begin main menu UI rendering
   - Integrate scenario entrance logic

2. **If tests fail**:
   - Document failures with console output
   - Check build output for linker errors
   - Verify Emscripten SDK installation
   - Review error messages for missing stubs

3. **Performance optimization**:
   - Profile with Chrome DevTools
   - Optimize WebAssembly module size
   - Consider streaming asset loading

---

## Testing Environment Requirements

**Minimum**:
- Windows 11 or equivalent
- Chrome/Firefox/Safari latest version
- Emscripten SDK 3.1.27+ (for build)
- 4GB RAM minimum
- 500MB free disk space

**Recommended**:
- 8GB+ RAM
- SSD for faster builds
- Dual-monitor setup (DevTools on second monitor)
- Modern hardware for consistent frame rates

---

## Contact & Support

For test failures or issues:
1. Check browser console (F12)
2. Capture full build output
3. Note exact error messages
4. Document OS and browser version
5. Report with test number and results

---

**Test Plan Version**: 1.0  
**Last Updated**: 2026-06-22 12:49 UTC  
**Next Review**: After initial test run completion