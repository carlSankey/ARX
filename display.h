
#ifndef _displayh 
#define _displayh 

#include <string>

// SFML REMOVED: Was using sf::RenderWindow for rendering
// SFML REMOVED: Was using SFML graphics for sprite/texture rendering
#include "platform/PlatformWindow.h"

using std::string;

void createGameWindow();

void SetTileImage(int tile_no);
void drawAtariAnimation();
//void drawEncounterAnimation();

void loadResources();
void displayQuitMenu();
void displayOptionsMenu();
void drawShopImage(int imageno);
void displayDungeonGateImage();
void displayCityGateImage();
void loadCounterImages(); // load counter images for Dungeon character creation
float getGateScale();     // uniform scale applied to gate image
float getGateOffsetX();   // horizontal letterbox offset for gate image
float getGateOffsetY();   // vertical letterbox offset for gate and counter alignment
void dispInit();

void drawConsoleBackground();
void displayLoading();
void displayMainMenu();
void displayAcknowledgements();

void displayError();


void clearDisplay(); // clear display prior to displaying non openGL scene

void updateDisplay();
void clearDisplay(); // clear display prior to displaying non openGL scene
void updateDisplay();

void dispMain();
//void draw3DBackground();
void drawImage(std::string imagename, int x, int y); // draw image on screen at x,y
void drawText(int x, int y, std::string txt);
void initGL();
void setScreenValues();


void drawInfoPanels();
void drawStatsPanel();

void drawCompass();
void flashView();

void clearShopDisplay();
void loadShopImage(int imageno);


int checkCityDoors();

void loadLogoImage();
void drawLogo();
void shutdownDisplay();  // release sprites and close SFML window before process exit
void draw3DView();
void loadBackgroundNames();
void loadTextureNames();
void initTextures();
void flashView();

// SFML REMOVED: sf::RenderWindow was used for main render window
extern int graphicMode;
extern int windowWidth, windowHeight;
extern int viewWidth, viewHeight, viewPortX, viewPortY;
extern bool animationNotStarted;
extern int firstFrame;
extern int lastFrame;
extern float uiScale; // global UI scale factor

 
// Your existing header content goes here 
 
#endif // _displayh 

