#pragma once

// Sprite2D - lightweight 2D sprite drawing using SDL2_image + OpenGL.
// Replaces sf::Texture + sf::Sprite for the web port.
//
// Usage:
//   arx::Sprite2D sprite;
//   sprite.load("path/to/image.png");
//   sprite.draw(100, 50);              // draw at position
//   sprite.draw(100, 50, 0.5f, 0.5f);  // draw at position with scale
//   sprite.setColor(255, 0, 0, 255);   // tint red
//   sprite.setTextureRect(0, 0, 16, 16); // sub-rect from loaded texture

#include <cstdint>
#include <string>

namespace arx {

struct Rect {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
};

class Sprite2D {
public:
    Sprite2D() = default;
    ~Sprite2D();

    // Load a texture from a PNG file. Returns true on success.
    bool load(const std::string& filepath);

    // Load from raw pixel data (RGBA).
    bool loadFromMemory(const uint8_t* data, int width, int height);

    // Set the source rectangle from the loaded texture (for spritesheets).
    void setTextureRect(int x, int y, int w, int h);

    // Set tint color (RGBA 0-255).
    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

    // Draw at position (x, y) with optional scaling.
    void draw(float x, float y, float scaleX = 1.0f, float scaleY = 1.0f) const;

    // Getters.
    int getWidth() const { return m_texWidth; }
    int getHeight() const { return m_texHeight; }
    bool isValid() const { return m_texId != 0; }

    // Get the total loaded texture dimensions (for spritesheet atlas).
    int getTextureWidth() const { return m_texWidth; }
    int getTextureHeight() const { return m_texHeight; }

private:
    unsigned int m_texId = 0;       // OpenGL texture name
    int m_texWidth = 0;             // Full texture width in pixels
    int m_texHeight = 0;            // Full texture height in pixels

    // Current source rect (subset of the texture to draw).
    int m_srcX = 0;
    int m_srcY = 0;
    int m_srcW = 0;
    int m_srcH = 0;

    // Current tint color (pre-multiplied for OpenGL).
    float m_colorR = 1.0f;
    float m_colorG = 1.0f;
    float m_colorB = 1.0f;
    float m_colorA = 1.0f;

#ifdef __EMSCRIPTEN__
    // WebGL shader-based rendering state
    mutable unsigned int m_shader = 0;   // GL program object
    mutable unsigned int m_vbo = 0;      // GL vertex buffer
    mutable int m_uColorLoc = -1;        // uniform location for color
    mutable int m_uTextureLoc = -1;      // uniform location for texture
    mutable int m_aPositionLoc = -1;     // attribute location for position
    mutable int m_aTexCoordLoc = -1;     // attribute location for texcoord
#endif
};

} // namespace arx