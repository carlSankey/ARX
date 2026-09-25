# Text-Only Implementation Plan

## Objective
Create a text-only version of the game that:
- Removes all graphics rendering (SFML sprites, textures, 3D view)
- Keeps all game logic and input handling
- Allows testing of character creation and game flow
- Provides clear console output for debugging

## Phase 1: SFML Audio Stubs

Replace all SFML audio with no-op implementations.

### Files to Modify:
1. `audio.h` - Create stub classes
2. `audio.cpp` - Remove SFML includes, add stubs

### Implementation:

**audio.h:**
```cpp
#pragma once
#include <string>

// SFML Audio Stubs - No-op implementations for web build
namespace sf {
    class SoundBuffer {
    public:
        bool loadFromFile(const std::string& filename) { return true; }
    };

    class Sound {
    public:
        Sound() { }
        Sound(const SoundBuffer&) { }
        void play() { }
        void stop() { }
        int getStatus() { return 0; } // 0 = Stopped
        void setVolume(float v) { }
    };

    class Music {
    public:
        bool openFromFile(const std::string& filename) { return true; }
        void play() { }
        void stop() { }
        int getStatus() { return 0; }
        void setVolume(float v) { }
    };

    namespace SoundSource {
        enum Status { Stopped = 0, Playing = 1, Paused = 2 };
    }
}

extern void playSound(const std::string& soundName);
extern void stopAllSounds();
```

## Phase 2: SFML Input Stubs

Replace SFML event system with simple key handlers.

### Files to Modify:
1. `misc.cpp` - Replace sf::Event with simple key processing
2. Input queue system already exists - use it

### Implementation:

Replace the entire sf::Event processing block with:
```cpp
// Input handling - web/text version
if (Event->is<sf::Event::TextEntered>()) {
    // This will be handled by InputQueue from web layer
}
if (Event->is<sf::Event::KeyPressed>()) {
    // Key handling through InputQueue
}
```

Actually: Keep using InputQueue (already web-enabled), just remove SFML::Event dependency.

## Phase 3: Remove All Graphics Rendering

### Files to Modify:
1. `display.h` - Remove App window reference
2. `display.cpp` - Remove all rendering calls
3. `lyrics.cpp` - Remove sprite rendering
4. `automap.cpp` - Remove sprite rendering
5. `renderer/Sprite2D.cpp` - Stub out drawing

### Strategy: Comment out all drawing, keep state updates

**display.cpp - clearDisplay():**
```cpp
void clearDisplay() {
    // TEXT ONLY: No graphics rendering
    // Previously: App.clear(sf::Color::Black);
}
```

**display.cpp - updateDisplay():**
```cpp
void updateDisplay() {
    // TEXT ONLY: No graphics rendering
    // Previously: App.display();
}
```

**display.cpp - drawImage():**
```cpp
void drawImage(std::string imagename, int x, int y) {
    // TEXT ONLY: No image rendering
    // std::cout << "IMAGE: " << imagename << " at (" << x << "," << y << ")\n";
}
```

## Phase 4: Create Text Output System

Add simple text-based output for debugging.

### New File: `text_output.h`

```cpp
#pragma once
#include <string>
#include <iostream>

class TextDisplay {
public:
    static void clear() {
        std::cout << "\n\n=== GAME STATE ===\n";
    }
    
    static void drawText(int x, int y, const std::string& text) {
        std::cout << "[" << x << "," << y << "] " << text << "\n";
    }
    
    static void drawPanel(const std::string& title, const std::vector<std::string>& lines) {
        std::cout << "\n--- " << title << " ---\n";
        for (const auto& line : lines) {
            std::cout << line << "\n";
        }
    }
    
    static void update() {
        // No-op for text mode
    }
};
```

## Phase 5: Audio Stub Implementations

### audio.cpp Changes:

```cpp
// Remove: #include <SFML/Audio.hpp>

// Add SFML stub implementations (from audio.h)

// Keep all existing game code
// Just comment out actual audio loading:

/*
if (!spellSoundBuffer.loadFromFile("data/audio/spells.ogg")) {
    // error
}
*/

void playSound(const std::string& soundName) {
    std::cout << "AUDIO: Playing " << soundName << "\n";
}

void stopAllSounds() {
    std::cout << "AUDIO: Stop all\n";
}
```

## Phase 6: Minimal Graphics Stubs

### renderer/Sprite2D.h/cpp

Add stubs for any remaining graphics calls:

```cpp
// Sprite2D.h
class Sprite2D {
public:
    void loadFromFile(const std::string& filename) { 
        std::cout << "SPRITE: Loading " << filename << "\n";
    }
    void draw(int x, int y, int width, int height) {
        std::cout << "SPRITE: Draw at (" << x << "," << y << ") " 
                  << width << "x" << height << "\n";
    }
    void setPosition(float x, float y) { }
    void setTextureRect(int x, int y, int w, int h) { }
};
```

## Phase 7: Fix SFML Includes

Once stubs are in place, replace actual #include statements:

**audio.cpp:**
- Remove: `#include <SFML/Audio.hpp>`
- Add: `#include "audio.h"` (which has stubs)

**misc.cpp:**
- Remove: `#include <SFML\Graphics.hpp>`, `#include <SFML\OpenGL.hpp>`
- Keep: Input handling logic

**game.cpp:**
- Remove: `#include <SFML/Graphics.hpp>`, `#include <SFML/Audio.hpp>`
- Add: Conditional include for stubs

## Test Cases for Text-Only Mode

### Test 1: Character Name Entry
**Input:** "Hardwin", Enter
**Expected Output:**
```
CHARACTER CREATION
Name: Hardwin
Ready for gender selection? (M/F)
```

### Test 2: Gender Selection
**Input:** M
**Expected Output:**
```
Gender: Male
Confirm? (Y/N)
```

### Test 3: Gate Animation (Text Mode)
**Input:** Y (from gender confirmation)
**Expected Output:**
```
AUDIO: Playing cityGate1
[Counting down to character stats...]
STR: 15, DEX: 12, CON: 14, INT: 11, WIS: 13, CHA: 10
Press any key to lock stats:
```

### Test 4: Character Display
**Input:** (any key after stats)
**Expected Output:**
```
=== CHARACTER STATS ===
Name: Hardwin
Gender: Male
Level: 1
Experience: 0
Health: 20/20
STR: 15, DEX: 12, CON: 14
INT: 11, WIS: 13, CHA: 10
Location: (35, 15)
Facing: South
```

## Rollout Order

1. **Step 1:** Create audio stubs (1 hour)
   - Won't break anything, just replaces SFML with no-ops

2. **Step 2:** Create text output system (1 hour)
   - Add console logging for all major game events

3. **Step 3:** Stub out graphics rendering (2 hours)
   - Comment out all drawing calls
   - Game logic continues to run

4. **Step 4:** Update includes (1 hour)
   - Replace SFML includes with stub includes

5. **Step 5:** Test text mode (1 hour)
   - Verify character creation works
   - Check data flow

6. **Step 6:** Debug data issues (As needed)
   - Character name/stats/XP mismatch
   - Consumable display bugs

## Expected Outcome

After text-only implementation:
- ✅ No SFML dependency errors on compile
- ✅ Game logic runs without graphics
- ✅ Can test character creation flow
- ✅ Can debug data transfer issues
- ✅ Input handling works correctly
- ✅ Audio calls execute (silent no-ops)
- ✅ Clear console output for debugging

## Not Included (Phase 2+)

- 3D corridor rendering
- Sprite/texture display
- Logo image
- Info panels (graphics)
- Compass (graphics)
- Door/collision visuals

These will be added when graphics system is implemented.