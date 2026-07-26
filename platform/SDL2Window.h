#pragma once

#include "PlatformWindow.h"

#ifdef ARX_USE_SDL2

#include <SDL.h>
#include <queue>

namespace arx {

class SDL2Window : public IWindow {
public:
    SDL2Window() = default;
    ~SDL2Window() override;

    bool create(const WindowConfig& config) override;
    void destroy() override;
    bool isOpen() const override { return m_window != nullptr; }

    std::optional<Event> pollEvent() override;
    void display() override;
    void clear() override;

    void* getNativeHandle() const override;
    uint32_t getWidth() const override { return m_width; }
    uint32_t getHeight() const override { return m_height; }

    void pushGLStates() override {}  // SDL2 doesn't need this
    void popGLStates() override {}

    SDL_Window* getSDLWindow() { return m_window; }

private:
    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
    uint32_t m_width = 640;
    uint32_t m_height = 400;

    static Key sdlKeyToArxKey(SDL_Keycode sdl);
};

} // namespace arx

#endif // ARX_USE_SDL2
