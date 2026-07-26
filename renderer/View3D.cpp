// View3D.cpp - WebGL ES 2.0 3D corridor renderer for the web/SDL2 build.
// Mirrors the logic of 3Dview.cpp but uses shader-based rendering.
// No fixed-function pipeline (no glBegin/glEnd, no gluPerspective, no glFog).

#ifdef ARX_USE_SDL2

#include "View3D.h"

#include <SDL_opengles2.h>
#include <SDL_image.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstring>

#include "constants.h"
#include "display.h"
#include "player.h"
#include "level.h"
#include "globals.h"

extern int windowWidth, windowHeight;
extern int viewPortX, viewPortY, viewWidth, viewHeight;
extern int graphicMode;

static GLuint s_wallTex[noOfTextures] = {};
static GLuint s_bgTex[noOfBackgrounds] = {};
static std::string s_textureNames[noOfTextures];
static std::string s_bgNames[noOfBackgrounds];

static GLuint s_prog3D = 0;
static GLuint s_vbo3D = 0;
static GLuint s_bgProg = 0;
static GLuint s_bgVbo = 0;
static GLuint s_bgVao = 0;

static GLint s_aPos3D = -1;
static GLint s_aUV3D = -1;
static GLint s_uMVP = -1;
static GLint s_uTex3D = -1;
static GLint s_uColor3D = -1;

static GLint s_aBgPos = -1;
static GLint s_aBgUV = -1;
static GLint s_uBgTex = -1;

static const int k_depth = 13;
static const int k_columns = 25;
static const char* BUILD_VERSION = "View3D v0.3.0-INDICATOR-TEST";

static const char* k_vert3D = R"(
attribute vec3 a_pos;
attribute vec2 a_uv;
uniform mat4 u_mvp;
varying vec2 v_uv;
void main() {
    gl_Position = u_mvp * vec4(a_pos, 1.0);
    v_uv = a_uv;
    gl_PointSize = 1.0;
}
)";

static const char* k_frag3D = R"(
precision mediump float;
varying vec2 v_uv;
uniform sampler2D u_tex;
uniform vec4 u_color;
void main() {
    vec4 tc = texture2D(u_tex, v_uv);
    gl_FragColor = vec4(tc.rgb * u_color.rgb, 1.0);
}
)";

static const char* k_vertBg = R"(
attribute vec2 a_pos;
attribute vec2 a_uv;
varying vec2 v_uv;
void main() {
    gl_Position = vec4(a_pos, 0.0, 1.0);
    v_uv = a_uv;
}
)";

static const char* k_fragBg = R"(
precision mediump float;
varying vec2 v_uv;
uniform sampler2D u_tex;
void main() {
    vec4 tc = texture2D(u_tex, v_uv);
    gl_FragColor = vec4(tc.rgb, 1.0);
}
)";

static GLuint compileShader(GLenum type, const char* src)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(s, sizeof(log), nullptr, log);
        std::cerr << "View3D shader error: " << log << std::endl;
        glDeleteShader(s);
        return 0;
    }
    return s;
}

static GLuint linkProgram(const char* vs, const char* fs)
{
    GLuint v = compileShader(GL_VERTEX_SHADER, vs);
    if (!v) return 0;
    GLuint f = compileShader(GL_FRAGMENT_SHADER, fs);
    if (!f) { glDeleteShader(v); return 0; }
    GLuint p = glCreateProgram();
    glAttachShader(p, v);
    glAttachShader(p, f);
    glLinkProgram(p);
    glDeleteShader(v);
    glDeleteShader(f);
    GLint ok = 0;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetProgramInfoLog(p, sizeof(log), nullptr, log);
        std::cerr << "View3D program error: " << log << std::endl;
        glDeleteProgram(p);
        return 0;
    }
    return p;
}

static void mat4Identity(float m[16])
{
    memset(m, 0, 64);
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

static void mat4Multiply(float out[16], const float a[16], const float b[16])
{
    float tmp[16];
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++) {
            tmp[c * 4 + r] = 0;
            for (int k = 0; k < 4; k++)
                tmp[c * 4 + r] += a[k * 4 + r] * b[c * 4 + k];
        }
    memcpy(out, tmp, 64);
}

static void mat4Perspective(float m[16], float fovY, float aspect, float zNear, float zFar)
{
    mat4Identity(m);
    float f = 1.0f / tanf(fovY * 3.14159265f / 360.0f);
    m[0] = f / aspect;
    m[5] = f;
    m[10] = (zFar + zNear) / (zNear - zFar);
    m[11] = -1.0f;
    m[14] = (2.0f * zFar * zNear) / (zNear - zFar);
    m[15] = 0.0f;
}

static void mat4Translate(float m[16], float tx, float ty, float tz)
{
    mat4Identity(m);
    m[12] = tx;
    m[13] = ty;
    m[14] = tz;
}

static GLuint loadTexture(const std::string& path)
{
    std::cout << "loadTexture: Attempting to load " << path << std::endl;
    SDL_Surface* surf = IMG_Load(path.c_str());
    if (!surf) {
        std::cerr << "loadTexture: IMG_Load failed for " << path << std::endl;
        // Note: getcwd is not available in Emscripten; skip working directory logging
        return 0;
    }

    // Convert to standard RGBA format to ensure WebGL compatibility
    SDL_Surface* rgbaSurf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surf);
    if (!rgbaSurf) {
        std::cerr << "loadTexture: SDL_ConvertSurfaceFormat failed for " << path << std::endl;
        return 0;
    }

    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Set unpack alignment for proper byte alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Always use GL_RGBA since we converted the surface
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbaSurf->w, rgbaSurf->h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, rgbaSurf->pixels);

    // Reset to default
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    SDL_FreeSurface(rgbaSurf);
    return tex;
}

namespace arx {

    void view3D_loadTextures()
    {
        // Use textures.txt (Atari 8-bit names) to load original pixel-art textures
        // from textures_original/ directory (e.g. citywall.png, wall.png, floor.png)
        std::ifstream f("data/map/core/textures.txt");
        if (!f) { std::cerr << "View3D: cannot open textures.txt\n"; return; }
        int loaded = 0, failed = 0;
        for (int i = 0; i < noOfTextures; i++) {
            std::string line;
            if (!std::getline(f, line)) break;
            auto idx = line.find('=');
            if (idx == std::string::npos) continue;
            std::string name = line.substr(idx + 2);
            while (!name.empty() && (name.back() == '\r' || name.back() == '\n' || name.back() == ' '))
                name.pop_back();
            s_textureNames[i] = name;

            // Prefer original pixel-art textures
            s_wallTex[i] = loadTexture("data/images/textures_original/" + name + ".png");
            if (!s_wallTex[i]) {
                std::string uc = name;
                if (!uc.empty()) uc[0] = (char)toupper(uc[0]);
                s_wallTex[i] = loadTexture("data/images/textures_original/" + uc + ".png");
            }
            // Fallback to alternate textures
            if (!s_wallTex[i])
                s_wallTex[i] = loadTexture("data/images/textures_alternate/" + name + ".png");
            if (!s_wallTex[i]) {
                std::string uc = name;
                if (!uc.empty()) uc[0] = (char)toupper(uc[0]);
                s_wallTex[i] = loadTexture("data/images/textures_alternate/" + uc + ".png");
            }
            if (s_wallTex[i]) loaded++;
            else failed++;
        }
        std::cout << "View3D: textures loaded " << loaded << "/" << noOfTextures
            << " (failed=" << failed << ")" << std::endl;
    }

    void view3D_loadBackgrounds()
    {
        std::ifstream f("data/map/core/backgrounds.txt");
        if (!f) { std::cerr << "View3D: cannot open backgrounds.txt\n"; return; }
        int loaded = 0, failed = 0;

        // Determine which scenario folder to load from
        std::string scenarioFolder = "data/images/Scenario_0/";  // Default to city (relative path for web)
        if (plyr.scenario == 1) {
            scenarioFolder = "data/images/Scenario_1/";  // Dungeon
        }
        else if (plyr.scenario == 6) {
            scenarioFolder = "data/images/Scenario_6/";  // Wilderness
        }

        std::cout << "View3D: ===== LOADING BACKGROUNDS =====" << std::endl;
        std::cout << "View3D: Scenario folder: " << scenarioFolder << " (scenario=" << plyr.scenario << ")" << std::endl;

        // Try to load a default placeholder image (for any missing backgrounds)
        GLuint defaultBgTex = loadTexture("data/images/backgrounds/sky.png");
        if (defaultBgTex) {
            std::cout << "View3D: Default placeholder loaded (tex=" << defaultBgTex << ")" << std::endl;
        }

        for (int i = 0; i < noOfBackgrounds; i++) {
            std::string line;
            if (!std::getline(f, line)) break;
            auto idx = line.find('=');
            if (idx == std::string::npos) continue;
            std::string name = line.substr(idx + 2);
            while (!name.empty() && (name.back() == '\r' || name.back() == '\n' || name.back() == ' '))
                name.pop_back();
            s_bgNames[i] = name;

            // Load from scenario-specific folder ONLY
            std::string path = scenarioFolder + name + ".png";
            s_bgTex[i] = loadTexture(path);
            if (s_bgTex[i]) {
                std::cout << "View3D:   [" << i << "] LOADED: " << name << " (tex=" << s_bgTex[i] << ")" << std::endl;
                loaded++;
            }
            else {
                // Use default placeholder if file not found
                if (defaultBgTex) {
                    s_bgTex[i] = defaultBgTex;
                    std::cout << "View3D:   [" << i << "] PLACEHOLDER: " << name << " (using default)" << std::endl;
                    loaded++;
                }
                else {
                    if (i <= 10 || (i >= 4 && i <= 7)) {  // Log city backgrounds and first few
                        std::cout << "View3D:   [" << i << "] FAILED: " << name << " - no fallback available" << std::endl;
                    }
                    failed++;
                }
            }
        }
        std::cout << "View3D: ===== LOAD COMPLETE =====" << std::endl;
        std::cout << "View3D: Loaded " << loaded << "/" << noOfBackgrounds << " (failed=" << failed << ")" << std::endl;
    }

    static bool s_glInited = false;
    static void initGL()
    {
        if (s_glInited) return;
        s_glInited = true;
        s_prog3D = linkProgram(k_vert3D, k_frag3D);
        if (s_prog3D) {
            s_aPos3D = glGetAttribLocation(s_prog3D, "a_pos");
            s_aUV3D = glGetAttribLocation(s_prog3D, "a_uv");
            s_uMVP = glGetUniformLocation(s_prog3D, "u_mvp");
            s_uTex3D = glGetUniformLocation(s_prog3D, "u_tex");
            s_uColor3D = glGetUniformLocation(s_prog3D, "u_color");
            std::cout << "View3D: 3D shader program compiled successfully" << std::endl;
        }
        else {
            std::cerr << "View3D: ERROR - 3D shader program failed to compile!" << std::endl;
        }
        glGenBuffers(1, &s_vbo3D);
        s_bgProg = linkProgram(k_vertBg, k_fragBg);
        if (s_bgProg) {
            s_aBgPos = glGetAttribLocation(s_bgProg, "a_pos");
            s_aBgUV = glGetAttribLocation(s_bgProg, "a_uv");
            s_uBgTex = glGetUniformLocation(s_bgProg, "u_tex");
            std::cout << "View3D: Background shader - aBgPos=" << s_aBgPos
                << " aBgUV=" << s_aBgUV << " uBgTex=" << s_uBgTex << std::endl;
            if (s_uBgTex < 0) {
                std::cerr << "View3D: WARNING - u_tex uniform not found (optimized out?)" << std::endl;
            }
            // If u_tex was optimized out, try binding via texture unit 0 default
            std::cout << "View3D: Background shader program compiled successfully" << std::endl;
        }
        else {
            std::cerr << "View3D: ERROR - Background shader program failed to compile!" << std::endl;
        }
        glGenBuffers(1, &s_bgVbo);
    }

    void view3D_drawBackground()
    {
        std::cout << "View3D_drawBackground: ENTRY - scenario=" << plyr.scenario << " zone=" << plyr.zone << std::endl;
        int bgIdx = 0;

        // City interior zones (not zone 99 outdoor) have no sky - draw black
        if ((plyr.scenario == 0) && (plyr.zone != 99))
        {
            std::cout << "View3D_drawBackground: City interior (zone != 99) - drawing black" << std::endl;
            // Draw solid black
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
            glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glDepthMask(GL_TRUE);
            glEnable(GL_DEPTH_TEST);
            return;
        }

        // If background shader failed, use solid fallback
        if (!s_bgProg) {
            std::cout << "View3D_drawBackground: Background shader NOT loaded - drawing blue fallback" << std::endl;
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
            glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glDepthMask(GL_TRUE);
            glEnable(GL_DEPTH_TEST);
            return;
        }

        // City (scenario 0): Draw direction-appropriate city backdrop
        // backgrounds.txt indices 4-7: cityBackE, cityBackS, cityBackN, cityBackW
        if (plyr.scenario == 0)
        {
            std::cout << "View3D_drawBackground: City scenario detected, s_bgProg=" << s_bgProg << std::endl;
            // Select background based on facing direction
            switch (plyr.facing) {
            case EAST:  bgIdx = 4; break; // cityBackE
            case SOUTH: bgIdx = 5; break; // cityBackS
            case NORTH: bgIdx = 6; break; // cityBackN
            case WEST:  bgIdx = 7; break; // cityBackW
            default:    bgIdx = 4; break; // default to east
            }
            GLuint tex = s_bgTex[bgIdx];
            std::cout << "View3D_drawBackground: city bgIdx=" << bgIdx << " facing=" << plyr.facing << " tex=" << tex << " s_bgTex[4]=" << s_bgTex[4] << " name=" << s_bgNames[bgIdx] << std::endl;

            // The GL viewport is ALREADY set to the scissored box (viewPortX, glY, viewWidth, viewHeight)
            // by the caller (view3D_draw), so a full clip-space quad (-1..1) is all that's needed here.
            // Do NOT recompute box-in-window NDC offsets - the viewport transform already maps
            // -1..1 onto the box; doing both applies the offset twice and shrinks the quad.
            float verts[] = {
                -1.f, -1.f, 0.f, 1.f,
                 1.f, -1.f, 1.f, 1.f,
                -1.f,  1.f, 0.f, 0.f,
                 1.f,  1.f, 1.f, 0.f,
            };

            // Draw textured quad with the background image
            if (tex && s_bgProg) {
                glDisable(GL_DEPTH_TEST);
                glDepthMask(GL_FALSE);
                glUseProgram(s_bgProg);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, tex);
                if (s_uBgTex >= 0) {
                    glUniform1i(s_uBgTex, 0);
                }
                glBindBuffer(GL_ARRAY_BUFFER, s_bgVbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);
                glEnableVertexAttribArray(s_aBgPos);
                glVertexAttribPointer(s_aBgPos, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(s_aBgUV);
                glVertexAttribPointer(s_aBgUV, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glDisableVertexAttribArray(s_aBgPos);
                glDisableVertexAttribArray(s_aBgUV);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glUseProgram(0);
                glDepthMask(GL_TRUE);
                glEnable(GL_DEPTH_TEST);
                return;
            }
            else {
                // Fallback: draw solid red if no texture
                std::cout << "View3D_drawBackground: No valid texture - drew RED fallback (tex=" << tex << " s_bgProg=" << s_bgProg << ")" << std::endl;
                glDisable(GL_DEPTH_TEST);
                glDepthMask(GL_FALSE);
                glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glDepthMask(GL_TRUE);
                glEnable(GL_DEPTH_TEST);
            }
            glDepthMask(GL_TRUE);
            glEnable(GL_DEPTH_TEST);
            return;
        }
        else {
            // Fallback: draw solid blue sky so we can see the background is rendering
            std::cout << "View3D_drawBackground: Non-city scenario detected - drawing blue fallback" << std::endl;
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
            glClearColor(0.3f, 0.6f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glDepthMask(GL_TRUE);
            glEnable(GL_DEPTH_TEST);
            return;
        }

        // ZoneSet-based backgrounds for non-city areas
        if (plyr.zoneSet == 0)  bgIdx = 15;
        else if (plyr.zoneSet == 1)  bgIdx = 10;
        else if (plyr.zoneSet == 2)  bgIdx = 8;
        else if (plyr.zoneSet == 4)  bgIdx = 48;
        else if (plyr.zoneSet == 5)  bgIdx = 46;
        else if (plyr.zoneSet == 11) bgIdx = 12;
        else if (plyr.zoneSet == 14) bgIdx = 15;
        else if (plyr.zoneSet == 15) bgIdx = 15;
        else if (plyr.zoneSet == 16) bgIdx = 16;
        else if (plyr.zoneSet == 17) bgIdx = 17;
        else if (plyr.zoneSet == 18) bgIdx = 17;
        else if (plyr.zoneSet == 21 || plyr.zoneSet == 22) bgIdx = 44;
        else if (plyr.zoneSet == 23) bgIdx = 45;
        else if (plyr.zoneSet == 24) bgIdx = 8;
        else if (plyr.zoneSet == 25) bgIdx = 15;
        else if (plyr.zoneSet == 26) bgIdx = 12;
        else if (plyr.zoneSet == 27) bgIdx = 16;

        if (bgIdx < 0 || bgIdx >= noOfBackgrounds) bgIdx = 0;
        GLuint tex = s_bgTex[bgIdx];
        if (!tex) return;

        // Calculate NDC coordinates that fill only the viewport area
        float ndcLeft = -1.f + 2.f * (viewPortX / (float)windowWidth);
        float ndcRight = -1.f + 2.f * ((viewPortX + viewWidth) / (float)windowWidth);
        float ndcBottom = -1.f + 2.f * ((windowHeight - viewPortY - viewHeight) / (float)windowHeight);
        float ndcTop = -1.f + 2.f * ((windowHeight - viewPortY) / (float)windowHeight);

        float verts[] = {
            ndcLeft, ndcBottom, 0.f, 1.f,
            ndcRight, ndcBottom, 1.f, 1.f,
            ndcLeft, ndcTop, 0.f, 0.f,
            ndcRight, ndcTop, 1.f, 0.f,
        };
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glUseProgram(s_bgProg);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glUniform1i(s_uBgTex, 0);
        glBindBuffer(GL_ARRAY_BUFFER, s_bgVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);
        glEnableVertexAttribArray(s_aBgPos);
        glVertexAttribPointer(s_aBgPos, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(s_aBgUV);
        glVertexAttribPointer(s_aBgUV, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableVertexAttribArray(s_aBgPos);
        glDisableVertexAttribArray(s_aBgUV);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glUseProgram(0);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
    }

    static int getTexIdx(int wallType)
    {
        switch (wallType) {
        case 1: case 2:       return zones[plyr.zoneSet].arch;
        case 3: case 4:
        case 7: case 8: case 9: case 10: case 11: case 12:
            return zones[plyr.zoneSet].door;
        case 5: case 6:       return (plyr.supervision == 1) ? zones[plyr.zoneSet].door : zones[plyr.zoneSet].wall;
        case 13: case 14:     return zones[plyr.zoneSet].wall;
        case 27: return 27; case 28: return 28; case 29: return 29;
        case 30: return 30; case 31: return 31; case 32: return 32;
        case 33: return 33; case 68: return 68; case 69: return 69;
        case 70: return 70;
        default: return wallType;
        }
    }

    static void drawQuad(const float mvp[16], GLuint tex,
        float x0, float y0, float z0,
        float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3,
        bool blend = false)
    {
        if (!tex || !s_prog3D) return;
        if (blend) { glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); }
        float verts[] = {
            x0, y0, z0, 0.f, 1.f,
            x1, y1, z1, 1.f, 1.f,
            x2, y2, z2, 1.f, 0.f,
            x0, y0, z0, 0.f, 1.f,
            x2, y2, z2, 1.f, 0.f,
            x3, y3, z3, 0.f, 0.f,
        };
        glUseProgram(s_prog3D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glUniform1i(s_uTex3D, 0);
        glUniformMatrix4fv(s_uMVP, 1, GL_FALSE, mvp);
        glUniform4f(s_uColor3D, 1.2f, 1.1f, 1.0f, 1.f);
        glBindBuffer(GL_ARRAY_BUFFER, s_vbo3D);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);
        glEnableVertexAttribArray(s_aPos3D);
        glVertexAttribPointer(s_aPos3D, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(s_aUV3D);
        glVertexAttribPointer(s_aUV3D, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(s_aPos3D);
        glDisableVertexAttribArray(s_aUV3D);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glUseProgram(0);
        if (blend) glDisable(GL_BLEND);
    }

    static void drawCellWalls(const float mvp[16], int c, int d,
        float xm, float zm,
        int frontwall, int leftwall, int rightwall,
        int frontheight, int leftheight, int rightheight)
    {
        const float dff = (float)(-k_depth * 2) + 1.f;
        const float dfn = (float)(-k_depth * 2) + 3.f;
        const int midcol = (k_columns - 1) / 2;

        // Force city walls to have a minimum height of 1
        if (plyr.scenario == 0) {
            if (frontheight < 1) frontheight = 1;
            if (leftheight < 1) leftheight = 1;
            if (rightheight < 1) rightheight = 1;
        }

        // Ceiling - matches 3Dview.cpp lines 687-706
        int texNoCeiling = 0;
        // City outdoor (zone 99) has no ceiling - just sky background
        if ((plyr.scenario == 0) && (plyr.zone == 99)) {
            // No ceiling for city outdoor - leave as 0
        }
        else if ((plyr.zone == 99) && (plyr.map == 1)) { texNoCeiling = 61; } // dungeon level 1 ceiling
        else if ((plyr.zone == 99) && (plyr.map == 2)) { texNoCeiling = 36; } // dungeon level 2 ceiling
        else if ((plyr.zone == 99) && (plyr.map == 4)) { texNoCeiling = 52; } // dungeon level 4 ceiling
        else if (plyr.zone != 99) { // interior zones
            texNoCeiling = (plyr.ceiling == 0) ? zones[plyr.zoneSet].ceiling : plyr.ceiling;
        }

        if (texNoCeiling > 0 && texNoCeiling < noOfTextures && s_wallTex[texNoCeiling])
            drawQuad(mvp, s_wallTex[texNoCeiling], -25.f + xm, 0.5f, dff + zm, -23.f + xm, 0.5f, dff + zm, -23.f + xm, 0.5f, dfn + zm, -25.f + xm, 0.5f, dfn + zm);

        // Floor - DISABLED (was causing rendering issues with original pixel-art textures)
        // int texNoFloor = 0;
        // if (zones[plyr.zoneSet].floor > 0) texNoFloor = zones[plyr.zoneSet].floor;
        // if (plyr.floorTexture > 0) texNoFloor = plyr.floorTexture;
        // if ((plyr.scenario == 0) && (texNoFloor == 0)) texNoFloor = 3; // default city floor
        // if (plyr.zone != 99) { // interior zones: use zoneSet floor
        //     texNoFloor = (plyr.floorTexture == 0) ? zones[plyr.zoneSet].floor : plyr.floorTexture;
        // }
        // if (texNoFloor > 0 && texNoFloor < noOfTextures && s_wallTex[texNoFloor])
        //     drawQuad(mvp, s_wallTex[texNoFloor], -25.f+xm,-0.5f,dff+zm, -23.f+xm,-0.5f,dff+zm, -23.f+xm,-0.5f,dfn+zm, -25.f+xm,-0.5f,dfn+zm);

        // Left wall
        if (leftwall >= 1 && c <= midcol) {
            int ti = getTexIdx(leftwall);
            if (ti >= 0 && ti < noOfTextures && s_wallTex[ti])
                drawQuad(mvp, s_wallTex[ti], -25.f + xm, -0.5f, dfn + zm, -25.f + xm, -0.5f, dff + zm, -25.f + xm, -0.5f + leftheight, dff + zm, -25.f + xm, -0.5f + leftheight, dfn + zm, (leftwall == 1 || leftwall == 2));
        }
        // Right wall
        if (rightwall >= 1 && c >= midcol) {
            int ti = getTexIdx(rightwall);
            if (ti >= 0 && ti < noOfTextures && s_wallTex[ti])
                drawQuad(mvp, s_wallTex[ti], -23.f + xm, -0.5f, dff + zm, -23.f + xm, -0.5f, dfn + zm, -23.f + xm, -0.5f + rightheight, dfn + zm, -23.f + xm, -0.5f + rightheight, dff + zm, (rightwall == 1 || rightwall == 2));
        }
        // Front wall
        if (frontwall >= 1) {
            int ti = getTexIdx(frontwall);
            if (ti >= 0 && ti < noOfTextures && s_wallTex[ti])
                drawQuad(mvp, s_wallTex[ti], -25.f + xm, -0.5f, dff + zm, -23.f + xm, -0.5f, dff + zm, -23.f + xm, -0.5f + frontheight, dff + zm, -25.f + xm, -0.5f + frontheight, dff + zm, (frontwall == 1 || frontwall == 2));
        }
    }

    static void processCell(const float mvp[16], int c, int d)
    {
        int x = 0, y = 0;
        switch (plyr.facing) {
        case NORTH: x = plyr.x - (k_columns - 1) / 2 + c;  y = plyr.y - (k_depth - 1) + d;  break;
        case WEST:  x = plyr.x - (k_depth - 1) + d;       y = plyr.y + (k_columns - 1) / 2 - c;  break;
        case EAST:  x = plyr.x + (k_depth - 1) - d;       y = plyr.y - (k_columns - 1) / 2 + c;  break;
        case SOUTH: x = plyr.x + (k_columns - 1) / 2 - c;   y = plyr.y + (k_depth - 1) - d;  break;
        }
        if (x < 0 || x >= plyr.mapWidth || y < 0 || y >= plyr.mapHeight) return;
        int ind = getMapIndex(x, y);
        transMapIndex(ind);
        float xm = (float)(c * 2);
        float zm = (float)(d * 2);
        zm = (zm + plyr.z_offset) - 1.0f;
        drawCellWalls(mvp, c, d, xm, zm,
            plyr.front, plyr.left, plyr.right,
            plyr.frontheight, plyr.leftheight, plyr.rightheight);
    }

    void view3D_draw()
    {
        initGL();

        // First, clear the entire window to a dark color so the viewport boundary is visible
        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(0.2f, 0.2f, 0.3f, 1.0f);  // Dark gray-blue for outer area
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Now set viewport to the 3D view area
        int glY = windowHeight - (viewPortY + viewHeight);
        glViewport(viewPortX, glY, viewWidth, viewHeight);

        // IMPORTANT: Enable scissor test to restrict all drawing to viewport area
        glEnable(GL_SCISSOR_TEST);
        glScissor(viewPortX, glY, viewWidth, viewHeight);

        // Clear color buffer in viewport area BEFORE drawing background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background fallback
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw background ONLY for outdoor city (zone 99)
        // Interior zones (zone != 99) have no background - just black behind walls
        // NOTE: dungeon (scenario 1) and wilderness (scenario 6) never reach this call at all,
        // so they currently only ever show the black clear above. If you want a background
        // (or an explicit "no background") for those modes too, this condition needs revisiting.
        if ((plyr.scenario == 0) && (plyr.zone == 99)) {
            view3D_drawBackground();
        }

        // Build the perspective projection matrix (matching original gluPerspective(45, aspect, 0.1, 100))
        float proj[16], view[16], mvp[16];
        mat4Perspective(proj, 45.0f, (float)viewWidth / (float)viewHeight, 0.1f, 100.0f);

        // Apply camera Z-translation (matching original translatef(0,0,-1.0))
        mat4Translate(view, 0.0f, 0.0f, -1.0f);

        // Combine: mvp = proj * view
        mat4Multiply(mvp, proj, view);

        // DO NOT clear color buffer again - background is already drawn!
        // Just prepare depth for wall rendering
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT);  // Only clear depth, not color!

        // Check current viewport before wall rendering
        GLint curVP2[4];
        glGetIntegerv(GL_VIEWPORT, curVP2);
        std::cout << "View3D: viewport before walls=(" << curVP2[0] << "," << curVP2[1] << ") size=(" << curVP2[2] << "x" << curVP2[3] << ")" << std::endl;

        // Build the level view - iterate over the depth x columns grid
        // This mirrors the original buildLevelView() + calculateWallPositions() logic
        const int midcol = (k_columns - 1) / 2;

        // Left side: columns 0 to midcol-1, drawn front-to-back (d=0=near, d=depth-1=far)
        for (int d = 0; d < k_depth; d++) {
            for (int c = 0; c < midcol; c++) {
                processCell(mvp, c, d);
            }
        }

        // Right side: columns midcol+1 to k_columns-1, drawn back-to-front for correct overlap
        for (int d = 0; d < k_depth; d++) {
            for (int c = k_columns - 1; c > midcol; c--) {
                processCell(mvp, c, d);
            }
        }

        // Center column: drawn front-to-back
        for (int d = 0; d < k_depth; d++) {
            processCell(mvp, midcol, d);
        }
        // Disable scissor test so nothing is clipped
        glDisable(GL_SCISSOR_TEST);

        // Restore full window viewport and disable depth test
        glViewport(0, 0, windowWidth, windowHeight);
        glDisable(GL_DEPTH_TEST);

        std::cout << "View3D: Disabled scissor and restored full viewport for UI rendering" << std::endl;

        // Flush all rendering to the screen
        glFlush();
    }

    void view3D_shutdown()
    {
        glDeleteTextures(noOfTextures, s_wallTex);
        glDeleteTextures(noOfBackgrounds, s_bgTex);
        memset(s_wallTex, 0, sizeof(s_wallTex));
        memset(s_bgTex, 0, sizeof(s_bgTex));
        if (s_prog3D) { glDeleteProgram(s_prog3D); s_prog3D = 0; }
        if (s_bgProg) { glDeleteProgram(s_bgProg); s_bgProg = 0; }
        if (s_vbo3D) { glDeleteBuffers(1, &s_vbo3D); s_vbo3D = 0; }
        if (s_bgVbo) { glDeleteBuffers(1, &s_bgVbo); s_bgVbo = 0; }
        s_glInited = false;
    }

} // namespace arx

#endif // ARX_USE_SDL2