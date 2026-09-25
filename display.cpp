#include "platform/GLESLoader.h"

#include <cstdint>
#include <optional>
//#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <memory>

#include "globals.h"
#include "player.h"
#include "display.h"
#include "level.h"
#include "game.h"
#include "font.h"
#include "encounter.h"
#include "misc.h"
#include "items.h"
#include "automap.h"
#include "lyrics.h"
#include "actor.h" // For weapons
#include "spells.h"
#include "effects.h"
#include "renderer/Sprite2D.h"
#include "renderer/View3D.h"
#include "platform/Window.h"

// using namespace std;
using std::string;
using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::hex;
using std::dec;



string version = "0.90.1";

int windowMode, graphicMode = 5, windowWidth, windowHeight, viewWidth, viewHeight;
int viewPortX;
int viewPortY;
int statPanelX; // x starting position for displaying the stats banner for centering
int statPanelY;
int consoleY; // y position for displaying the bottom screen info panel
int consoleX; // x starting position for displaying the panel for centering
int miniMapY; // y position for displaying the bottom screen info panel
int miniMapX; // x starting position for displaying the panel for centering
int lyricX;
int lyricY;
int loadingX;
int loadingY;
int gateX;
int gateY;
int shopStatsY;   // y position for stats in shops
int shopConsoleY; // y position for console in shops
int shopPictureY;

extern std::string roomMessages[noOfRoomMessages];
extern string descriptions[255];

//extern buffer_item itemBuffer[100];


string olddrawText; // text string used for setting and passing strings to the print routine
float uiScale = 1.0f; // global UI scale factor, set by setScreenValues()

struct animFrame
{
    int xOffset;    // 0 for most animations
int yOffset;    // 0 for most animations
int image;
int duration;
};

// If image2 == 255 then just display image1 rather than use animations below
//Dungeon Monster Animation Scripts

animFrame encounterAnim[123] =
{
    {0,0,0,12},     // 0-2 devourer
    {0,0,1,12},
    {0,0,2,12},

    {0,0,3,111},    // 3-4 healer
    {0,0,3,111},

    {0,0,6,100},     // 5-6 pauper
    {0,0,7,36},

    {0,0,8,36},     // 7 nobleman

    {0,0,9,48},     // 8-9 alien
    {0,0,10,48},

    {0,0,11,32},     // 10-14 robot
    {0,0,12,16},
    {0,0,13,16},
    {0,0,14,16},
    {0,0,15,16},

    {0,0,16,36},     // 15 acolyte

    {0,0,17,100},    // 16-18 wizard
    {0,0,18,50},
    {0,0,19,50},

    {0,0,20,144},    // 19 orc

    {0,0,21,144},    // 20-22 dwarf
    {0,0,22,144},
    {0,0,23,144},

    {0,0,24,20},     // 23-26 mold
    {0,0,25,18},
    {0,0,26,18},
    {0,0,27,18},

    {0,0,28,88},     // 27-28 thief
    {0,0,29,88},

    {154,9,30,20},  // 29-36 bat
    {150,11,31,24},
    {153,13,30,20},
    {156,11,31,24},
    {157,10,30,20},
    {160,9,31,24},
    {159,11,30,20},
    {157,12,31,24},

    {154,9,32,20},  // 37-44 phoenix
    {150,11,33,24},
    {153,13,32,20},
    {156,11,33,24},
    {157,10,32,20},
    {160,9,33,24},
    {159,11,32,20},
    {157,12,33,24},

    {0,0,34,88},     // 45 - 46 sorceress
    {0,0,35,88},

    {0,0,36,16},     // 47-51 whirlwind
    {0,0,37,16},
    {0,0,38,16},
    {0,0,39,16},
    {0,0,38,16},

    {0,0,40,42},     // 52-53 rat
    {0,0,41,42},

    {0,0,43,20},     // 54-57 dragon
    {0,0,44,18},
    {0,0,45,18},
    {0,0,42,18},

    {0,0,46,35},     // 58-60 skeleton
    {0,0,47,35},
    {0,0,48,35},

    {156,11,49,42},  // 61-65  ghost?
    {155,10,49,42},
    {155,9,49,42},
    {156,10,49,42},
    {157,11,49,42},

    {0,0,50,22},     // 66-69 vampire
    {0,0,51,22},
    {0,0,52,22},
    {0,0,53,22},

    {0,0,54,40},     // 70-71 hellhound
    {0,0,55,10},

    {0,0,56,56},     // 72-73 assassin
    {0,0,57,28},

    {0,0,58,33},     // 74-75 harpy
    {0,0,59,33},

    {0,0,60,64},     // 76-77 gremlin
    {0,0,61,64},

    {0,0,62,10},     // 78-83 flame demon
    {0,0,63,10},
    {0,0,64,10},
    {0,0,65,10},
    {0,0,66,10},
    {0,0,67,10},

    {0,0,68,56},     // 84-85 ronin
    {0,0,69,28},

    {0,0,70,40},     // 86-87 valkyrie
    {0,0,71,40},

    {0,0,72,40},    // 88 female doppleganger

    {157,8,73,18}, // 89-91 adventurer
    {157,8,74,19},
    {157,8,75,19},

    {0,0,76,100},    // 92-93 water sprite
    {0,0,77,100},

    {0,0,78,32},     // 94-95 mind flayer
    {0,0,79,32},

    {0,0,80,32},     // 96-97 serpent man
    {0,0,81,32},

    {0,0,82,32},     // 98-99 goblin
    {0,0,83,32},

    {0,0,84,18},     // 100-103 snake
    {0,0,85,18},
    {0,0,86,10},
    {0,0,87,14},

    {0,0,88,32},     // 104-105 demon
    {0,0,89,32},

    {0,0,90,32},     // 106-107 berserker
    {0,0,91,32},

    {0,0,92,32},     // 108 male doppelganer

    {0,0,93,32},     // 109-110 troll
    {0,0,94,32},

    {0,0,95,32},     // 111-112 ghoul
    {0,0,96,32},

    {0,0,97,32},     // 113-114 black knight
    {0,0,98,32},

    {0,0,99,32},     // 115-116 guard
    {0,0,100,32},

    {0,0,101,48},     // 117-122 white knight
    {0,0,102,48},
    {0,0,101,48},
    {0,0,102,48},
    {0,0,103,48},
    {0,0,102,48}
};
// end of animation sequences excluding city images

bool animationNotStarted;
int firstFrame;
int lastFrame;
int currentFrame;   // within encounterAnim 0-7
int yOffset;
int xOffset;
int animImage;
int animDuration;

// Draw a filled rectangle (bar) at pixel position (x,y) with given width, height, and RGBA color.
// Shader path shared by web and native: OpenGL ES has no immediate mode
// (glBegin/glEnd, glColor, GL_TEXTURE_2D enable), so the native build uses
// the same minimal colored-quad shader as the web build.
static void drawFilledRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if (w <= 0 || h <= 0) return;
    // Shader path: use raw GL with a simple colored quad
    // Since Sprite2D shader always expects a texture, we draw raw GL primitives
    // with a minimal built-in shader
    static bool s_initialized = false;
    static GLuint s_barShader = 0;
    static GLuint s_barVbo = 0;
    static GLint s_barColorLoc = -1;
    static GLint s_barPosLoc = -1;

    if (!s_initialized) {
        // Simple vertex shader
        const char* vs = "attribute vec2 a_pos; uniform vec4 u_color; void main() { gl_Position = vec4(a_pos, 0.0, 1.0); }";
        const char* fs = "precision mediump float; uniform vec4 u_color; void main() { gl_FragColor = u_color; }";
        s_barShader = glCreateProgram();
        GLuint vsh = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vsh, 1, &vs, nullptr);
        glCompileShader(vsh);
        glAttachShader(s_barShader, vsh);
        GLuint fsh = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fsh, 1, &fs, nullptr);
        glCompileShader(fsh);
        glAttachShader(s_barShader, fsh);
        glLinkProgram(s_barShader);
        glDeleteShader(vsh);
        glDeleteShader(fsh);
        s_barColorLoc = glGetUniformLocation(s_barShader, "u_color");
        s_barPosLoc = glGetAttribLocation(s_barShader, "a_pos");
        glGenBuffers(1, &s_barVbo);
        s_initialized = true;
    }
    if (!s_barShader) return;

    glUseProgram(s_barShader);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float vpHalfW = viewport[2] / 2.0f;
    float vpHalfH = viewport[3] / 2.0f;

    float x1 = (x - vpHalfW) / vpHalfW;
    float y1 = -(y - vpHalfH) / vpHalfH;
    float x2 = (x + w - vpHalfW) / vpHalfW;
    float y2 = -(y + h - vpHalfH) / vpHalfH;

    float verts[12] = {
        x1, y1, x2, y1, x1, y2,
        x2, y1, x2, y2, x1, y2
    };

    glBindBuffer(GL_ARRAY_BUFFER, s_barVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);
    glEnableVertexAttribArray(s_barPosLoc);
    glVertexAttribPointer(s_barPosLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glUniform4f(s_barColorLoc, r/255.0f, g/255.0f, b/255.0f, a/255.0f);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(s_barPosLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void drawAtariAnimation()
{
    // SDL2/web implementation - animation display handled through sprite system
}

void createGameWindow()
{
    // Window creation handled by platform abstraction layer
    std::cout << "Welcome to Alternate Reality X " << version << " ..." << std::endl << std::endl;
}

void drawConsoleBackground()
{
    /* Console background drawing - disabled for web build as text-based rendering doesn't support filled backgrounds */
    // The location text will display directly on the screen background
}

void setScreenValues()
{
    // Determines screen element locations based on window dimensions
    // Compute a UI scale factor based on window size relative to the base 640x480 design
    float scaleX = windowWidth  / 640.0f;
    float scaleY = windowHeight / 480.0f;
    uiScale = (scaleX < scaleY) ? scaleX : scaleY; // uniform scale, no stretching
    if (uiScale < 1.0f) uiScale = 1.0f; // never shrink below original size

    // Base content width scales with the window
    int contentWidth = static_cast<int>(640 * uiScale);

    gateX = (windowWidth - contentWidth) / 2;
    gateY = ((windowHeight - static_cast<int>(384 * uiScale)) / 2) - static_cast<int>(78 * uiScale);
    loadingX = 16;
    loadingY = 11;
    int spacer = 0;
    int lineHeight = static_cast<int>(18 * uiScale); // scaled font line height
    int consoleHeight = lineHeight * 10;

    statPanelX = (windowWidth - contentWidth) / 2;
    consoleX   = (windowWidth - contentWidth) / 2;

    /* Original small 3D view */
    if (graphicMode < ALTERNATE_LARGE)
    {
        viewWidth  = static_cast<int>(288 * uiScale);
        viewHeight = static_cast<int>(144 * uiScale);
        viewPortX  = (windowWidth - viewWidth) / 2;
        int statPanelHeight = static_cast<int>(110 * uiScale);
        int temp = statPanelHeight + spacer + viewHeight + spacer + consoleHeight;
        statPanelY = (windowHeight - temp) / 2;
        if (statPanelY < 0) statPanelY = 0;
        viewPortY  = statPanelY + statPanelHeight + spacer;
        consoleY   = viewPortY + viewHeight + 4;
        miniMapX   = windowWidth - (((((windowWidth - viewWidth) / 2) - static_cast<int>(144 * uiScale)) / 2) + static_cast<int>(144 * uiScale));
        miniMapY   = spacer + viewPortY + ((viewHeight - static_cast<int>(144 * uiScale)) / 2);
    }

    /* New large 3D view */
    if (graphicMode == ALTERNATE_LARGE)
    {
        viewWidth  = windowWidth;
        viewHeight = windowHeight;
        viewPortX  = 0;
        viewPortY  = windowHeight - viewHeight;
        statPanelY = 16;
        consoleY   = (windowHeight - 32) - 144;
        miniMapX   = (windowWidth - 16) - 176;
        miniMapY   = (windowHeight - 176) / 2;
    }

    /* Shop positions */
    int statPanelHeight = static_cast<int>(110 * uiScale);
    int temp = statPanelHeight + spacer + static_cast<int>(144 * uiScale) + 16 + consoleHeight;
    shopStatsY   = statPanelY;
    shopPictureY = shopStatsY + statPanelHeight + spacer;
    shopConsoleY = shopPictureY + static_cast<int>(144 * uiScale) + spacer;

    lyricX = (windowWidth - contentWidth) / 2;
    lyricY = shopPictureY - static_cast<int>(18 * uiScale);
}

void dispInit()
{
    setScreenValues();
    /* Set up window based on choice of display option NOT screen resolution */
    //setScreenValues(); // Determine positions of screen elements from array
    
    // OpenGL setup handled by View3D renderer in web build
}

void clearDisplay()
{
    // Set viewport to full window for 2D sprite/text rendering
    glViewport(0, 0, windowWidth, windowHeight);
    
    // Disable depth test so 2D sprites always render
    glDisable(GL_DEPTH_TEST);
    
    // Enable alpha blending for sprite compositing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Clear the color buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void updateDisplay()
{
    // SDL2/web implementation: swap buffers to present rendered frame to screen
    if (arx::g_window) {
        arx::g_window->display();
    }
}

void displayLoading()
{
    clearDisplay();
    drawText(loadingX,loadingY,"Loading...");
    updateDisplay();
}

void displayMainMenu()
{
srand(static_cast<unsigned int>(time(NULL)));
drawLogo();
int tempy = (windowHeight-(180+240))/2;
int z = (240)/18;
//int z = 12;
int x = 2;
//drawText(1,0,"x");
drawText(x+3,z,   "(1) Create a new City character");
drawText(x+3,z+1, "(2) Create a new Dungeon character");
drawText(x+3,z+2, "(3) Resume a character");
drawText(x+3,z+3, "(4) Acknowledgements");
drawText(x+3,z+4, "(5) Modify view:");
drawText(x+3,z+6, "(6) Modify audio:");
drawText(x+3,z+7, "(7) Modify font:");
drawText(x+3,z+9, "(0) Leave the game");

if (graphicMode == ATARI_SMALL) { drawText(x+7,z+5,     "Texture       Atari 8bit"); }
if (graphicMode == A16BIT_SMALL) { drawText(x+7,z+5,    "Texture       Amiga 16bit"); }
if (graphicMode == ALTERNATE_SMALL) { drawText(x+7,z+5, "Texture       Small 3D"); }
if (graphicMode == ALTERNATE_LARGE) { drawText(x+7,z+5, "Texture       Large 3D"); }

if (plyr.musicStyle == 0) { drawText(x+21,z+6,"Atari 8bit"); } else { drawText(x+21,z+6,"Alternate"); }
if (plyr.fontStyle == 0) { drawText(x+21,z+7,"Smooth"); } else { drawText(x+21,z+7,"Atari 8bit"); }
}

void dispMain()
{
// Clear the frame at the start of every render
clearDisplay();

    draw3DView();
    drawStatsPanel();
    drawInfoPanels();
    drawCompass();
    drawAutomap();
    if ((graphicMode==ALTERNATE_LARGE) && (plyr.status != 3 )) drawConsoleBackground();
}

static std::unordered_map<std::string, std::unique_ptr<arx::Sprite2D>> g_imageSprites;

void drawImage(string imagename, int x, int y)
{
    // Check if sprite is already cached
    auto it = g_imageSprites.find(imagename);
    if (it == g_imageSprites.end()) {
        // Load the image on demand
        auto sprite = std::make_unique<arx::Sprite2D>();
        std::string path;
        
        // Counter digit images are in data/images/core/
        if (imagename.length() == 1 && imagename[0] >= '0' && imagename[0] <= '9') {
            path = "data/images/core/" + imagename + ".png";
        } else {
            path = imagename; // treat as full path
        }
        
        if (sprite->load(path)) {
            auto result = g_imageSprites.emplace(imagename, std::move(sprite));
            it = result.first;
        } else {
            std::cerr << "drawImage: failed to load '" << path << "'" << std::endl;
            return;
        }
    }
    
    // Draw the sprite at the specified position
    it->second->draw(static_cast<float>(x), static_cast<float>(y));
}

void drawCompass()
{
    if (graphicMode != ALTERNATE_LARGE) return;
    
    // Draw compass in top-right corner showing player facing direction
    int compassX = miniMapX;
    int compassY = miniMapY;
    int compassSize = 16;
    
    // Draw compass background
    SetFontColour(80, 80, 80, 128);
    drawText(compassX / 8, compassY / 8, "┌──────┐");
    drawText(compassX / 8, (compassY + 8) / 8, "│COMPAS│");
    drawText(compassX / 8, (compassY + 16) / 8, "│      │");
    drawText(compassX / 8, (compassY + 24) / 8, "└──────┘");
    
    // Draw direction indicator based on player facing
    SetFontColour(255, 255, 0, 255);
    int arrowX = compassX / 8 + 3;
    int arrowY = compassY / 8 + 2;
    
    switch (plyr.facing) {
        case NORTH:
            drawText(arrowX, arrowY, "N");
            break;
        case SOUTH:
            drawText(arrowX, arrowY, "S");
            break;
        case EAST:
            drawText(arrowX, arrowY, "E");
            break;
        case WEST:
            drawText(arrowX, arrowY, "W");
            break;
    }
    
    SetFontColour(215, 215, 215, 255);
}

void loadResources()
{
loadBackgroundNames();
loadTextureNames();
initTextures();
initLyricFont();
initMaps();
// SDL2/web - sprite resources loaded through sprite system
}

void loadCounterImages()
{
    // Preload the 10 counter digit images (0-9) from data/images/core/
    for (char c = '0'; c <= '9'; c++) {
        std::string name(1, c);
        std::string path = "data/images/core/" + name + ".png";
        
        auto sprite = std::make_unique<arx::Sprite2D>();
        if (sprite->load(path)) {
            g_imageSprites[name] = std::move(sprite);
        } else {
            std::cerr << "loadCounterImages: failed to load '" << path << "'" << std::endl;
        }
    }
    std::cout << "loadCounterImages: loaded digit sprites 0-9" << std::endl;
}

// Gate sprites
static arx::Sprite2D g_dungeonGateSprite;
static arx::Sprite2D g_cityGateSprite;
static bool g_dungeonGateLoaded = false;
static bool g_cityGateLoaded = false;

// Gate transform info so counters can be positioned relative to the scaled/centered gate
// The gate and counter positions are designed for a 640x480 viewport, so we need to
// transform counter coordinates by the same scale/offset as the gate image.
static float s_gateScale = 1.0f;
static float s_gateOffsetX = 0.0f;
static float s_gateOffsetY = 0.0f;

// Expose gate transform to AppLoop.cpp for counter digit positioning
float getGateScale()    { return s_gateScale; }
float getGateOffsetX()  { return s_gateOffsetX; }
float getGateOffsetY()  { return s_gateOffsetY; }

void displayDungeonGateImage()
{
    if (!g_dungeonGateLoaded) {
        g_dungeonGateLoaded = g_dungeonGateSprite.load("data/images/Scenario_1/Gate.png");
        if (!g_dungeonGateLoaded) {
            std::cerr << "displayDungeonGateImage: failed to load gate image" << std::endl;
            return;
        }
    }
    
    // Scale the gate to fill width, maintain aspect ratio, center vertically
    float imgW = static_cast<float>(g_dungeonGateSprite.getWidth());
    float imgH = static_cast<float>(g_dungeonGateSprite.getHeight());
    float scaleX = static_cast<float>(windowWidth) / imgW;
    float scaleY = static_cast<float>(windowHeight) / imgH;
    // Use the smaller uniform scale so nothing is clipped
    float scale = (scaleX < scaleY) ? scaleX : scaleY;
    float scaledW = imgW * scale;
    float scaledH = imgH * scale;
    float offsetX = (static_cast<float>(windowWidth) - scaledW) / 2.0f;
    float offsetY = (static_cast<float>(windowHeight) - scaledH) / 2.0f;
    s_gateScale = scale;
    s_gateOffsetX = offsetX;
    s_gateOffsetY = offsetY;
    g_dungeonGateSprite.draw(offsetX, offsetY, scale, scale);
}

void displayCityGateImage()
{
    if (!g_cityGateLoaded) {
        g_cityGateLoaded = g_cityGateSprite.load("data/images/Scenario_0/Gate.png");
        if (!g_cityGateLoaded) {
            std::cerr << "displayCityGateImage: failed to load gate image" << std::endl;
            return;
        }
    }
    
    // Scale the gate to fill width, maintain aspect ratio, center vertically
    float imgW = static_cast<float>(g_cityGateSprite.getWidth());
    float imgH = static_cast<float>(g_cityGateSprite.getHeight());
    float scaleX = static_cast<float>(windowWidth) / imgW;
    float scaleY = static_cast<float>(windowHeight) / imgH;
    // Use the smaller uniform scale so nothing is clipped
    float scale = (scaleX < scaleY) ? scaleX : scaleY;
    float scaledW = imgW * scale;
    float scaledH = imgH * scale;
    float offsetX = (static_cast<float>(windowWidth) - scaledW) / 2.0f;
    float offsetY = (static_cast<float>(windowHeight) - scaledH) / 2.0f;
    s_gateScale = scale;
    s_gateOffsetX = offsetX;
    s_gateOffsetY = offsetY;
    g_cityGateSprite.draw(offsetX, offsetY, scale, scale);
}

int checkCityDoors() // currently only forward!!!
{
return 0;
}

void drawInfoPanels()
{
    drawConsoleBackground();
    
    // Draw status text under the banner, above the viewport
    // Check if status_text is not empty and not just whitespace
    bool hasStatusText = false;
    if (!plyr.status_text.empty()) {
        hasStatusText = true;
        for (char c : plyr.status_text) {
            if (!isspace(c)) {
                hasStatusText = true;
                break;
            }
        }
    }
    
    if (hasStatusText && (plyr.status != 3) && (plyr.alive)) {
        // Draw a dark green background bar behind the status text, full screen width, text height
        int barY = statPanelY + static_cast<int>(5 * 18 * uiScale);
        int barH = static_cast<int>(18 * uiScale);
        drawFilledRect(0, barY, windowWidth, barH, 40, 70, 15, 255);
        // Draw status text on top of the bar
        SetFontColour(102,149,40, 255);
        drawText(2, 5, plyr.status_text);
    }
    
    // Always reset to white for info panel text
    SetFontColour(215, 215, 215, 255);

if (plyr.status!=3)
{
    // Default to panel 1 if infoPanel is 0 (unset or invalid value)
    if (plyr.infoPanel == 0) plyr.infoPanel = 1;

    if (plyr.infoPanel == 1)
    {

        if ( (plyr.special>=0xc0) && (plyr.special<=0xff) && (plyr.scenario==1) ) // 224 - 255 was d2
        {
            string str;
            int ind = (plyr.special-0xc0);
      str = roomMessages[ind];
            cText(str);
        }
        else
        {
bText(2, 1, "Food Packets    Torches   Water Flasks"); //was 3
            bText(7, 2,plyr.food);
            bText (20, 2, plyr.torches);
            bText (33, 2, plyr.water);
            if ( plyr.scenario == 0)
            {
                  bText(11, 5, "You are in the city");
                  bText(12, 6, "of Xebec's Demise");
            }
            if (plyr.scenario == 1)
            {
if (plyr.map==1) { bText(12, 5, "You are on level 1"); }
if (plyr.map==2) { bText(12, 5, "You are on level 2"); }
if (plyr.map==3) { bText(12, 5, "You are on level 3"); }
if (plyr.map==4) { bText(12, 5, "You are on level 4"); }
                bText(14, 6, "of the Dungeon");
            }
            if ( plyr.scenario == 2)
            {
                  bText(11, 5, "You are in the Arena");
                  bText(12, 6, "of Xebec's Demise");
            }
if (plyr.scenario == 6)
{
bText(11, 5, "You are in the Wilderness");
bText(12, 6, "of Xebec's Demise");
}
string thirstDesc = checkThirst();
bText (1,7,thirstDesc);
string hungerDesc = checkHunger();
bText (1,8,hungerDesc);
string alcoholDesc = checkAlcohol();
bText (1,9,alcoholDesc);
string weightDesc = checkEncumbrance();
bText (30,7,weightDesc);
string poisonDesc = checkPoison();
bText (31,8,poisonDesc);
string diseaseDesc = checkDisease();
bText (31,9,diseaseDesc);
string fatigueDesc = checkFatigue();
    bText (1, 6, fatigueDesc);

        }

    }

       if (plyr.infoPanel == 2)
    {

//        int weapon = objectBuffer[plyr.priWeapon].index;
        //drawTop();
        bText(1, 1, "Gold Coins   Silver Coins   Copper Coins");
        bText(3, 2,plyr.gold);
bText(17, 2,plyr.silver);
bText(32, 2,plyr.copper);

//drawText(1, 1, "  %d             %d             %d",plyr.gold, plyr.silver, plyr.copper);

        bText(1, 4,"            Other Possessions");

   bText(8, 6, "Gems:");
       bText(6, 7, "Jewels:");
       bText(4, 8,"Crystals:");
       bText(29, 6,"Keys:");
       bText(24, 7,"Compasses:");
       bText(23, 8,"Timepieces:");

       bText(13, 6,plyr.gems);
   bText(13, 7,plyr.jewels);
   bText(13, 8,plyr.crystals);
   bText(34, 6,plyr.keys);
   bText(34, 7,plyr.compasses);
   bText(34, 8,plyr.timepieces);

    }

    if (plyr.infoPanel == 3)
    {

//        int weapon = objectBuffer[plyr.priWeapon].index;
        //drawTop();
        bText(18, 1, "Weapons");
string str = "Primary: Bare hand";
if (plyr.priWeapon != 255) { str = "Primary: "+itemBuffer[plyr.priWeapon].name; }
bText (1, 2, str);
str = "Secondary: Bare hand";
if (plyr.secWeapon != 255) { str = "Secondary: "+itemBuffer[plyr.secWeapon].name; }
bText (1, 3, str);

// plyr.headArmor MUST be set by USE command in game not manually!
        bText (19, 5, "Armour");
        bText (1, 6, "Head:");
bText (1, 7,"Body:");
        bText (1, 8,"Arms:");
        bText (1, 9,"Legs:");

str = "None";
if (plyr.headArmour != 255) { str = itemBuffer[plyr.headArmour].name; }
bText (7, 6, str);
str = "None";
if (plyr.bodyArmour != 255) { str = itemBuffer[plyr.bodyArmour].name; }
        bText (7, 7, str);
str = "None";
if (plyr.armsArmour != 255) { str = itemBuffer[plyr.armsArmour].name; }
bText (7, 8, str);
str = "None";
if (plyr.legsArmour != 255) { str = str = itemBuffer[plyr.legsArmour].name;; }
bText (7, 9, str);
    }

if (plyr.infoPanel == 4)
    {
        cyText(1, "Apparel");
if ((plyr.clothing[0]==255) && (plyr.clothing[1]==255) && (plyr.clothing[2]==255) && (plyr.clothing[3]==255))
            cyText(3, "Birthday suit");
int y = 3;
//int c3 = plyr.clothing[3];
//itemBuffer[plyr.clothing[3]].index;
if (plyr.clothing[0]!=255) { cyText(y,itemBuffer[plyr.clothing[0]].name); y++; }
if (plyr.clothing[1]!=255) { cyText(y,itemBuffer[plyr.clothing[1]].name); y++; }
if (plyr.clothing[2]!=255) { cyText(y,itemBuffer[plyr.clothing[2]].name); y++; }
if (plyr.clothing[3]!=255) { cyText(y,itemBuffer[plyr.clothing[3]].name); }
}

if (plyr.infoPanel == 5)
    {
        cyText(1, "Active Magic");
        int y = 3; // starting value for displaying items
     
if (plyr.ActiveSpell[0] != 0) { cyText(y, spells[plyr.ActiveSpell[0]].name); y++; }
if (plyr.ActiveSpell[1] != 0) { cyText(y, spells[plyr.ActiveSpell[1]].name); y++; }
if (plyr.ActiveSpell[2] != 0) { cyText(y, spells[plyr.ActiveSpell[2]].name); y++; }
if (plyr.ActiveSpell[3] != 0) { cyText(y, spells[plyr.ActiveSpell[3]].name); y++; }
    }

if (plyr.infoPanel == 6)
    {
        cyText(1, "Known Diseases");
        int y = 3; // starting value for displaying items
        if (plyr.diseases[0] > 14) { cyText(y,"Rabies"); y++; }
        if (plyr.diseases[1] > 47) { cyText(y,"Mold"); y++; }
        if (plyr.diseases[2] > 47) { cyText(y,"Fungus"); y++; }
    }

if (plyr.infoPanel == 7)
    {
        cyText(1, "Curses");
    }

    if (plyr.infoPanel == 8)
    {
        cyText(1, "Titles");
    }

    if (plyr.infoPanel == 9)
    {
        cyText(1, "USE");
        bText(5, 3, "(1) Food Packets: " + itos(plyr.food));
        bText(5, 4, "(2) Water Flasks: " + itos(plyr.water));
        bText(5, 5, "(3) Unlit Torches: " + itos(plyr.torches));
        bText(5, 6, "(4) Timepieces: " + itos(plyr.timepieces));
        bText(2, 8, "Item #, Forward, Back, or ESC to exit");
        SetFontColour(40, 96, 244, 255);
        bText(2, 8, "     #  F        B        ESC");
        SetFontColour(215, 215, 215, 255);
    }

//if (plyr.status==3) App.clear(sf::Color(0,0,0,192)); // if in combat wipe the panel
}
}

void clearShopDisplay()
{
    // SDL2/web - shop display cleared through main rendering
    drawStatsPanel();
}

void loadShopImage(int imageno)
{
    // SDL2/web - shop image loading handled through sprite system
}

void drawStatsPanel()
{
    // SDL2/web implementation of stats panel drawing with banner background
    std::cout << "ARXX: drawStatsPanel ENTRY - statPanelY=" << statPanelY 
              << " graphicMode=" << graphicMode << " scenario=" << plyr.scenario << std::endl;
    
    // Draw banner background image for all graphic modes
    static arx::Sprite2D bannerSprite;
    static bool bannerLoaded = false;
    static int lastScenario = -1;
    
    // Reload banner if scenario changed
    if (lastScenario != plyr.scenario) {
        bannerLoaded = false;
        lastScenario = plyr.scenario;
    }
    
    if (!bannerLoaded) {
        std::string bannerPath = "data/images/Scenario_" + std::to_string(plyr.scenario) + "/Banner.png";
        std::cout << "drawStatsPanel: attempting to load banner from " << bannerPath << std::endl;
        if (bannerSprite.load(bannerPath)) {
            bannerLoaded = true;
            std::cout << "drawStatsPanel: SUCCESS - banner loaded, size=" << bannerSprite.getWidth() 
                      << "x" << bannerSprite.getHeight() << std::endl;
        } else {
            std::cerr << "drawStatsPanel: FAILED to load banner from " << bannerPath << std::endl;
        }
    }
    
    if (bannerLoaded && bannerSprite.isValid()) {
        // Draw banner stretched to full content width, at statPanelY position
        float bannerW = 640.0f * uiScale;
        float bannerH = bannerW * (bannerSprite.getHeight() / (float)bannerSprite.getWidth());
        float bannerX = (windowWidth - bannerW) / 2.0f;
        float bannerY = (float)statPanelY;
        
        // Clamp banner to stay within window bounds
        if (bannerY < 0.0f) bannerY = 0.0f;
        if (bannerY + bannerH > windowHeight) bannerY = windowHeight - bannerH;
        
        std::cout << "drawStatsPanel: drawing banner at (" << bannerX << "," << bannerY 
                  << ") size=" << bannerW << "x" << bannerH << std::endl;
        bannerSprite.draw(bannerX, bannerY, bannerW / bannerSprite.getWidth(), bannerH / bannerSprite.getHeight());
    } else {
        std::cout << "drawStatsPanel: banner not drawn (loaded=" << bannerLoaded 
                  << " valid=" << (bannerLoaded ? bannerSprite.isValid() : false) << ")" << std::endl;
    }
    
    if (true)  // Temporarily force text display for testing
    {
        int y = 0; // Text rows relative to top of banner (drawText already adds statPanelY offset)
        SetFontColour(162, 114, 64, 255);
        drawText(2, y, plyr.name);
        drawText(32,y,"Level:");
        drawText(38,y,plyr.level);
        SetFontColour(147, 69, 130, 255);
        drawText(2,y+1,"Stats:  STA  CHR  STR  INT  WIS  SKL");
        SetFontColour(138,68,158, 255);
        drawText(11,y+2,plyr.sta);
        drawText(16,y+2,plyr.chr);
        drawText(21,y+2,plyr.str);
        drawText(26,y+2,plyr.inte);
        drawText(31,y+2,plyr.wis);
        drawText(36,y+2,plyr.skl);

        SetFontColour(62,106,162, 255);
        drawText(2,y+3,"Experience:");
        drawText(14,y+3,plyr.xp);
        if (plyr.hp < 0)
        {
           drawText(24,y+3,"Hit Points: !!!!!");
        }
        if (plyr.hp == plyr.maxhp)
        {
            drawText(25,y+3,"Hit Points=");
            drawText(36,y+3,plyr.hp);
        }
        if ((plyr.hp < plyr.maxhp) && (plyr.hp > -1))
        {
            drawText(25,y+3,"Hit Points:");
            drawText(36,y+3,plyr.hp);
        }

        std::string str;
        str = "You are "+ descriptions[plyr.location];

        checkForItemsHere();

        SetFontColour(102,149,40, 255);
        if (plyr.alive) { drawText(2,y+4,str); }
        else { drawText(12,y+4,"$ Where are you? $"); }

        SetFontColour(215, 215, 215, 255);
    }
}

void displayOptionsMenu()
{
//string str;

//SetFontColour(40, 96, 244, 255);
drawText(17,0,"Options");
drawText(8,2,"( ) Save current character");
drawText(8,4,"( ) Quit to main menu");

drawText(1,8,"Keys");
drawText(1,10,"F1-F7 Hotkeys for information screens");
drawText(1,11,", .   Toggle through information screens");
drawText(1,12,"I K   Move forward and backward");
drawText(1,13,"J L   Turn left and right");
drawText(1,14,"      (Arrow keys may also be used)");
drawText(1,15,"U     Use or equip items");
drawText(1,16,"0-9   Select options from menus");
drawText(1,17,"A     Display mini map");
drawText(1,18,"M     Display full screen map");
drawText(1,19,"W     Wait for an encounter");
drawText(1,20,"G     Get a weapon or item");
drawText(1,21,"D     Drop a weapon or item");
drawText(1,22,"C     Cast a spell");
drawText(1,23,"ESC   Display this screen");

SetFontColour(40, 96, 244, 255);
drawText (8,2," S");
drawText (8,4," Q");

SetFontColour(215, 215, 215, 255);

drawText(13,7,"Or ESC to cancel");
SetFontColour(40, 96, 244, 255);
drawText(13,7,"   ESC");
SetFontColour(215, 215, 215, 255);

//SetFontColour(215, 215, 215, 255);
}

void displayQuitMenu()
{
drawText(6,11," Are you sure you want to quit?");
drawText(15,13," ( es or  o)");
SetFontColour(40, 96, 244, 255);
drawText(15,13,"  Y      N");
SetFontColour(215, 215, 215, 255);
}

void displayAcknowledgements()
{
    bool acknowledgements = true;

    while (acknowledgements)
    {
        clearDisplay();
        SetFontColour(40, 96, 244, 255);
        drawText(1,3,"Alternate Reality X       New Music");
        drawText(1,6,"Alternate Reality Copyright and Concept");
        drawText(1,9,"Alternate Reality: The Dungeon");
        drawText(1,12,"Original AR Music");
        drawText(1,15,"Original AR Artwork          New Art");
        drawText(1,19,"Disassembly Genius");
        drawText(1,22,"Additional Programming");

        SetFontColour(215, 215, 215, 255);
        drawText(13,0,"Acknowledgements");
        drawText(1,4,"acrin1 AT gmail.com       Furious");
        drawText(1,7,"Philip Price");
        drawText(1,10,"Dan Pinal, Ken Jordan");
        drawText(1,13,"Gary Gilbertson");
        drawText(1,16,"Craig Skinner, Bonita Reid,  Ted Cox");
        drawText(1,17,"Steve Hofmann                Wisecat");
        drawText(1,20,"Jim Norris, Kroah & Brian Herlihy");
        drawText(1,23,"M Scott Adams");
        updateDisplay();
        if ( keyPressed() ) acknowledgements = false;
    }

        acknowledgements = true;

    while (acknowledgements)
    {
        clearDisplay();
        SetFontColour(40, 96, 244, 255);
        drawText(1,3,"   Special thanks for supporting the");
        drawText(1,5,"    development of release 0.82 to:");

        SetFontColour(215, 215, 215, 255);
        drawText(13,0,"Acknowledgements");
        drawText(1,8, "   Brian Herlihy       Dennis Hughes");
        drawText(1,10,"   Danny Belvin        Marco Fraolini");
        drawText(1,12,"   Allan van Leeuwen   Cliff Friedel");
        drawText(1,14,"   GameBanshee.com     Matthew Zagacki");
        drawText(1,16,"   Eric Koh            Stephen Mahoney");
        drawText(1,18,"   Tim Georgic");
        updateDisplay();
        if ( keyPressed() ) acknowledgements = false;
    }

    acknowledgements = true;

    while (acknowledgements)
    {
        clearDisplay();
        SetFontColour(40, 96, 244, 255);
        drawText(1,3,"    Special thanks for their support");

        SetFontColour(215, 215, 215, 255);
        drawText(13,0,"Acknowledgements");
        drawText(4,5, "Eric Koh           Brian Herlihy    ");
        drawText(4,6, "Maxzius            Stefano Peracchi ");
        drawText(4,7, "BelriX             Thomas Eibl     ");
        drawText(4,8, "James Denson       Aria             ");
        drawText(4,9, "Dramon Glover      Dalimar          ");
        drawText(4,10,"Paul Moore         Marco Fraolini   ");
        drawText(4,11,"Allan van Leeuwen  Jerry Stabell");
        drawText(4,12,"Stephen Mahoney    Goodman Gear");
        drawText(4,13,"Steven Kovach      Kara Cordner");
        drawText(4,14,"Andrew Hancock     Michael McCloskey");
        drawText(4,15,"Stefano Peracchi   Timothy Jones");
        drawText(4,16,"Stephen Latz       Chris Larson");
        drawText(4,17,"Matthew Zagacki    David Parslow");
        drawText(4,18,"Ross Lemke         Dennis Hughes");
        drawText(4,19,"Danny Belvin       Rudolf Kraus");
        drawText(4,20,"Jack Webb          Richard Milks");
        drawText(4,21,"Jaime Soltys");

        updateDisplay();
        if ( keyPressed() ) acknowledgements = false;
    }

}

void displayError()
{
    bool acknowledgements = true;

    while (acknowledgements)
    {
        clearDisplay();
        SetFontColour(40, 96, 244, 255);
        drawText(1, 3, "An Error has occured");
       
        updateDisplay();
        if (keyPressed()) acknowledgements = false;
    }
}

void SetTileImage(int tile_no)
{
    // SDL2/web - tile image setting handled through sprite system
}

// Logo sprite for main menu
static arx::Sprite2D g_logoSprite;

void loadLogoImage()
{
    // Load the logo image
    g_logoSprite.load("data/images/core/logo640x240.png");
}

void drawLogo()
{
    // Draw the logo image at the top of the screen
    // Logo is 640x240 pixels
    int logoX = (windowWidth - 640) / 2;
    int logoY = 16; // Near the top of the screen
    g_logoSprite.draw(static_cast<float>(logoX), static_cast<float>(logoY));
}

void shutdownDisplay()
{
#ifndef ARX_USE_SDL2
    // SFML cleanup
#else
    // SDL2/web cleanup - handled by Emscripten
#endif
}

void loadBackgroundNames()
{
#ifndef ARX_USE_SDL2
    // SFML background loading
#else
    // SDL2/web - background loading via View3D renderer
    arx::view3D_loadBackgrounds();
#endif
}

void initTextures()
{
#ifndef ARX_USE_SDL2
    // SFML texture loading
#else
    // SDL2/web - texture loading via View3D renderer
    arx::view3D_loadTextures();
#endif
}

// Disabled large and small_alternate options for release 0.75
/*
void drawEncounterAnimation()
{
sf::Texture encImageSheet;
sf::Sprite largeEncImage; // only used for mode 2 full screen 3d view
    int encWidth, encHeight, encX, encY;

   // SET IMAGE WHICH IS TO BE USED (AT ITS ORIGINAL SIZE - UNSCALED)

if (graphicMode==ATARI_SMALL) encImageSheet.loadFromFile("data/images/encounters/encounters.png"); // Atari 8bit

if ((graphicMode==ALTERNATE_LARGE) || (graphicMode==ALTERNATE_SMALL))
{
encImageSheet.loadFromFile("data/images/encounters/nobleman.png");
if (plyr.encounterRef<6) encImageSheet.loadFromFile("data/images/encounters/thief.png");
if (plyr.encounterRef==0) encImageSheet.loadFromFile("data/images/encounters/devourer.png"); // Devourer
if (plyr.encounterRef==9) encImageSheet.loadFromFile("data/images/encounters/knight.png");
if (plyr.encounterRef==10) encImageSheet.loadFromFile("data/images/encounters/guard.png");
if (plyr.encounterRef==12) encImageSheet.loadFromFile("data/images/encounters/knight.png");
if (plyr.encounterRef==14) encImageSheet.loadFromFile("data/images/encounters/wizard.png");
if (plyr.encounterRef==15) encImageSheet.loadFromFile("data/images/encounters/knight.png");
if (plyr.encounterRef==17) encImageSheet.loadFromFile("data/images/encounters/nobleman.png");
if (plyr.encounterRef==16) encImageSheet.loadFromFile("data/images/encounters/pauper2.png");
if (plyr.encounterRef==20) encImageSheet.loadFromFile("data/images/encounters/novice.png");
if (plyr.encounterRef==21) encImageSheet.loadFromFile("data/images/encounters/novice.png");
if ((plyr.encounterRef>21) && (plyr.encounterRef<26)) encImageSheet.loadFromFile("data/images/encounters/wizard.png");
if (plyr.encounterRef==35) encImageSheet.loadFromFile("data/images/encounters/rat.png");
if (plyr.encounterRef==81) encImageSheet.loadFromFile("data/images/encounters/wizard.png");
if (plyr.encounterRef==11) encImageSheet.loadFromFile("data/images/encounters/guard.png");
if (plyr.encounterRef==12) encImageSheet.loadFromFile("data/images/encounters/guard.png");
if (plyr.encounterRef==13) encImageSheet.loadFromFile("data/images/encounters/guard.png");
if (plyr.encounterRef==31) encImageSheet.loadFromFile("data/images/encounters/bat.png");
if (plyr.encounterRef==44) encImageSheet.loadFromFile("data/images/encounters/bat.png");
if (plyr.encounterRef==53) encImageSheet.loadFromFile("data/images/encounters/guard.png");
if (plyr.encounterRef==54) encImageSheet.loadFromFile("data/images/encounters/guard.png");
if (plyr.encounterRef==56) encImageSheet.loadFromFile("data/images/encounters/guard.png");
if (plyr.encounterRef==57) encImageSheet.loadFromFile("data/images/encounters/guard.png");
if ((plyr.encounterRef==58) && (plyr.gender==1)) encImageSheet.loadFromFile("data/images/encounters/doppleganger-m.png");
if ((plyr.encounterRef==58) && (plyr.gender==2)) encImageSheet.loadFromFile("data/images/encounters/doppleganger-f.png");
if (plyr.encounterRef==57) encImageSheet.loadFromFile("data/images/encounters/adventurer512.png");
if (plyr.encounterRef==76) encImageSheet.loadFromFile("data/images/encounters/guard.png");
if (plyr.encounterRef==77) encImageSheet.loadFromFile("data/images/encounters/guard.png");
if (plyr.encounterRef==29) encImageSheet.loadFromFile("data/images/encounters/slime.png");
if (plyr.encounterRef==86) encImageSheet.loadFromFile("data/images/encounters/noblewoman.png");
if (plyr.encounterRef==59) encImageSheet.loadFromFile("data/images/encounters/adventurer.png");
}

// Alternate and set animation frame as required
encImage.setTexture(encImageSheet);
encImageSheet.setSmooth(true);
if (graphicMode == ALTERNATE_LARGE) largeEncImage.setTexture(encImageSheet);
if (graphicMode == ALTERNATE_SMALL) largeEncImage.setTexture(encImageSheet);

    // Original Atari 8bit image at original size
if (graphicMode==ATARI_SMALL)
{
SetTileImage(plyr.encounterAnimationRef);
if ((plyr.encounterAnimationRef==92) && (plyr.gender==2)) { SetTileImage(72); } // female doppelganger image
// Nightstalker & dragon image position changes
//if (plyr.encounterAnimationRef==86) { encImage.setPosition(306, 168); }
//if (plyr.encounterAnimationRef==82) { encImage.setPosition(306, 176); }
}

    // NOW IMAGE CHOSEN NEED TO SET SCALE - SHRINK OR ENLARGE

    // Calculate new image width and height based on viewport size
    //encWidth = viewWidth / 4.5;
    //encHeight = viewHeight / 1.125;

    if (graphicMode==ATARI_SMALL)
{
// Scale large image based on viewport height and width
//float scaleX = float(encWidth) / float(64);
//float scaleY = float(encHeight) / float(128);
//encImage.setScale(scaleX, scaleY);
}

    if (graphicMode==ALTERNATE_SMALL)
{
    // Adjustment for new art in small window
// Scale large image based on window height and width
        encWidth = viewWidth / 4.0; // adjustment to provide larger encounter image
encHeight = encWidth*2;
//if ((plyr.encounterRef==59)||(plyr.encounterRef==86)||(plyr.encounterRef==29)) encWidth=encWidth*2;

float scaleX = float(encWidth) / float(256);
//if ((plyr.encounterRef==59) || (plyr.encounterRef==86)) scaleX = float(encWidth) / float(512);
float scaleY = float(encHeight) / float(512);
largeEncImage.setScale(scaleX, scaleY);
}

    if (graphicMode==ALTERNATE_LARGE)
{
// For fullscreen mode ensure that aspect ratio of 1:2 is maintained for encounter image
encWidth = viewWidth / 4.0; // adjustment to provide larger encounter image
encHeight = encWidth*2;
//if ((plyr.encounterRef==59)||(plyr.encounterRef==86)||(plyr.encounterRef==29)) encWidth=encWidth*2;

float scaleX = float(encWidth) / float(256);
//if ((plyr.encounterRef==59) || (plyr.encounterRef==86)) scaleX = float(encWidth) / float(512);
float scaleY = float(encHeight) / float(512);
largeEncImage.setScale(scaleX, scaleY);
}

    // SET POSITION OF RESIZED IMAGE ON SCREEN

    encX = (windowWidth - encWidth)/2;
    encY = ((viewPortY+viewHeight)-2)-encHeight;
    if ((plyr.encounterRef==59)||(plyr.encounterRef==86)) encX = (windowWidth/2)-(encWidth);

    if (graphicMode==ATARI_SMALL) { encImage.setPosition(encX,encY); }
    if ((graphicMode==ALTERNATE_SMALL) || (graphicMode==2)) { largeEncImage.setPosition(encX,encY); }

    // DRAW DISPLAY AND FINAL ENCOUNTER IMAGE
    dispMain();
if (graphicMode != ATARI_SMALL )  App.draw(largeEncImage);
if (graphicMode == ATARI_SMALL ) App.draw(encImage);
}
*/
