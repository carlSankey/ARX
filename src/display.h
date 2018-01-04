#include <SFML/Graphics.hpp>

#include <string>

using namespace std;

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
void dispInit();

void drawConsoleBackground();
void displayLoading();
void displayMainMenu();
void displayAcknowledgements();


void clearDisplay(); // clear display prior to displaying non openGL scene

void updateDisplay();
void clearDisplay(); // clear display prior to displaying non openGL scene
void updateDisplay();

void dispMain();
//void draw3DBackground();
void drawImage(string imagename, int x, int y); // draw image on screen at x,y
void drawText(int x, int y, string txt);
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

extern sf::RenderWindow App;
extern int graphicMode;
extern int viewWidth, viewHeight, viewPortX, viewPortY;
extern bool animationNotStarted;
extern int firstFrame;
extern int lastFrame;
