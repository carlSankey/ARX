#pragma once

// Application lifecycle for native desktop and browser (Emscripten).
// One appTick() == one frame of menu/load or one gameTick() while InGame (native).

enum class AppState {
    Boot,
    MainMenu,
    LoadGameMenu,
    CharCreate,   // spinning counter animation (city or dungeon gate)
    NameEntry,    // player name / gender / confirm
    InGame,       // entered while startGame() / gameLoop() run (blocking on native)
    Shutdown
};

bool appInit();
bool appTick();       // false = stop the outer loop (quit)
void appShutdown();

bool appIsRunning();
AppState appGetState();
const char* appStateName(AppState state);
