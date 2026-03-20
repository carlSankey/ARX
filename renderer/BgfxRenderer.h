#pragma once

#ifndef BGFX_RENDERER_H
#define BGFX_RENDERER_H

#include <cstdint>
#include <SFML/Graphics.hpp>

// BgfxRenderer - Modern GPU renderer for the 3D corridor view.
// Replaces legacy OpenGL immediate-mode (glBegin/glEnd) with bgfx.
// SFML continues to handle: window, input, 2D UI, audio.
class BgfxRenderer
{
public:
    BgfxRenderer();
    ~BgfxRenderer();

    // Initialize bgfx with the SFML window. Call after createGameWindow().
    bool init(sf::RenderWindow& window);

    // Shutdown bgfx. Call before closing the window.
    void shutdown();

    // Advance to next bgfx frame. Call before SFML's display().
    // Pass false so SFML handles the swap.
    void frame();

    // Resize the view when the window size changes.
    void resize(uint32_t width, uint32_t height);

    bool isInitialized() const { return m_initialized; }

private:
    bool m_initialized = false;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
};

#endif // BGFX_RENDERER_H
