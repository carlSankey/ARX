#include "AppLoop.h"

#include <iostream>
#include <string>

#ifdef __EMSCRIPTEN__

#include <SDL_opengles2.h>
#include "Window.h"
#include "InputQueue.h"
#include "display.h"
#include "font.h"
#include "player.h"
#include "globals.h"
#include "misc.h"
#include "audio.h"
#include "level.h"
#include "actor.h"
#include "automap.h"
#include "renderer/Sprite2D.h"
#include "game.h"

// Define global player state (extern in player.h, needed by font.cpp and display.cpp)
Player plyr;

namespace {
    bool g_running = true;
    AppState g_state = AppState::Boot;

    // NameEntry sub-state
    enum class NameStage { TypingName, ChoosingGender, Confirming };
    NameStage g_nameStage = NameStage::TypingName;
    std::string g_typedName;
    std::string g_sex;

    // CharCreate counter animation
    struct WebCounter { int value1, value2, x, y, speed, speed_initial; };
    WebCounter g_counters[8];

    // City counter layout (from createCharacter.cpp cityCounters)
    // City: stats at y=18, copper at y=304
    const WebCounter k_cityCounters[8] = {
        {8,  16, 48,  18,  2, 2},
        {13, 21, 128, 18,  0, 0},
        {15, 10, 208, 18,  2, 2},
        {10, 10, 288, 18,  4, 4},
        {18, 11, 368, 18,  5, 5},
        {10, 12, 444, 18,  1, 1},
        {23, 15, 528, 18,  4, 4},
        {54, 47, 514, 304, 5, 5},
    };
    // Dungeon: stats at y=34, silver at y=284
    const WebCounter k_dungeonCounters[8] = {
        {10, 16, 48,  34,  2, 2},
        {13, 21, 128, 34,  0, 0},
        {15, 10, 208, 34,  2, 2},
        {10, 10, 288, 34,  3, 3},
        {18, 11, 368, 34,  4, 4},
        {10, 12, 444, 34,  1, 1},
        {23, 15, 524, 34,  3, 3},
        {54, 47, 544, 284, 5, 5},
    };

    enum class CharCreateStage { Spinning, ShowingFinal, PlayingEntrySound, ShowingJoined };
    CharCreateStage g_charCreateStage = CharCreateStage::Spinning;
    float g_gateTimer = 0.0f;

}

// Draw two-digit counter value at (x, y) using drawImage
// Positions are hardcoded for the original 640x480 design viewport. The gate image
// is now uniformly scaled and centered, so we apply the same transform to counter coordinates.
static void webDrawCounter(int x, int y, int value)
{
    static const char* digits[] = {"0","1","2","3","4","5","6","7","8","9"};
    int d1 = (value < 10) ? 0 : (value / 10);
    int d2 = value % 10;
    // Transform counter positions to match the scaled/centered gate image
    float scale = getGateScale();
    float ox = getGateOffsetX();
    float oy = getGateOffsetY();
    int sx = static_cast<int>(x * scale + ox);
    int sy = static_cast<int>(y * scale + oy);
    drawImage(digits[d1], sx,      sy);
    drawImage(digits[d2], sx + static_cast<int>(32 * scale), sy);
}

// Draw all 8 counters (value1 at y, value2 at y+16)
static void webDisplayCounters()
{
    for (int i = 0; i < 8; i++) {
        webDrawCounter(g_counters[i].x, g_counters[i].y,      g_counters[i].value1);
        webDrawCounter(g_counters[i].x, g_counters[i].y + 16, g_counters[i].value2);
    }
}

// Advance counter animation one frame
static void webTickCounters()
{
    for (int i = 0; i < 8; i++) {
        if (g_counters[i].speed == 0) {
            g_counters[i].speed = g_counters[i].speed_initial;
            g_counters[i].y--;
            // City/dungeon wrap points differ for counter 7 (copper/silver)
            if (i < 7) {
                // Stats counters: city wraps at y=2→18, dungeon wraps at y=14→34
                int statsWrapY  = (plyr.scenario == 0) ? 2 : 14;
                int statsResetY = (plyr.scenario == 0) ? 18 : 34;
                if (g_counters[i].y == statsWrapY) {
                    g_counters[i].value1 = g_counters[i].value2;
                    g_counters[i].value2 = randn(0, 12) + 10;
                    g_counters[i].y = statsResetY;
                }
            } else {
                // counter 7: city y=290→304, dungeon y=278→284
                int wrapY  = (plyr.scenario == 0) ? 290 : 278;
                int resetY = (plyr.scenario == 0) ? 304 : 284;
                int minVal = (plyr.scenario == 0) ? 49  : 50;
                int range  = (plyr.scenario == 0) ? 50  : 30;
                if (g_counters[i].y == wrapY) {
                    g_counters[i].value1 = g_counters[i].value2;
                    g_counters[i].value2 = randn(0, range) + minVal;
                    g_counters[i].y = resetY;
                }
            }
        } else {
            g_counters[i].speed--;
        }
    }
}


// Draw the name/gender/confirm screen (mirrors drawPlayerDetails from createCharacter.cpp)
static void webDrawPlayerDetails()
{
    clearDisplay();
    drawText(2, 2, "Create a new character");
    std::string prompt = "Enter name: " + g_typedName + "_";
    drawText(2, 5, prompt);

    if (plyr.name != " " && plyr.name != "") {
        drawText(2, 16, "Art thou male or female ? (M or F)");
    }
    if (plyr.gender != 0) {
        drawText(2, 18, "Thy name is " + g_typedName);
        if (plyr.gender == 1) {
            drawText(12, 20, "Thy sex is male.");
        } else {
            drawText(11, 20, "Thy sex is female.");
        }
        drawText(8, 22, "Is this correct? (Y or N)");
    }
    updateDisplay();
}

static void pushKeyToQueue(arx::Key key)
{
    std::cout << "TRACE: pushKeyToQueue called with key=" << (int)key << std::endl;
    switch (key) {
    case arx::Key::Escape:    std::cout << "TRACE: Escape" << std::endl; arx::inputPush("ESC");      break;
    case arx::Key::Space:     std::cout << "TRACE: Space" << std::endl; arx::inputPush("SPACE");    break;
    case arx::Key::Enter:     std::cout << "TRACE: Enter" << std::endl; arx::inputPush("RETURN");   break;
    case arx::Key::Backspace: std::cout << "TRACE: Backspace" << std::endl; arx::inputPush("BACKSPACE");break;
    case arx::Key::Tab:       std::cout << "TRACE: Tab" << std::endl; arx::inputPush("TAB");      break;
    case arx::Key::Left:      std::cout << "TRACE: Left" << std::endl; arx::inputPush("LEFT");     break;
    case arx::Key::Right:     std::cout << "TRACE: Right" << std::endl; arx::inputPush("RIGHT");    break;
    case arx::Key::Up:        std::cout << "TRACE: Up" << std::endl; arx::inputPush("UP");       break;
    case arx::Key::Down:      std::cout << "TRACE: Down" << std::endl; arx::inputPush("DOWN");     break;
    case arx::Key::A:         std::cout << "TRACE: A" << std::endl; arx::inputPush("A");        break;
    case arx::Key::B:         std::cout << "TRACE: B" << std::endl; arx::inputPush("B");        break;
    case arx::Key::C:         std::cout << "TRACE: C" << std::endl; arx::inputPush("C");        break;
    case arx::Key::D:         std::cout << "TRACE: D" << std::endl; arx::inputPush("D");        break;
    case arx::Key::E:         std::cout << "TRACE: E" << std::endl; arx::inputPush("E");        break;
    case arx::Key::F:         std::cout << "TRACE: F" << std::endl; arx::inputPush("F");        break;
    case arx::Key::G:         std::cout << "TRACE: G" << std::endl; arx::inputPush("G");        break;
    case arx::Key::H:         std::cout << "TRACE: H" << std::endl; arx::inputPush("H");        break;
    case arx::Key::I:         std::cout << "TRACE: I" << std::endl; arx::inputPush("I");        break;
    case arx::Key::J:         std::cout << "TRACE: J" << std::endl; arx::inputPush("J");        break;
    case arx::Key::K:         std::cout << "TRACE: K" << std::endl; arx::inputPush("K");        break;
    case arx::Key::L:         std::cout << "TRACE: L" << std::endl; arx::inputPush("L");        break;
    case arx::Key::M:         std::cout << "TRACE: M" << std::endl; arx::inputPush("M");        break;
    case arx::Key::N:         std::cout << "TRACE: N" << std::endl; arx::inputPush("N");        break;
    case arx::Key::O:         std::cout << "TRACE: O" << std::endl; arx::inputPush("O");        break;
    case arx::Key::P:         std::cout << "TRACE: P" << std::endl; arx::inputPush("P");        break;
    case arx::Key::Q:         std::cout << "TRACE: Q" << std::endl; arx::inputPush("Q");        break;
    case arx::Key::R:         std::cout << "TRACE: R" << std::endl; arx::inputPush("R");        break;
    case arx::Key::S:         std::cout << "TRACE: S" << std::endl; arx::inputPush("S");        break;
    case arx::Key::T:         std::cout << "TRACE: T" << std::endl; arx::inputPush("T");        break;
    case arx::Key::U:         std::cout << "TRACE: U" << std::endl; arx::inputPush("U");        break;
    case arx::Key::V:         std::cout << "TRACE: V" << std::endl; arx::inputPush("V");        break;
    case arx::Key::W:         std::cout << "TRACE: W" << std::endl; arx::inputPush("W");        break;
    case arx::Key::X:         std::cout << "TRACE: X" << std::endl; arx::inputPush("X");        break;
    case arx::Key::Y:         std::cout << "TRACE: Y" << std::endl; arx::inputPush("Y");        break;
    case arx::Key::Z:         std::cout << "TRACE: Z" << std::endl; arx::inputPush("Z");        break;
    case arx::Key::Num0:      std::cout << "TRACE: Num0" << std::endl; arx::inputPush("0");        break;
    case arx::Key::Num1:      std::cout << "TRACE: Num1" << std::endl; arx::inputPush("1");        break;
    case arx::Key::Num2:      std::cout << "TRACE: Num2" << std::endl; arx::inputPush("2");        break;
    case arx::Key::Num3:      std::cout << "TRACE: Num3" << std::endl; arx::inputPush("3");        break;
    case arx::Key::Num4:      std::cout << "TRACE: Num4" << std::endl; arx::inputPush("4");        break;
    case arx::Key::Num5:      std::cout << "TRACE: Num5" << std::endl; arx::inputPush("5");        break;
    case arx::Key::Num6:      std::cout << "TRACE: Num6" << std::endl; arx::inputPush("6");        break;
    case arx::Key::Num7:      std::cout << "TRACE: Num7" << std::endl; arx::inputPush("7");        break;
    case arx::Key::Num8:      std::cout << "TRACE: Num8" << std::endl; arx::inputPush("8");        break;
    case arx::Key::Num9:      std::cout << "TRACE: Num9" << std::endl; arx::inputPush("9");        break;
    case arx::Key::F1:        std::cout << "TRACE: F1" << std::endl; arx::inputPush("F1");       break;
    case arx::Key::F2:        std::cout << "TRACE: F2" << std::endl; arx::inputPush("F2");       break;
    case arx::Key::F3:        std::cout << "TRACE: F3" << std::endl; arx::inputPush("F3");       break;
    case arx::Key::F4:        std::cout << "TRACE: F4" << std::endl; arx::inputPush("F4");       break;
    case arx::Key::F5:        std::cout << "TRACE: F5" << std::endl; arx::inputPush("F5");       break;
    case arx::Key::F6:        std::cout << "TRACE: F6" << std::endl; arx::inputPush("F6");       break;
    case arx::Key::F7:        std::cout << "TRACE: F7" << std::endl; arx::inputPush("F7");       break;
    case arx::Key::F8:        std::cout << "TRACE: F8" << std::endl; arx::inputPush("F8");       break;
    case arx::Key::F9:        std::cout << "TRACE: F9" << std::endl; arx::inputPush("F9");       break;
    case arx::Key::F10:       std::cout << "TRACE: F10" << std::endl; arx::inputPush("F10");      break;
    case arx::Key::F11:       std::cout << "TRACE: F11" << std::endl; arx::inputPush("F11");      break;
    case arx::Key::F12:       std::cout << "TRACE: F12" << std::endl; arx::inputPush("F12");      break;
    default:
        std::cout << "TRACE: Unknown key=" << (int)key << std::endl;
        break;
    }
}

static void processInputQueue()
{
    if (!arx::g_window) {
        return;
    }

    int keyCount = 0;
    int textCount = 0;
    while (const std::optional<arx::Event> event = arx::g_window->pollEvent()) {
        switch (event->type) {
        case arx::EventType::Closed:
            arx::inputPush("QUIT");
            g_running = false;
            break;
        case arx::EventType::KeyPressed:
            pushKeyToQueue(event->key);
            keyCount++;
            break;
        case arx::EventType::TextInput:
            arx::textPush(event->text);
            textCount++;
            break;
        default:
            // Ignore KeyReleased, MouseButton, Resized events
            break;
        }
    }
    if (keyCount > 0 || textCount > 0) {
        std::cout << "INPUT: processed " << keyCount << " keys, " << textCount << " text chars" << std::endl;
    }
}

bool appInit()
{
    std::cout << "ARXX web: appInit (SDL2 window + input path)" << std::endl;
    arx::WindowConfig config;
    config.title = "ARXX Web";
    config.width = 640;
    config.height = 520;
    config.fullscreen = false;
    config.vsync = true;

    arx::IWindow* window = arx::createPlatformWindow();
    if (!window) {
        std::cerr << "ARXX web: failed to create platform window" << std::endl;
        return false;
    }

    if (!window->create(config)) {
        std::cerr << "ARXX web: window creation failed" << std::endl;
        window->destroy();
        delete window;
        return false;
    }

    arx::setWindow(window);
    arx::inputClear();

    // Initialize display state
    windowWidth = 640;
    windowHeight = 520;
    plyr.fontStyle = 1;
    plyr.scenario = 0;  // City
    dispInit();
    InitFont();
    loadLogoImage();

    // Set GL viewport to match canvas size
    glViewport(0, 0, windowWidth, windowHeight);

    std::cout << "ARXX web: display initialized (" << windowWidth << "x" << windowHeight << ")" << std::endl;

    // Phase 7a: load scenario-independent data
    std::cout << "ARXX web: loading maps index..." << std::endl;
    initMaps();
    clearAutoMaps();
    std::cout << "ARXX web: maps index loaded OK" << std::endl;

    // Load 3D textures and backgrounds for Phase 7b
    std::cout << "ARXX web: loading 3D resources..." << std::endl;
    loadBackgroundNames();
    initTextures();
    std::cout << "ARXX web: 3D resources loaded" << std::endl;

    g_state = AppState::MainMenu;
    g_running = true;
    return true;
}

bool appTick()
{
    if (!g_running || !arx::g_window || !arx::g_window->isOpen()) {
        return false;
    }

    processInputQueue();

    switch (g_state) {
    case AppState::MainMenu: {
        // Draw the main menu
        clearDisplay();
        displayMainMenu();
        updateDisplay();

        // Poll one key per frame
        std::string key = arx::inputPoll();
        if (!key.empty()) {
            std::cout << "KEY: MainMenu key='" << key << "'" << std::endl;
        }

        if (key == "1") {
            plyr.scenario = 0;  // City
            arx::inputClear();
            arx::textClear();
            g_typedName.clear();
            plyr.name = " ";
            plyr.gender = 0;
            g_sex.clear();
            g_nameStage = NameStage::TypingName;
            g_state = AppState::NameEntry;   // Name first
        } else if (key == "2") {
            plyr.scenario = 1;  // Dungeon
            arx::inputClear();
            arx::textClear();
            g_typedName.clear();
            plyr.name = " ";
            plyr.gender = 0;
            g_sex.clear();
            g_nameStage = NameStage::TypingName;
            g_state = AppState::NameEntry;   // Name first
        } else if (key == "3") {
            std::cout << "ARXX: Load game (not yet implemented)" << std::endl;
        } else if (key == "5") {
            // Cycle through graphics modes: 0 (Atari 8bit) -> 1 (16bit) -> 2 (Small 3D) -> 0
            std::cout << "ARXX: Key 5 pressed, current graphicMode=" << graphicMode << std::endl;
            if (graphicMode == ATARI_SMALL) {
                graphicMode = A16BIT_SMALL;
                std::cout << "ARXX: Switched to A16BIT_SMALL (1)" << std::endl;
            } else if (graphicMode == A16BIT_SMALL) {
                graphicMode = ALTERNATE_SMALL;
                std::cout << "ARXX: Switched to ALTERNATE_SMALL (2)" << std::endl;
            } else if (graphicMode == ALTERNATE_SMALL) {
                graphicMode = ATARI_SMALL;
                std::cout << "ARXX: Switched to ATARI_SMALL (0)" << std::endl;
            } else {
                graphicMode = ATARI_SMALL;
                std::cout << "ARXX: Switched to ATARI_SMALL (0) from unknown mode" << std::endl;
            }
            std::cout << "ARXX: New graphicMode=" << graphicMode << std::endl;
            dispInit();
        } else if (key == "7") {
            plyr.fontStyle = (plyr.fontStyle == 0) ? 1 : 0;
            InitFont();
        } else if (key == "0" || key == "QUIT") {
            g_running = false;
            return false;
        }
        break;
    }
    case AppState::CharCreate: {
        // Phase 5b: spinning counter animation
        if (g_charCreateStage == CharCreateStage::Spinning) {
            clearDisplay();
            // Draw counters first, then gate background on top
            webTickCounters();
            webDisplayCounters();
            if (plyr.scenario == 0) {
                displayCityGateImage();
            } else {
                displayDungeonGateImage();
            }
            updateDisplay();

            std::string key = arx::inputPoll();
            if (!key.empty()) {
                std::cout << "KEY: CharCreate(Spinning) key='" << key << "'" << std::endl;
                // Lock counters: snap each to its current value1
                for (int i = 0; i < 7; i++) {
                    int snapY = (plyr.scenario == 0) ? 8 : 20;
                    int resetY = (plyr.scenario == 0) ? 18 : 34;
                    if (g_counters[i].y < snapY) {
                        g_counters[i].value1 = g_counters[i].value2;
                    }
                    g_counters[i].y = resetY;
                }
                int lockY = (plyr.scenario == 0) ? 304 : 284;
                int wrapY = (plyr.scenario == 0) ? 296 : 278;
                if (g_counters[7].y < wrapY) {
                    g_counters[7].value1 = g_counters[7].value2;
                }
                g_counters[7].y = lockY;

                // Initialize all player stats and inventory
                int selectedScenario = plyr.scenario; // preserve scenario choice
                initStats();
                plyr.scenario = selectedScenario; // restore scenario choice
                // Assign rolled stats to player
                plyr.sta   = g_counters[0].value1;
                plyr.chr   = g_counters[1].value1;
                plyr.str   = g_counters[2].value1;
                plyr.inte  = g_counters[3].value1;
                plyr.wis   = g_counters[4].value1;
                plyr.skl   = g_counters[5].value1;
                plyr.hp    = g_counters[6].value1;
                plyr.maxhp = g_counters[6].value1;
                if (plyr.scenario == 0) {
                    plyr.copper = g_counters[7].value1 + 120;
                } else {
                    plyr.silver = g_counters[7].value1;
                }

                std::cout << "ARXX: Stats locked — STA:" << plyr.sta
                          << " CHR:" << plyr.chr << " STR:" << plyr.str
                          << " INT:" << plyr.inte << " WIS:" << plyr.wis
                          << " SKL:" << plyr.skl << " HP:" << plyr.hp << std::endl;

                // Stop gate sounds
                if (plyr.scenario == 0) {
                    stopCityGateSound1();
                    stopCityGateSound3();
                } else {
                    stopDungeonGateSound1();
                    stopDungeonGateSound2();
                }
                g_charCreateStage = CharCreateStage::ShowingFinal;
                g_gateTimer = 0.1f; // Show final stats for 0.1 second
                arx::inputClear();
                arx::textClear();
            }
        } else if (g_charCreateStage == CharCreateStage::ShowingFinal) {
            std::cout << "ARXX: ShowingFinal stage" << std::endl;
            // Show final locked stats briefly
            clearDisplay();
            webDisplayCounters();
            if (plyr.scenario == 0) {
                displayCityGateImage();
            } else {
                displayDungeonGateImage();
            }
            updateDisplay();

             g_gateTimer -= 0.016f; // Approximate 60fps delta
             if (g_gateTimer <= 0.0f) {
                 // Play entry sound
                 if (plyr.scenario == 0) {
                     playCityGateSound3();
                 } else {
                     playDungeonGateSound2();
                 }
                 g_charCreateStage = CharCreateStage::PlayingEntrySound;
                 g_gateTimer = 0.1f; // Wait for entry sound to finish (0.1 seconds)
                std::cout << "ARXX: Transitioned to PlayingEntrySound, timer=" << g_gateTimer << std::endl;
            }
        } else if (g_charCreateStage == CharCreateStage::PlayingEntrySound) {
            std::cout << "ARXX: PlayingEntrySound stage, timer=" << g_gateTimer << std::endl;
            // Wait for entry sound to finish
            clearDisplay();
            webDisplayCounters();
            if (plyr.scenario == 0) {
                displayCityGateImage();
            } else {
                displayDungeonGateImage();
            }
            updateDisplay();

             g_gateTimer -= 0.016f;
             if (g_gateTimer <= 0.0f) {
                 g_charCreateStage = CharCreateStage::ShowingJoined;
                 g_gateTimer = 0.1f; // Reset timer for ShowingJoined stage (0.1 second)
                std::cout << "ARXX: Transitioned to ShowingJoined, timer reset to " << g_gateTimer << std::endl;
            }
        } else if (g_charCreateStage == CharCreateStage::ShowingJoined) {
            std::cout << "ARXX: ShowingJoined stage ENTERED" << std::endl;
            // Show "You are now joined" message
            clearDisplay();
            drawText(2, 11, "You are now joined.  Prepare to enter");
            if (plyr.scenario == 0) {
                drawText(7, 13, "Alternate Reality, The City.");
            } else {
                drawText(6, 13, "Alternate Reality, The Dungeon.");
            }
            updateDisplay();

            // Auto-transition after 2 seconds (no key press needed)
            g_gateTimer -= 0.016f;
            std::cout << "ARXX: ShowingJoined timer=" << g_gateTimer << std::endl;
            if (g_gateTimer <= 0.0f) {
                std::cout << "ARXX: Timer expired, transitioning to InGame" << std::endl;
                arx::inputClear();
                arx::textClear();

                // Full game initialization (mirrors enterGame() from native build)
                std::cout << "ARXX web: starting full game initialization..." << std::endl;
                
                // Set graphics mode to small 3D view with banner visible
                graphicMode = ALTERNATE_SMALL;
                setScreenValues();
                std::cout << "ARXX web: set graphicMode to ALTERNATE_SMALL, viewport=" << viewPortX << "," << viewPortY << " size=" << viewWidth << "x" << viewHeight << std::endl;
                
                // Load scenario data
                int scenarioMap = (plyr.scenario == 0) ? 0 : 1;
                plyr.map = scenarioMap;
                plyr.mapWidth = 64;
                plyr.mapHeight = 64;
                std::cout << "ARXX web: loading map " << scenarioMap << std::endl;
                loadMapData(scenarioMap);
                std::cout << "ARXX web: loadMapData OK" << std::endl;
                loadDescriptions(scenarioMap);
                std::cout << "ARXX web: loadDescriptions OK" << std::endl;
                loadZoneData(scenarioMap);
                std::cout << "ARXX web: loadZoneData OK" << std::endl;
                
                std::cout << "ARXX web: loading monsters..." << std::endl;
                loadMonstersBinary();
                std::cout << "ARXX web: loadMonstersBinary OK" << std::endl;
                initialiseMonsterOffsets();
                convertMonstersBinary();
                std::cout << "ARXX web: convertMonstersBinary OK" << std::endl;
                convertMessagesBinary();
                std::cout << "ARXX web: convertMessagesBinary OK" << std::endl;
                
                // Additional initialization from native enterGame()
                // loadDamonBinary(); // commented out for web build
                // loadCitySmithyBinary(); // commented out for web build
                // loadDwarvenBinary(); // commented out for web build
                
                checkBackgroundTime();
                // checkDailyInnJobOpenings(); // commented out for web build
                // checkDailyTavernJobOpenings(); // commented out for web build
                // checkDailybankJobOpenings(); // commented out for web build
                
                initialiseNewGame();
                plyr.status = 1; // EXPLORE
                
                // UpdateShopStock(); // commented out for web build
                // UpdateSmithyStock(); // commented out for web build
                // readFTData(); // commented out for web build
                
                gameInit();
                std::cout << "ARXX web: gameInit OK" << std::endl;
                
                // Set starting position
                if (plyr.scenario == 0) {
                    // City: south area, facing north into streets with walls
                    plyr.x = 1; plyr.y = 1;
                    plyr.facing = NORTH;
                } else {
                    // Dungeon level 1
                    plyr.x = 30; plyr.y = 30;
                    plyr.facing = WEST;
                }
                plyr.location = 0;
                plyr.alive = true;
                plyr.timeOfDay = 0; // Full day
                plyr.hours = 6;     // Morning
                plyr.minutes = 0;
                plyr.darkness = 0;
                plyr.light = 0;
                plyr.z_offset = 1.0f;
                plyr.floorTexture = 0;
                plyr.ceiling = 0;
                setCurrentZone();
                // Force outdoor city zone AFTER setCurrentZone() so it doesn't get overwritten
                plyr.zone = 99;
                std::cout << "ARXX web: starting at (" << plyr.x << "," << plyr.y << ") facing=" << plyr.facing
                          << " zoneSet=" << plyr.zoneSet << " timeOfDay=" << plyr.timeOfDay << std::endl;

                std::cout << "ARXX: Setting g_state to InGame (value=" << (int)AppState::InGame << ")" << std::endl;
                g_state = AppState::InGame;
                std::cout << "ARXX: g_state is now " << (int)g_state << std::endl;
            }
        }
        break;
    }
    case AppState::NameEntry: {
        // Phase 5c: real name/gender/confirm entry
        webDrawPlayerDetails();

        switch (g_nameStage) {
        case NameStage::TypingName: {
            // Use text input queue for character-by-character typing
            std::string ch = arx::textPoll();
            std::string key = arx::inputPoll();

            if (!ch.empty() && ch != "\n" && ch != "\r") {
                std::cout << "KEY: NameEntry text char='" << ch << "'" << std::endl;
            }
            if (!key.empty()) {
                std::cout << "KEY: NameEntry key='" << key << "'" << std::endl;
            }

            if (key == "BACKSPACE") {
                if (!g_typedName.empty()) {
                    g_typedName.pop_back();
                }
            } else if (key == "RETURN" && !g_typedName.empty()) {
                plyr.name = g_typedName;
                g_nameStage = NameStage::ChoosingGender;
            } else if (key == "ESC") {
                // Cancel — back to main menu
                g_state = AppState::MainMenu;
                g_nameStage = NameStage::TypingName;
                g_typedName.clear();
                plyr.name = " ";
                plyr.gender = 0;
            } else if (!ch.empty() && ch != "\n" && ch != "\r") {
                if (g_typedName.size() < 24) {
                    g_typedName += ch;
                }
            }
            break;
        }
        case NameStage::ChoosingGender: {
            std::string key = arx::inputPoll();
            if (!key.empty()) {
                std::cout << "KEY: Gender choice key='" << key << "'" << std::endl;
            }
            if (key == "M") {
                plyr.gender = 1;
                g_sex = "male";
                g_nameStage = NameStage::Confirming;
            } else if (key == "F") {
                plyr.gender = 2;
                g_sex = "female";
                g_nameStage = NameStage::Confirming;
            }
            break;
        }
        case NameStage::Confirming: {
            std::string key = arx::inputPoll();
            if (!key.empty()) {
                std::cout << "KEY: Confirm key='" << key << "'" << std::endl;
            }
            if (key == "Y") {
                // Name confirmed — now go to spinning counters
                std::cout << "ARXX: Name confirmed: " << plyr.name
                          << " (" << g_sex << ") — entering gate" << std::endl;
                arx::inputClear();
                arx::textClear();
                if (plyr.scenario == 0) {
                    for (int i = 0; i < 8; i++) g_counters[i] = k_cityCounters[i];
                } else {
                    for (int i = 0; i < 8; i++) g_counters[i] = k_dungeonCounters[i];
                }
                g_charCreateStage = CharCreateStage::Spinning;
                g_nameStage = NameStage::TypingName;
                g_typedName.clear();
                // Init and start gate sounds
                if (plyr.scenario == 0) {
                    initCityGateSounds();
                    playCityGateSound1();
                } else {
                    initDungeonGateSounds();
                    playDungeonGateSound1();
                }
                g_state = AppState::CharCreate;
            } else if (key == "N") {
                // Start over
                g_typedName.clear();
                plyr.name = " ";
                plyr.gender = 0;
                g_sex.clear();
                g_nameStage = NameStage::TypingName;
            }
            break;
        }
        }
        break;
    }
    case AppState::InGame: {
        // Call the full game tick which handles rendering, input, and game logic
        bool tickResult = gameTick();
        if (!tickResult) {
            // gameTick() returned false = session ended (player died or quit)
            // Don't return to MainMenu - stay in game for testing
            // g_state = AppState::MainMenu;
        }
        break;
    }
    case AppState::Shutdown:
        std::cout << "STATE: Shutdown" << std::endl;
        break;
    default:
        break;
    }

    return g_running;
}

void appShutdown()
{
    std::cout << "ARXX web: appShutdown" << std::endl;
    shutdownDisplay();
    g_state = AppState::Shutdown;
    g_running = false;
}

bool appIsRunning()
{
    return g_running;
}

AppState appGetState()
{
    return g_state;
}

const char* appStateName(AppState state)
{
    switch (state) {
    case AppState::Boot:         return "Boot";
    case AppState::MainMenu:     return "MainMenu";
    case AppState::LoadGameMenu: return "LoadGameMenu";
    case AppState::CharCreate:   return "CharCreate";
    case AppState::NameEntry:    return "NameEntry";
    case AppState::InGame:       return "InGame";
    case AppState::Shutdown:     return "Shutdown";
    }
    return "Unknown";
}

#endif
