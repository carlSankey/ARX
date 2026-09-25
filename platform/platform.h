#pragma once

// Platform layer — Stage 1 (time + audio).
//
// Adapted for this tree: the game logic already runs on SDL2 / SDL_mixer
// (arx::Clock, arx::IWindow, audio.cpp), not SFML, so this header exposes a
// small `plat::` facade over those backends. No SFML or SDL headers are
// included here; the implementation lives in platform/platform.cpp.
//
// Behaviour is unchanged: plat::Timer matches arx::Clock (steady_clock),
// plat::sleepMs matches arx::sleep(arx::seconds()), plat::Music / plat::Sound
// wrap SDL_mixer handles with pimpl so callers never see mixer types.

#include <cstdint>
#include <memory>
#include <string>

namespace plat {

// ---- Time ----
void     sleepMs(std::uint32_t ms); // replaces sf::sleep(sf::seconds(x)) / sf::milliseconds
std::uint64_t ticksMs();            // milliseconds since first call (steady clock)

class Timer {                       // replaces sf::Clock + sf::Time
public:
    Timer();
    float restart();               // returns elapsed seconds, then resets (like sf::Clock::restart)
    float elapsedSeconds() const;  // like getElapsedTime().asSeconds()
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

// ---- Audio ----
class Music {                       // replaces sf::Music (streamed .ogg)
public:
    Music();
    ~Music();
    Music(const Music&) = delete;
    Music& operator=(const Music&) = delete;
    bool openFromFile(const std::string& path);
    void play();
    void pause();
    void stop();
    bool isPlaying() const;
    void setLoop(bool loop);
    void setVolume(float volume);  // 0-100, same as SFML
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

class Sound {                       // replaces sf::SoundBuffer + sf::Sound pairs
public:
    Sound();
    ~Sound();
    Sound(const Sound&) = delete;
    Sound& operator=(const Sound&) = delete;
    bool loadFromFile(const std::string& path);
    void play();
    void stop();
    bool isPlaying() const;
    void setLoop(bool loop);
    void setVolume(float volume);  // 0-100, same as SFML
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

} // namespace plat
