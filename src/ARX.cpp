#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <gflw/glfw3.h>


#include "globals.h"
#include "dev.h"
#include "config.h"

#include "game.h"
#include "player.h"
#include "saveGame.h"
#include "display.h"
#include "level.h"
#include "font.h"
#include "misc.h"
#include "actor.h"
#include "inn.h"
#include "smithy.h"
#include "tavern.h"
#include "bank.h"
#include "shop.h"
#include "items.h"
#include "audio.h"


#include "damon.h"
#include "dwarvenSmithy.h"

using namespace std;

void startGame();

bool programRunning = true;
int loadgameMenu = 0;

int main()
{
    string key;

	


    if (!loadConfig()) return 0; // load screen resolution from arx.ini
    //FreeConsole(); // Hides the console window - make ARX.ini option
	setDeveloperFlags();
	createGameWindow();
	//setScreenValues();
	plyr.fontStyle = 1;
    dispInit();
	InitFont();
	loadLogoImage();

	initSaveGameDescriptions();

	while (programRunning)
	{
		clearDisplay();
		displayMainMenu();
		updateDisplay();

		key = getSingleKey();

		if ( key=="1" )
		{
            createNewCharacter(CITY);
            startGame();
		}

		if ( key=="2" )
		{
            createNewCharacter(DUNGEON);
            startGame();
		}


		if ( key=="3" )
		{
			loadgameMenu = 255; // high level menu
			while (loadgameMenu < 256)
			{
					clearDisplay();
					displayLoadGame();
					drawText(12,0,"Load a character");
					updateDisplay();

					key = getSingleKey();
					if ( key=="0" ) { newloadCharacter(0, plyr); startGame(); loadgameMenu = 256; }
					if ( key=="1" ) { newloadCharacter(1, plyr); startGame(); loadgameMenu = 256; }
					if ( key=="2" ) { newloadCharacter(2, plyr); startGame(); loadgameMenu = 256; }
					if ( key=="3" ) { newloadCharacter(3, plyr); startGame(); loadgameMenu = 256; }
					if ( key=="4" ) { newloadCharacter(4, plyr); startGame(); loadgameMenu = 256; }
					if ( key=="5" ) { newloadCharacter(5, plyr); startGame(); loadgameMenu = 256; }
					if ( key=="6" ) { newloadCharacter(6, plyr); startGame(); loadgameMenu = 256; }
					if ( key=="7" ) { newloadCharacter(7, plyr); startGame(); loadgameMenu = 256; }
					if ( key=="8" ) { newloadCharacter(8, plyr); startGame(); loadgameMenu = 256; }
					if ( key=="9" ) { newloadCharacter(9, plyr); startGame(); loadgameMenu = 256; }
					if ( key=="ESC" ) { loadgameMenu = 256; }
			}
			key="";
		}



		if ( key=="4" ) { displayAcknowledgements(); }


		if ( key=="5" )
		{
			if (graphicMode==0) { graphicMode=1; dispInit();}
			else
			if (graphicMode==1) { graphicMode=2; createGameWindow();dispInit();}
			else
			if (graphicMode==2) { graphicMode=0; createGameWindow();dispInit();}

		}

		if ( key=="6" )
		{
			if (plyr.musicStyle==0) { plyr.musicStyle=1;}
			else { plyr.musicStyle=0; }
		}


		if ( key=="7" )
		{
			if (plyr.fontStyle==0) { plyr.fontStyle=1;}
			else { plyr.fontStyle=0;}
			InitFont();
		}

		if ( key=="0" ) { programRunning = false; }
		if ( key=="QUIT" ) { programRunning = false; }
	}

	return EXIT_SUCCESS;
}


void startGame()
{
    displayLoading(); // display loading message on screen
	loadResources(); // load up images for textures and backdrops once the graphic style has been selected
	initEncounterThemes();
	initMaps();
	loadMapData(plyr.map);
	loadDescriptions(plyr.map);
	loadZoneData(plyr.map);
	loadMessages(plyr.map);
	//loadEncounters();

	// Load monsters
	loadMonstersBinary();
	initialiseMonsterOffsets();
	convertMonstersBinary();

	// Load shop item binary files

	loadDamonBinary();              // Load weapons, armour and clothing for the Damon & Pythias
    loadCitySmithyBinary();         // Load weapons and armour for the City Smithies
    loadDwarvenBinary();            // Load weapons and armour for the Dwarven Smithy

	checkBackgroundTime(); // determine background graphics based on time of day

	checkDailyInnJobOpenings();
	checkDailyTavernJobOpenings();
	checkDailybankJobOpenings();

    initialiseNewGame(); // Only sound and graphic resources
    plyr.status = EXPLORE;

	int initialIndex = 10;

	UpdateShopStock();
    //clearInventory();           // Set the inventory char array to zeros

	readFTData();

    // Not in use yet - dungeonItems()
    //loadDungeonItems();         // Sets up the Dungeon items char array

    gameLoop(); // Enter the main game loop
}


