// ============================================================
// SDL2_mixer audio implementation
// ============================================================
#include <SDL_mixer.h>
#include <iostream>

static bool g_mixerReady = false;

static void ensureMixerInit()
{
    if (g_mixerReady) return;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
        return;
    }
    Mix_AllocateChannels(16);
    g_mixerReady = true;
}

// Channels reserved for specific sounds
static const int CH_DUNGEON_GATE1 = 0;
static const int CH_DUNGEON_GATE2 = 1;
static const int CH_CITY_GATE1    = 2;
static const int CH_CITY_GATE2    = 3;
static const int CH_CITY_GATE3    = 4;
static const int CH_ENCOUNTER     = 5;
static const int CH_SPELL         = 6;

static Mix_Chunk* g_dungeonGate1 = nullptr;
static Mix_Chunk* g_dungeonGate2 = nullptr;
static Mix_Chunk* g_cityGate1    = nullptr;
static Mix_Chunk* g_cityGate3    = nullptr;
static Mix_Chunk* g_spellSound   = nullptr;
static Mix_Chunk* g_encounterChunks[5] = {};
static Mix_Music* g_shopMusic    = nullptr;
static bool       g_musicPlaying = false;

void initEncounterThemes()
{
    ensureMixerInit();
    const char* files[5] = {
        "data/audio/cityEncounter2.ogg",
        "data/audio/cityEncounter1.ogg",
        "data/audio/e1.ogg",
        "data/audio/e2.ogg",
        "data/audio/e3.ogg"
    };
    for (int i = 0; i < 5; i++) {
        if (g_encounterChunks[i]) { Mix_FreeChunk(g_encounterChunks[i]); }
        g_encounterChunks[i] = Mix_LoadWAV(files[i]);
        if (!g_encounterChunks[i]) {
            std::cerr << "Mix_LoadWAV " << files[i] << ": " << Mix_GetError() << std::endl;
        }
    }
}

void playEncounterTheme(int number)
{
    if (!g_mixerReady || number < 0 || number > 4) return;
    if (g_encounterChunks[number]) {
        Mix_PlayChannel(CH_ENCOUNTER, g_encounterChunks[number], 0);
    }
}

bool encounterThemeNotPlaying()
{
    return !Mix_Playing(CH_ENCOUNTER);
}

void playShopMusic(int musicNo)
{
    if (!g_mixerReady || g_musicPlaying) return;
    const char* file = nullptr;
    switch (musicNo) {
    case 1: file = "data/audio/trolls.ogg";   break;
    case 2: file = "data/audio/goblins.ogg";  break;
    case 3: file = "data/audio/chapel.ogg";   break;
    case 4: file = "data/audio/B/trolls.ogg"; break;
    case 5: file = "data/audio/B/goblins.ogg";break;
    case 6: file = "data/audio/B/Chapel.ogg"; break;
    default: return;
    }
    if (g_shopMusic) { Mix_FreeMusic(g_shopMusic); g_shopMusic = nullptr; }
    g_shopMusic = Mix_LoadMUS(file);
    if (g_shopMusic) {
        Mix_PlayMusic(g_shopMusic, -1);
        g_musicPlaying = true;
    } else {
        std::cerr << "Mix_LoadMUS " << file << ": " << Mix_GetError() << std::endl;
    }
}

void stopShopMusic()
{
    Mix_HaltMusic();
    g_musicPlaying = false;
}

void initCityGateSounds()
{
    ensureMixerInit();
    if (g_cityGate1) { Mix_FreeChunk(g_cityGate1); }
    g_cityGate1 = Mix_LoadWAV("data/audio/cityGate1.ogg");
    if (!g_cityGate1) std::cerr << "cityGate1: " << Mix_GetError() << std::endl;

    if (g_cityGate3) { Mix_FreeChunk(g_cityGate3); }
    g_cityGate3 = Mix_LoadWAV("data/audio/citygate4.ogg");
    if (!g_cityGate3) std::cerr << "cityGate4: " << Mix_GetError() << std::endl;
}

void initDungeonGateSounds()
{
    ensureMixerInit();
    if (g_dungeonGate1) { Mix_FreeChunk(g_dungeonGate1); }
    g_dungeonGate1 = Mix_LoadWAV("data/audio/gate1.wav");
    if (!g_dungeonGate1) std::cerr << "gate1: " << Mix_GetError() << std::endl;

    if (g_dungeonGate2) { Mix_FreeChunk(g_dungeonGate2); }
    g_dungeonGate2 = Mix_LoadWAV("data/audio/gate2.wav");
    if (!g_dungeonGate2) std::cerr << "gate2: " << Mix_GetError() << std::endl;
}

void playDungeonGateSound1() { if (g_dungeonGate1) Mix_PlayChannel(CH_DUNGEON_GATE1, g_dungeonGate1, -1); }
void stopDungeonGateSound1() { Mix_HaltChannel(CH_DUNGEON_GATE1); }
void playDungeonGateSound2() { if (g_dungeonGate2) Mix_PlayChannel(CH_DUNGEON_GATE2, g_dungeonGate2, 0); }
void stopDungeonGateSound2() { Mix_HaltChannel(CH_DUNGEON_GATE2); }

void playCityGateSound1() { if (g_cityGate1) Mix_PlayChannel(CH_CITY_GATE1, g_cityGate1, 0); }
void stopCityGateSound1() { Mix_HaltChannel(CH_CITY_GATE1); }
void playCityGateSound2() { Mix_HaltChannel(CH_CITY_GATE2); } // no buffer loaded — no-op
void stopCityGateSound2() { Mix_HaltChannel(CH_CITY_GATE2); }
void playCityGateSound3() { if (g_cityGate3) Mix_PlayChannel(CH_CITY_GATE3, g_cityGate3, 0); }
void stopCityGateSound3() { Mix_HaltChannel(CH_CITY_GATE3); }

void playSpellSound()
{
    ensureMixerInit();
    if (!g_spellSound) {
        g_spellSound = Mix_LoadWAV("data/audio/spell.wav");
    }
    if (g_spellSound) Mix_PlayChannel(CH_SPELL, g_spellSound, 0);
}