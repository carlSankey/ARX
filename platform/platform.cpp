// Platform layer implementation — Stage 1 (time + audio).
//
// Time is backed by std::chrono::steady_clock (same as arx::Clock).
// Audio is backed by SDL_mixer (same backend as audio.cpp). The pimpl
// structs hold Mix_Chunk / Mix_Music handles so platform.h exposes no
// SDL_mixer or SFML types. Behaviour matches the existing game code:
//   - sounds play once unless setLoop(true) was called
//   - music streams, loops only when setLoop(true) was called
//   - volume is 0-100 like SFML and maps to MIX_MAX_VOLUME
//   - stopping happens explicitly via stop(); destruction also halts its
//     own handle (mirrors sf::Music/sf::Sound going out of scope) without
//     touching other channels or global music state.

#include "platform.h"

#include <SDL_mixer.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

namespace plat {
namespace {

const std::chrono::steady_clock::time_point& startTime()
{
    static const std::chrono::steady_clock::time_point start =
        std::chrono::steady_clock::now();
    return start;
}

int volumeToMixer(float volume)
{
    const float clamped = std::clamp(volume, 0.0f, 100.0f);
    return static_cast<int>(clamped * MIX_MAX_VOLUME / 100.0f);
}

void ensureMixerInit()
{
    static bool ready = false;
    if (ready) {
        return;
    }
    // audio.cpp owns the canonical mixer init; opening twice is harmless and
    // keeps this translation unit usable standalone (tests, later SDL3 port).
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "plat: Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
        return;
    }
    Mix_AllocateChannels(16);
    ready = true;
}

} // namespace

void sleepMs(std::uint32_t ms)
{
    if (ms > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

std::uint64_t ticksMs()
{
    const auto now = std::chrono::steady_clock::now();
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime()).count());
}

struct Timer::Impl {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
};

Timer::Timer() : impl(std::make_unique<Impl>()) {}

float Timer::restart()
{
    const auto now = std::chrono::steady_clock::now();
    const float secs = std::chrono::duration<float>(now - impl->start).count();
    impl->start = now;
    return secs;
}

float Timer::elapsedSeconds() const
{
    const auto now = std::chrono::steady_clock::now();
    return std::chrono::duration<float>(now - impl->start).count();
}

struct Music::Impl {
    Mix_Music* music = nullptr;
    bool loop = false;
    float volume = 100.0f;
    bool playing = false;
};

Music::Music() : impl(std::make_unique<Impl>()) {}

Music::~Music()
{
    stop();
    if (impl->music) {
        Mix_FreeMusic(impl->music);
        impl->music = nullptr;
    }
}

bool Music::openFromFile(const std::string& path)
{
    ensureMixerInit();
    stop();
    if (impl->music) {
        Mix_FreeMusic(impl->music);
        impl->music = nullptr;
    }
    impl->music = Mix_LoadMUS(path.c_str());
    if (!impl->music) {
        std::cerr << "plat: Mix_LoadMUS " << path << ": " << Mix_GetError() << std::endl;
        return false;
    }
    Mix_VolumeMusic(volumeToMixer(impl->volume));
    return true;
}

void Music::play()
{
    if (!impl->music) {
        return;
    }
    ensureMixerInit();
    Mix_VolumeMusic(volumeToMixer(impl->volume));
    if (Mix_PlayMusic(impl->music, impl->loop ? -1 : 0) == 0) {
        impl->playing = true;
    }
}

void Music::pause()
{
    if (isPlaying()) {
        Mix_PauseMusic();
    }
}

void Music::stop()
{
    // Only halt global music if this instance started it; other owners
    // (audio.cpp shop music) must keep playing.
    if (impl->playing) {
        Mix_HaltMusic();
        impl->playing = false;
    }
}

bool Music::isPlaying() const
{
    return impl->playing && Mix_PlayingMusic() != 0;
}

void Music::setLoop(bool loop)
{
    impl->loop = loop;
}

void Music::setVolume(float volume)
{
    impl->volume = std::clamp(volume, 0.0f, 100.0f);
    Mix_VolumeMusic(volumeToMixer(impl->volume));
}

struct Sound::Impl {
    Mix_Chunk* chunk = nullptr;
    int channel = -1;
    bool loop = false;
    float volume = 100.0f;
};

Sound::Sound() : impl(std::make_unique<Impl>()) {}

Sound::~Sound()
{
    stop();
    if (impl->chunk) {
        Mix_FreeChunk(impl->chunk);
        impl->chunk = nullptr;
    }
}

bool Sound::loadFromFile(const std::string& path)
{
    ensureMixerInit();
    stop();
    if (impl->chunk) {
        Mix_FreeChunk(impl->chunk);
        impl->chunk = nullptr;
    }
    impl->chunk = Mix_LoadWAV(path.c_str());
    if (!impl->chunk) {
        std::cerr << "plat: Mix_LoadWAV " << path << ": " << Mix_GetError() << std::endl;
        return false;
    }
    Mix_VolumeChunk(impl->chunk, volumeToMixer(impl->volume));
    return true;
}

void Sound::play()
{
    if (!impl->chunk) {
        return;
    }
    ensureMixerInit();
    Mix_VolumeChunk(impl->chunk, volumeToMixer(impl->volume));
    impl->channel = Mix_PlayChannel(-1, impl->chunk, impl->loop ? -1 : 0);
}

void Sound::stop()
{
    if (impl->channel >= 0) {
        Mix_HaltChannel(impl->channel);
        impl->channel = -1;
    }
}

bool Sound::isPlaying() const
{
    return impl->channel >= 0 && Mix_Playing(impl->channel) != 0;
}

void Sound::setLoop(bool loop)
{
    impl->loop = loop;
}

void Sound::setVolume(float volume)
{
    impl->volume = std::clamp(volume, 0.0f, 100.0f);
    if (impl->chunk) {
        Mix_VolumeChunk(impl->chunk, volumeToMixer(impl->volume));
    }
}

} // namespace plat
