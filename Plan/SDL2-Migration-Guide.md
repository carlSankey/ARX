# SDL2/bgfx Web Port Migration Guide

## Status: Foundation Complete

### What's Done ✅
- Platform abstraction layer (`platform/PlatformWindow.h`)
- SDL2 window implementation (`platform/SDL2Window.cpp`)
- vcpkg.json updated with SDL2 dependencies
- Fixed GL_STACK_OVERFLOW and weaponProbabilities bugs

### Architecture

```
Native Build:         Web Build:
┌─────────────┐      ┌─────────────┐
│ SFML Window │      │ SDL2Window  │
│ (legacy)    │      │ + Emscripten│
└─────────────┘      └─────────────┘
       │                     │
       v                     v
┌──────────────────────────────┐
│   Common Game Code           │
│   (explore, encounter, etc)  │
└──────────────────────────────┘
       │                     │
       v                     v
┌─────────────┐      ┌─────────────┐
│ OpenGL      │      │ bgfx        │
│ (immediate) │      │ (WebGL)     │
└─────────────┘      └─────────────┘
```

## Next Steps

### Phase 1: Window Migration (1-2 days)
1. Add `#define ARX_USE_SDL2 0` to project (keep SFML for now)
2. Create `SFMLWindow.cpp` adapter implementing `IWindow`
3. Replace `extern sf::RenderWindow App` with `extern arx::IWindow* App`
4. Update all `App.method()` calls to `App->method()`
5. Test native build still works

### Phase 2: Input Migration (1 day)
1. Replace `sf::Keyboard`, `sf::Mouse` with platform abstraction
2. Update all input polling in `input.cpp`, `misc.cpp`, `game.cpp`
3. Map SFML key codes to `arx::Key` enum

### Phase 3: 2D Rendering (2-3 days)
**Option A:** SDL2_image + SDL_Renderer (simpler, web compatible)
- Create `Texture2D`, `Sprite2D` wrappers
- Replace all `sf::Texture`, `sf::Sprite` usage
- ~163 occurrences across 24 files

**Option B:** bgfx (more complex, better performance)
- Implement 2D sprite batching in bgfx
- Texture atlas for better performance
- More work but unified renderer

### Phase 4: Audio Migration (1 day)
- Replace `sf::Sound`/`sf::Music` with SDL2_mixer
- Update `audio.cpp` and all audio call sites
- ~18 files using SFML audio

### Phase 5: bgfx 3D Renderer (2-3 days)
- Finish `BgfxRenderer` implementation
- Replace `glBegin/glEnd` in `3Dview.cpp`, `3dviewnew.cpp`
- Vertex buffers, shaders, textures
- Test corridor rendering

### Phase 6: Web Build (1 day)
- Update `web/build.cmd` to compile all source files
- Add SDL2 and bgfx Emscripten flags
- Package assets with `--preload-file data`
- Test with `emrun build-web/arxx_web.html`

### Phase 7: Testing & Polish (2-3 days)
- Native Windows build testing
- Web build testing (Chrome, Firefox, Safari)
- Performance optimization
- Asset loading optimization

## Estimated Timeline
**Total: 10-15 days of focused development**

## Build Commands

### Native (after migration):
```cmd
# Install dependencies
vcpkg install

# Build
msbuild ARXX.sln /p:Configuration=Debug /p:ARX_USE_SDL2=1 /p:ARX_USE_BGFX=1
```

### Web:
```cmd
# Set up Emscripten
call C:\dev\emsdk\emsdk_env.bat

# Build
cd web
build.cmd
```

## Key Files to Update

### Core Window/Display:
- `display.h` - Change `extern sf::RenderWindow App`
- `display.cpp` - Update all App. calls
- `game.cpp` - Update main loop

### Input:
- `input.cpp` - Primary input handling
- `misc.cpp` - getSingleKey(), waitForSpacebar()
- `game.cpp` - Event polling in gameTick()

### 2D Rendering (163 calls):
- `font.cpp` - Text/sprite rendering
- `automap.cpp` - Map sprites
- `display.cpp` - UI sprites
- `lyrics.cpp` - Character sprites
- All module files (tavern, shop, inn, etc.)

### Audio (18 files):
- `audio.cpp` - Audio system
- `game.cpp` - Music/sound playback
- Module files with sound effects

### 3D Rendering:
- `3Dview.cpp` - Main corridor renderer
- `3dviewnew.cpp` - Alternative renderer
- `renderer/BgfxRenderer.cpp` - Modern implementation

## Testing Checklist

- [ ] Window creation/destruction
- [ ] Keyboard input (movement, menus)
- [ ] Mouse input (if used)
- [ ] 2D sprite rendering
- [ ] Text rendering
- [ ] 3D corridor rendering
- [ ] Audio playback (music + SFX)
- [ ] Save/load
- [ ] All modules (tavern, shop, encounters, etc.)
- [ ] Web build loads in browser
- [ ] Web build runs at acceptable FPS
- [ ] Web build audio works

## Notes

- Keep SFML in vcpkg.json until migration complete
- SDL2 + bgfx both support Emscripten out of the box
- Can migrate incrementally (window → input → rendering → audio)
- Test native build after each phase
- Web build comes last when SDL2 migration is done
