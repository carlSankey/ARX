#include "Sprite2D.h"

#ifdef ARX_USE_SDL2

#include <iostream>
#include <vector>
#include <cstring>

#ifdef __EMSCRIPTEN__
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#include <GL/glew.h>
#endif

#include <SDL_image.h>

namespace arx {

Sprite2D::~Sprite2D()
{
    if (m_texId != 0) {
        glDeleteTextures(1, &m_texId);
        m_texId = 0;
    }
#ifdef __EMSCRIPTEN__
    if (m_shader) {
        glDeleteProgram(m_shader);
        m_shader = 0;
    }
    if (m_vbo) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
#endif
}

#ifdef __EMSCRIPTEN__

// Simple vertex shader: transforms positions and passes texture coords
static const char* s_vertexSrc = R"(
    attribute vec2 a_position;
    attribute vec2 a_texCoord;
    varying vec2 v_texCoord;
    uniform vec4 u_color;
    void main() {
        gl_Position = vec4(a_position, 0.0, 1.0);
        v_texCoord = a_texCoord;
    }
)";

// Simple fragment shader: samples texture with proper alpha blending
static const char* s_fragmentSrc = R"(
    precision mediump float;
    varying vec2 v_texCoord;
    uniform sampler2D u_texture;
    uniform vec4 u_color;
    void main() {
        vec4 texColor = texture2D(u_texture, v_texCoord);
        gl_FragColor = vec4(texColor.rgb * u_color.rgb, texColor.a * u_color.a);
    }
)";

static GLuint createShader(GLenum type, const char* src)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        std::cerr << "Sprite2D shader compile error: " << log << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

static GLuint createProgram(const char* vertSrc, const char* fragSrc)
{
    GLuint vs = createShader(GL_VERTEX_SHADER, vertSrc);
    if (!vs) return 0;
    GLuint fs = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fs) {
        glDeleteShader(vs);
        return 0;
    }

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    GLint status = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (!status) {
        char log[512];
        glGetProgramInfoLog(prog, sizeof(log), nullptr, log);
        std::cerr << "Sprite2D program link error: " << log << std::endl;
        glDeleteProgram(prog);
        prog = 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}

#endif // __EMSCRIPTEN__

bool Sprite2D::load(const std::string& filepath)
{
    // Load image using SDL2_image
    SDL_Surface* surface = IMG_Load(filepath.c_str());
    if (!surface) {
        std::cout << "Sprite2D: FAILED to load image: " << filepath
                  << " (" << IMG_GetError() << ")" << std::endl;
        return false;
    }
    std::cout << "Sprite2D: loaded image " << filepath << " (" << surface->w << "x" << surface->h << ")" << std::endl;

    // Convert to RGBA format for OpenGL
    SDL_Surface* rgbaSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(surface);
    if (!rgbaSurface) {
        std::cout << "Sprite2D: Failed to convert surface: " << SDL_GetError() << std::endl;
        return false;
    }

    bool result = loadFromMemory(
        static_cast<const uint8_t*>(rgbaSurface->pixels),
        rgbaSurface->w,
        rgbaSurface->h
    );

    SDL_FreeSurface(rgbaSurface);
    return result;
}

bool Sprite2D::loadFromMemory(const uint8_t* data, int width, int height)
{
    if (m_texId != 0) {
        glDeleteTextures(1, &m_texId);
        m_texId = 0;
    }

    glGenTextures(1, &m_texId);
    std::cout << "Sprite2D: loaded texture " << m_texId << " (" << width << "x" << height << ")" << std::endl;
    glBindTexture(GL_TEXTURE_2D, m_texId);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    m_texWidth = width;
    m_texHeight = height;

    // Default source rect is the full texture
    m_srcX = 0;
    m_srcY = 0;
    m_srcW = width;
    m_srcH = height;

#ifdef __EMSCRIPTEN__
    // Lazily init shader and VBO on first texture load
    if (!m_shader) {
        m_shader = createProgram(s_vertexSrc, s_fragmentSrc);
        if (m_shader) {
            glUseProgram(m_shader);
            m_uColorLoc = glGetUniformLocation(m_shader, "u_color");
            m_uTextureLoc = glGetUniformLocation(m_shader, "u_texture");
            m_aPositionLoc = glGetAttribLocation(m_shader, "a_position");
            m_aTexCoordLoc = glGetAttribLocation(m_shader, "a_texCoord");

            // Create VBO for a single quad
            glGenBuffers(1, &m_vbo);

            // Set texture unit 0
            glUniform1i(m_uTextureLoc, 0);
        }
    }
#endif

    return true;
}

void Sprite2D::setTextureRect(int x, int y, int w, int h)
{
    m_srcX = x;
    m_srcY = y;
    m_srcW = w;
    m_srcH = h;
}

void Sprite2D::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    m_colorR = r / 255.0f;
    m_colorG = g / 255.0f;
    m_colorB = b / 255.0f;
    m_colorA = a / 255.0f;
}

void Sprite2D::draw(float x, float y, float scaleX, float scaleY) const
{
    if (m_texId == 0) {
        std::cerr << "Sprite2D::draw: m_texId == 0, skipping" << std::endl;
        return;
    }

    float w = static_cast<float>(m_srcW) * scaleX;
    float h = static_cast<float>(m_srcH) * scaleY;

    // Texture coordinates (normalized 0.0 - 1.0)
    float tx1 = static_cast<float>(m_srcX) / static_cast<float>(m_texWidth);
    float ty1 = static_cast<float>(m_srcY) / static_cast<float>(m_texHeight);
    float tx2 = static_cast<float>(m_srcX + m_srcW) / static_cast<float>(m_texWidth);
    float ty2 = static_cast<float>(m_srcY + m_srcH) / static_cast<float>(m_texHeight);

    glBindTexture(GL_TEXTURE_2D, m_texId);

#ifdef __EMSCRIPTEN__

    if (!m_shader) return;

    glUseProgram(m_shader);

    // Build vertex data: interleaved position (x,y) + texcoord (u,v)
    // Quad as two triangles: 4 vertices in triangle strip order (actually as separate triangles)
    // Using 4 vertices in triangle strip order: 0,1,2,3 -> two triangles (0,1,2) and (1,3,2)
    // But for simplicity, use 6 vertices for 2 triangles
    struct Vertex {
        float px, py;
        float tu, tv;
    };

    // NDC coordinates: map pixel coords to [-1, 1] range
    // We need the viewport size. We'll use current viewport.
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float vpHalfW = viewport[2] / 2.0f;
    float vpHalfH = viewport[3] / 2.0f;

    // Convert from pixel coords to NDC
    // Note: WebGL Y axis is up, so we negate Y
    float x1 = (x - vpHalfW) / vpHalfW;
    float y1 = -(y - vpHalfH) / vpHalfH;
    float x2 = (x + w - vpHalfW) / vpHalfW;
    float y2 = -(y + h - vpHalfH) / vpHalfH;

    Vertex verts[6] = {
        { x1, y1, tx1, ty1 },  // top-left
        { x2, y1, tx2, ty1 },  // top-right
        { x1, y2, tx1, ty2 },  // bottom-left
        { x2, y1, tx2, ty1 },  // top-right
        { x2, y2, tx2, ty2 },  // bottom-right
        { x1, y2, tx1, ty2 }   // bottom-left
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);

    constexpr GLsizei stride = sizeof(Vertex);

    glEnableVertexAttribArray(m_aPositionLoc);
    glVertexAttribPointer(m_aPositionLoc, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, px));

    glEnableVertexAttribArray(m_aTexCoordLoc);
    glVertexAttribPointer(m_aTexCoordLoc, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, tu));

    glUniform4f(m_uColorLoc, m_colorR, m_colorG, m_colorB, m_colorA);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(m_aPositionLoc);
    glDisableVertexAttribArray(m_aTexCoordLoc);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

#else

    // Native OpenGL compat path: immediate mode
    glEnable(GL_TEXTURE_2D);
    glColor4f(m_colorR, m_colorG, m_colorB, m_colorA);

    glBegin(GL_QUADS);
    glTexCoord2f(tx1, ty1); glVertex2f(x, y);
    glTexCoord2f(tx2, ty1); glVertex2f(x + w, y);
    glTexCoord2f(tx2, ty2); glVertex2f(x + w, y + h);
    glTexCoord2f(tx1, ty2); glVertex2f(x, y + h);
    glEnd();

    glDisable(GL_TEXTURE_2D);

#endif
}

} // namespace arx

#endif // ARX_USE_SDL2