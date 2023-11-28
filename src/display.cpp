#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "globals.h"
#include "player.h"
#include "display.h"
#include "3Dview.h"
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

using namespace std;
using namespace sf;


sf::Sprite encImage;

string version = "0.84.0";

int windowMode, graphicMode, windowWidth, windowHeight, viewWidth, viewHeight;
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

extern string roomMessages[255];
extern string descriptions[255];

//extern buffer_item itemBuffer[100];


// Main window
sf::RenderWindow App;

bool mainMenuQuit = false;

sf::Texture img0,img1,img2,img3,img4,img5,img6,img7,img8,img9, imgDungeonGate, imgCityGate;
sf::Texture imgc0,imgc1,imgc2,imgc3,imgc4,imgc5,imgc6,imgc7,imgc8,imgc9;
sf::Texture consoleImage, BannerImageCity, BannerImageStrip;
sf::Texture compassN,compassS,compassW,compassE;

sf::Sprite Banner, BannerStrip;
sf::Sprite counterImage;
sf::Sprite dungeonGate, cityGate;
sf::Sprite compass;
sf::Sprite ShopSprite, LogoSprite;
sf::Texture ShopImage, LogoImage;
string olddrawText; // text string used for setting and passing strings to the print routine


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
sf::Texture encImageSheet;
int firstFrame;
int lastFrame;
int currentFrame;   // within encounterAnim 0-7
int yOffset;
int xOffset;
int animImage;
int animDuration;



// drawAtariAnimation - draws single frame and updates counter

void drawAtariAnimation()
{
    if (animationNotStarted)
    {
        currentFrame = firstFrame;
        yOffset = encounterAnim[currentFrame].yOffset;
        xOffset = encounterAnim[currentFrame].xOffset;
        animImage = encounterAnim[currentFrame].image;
        animDuration = encounterAnim[currentFrame].duration;
        animationNotStarted = false;
    }

    if (animDuration == 0)
    {
        currentFrame++;
        if (currentFrame == (lastFrame+1)) { currentFrame = firstFrame; }
        yOffset = encounterAnim[currentFrame].yOffset;
        xOffset = encounterAnim[currentFrame].xOffset;
        animImage = encounterAnim[currentFrame].image;
        animDuration = encounterAnim[currentFrame].duration;
    }

    animDuration--;



	//sf::Sprite largeEncImage; // only used for mode 2 full screen 3d view
    int encWidth, encHeight, encX, encY;

	// Alternate and set animation frame as required
	encImage.setTexture(encImageSheet);
	//encImageSheet.setSmooth(true);
    // Original Atari 8bit image at original size

    SetTileImage(animImage);
    //if ((plyr.encounterAnimationRef==92) && (plyr.gender==2)) { SetTileImage(72); } // female doppelganger image

    // Calculate new image width and height based on viewport size
    encWidth = viewWidth / 4.5;
    encHeight = viewHeight / 1.125;

    /*
    if (graphicMode==ATARI_SMALL)
	{
		// Scale large image based on viewport height and width
		float scaleX = float(encWidth) / float(64);
		float scaleY = float(encHeight) / float(128);
		encImage.setScale(scaleX, scaleY);
	}

    */


    /* SET POSITION OF RESIZED IMAGE ON SCREEN */

    //encX = (windowWidth - encWidth)/2;
    //encY = ((viewPortY+viewHeight)-2)-encHeight;
    //if ((plyr.encounterRef==59)||(plyr.encounterRef==86)) encX = (windowWidth/2)-(encWidth);
    encX = (windowWidth/2)-32;
    encY = (viewPortY+viewHeight)-130;

    //encImage.setPosition(encX+(xOffset/2),encY+(yOffset/2));
    if ((xOffset==0) && (yOffset==0)) {  encImage.setPosition(encX,encY); }
    else { encImage.setPosition((viewPortX-32+(xOffset)),(viewPortY+(yOffset*2))); }

    // DRAW DISPLAY AND FINAL ENCOUNTER IMAGE
    dispMain();

//	if (graphicMode == ATARI_SMALL ) 
	App.draw(encImage);
}


void createGameWindow()
{
    string title = "Alternate Reality X " + version;

	if (windowMode == 0) { App.create(sf::VideoMode(windowWidth, windowHeight), title,sf::Style::Close); }
	else { App.create(sf::VideoMode(windowWidth, windowHeight), title, sf::Style::Fullscreen); }

	// Print OpenGL settings to game console for information
	sf::ContextSettings settings = App.getSettings();
	std::cout << "Welcome to Alternate Reality X " << version << " ..." << std::endl << std::endl;
	std::cout << "OpenGL Settings:\n\n";
	std::cout << "Depth bits:           " << settings.depthBits << std::endl;
	std::cout << "Stencil bits:         " << settings.stencilBits << std::endl;
	std::cout << "Anti-aliasing level:  " << settings.antialiasingLevel << std::endl;
	std::cout << "OpenGL Version:       " << settings.majorVersion << "." << settings.minorVersion << std::endl << std::endl;
	std::cout << "Window Size:          " << windowWidth << " x " << windowHeight << std::endl << std::endl << std::endl;

	// Limit the framerate to 60 frames per second (this step is optional)
	App.setFramerateLimit(60);
}


void drawConsoleBackground()
{
    /* Draws a transparent box with yellow border around the console window whilst exploring and in large 3D view mode */
    if ((plyr.status!=2) && (graphicMode== ALTERNATE_LARGE)) // Whilst not shopping
	{
		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(670, 182)); // 672, 184
		rectangle.setOutlineColor(sf::Color::Yellow);
		rectangle.setFillColor(sf::Color(0, 0, 0, 128));
		rectangle.setOutlineThickness(1);
		rectangle.setPosition(consoleX-16, consoleY); // Offset to give a 16 pixel border to text
		App.draw(rectangle);
	}
}


void setScreenValues()
{
    // Determines screen element locations based on window dimensions
    gateX = (windowWidth-640)/2;
    gateY = ((windowHeight-384)/2)-78;
    loadingX = 16;
    loadingY = 11;
    int temp = 0;
    int spacer = 0; // spacer value between screen elements - adjust here
    int consoleHeight = (16+2)*10; // How tall is 10 lines of console text (16 pixels with 2 pixel space between lines)
    statPanelX = (windowWidth-640)/2; // Centre in middle of window width
    consoleX = (windowWidth-640)/2; // Centre in middle of window width

    /* Original small 3D view */
	if (graphicMode< ALTERNATE_LARGE)
	{
		//viewWidth = windowWidth/2.22;
		//viewHeight = viewWidth/2;
		viewWidth = 288;
		viewHeight = 144;
		viewPortX = (windowWidth-viewWidth)/2;
		temp = 110 + spacer + viewHeight + spacer + consoleHeight;
        statPanelY = (windowHeight - temp) / 2;
        viewPortY = statPanelY +110+spacer;
        consoleY = viewPortY + viewHeight+4;
        miniMapX = windowWidth - (((((windowWidth-viewWidth)/2)-144)/2)+144);
        miniMapY = spacer+viewPortY+((viewHeight-144)/2);
	}

    /* New large 3D view */
	if (graphicMode== ALTERNATE_LARGE)
	{
		viewWidth = windowWidth;
		viewHeight = windowHeight;
		viewPortX = 0;
		viewPortY = windowHeight-viewHeight;
        statPanelY = 16;
        consoleY = (windowHeight - 32) - 144; // 16 x 10 not quite right
        miniMapX = (windowWidth-16)-176;
        miniMapY = (windowHeight-176)/2;
	}

    /* Shop positions are the same regardless of small / large 3D view choice */
    temp = 110 + spacer + 144 + 16 + consoleHeight;
    shopStatsY = ((windowHeight - temp) /2);
    shopStatsY = statPanelY;
    shopPictureY = shopStatsY + 110 + spacer;
    shopConsoleY = shopPictureY + 144 + spacer; // 16 to put space between image and console text

    /* Misc assignments for positioning */

    lyricX = (windowWidth - 640)/2;
    lyricY = shopPictureY-18;

}


void dispInit()
{
    setScreenValues();
    /* Set up window based on choice of display option NOT screen resolution */
    //setScreenValues(); // Determine positions of screen elements from array
	glEnable(GL_TEXTURE_2D); // enable texture mapping
	glShadeModel(GL_SMOOTH); // Enable Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Set color and depth clear value
    glClearDepth(1.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    /* Original small 3D view */
	if (graphicMode < ALTERNATE_LARGE)
	{
		gluPerspective(45.0f,(GLfloat)viewWidth/(GLfloat)viewHeight,0.1f,100.0f);
		int z = windowHeight - (viewPortY+viewHeight);
		glViewport (viewPortX, z, viewWidth, viewHeight);
		glTranslatef(0.0f,0.0f,-1.0f); // -2.4f  - move x units into the screen. was 1.0
	}
	else
	{
		gluPerspective(45.0f,(GLfloat)viewWidth/(GLfloat)viewHeight,0.1f,100.0f);
		//glViewport (0, windowHeight, windowWidth, windowHeight);
		glTranslatef(0.0f,0.0f,-1.2f); // -2.4f  - move x units into the screen. was 1.0
	}

	glMatrixMode(GL_MODELVIEW);
}


void clearDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	App.pushGLStates();
}


void updateDisplay()
{
	App.popGLStates();
	App.display();
}


void displayLoading()
{
    clearDisplay();
    drawText(loadingX,loadingY,"Loading...");
    updateDisplay();
}


void displayMainMenu()
{
	srand ( time(NULL) );
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
	// All non OpenGL drawing should be performed between the pushGLStates and popGLStates commands
    // Need to determine here if 3D view or shop image

    draw3DView();
    drawStatsPanel();
    drawCompass();
    drawAutomap();
    if ((graphicMode==ALTERNATE_LARGE) && (plyr.status != 3 )) drawConsoleBackground();
    //updateWeatherDisplay();
    //updateEncounterAnimation();
    //drawInfoPanels(); moved out of dispMain so as not to overlap with USE or ENCOUNTER text in bottom half of display
    //App.popGLStates() and App.Display() will be carried out later once encounter or use object text added to display

    // App.popGLStates will follow other SFML 2D drawing
}


void drawImage(string imagename, int x, int y)
{
	// Counter images for Dungeon gate character creation
	if (plyr.scenario==1)
	{
	if (imagename=="0") counterImage.setTexture(img0);
	if (imagename=="1") counterImage.setTexture(img1);
	if (imagename=="2") counterImage.setTexture(img2);
	if (imagename=="3") counterImage.setTexture(img3);
	if (imagename=="4") counterImage.setTexture(img4);
	if (imagename=="5") counterImage.setTexture(img5);
	if (imagename=="6") counterImage.setTexture(img6);
	if (imagename=="7") counterImage.setTexture(img7);
	if (imagename=="8") counterImage.setTexture(img8);
	if (imagename=="9") counterImage.setTexture(img9);
	}
	else
	{
	if (imagename=="0") counterImage.setTexture(imgc0);
	if (imagename=="1") counterImage.setTexture(imgc1);
	if (imagename=="2") counterImage.setTexture(imgc2);
	if (imagename=="3") counterImage.setTexture(imgc3);
	if (imagename=="4") counterImage.setTexture(imgc4);
	if (imagename=="5") counterImage.setTexture(imgc5);
	if (imagename=="6") counterImage.setTexture(imgc6);
	if (imagename=="7") counterImage.setTexture(imgc7);
	if (imagename=="8") counterImage.setTexture(imgc8);
	if (imagename=="9") counterImage.setTexture(imgc9);
	}

	counterImage.setPosition(gateX+x, gateY+y);
	App.draw(counterImage);

}


void drawCompass()
{
	if (plyr.compasses > 0)
	{
        if ((plyr.status!=2) && (graphicMode== ALTERNATE_LARGE)) // if exploring and full screen draw a background
        {
            int x = 16;
            int y = (windowHeight-128)/2;

            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(130, 130));
            rectangle.setOutlineColor(sf::Color::Yellow);
            rectangle.setFillColor(sf::Color(0, 0, 0, 128));
            rectangle.setOutlineThickness(1);
            rectangle.setPosition(x, y); // Offset to give a 16 pixel border to text
            App.draw(rectangle);
        }

		if (plyr.facing == WEST) { compass.setTexture(compassW); }
		if (plyr.facing == NORTH) { compass.setTexture(compassN); }
		if (plyr.facing == EAST) { compass.setTexture(compassE); }
		if (plyr.facing == SOUTH) { compass.setTexture(compassS); }

		if (graphicMode== ALTERNATE_LARGE)
		{
			int x = 16;
			int y = (windowHeight-128)/2;
			compass.setPosition(x, y);
		}
		else 	if (graphicMode == A16BIT_SMALL)
		{
			 int x = (viewPortX-78)/2;
			int y = viewPortY+((viewHeight-78)/2);
			compass.setPosition(x, y);
		} else 
		{
		    /* Normal Small 3D view mode */
			 int x = (viewPortX-128)/2;
			int y = viewPortY+((viewHeight-128)/2);
			compass.setPosition(x, y);
		}
		App.draw(compass);
	}
}


void loadResources()
{
	loadBackgroundNames();
	loadTextureNames();
	initTextures();
	initLyricFont();

	loadCounterImages();

	if (graphicMode != A16BIT_SMALL)
	{
		compassN.loadFromFile("data/images/compass_n.png");
		compassS.loadFromFile("data/images/compass_s.png");
		compassW.loadFromFile("data/images/compass_w.png");
		compassE.loadFromFile("data/images/compass_e.png");
	} else {
		compassN.loadFromFile("data/images/compass_n_16bit.png");
		compassS.loadFromFile("data/images/compass_s_16bit.png");
		compassW.loadFromFile("data/images/compass_w_16bit.png");
		compassE.loadFromFile("data/images/compass_e_16bit.png");
	}

	// Create a sprite for the stat banner
	BannerImageCity.loadFromFile("data/images/cityBanner.png");
	Banner.setTexture(BannerImageCity);
	BannerImageStrip.loadFromFile("data/images/cityBannerStatusLine.png");
	BannerStrip.setTexture(BannerImageStrip);
	//sf::Texture consoleImage, BannerImageCity, BannerImageStrip;
    //sf::Texture compassN,compassS,compassW,compassE;
    //sf::Sprite Banner, BannerStrip;

    // Load Atari 8 bit encounter images sheet
std::cout << "Loading encounters Texture" << std::endl;
    if (graphicMode== ATARI_SMALL)
	    encImageSheet.loadFromFile("data/images/encounters/encounters.png"); // Atari 8bit
    else     if (graphicMode== A16BIT_SMALL)
		encImageSheet.loadFromFile("data/images/encounters/encounters_16bit.png"); // Aminga 16bit
    else
		encImageSheet.loadFromFile("data/images/encounters/encounters_alternate.png"); // Alternate
}


void loadCounterImages()
{
	// Dungeon gate counters
	img0.loadFromFile("data/images/0.png");
	img1.loadFromFile("data/images/1.png");
	img2.loadFromFile("data/images/2.png");
	img3.loadFromFile("data/images/3.png");
	img4.loadFromFile("data/images/4.png");
	img5.loadFromFile("data/images/5.png");
	img6.loadFromFile("data/images/6.png");
	img7.loadFromFile("data/images/7.png");
	img8.loadFromFile("data/images/8.png");
	img9.loadFromFile("data/images/9.png");

	//City gate counters
	imgc0.loadFromFile("data/images/c0.png");
	imgc1.loadFromFile("data/images/c1.png");
	imgc2.loadFromFile("data/images/c2.png");
	imgc3.loadFromFile("data/images/c3.png");
	imgc4.loadFromFile("data/images/c4.png");
	imgc5.loadFromFile("data/images/c5.png");
	imgc6.loadFromFile("data/images/c6.png");
	imgc7.loadFromFile("data/images/c7.png");
	imgc8.loadFromFile("data/images/c8.png");
	imgc9.loadFromFile("data/images/c9.png");

	//imgDungeonGate.loadFromFile("data/images/gat.png");
	imgDungeonGate.loadFromFile("data/images/locations2/gate3.png");
	imgDungeonGate.setSmooth(false);
	imgCityGate.loadFromFile("data/images/cityGate.png");
	imgCityGate.setSmooth(false);

}


void displayDungeonGateImage()
{
	dungeonGate.setTexture(imgDungeonGate);
	dungeonGate.setPosition(gateX, gateY+64);
	App.draw(dungeonGate);
}


void displayCityGateImage()
{
	cityGate.setTexture(imgCityGate);
	cityGate.setPosition(gateX, gateY+78);
	App.draw(cityGate);
}


int checkCityDoors() // currently only forward!!!
{
	return 0;
}


void drawInfoPanels()
{

//if (graphicMode==2) drawConsoleBackground();

if (plyr.status!=3)
{
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
        //if (plyr.protection1 != 0) { cyText(y,"Protection +1"); y++; }
        //if (plyr.protection2 != 0) { cyText(y,"Protection +2"); y++; }
        //if (plyr.invulnerability[0] != 0)  { cyText(y,"Invulnerability Blunt"); y++; }
        //if (plyr.invulnerability[1] != 0)  { cyText(y,"Invulnerability Sharp"); y++; }
        //if (plyr.invulnerability[2] != 0)  { cyText(y,"Invulnerability Earth"); y++; }
        //if (plyr.invulnerability[3] != 0)  { cyText(y,"Invulnerability Air"); y++; }
        //if (plyr.invulnerability[4] != 0)  { cyText(y,"Invulnerability Fire"); y++; }
        //if (plyr.invulnerability[5] != 0)  { cyText(y,"Invulnerability Water"); y++; }
        //if (plyr.invulnerability[6] != 0)  { cyText(y,"Invulnerability Power"); y++; }
        //if (plyr.invulnerability[7] != 0)  { cyText(y,"Invulnerability Mental"); y++; }
        //if (plyr.invulnerability[8] != 0)  { cyText(y,"Invulnerability Cleric"); y++; }
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

	//if (plyr.status==3) App.clear(sf::Color(0,0,0,192)); // if in combat wipe the panel
}
}


void clearShopDisplay()
{
	//App.clear(sf::Color::Black);
	App.clear();
	App.pushGLStates();
	App.draw(ShopSprite);
	drawStatsPanel();
}


void loadShopImage(int imageno)
{
    //consoleY = ((windowHeight-144)/2)+144+16;
    //cout << consoleY;
    if (graphicMode == ATARI_SMALL)
    {
        if (imageno==1) { ShopImage.loadFromFile("data/images/locations/retreat.png"); }
        if (imageno==2) { ShopImage.loadFromFile("data/images/locations/rathskeller.png"); }
        if (imageno==3) { ShopImage.loadFromFile("data/images/locations/oDamon.png"); }
        if (imageno==4) { ShopImage.loadFromFile("data/images/locations/evilGuild.png"); }
        if (imageno==5) { ShopImage.loadFromFile("data/images/locations/goodGuild.png"); }
        if (imageno==6) { ShopImage.loadFromFile("data/images/locations/stairwayUp.png"); }
        if (imageno==7) { ShopImage.loadFromFile("data/images/locations/stairwayDown.png"); }
        if (imageno==8) { ShopImage.loadFromFile("data/images/locations/citySmithyNight.png"); }
        if (imageno==9) { ShopImage.loadFromFile("data/images/locations/imgCitySmithy.png"); }
        if (imageno==10) { ShopImage.loadFromFile("data/images/locations/imgCityTavern.png"); }
        if (imageno==11) { ShopImage.loadFromFile("data/images/locations/imgCityInn.png"); }
        if (imageno==12) { ShopImage.loadFromFile("data/images/locations/imgCityShop.png"); }
        if (imageno==13) { ShopImage.loadFromFile("data/images/locations/imgCityBank.png"); }
        if (imageno==14) { ShopImage.loadFromFile("data/images/locations/imgCityGuild.png"); }
        if (imageno==15) { ShopImage.loadFromFile("data/images/locations/imgCityHealer.png"); }
        if (imageno==16) { ShopImage.loadFromFile("data/images/locations/trolls.png"); }
        if (imageno==17) { ShopImage.loadFromFile("data/images/locations/goblins.png"); }
        if (imageno==18) { ShopImage.loadFromFile("data/images/locations/chapel.png"); }
        if (imageno==19) { ShopImage.loadFromFile("data/images/locations/fountain.png"); }
        if (imageno==20) { ShopImage.loadFromFile("data/images/locations/oracle.png"); }
        if (imageno==21) { ShopImage.loadFromFile("data/images/locations/imgCityHealer.png"); }
        if (imageno==22) { ShopImage.loadFromFile("data/images/locations/lift.png"); }
        if (imageno==23) { ShopImage.loadFromFile("data/images/locations/ferry.png"); }
        if (imageno==24) { ShopImage.loadFromFile("data/images/locations/undead.png"); }
        if (imageno==25) { ShopImage.loadFromFile("data/images/locations/arena.png"); }
        if (imageno==26) { ShopImage.loadFromFile("data/images/locations/dwarvenSmithy.png"); }
        if (imageno==27) { ShopImage.loadFromFile("data/images/locations/6.png"); }
    }
    if (graphicMode > ATARI_SMALL)
    {
        if (imageno==9) { ShopImage.loadFromFile("data/images/locations2/smithy.png"); }
        if (imageno==10) { ShopImage.loadFromFile("data/images/locations2/tavern.png"); }
        if (imageno==11) { ShopImage.loadFromFile("data/images/locations2/inn.png"); }
        if (imageno==12) { ShopImage.loadFromFile("data/images/locations2/shop.png"); }
        if (imageno==13) { ShopImage.loadFromFile("data/images/locations2/bank.png"); }
        if (imageno==14) { ShopImage.loadFromFile("data/images/locations2/guild.png"); }
        if (imageno==15) { ShopImage.loadFromFile("data/images/locations2/healer1.png"); }
        if (imageno==16) { ShopImage.loadFromFile("data/images/locations2/trolls.png"); }
        if (imageno==17) { ShopImage.loadFromFile("data/images/locations2/goblins.png"); }
        if (imageno==1) { ShopImage.loadFromFile("data/images/locations2/inn.png"); }
        if (imageno==2) { ShopImage.loadFromFile("data/images/locations2/rathskeller.png"); }
        if (imageno==3) { ShopImage.loadFromFile("data/images/locations2/shop.png"); }
        if (imageno==4) { ShopImage.loadFromFile("data/images/locations2/guild.png"); }
        if (imageno==5) { ShopImage.loadFromFile("data/images/locations2/guild.png"); }
        if (imageno==6) { ShopImage.loadFromFile("data/images/locations/stairwayUp.png"); }
        if (imageno==7) { ShopImage.loadFromFile("data/images/locations/stairwayDown.png"); }
        if (imageno==8) { ShopImage.loadFromFile("data/images/locations/citySmithyNight.png"); }
        if (imageno==18) { ShopImage.loadFromFile("data/images/locations/chapel.png"); }
        if (imageno==19) { ShopImage.loadFromFile("data/images/locations/fountain.png"); }
        if (imageno==20) { ShopImage.loadFromFile("data/images/locations/oracle.png"); }
        if (imageno==21) { ShopImage.loadFromFile("data/images/locations2/healer2.png"); }
        if (imageno==22) { ShopImage.loadFromFile("data/images/locations/lift.png"); }
        if (imageno==23) { ShopImage.loadFromFile("data/images/locations2/river.png"); }
        if (imageno==24) { ShopImage.loadFromFile("data/images/locations2/undead.png"); }
        if (imageno==25) { ShopImage.loadFromFile("data/images/locations/arena.png"); }
    }


	ShopSprite.setTexture(ShopImage);
	ShopSprite.setScale(2.0, 2.0);
	ShopSprite.setPosition(((windowWidth-640)/2),shopPictureY);

}


void drawStatsPanel()
{
	if ((graphicMode== ALTERNATE_LARGE) && (plyr.status!=5) && (plyr.status!=2)) // not shopping
	{
		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(640, 110)); // 640, 110
		rectangle.setOutlineColor(sf::Color::Yellow);
		//rectangle.setFillColor(sf::Color(255, 255, 255, 0));
		rectangle.setOutlineThickness(1);
		rectangle.setPosition(statPanelX, statPanelY-1);
		App.draw(rectangle);
	}

    Banner.setPosition(statPanelX,statPanelY-1);
    if (plyr.status==2)
    {
        int statsX = (windowWidth - 640)/2;
        int statsY = ((windowHeight-144)/2)-126; // 144 pixels for picture + 16 space + stats height
        Banner.setPosition(statsX,shopStatsY-1);
    }
    App.draw(Banner);
    if ((plyr.status==1)||(plyr.status==3)) { BannerStrip.setPosition(statPanelX,(statPanelY+89)); App.draw(BannerStrip); }
    if (plyr.status != MODULE) App.draw(BannerStrip);
	if (!plyr.diagOn)
	{
		SetFontColour(162, 114, 64, 255);
		drawText(2, 0, plyr.name);
		drawText(32,0,"Level:");
		drawText(38,0,plyr.level);
		SetFontColour(147, 69, 130, 255);
		drawText(2,1,"Stats:  STA  CHR  STR  INT  WIS  SKL");
		SetFontColour(138,68,158, 255);
		drawText(11,2,plyr.sta);
		drawText(16,2,plyr.chr);
		drawText(21,2,plyr.str);
		drawText(26,2,plyr.inte);
		drawText(31,2,plyr.wis);
		drawText(36,2,plyr.skl);

		SetFontColour(62,106,162, 255);
		drawText(2,3,"Experience:");
		drawText(14,3,plyr.xp);
		if (plyr.hp < 0)
			{
			   drawText(24,3,"Hit Points: !!!!!");
			}
		if (plyr.hp == plyr.maxhp)
			{
				drawText(25,3,"Hit Points=");
				drawText(36,3,plyr.hp);
			}
		if ((plyr.hp < plyr.maxhp) && (plyr.hp > -1))
			{
				drawText(25,3,"Hit Points:");
				drawText(36,3,plyr.hp);
			}

		std::string str;
		str = "You are "+ descriptions[plyr.location];

		// Draw status line text
		checkForItemsHere();

		SetFontColour(102,149,40, 255);

		if ( (plyr.status_text != " ") && (plyr.status != 3) && (plyr.alive) )
		{
		    //App.draw(BannerStrip);
			drawText(2,5,plyr.status_text);
        }

		SetFontColour(102,149,40, 255);
		if (plyr.alive) { drawText(2,4,str); }
		else { drawText(12,4,"$ Where are you? $"); }

		SetFontColour(215, 215, 215, 255); // set text colour to white for all other text
	}

	// Diag Text

	if (plyr.diagOn)
	{
		string zoneDesc = "X:"+itos(plyr.x)+"  Y:"+itos(plyr.y)+"  Special:"+itos(plyr.special)+"  Zone:"+itos(plyr.zone)+"  Set:"+itos(plyr.zoneSet);
		drawText(2,0,zoneDesc);
		zoneDesc = "Front:"+itos(plyr.front)+"  Left:"+itos(plyr.left)+"  Right:"+itos(plyr.right) +"  Back:"+itos(plyr.back);
		drawText(2,1,zoneDesc);
		std::string text;
		std::stringstream out;
		out << "Offset:" << plyr.z_offset;
		text = out.str();
		drawText(2,5,text);
		zoneDesc = "Floor:"+itos(zones[plyr.zoneSet].floor) + "  Ceiling:"+itos(zones[plyr.zoneSet].ceiling);
		drawText(2,2,zoneDesc);
		zoneDesc = "Location:"+itos(plyr.location);
		drawText(2,3,zoneDesc);
		int e = returnCarriedWeight();
		zoneDesc = "Encumbrance:"+itos(e);
		drawText(2,4,zoneDesc);
		zoneDesc = "T: " + itos(plyr.hours) + ":" + itos(plyr.minutes);
		drawText(30,4,zoneDesc);
		//zoneDesc = "Buffer Ref:"+itos(plyr.buffer_index);
		//drawText(2,2,zoneDesc);
	}
	//string hourDesc = "th";
	//str = "It is " + itos(plyr.minutes) + " minutes past the " + itos(plyr.hours) + hourDesc + " hour";
	//cyText(10,str);

	/*
	std::string text;
	std::stringstream out;
	out << plyr.z_offset;
	text = out.str();
	drawText(22,6,text);
	//out << plyr.front;
	//text = out.str();
	//drawText(35,6,text);
	*/

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


void SetTileImage(int tile_no)
{

	int row, column;
	int tilesPerRow = 8; // number of tiles per row in source image containing all tiles (8 default)
	//int tilesPerColumn = 8; // number of tiles per column in source image (8 default)

	//Select 64x128 section of tile sheet for tile

	if ( tile_no >= tilesPerRow)
	{
		column = (tile_no % tilesPerRow); // remainder
		row = ((tile_no-column)/tilesPerRow);
	}
	else
	{
		column = tile_no;
		row = 0; // = row 1 on the actual tile sheet at y=0
	}

	int tileX = (column)*64;	// x loc on tiles image in pixels
	int tileY = (row)*128;		// y loc on tiles image in pixels

	encImage.setTextureRect(sf::IntRect(tileX, tileY , 64, 128));
	//cout << "Tile X:" << tileX << "  ,  " << "Tile Y:" << tileY << "\n";

	// Nightstalker & Dragon exceptions due to larger image size
	//if (tile_no==86) { encImage.setTextureRect(sf::IntRect(tileX, tileY , 72, 136)); }
	//if (tile_no==82) { encImage.setTextureRect(sf::IntRect(tileX, tileY , 72, 128)); }

}


void loadLogoImage()
{
    int x,y;
    LogoImage.loadFromFile("data/images/logo640x240.png");
    LogoSprite.setTexture(LogoImage);
    x = (windowWidth - 640)/2;
    y = (windowHeight-(180+240))/2;
    LogoSprite.setPosition(x,y);

}


void drawLogo()
{
    App.draw(LogoSprite);
}


// Disabled large and small_alternate options for release 0.75

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


