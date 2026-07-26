#pragma once

// Platform-neutral clock/time/sleep replacements for SFML's sf::Clock, sf::Time, sf::sleep.
// Uses std::chrono internally. Compatible with native MSVC and Emscripten.

#include <chrono>
#include <thread>

namespace arx {

class Time {
public:
    Time() : m_duration(0.0f) {}
    explicit Time(float seconds) : m_duration(seconds) {}

    float asSeconds() const { return m_duration.count(); }
    int asMilliseconds() const {
        return static_cast<int>(m_duration.count() * 1000.0f);
    }

    bool operator==(const Time& other) const { return m_duration == other.m_duration; }
    bool operator!=(const Time& other) const { return m_duration != other.m_duration; }
    bool operator<(const Time& other) const { return m_duration < other.m_duration; }
    bool operator<=(const Time& other) const { return m_duration <= other.m_duration; }
    bool operator>(const Time& other) const { return m_duration > other.m_duration; }
    bool operator>=(const Time& other) const { return m_duration >= other.m_duration; }

    Time operator+(const Time& other) const { return Time(asSeconds() + other.asSeconds()); }
    Time operator-(const Time& other) const { return Time(asSeconds() - other.asSeconds()); }
    Time operator*(float scale) const { return Time(asSeconds() * scale); }
    Time& operator+=(const Time& other) { m_duration += other.m_duration; return *this; }
    Time& operator-=(const Time& other) { m_duration -= other.m_duration; return *this; }

    static const Time Zero;

private:
    using Duration = std::chrono::duration<float>;
    Duration m_duration;
};

inline const Time Time::Zero = Time(0.0f);

inline Time seconds(float amount) { return Time(amount); }
inline Time milliseconds(float amount) { return Time(amount / 1000.0f); }

class Clock {
public:
    Clock() : m_start(std::chrono::steady_clock::now()) {}

    Time getElapsedTime() const {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = now - m_start;
        float secs = std::chrono::duration<float>(elapsed).count();
        return Time(secs);
    }

    Time restart() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = now - m_start;
        float secs = std::chrono::duration<float>(elapsed).count();
        m_start = now;
        return Time(secs);
    }

private:
    std::chrono::steady_clock::time_point m_start;
};

inline void sleep(Time duration) {
    int ms = duration.asMilliseconds();
    if (ms > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

} // namespace arx