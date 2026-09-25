#pragma once

// Platform-agnostic window abstraction
// Implementations: SFML (legacy), SDL2 (native + web)

#include <cstdint>
#include <string>
#include <optional>

namespace arx {

struct WindowConfig {
    uint32_t width = 640;
    uint32_t height = 400;
    std::string title = "ARXX";
    bool fullscreen = false;
    bool vsync = true;
};

enum class Key {
    Unknown = 0,
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    Escape, Space, Enter, Backspace, Tab,
    Left, Right, Up, Down,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    Comma, Period
};

enum class EventType {
    Closed,
    KeyPressed,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    Resized,
    TextInput       // Character typed (text field populated)
};

struct Event {
    EventType type;
    Key key = Key::Unknown;
    int mouseButton = -1;
    uint32_t width = 0;
    uint32_t height = 0;
    std::string text;   // For TextInput events: the typed character as string
};

class IWindow {
public:
    virtual ~IWindow() = default;

    virtual bool create(const WindowConfig& config) = 0;
    virtual void destroy() = 0;
    virtual bool isOpen() const = 0;

    virtual std::optional<Event> pollEvent() = 0;
    virtual void display() = 0;
    virtual void clear() = 0;

    virtual void* getNativeHandle() const = 0;
    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;

    virtual void pushGLStates() = 0;
    virtual void popGLStates() = 0;
};

// Factory function (implemented per platform)
IWindow* createPlatformWindow();

} // namespace arx
