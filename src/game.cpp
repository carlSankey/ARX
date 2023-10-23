

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <iostream>

#include "game.h"
#include "globals.h"

#include "dev.h"
#include "player.h"
#include "font.h"
#include "display.h"
#include "level.h"
#include "misc.h"
#include "encounter.h"
#include "items.h"
#include "spells.h"
#include "module.h"

#include "vaults.h"
#include "damon.h"
#include "guild.h"
#include "bank.h"
#include "tavern.h"
#include "smithy.h"
#include "inn.h"
#include "shop.h"
#include "healer.h"
#include "goblins.h"
#include "trolls.h"
#include "chapel.h"
#include "oracle.h"
#include "fountain.h"
#include "ferry.h"
#include "undeadKing.h"
#include "staircase.h"
#include "lift.h"
#include "automap.h"
#include "lyrics.h"
#include "arena.h"
#include "actor.h"
#include "saveGame.h"
#include "input.h"

sf::Time dt;
sf::Time encounterCheckTime;

extern sf::Clock        clock1;
extern int              iCounter;

sf::Clock myclock;
float Framerate;

extern bool autoMapExplored[5][4096];
extern string mess[25];
extern string room_descriptions[203];
extern Mapcell levelmap[4096]; // 12288

extern Teleport teleports[32];

sf::SoundBuffer doorCityBuffer;
sf::SoundBuffer secretCityBuffer;
sf::SoundBuffer secretDungeonBuffer;
sf::SoundBuffer doorDungeonBuffer;

sf::Sound cityDoorSound;
sf::Sound citySecretSound;
sf::Sound dungeonDoorSound;
sf::Sound dungeonSecretSound;

sf::SoundBuffer smithyBuffer;
sf::Sound smithySound;
bool smithyPlaying;
sf::Music deathMusic;
bool gameQuit;

int teleColour = 1;
sf::Texture teleBlack,teleGold;
sf::Sprite tBackground;

bool Running;


// Main game loop (excluding main menu)

void gameLoop()
{
	Running = true;
    gameQuit = false;

	bool smithyPlaying = false;
	int timer=0;

    while (Running)
    {
		while ((Running) && (plyr.alive))
		{
		    dt = myclock.restart();

		if (plyr.scenario == 1) {checkTeleport();}

		if (plyr.hp<0) { plyr.alive=false; }

		checkShop();
			 
			 
		/* Update player loc details */

		int ind = getMapIndex(plyr.x,plyr.y);
		autoMapExplored[plyr.map][ind] = true;
		transMapIndex(ind);
		plyr.special = levelmap[ind].special;
		plyr.location = levelmap[ind].location;
		setCurrentZone();
		if ((plyr.scenario == DUNGEON) && ((plyr.zone == 17) || (plyr.zone == 16)))  // in the well lit area
		{
			plyr.darkness = 0;
		}
		else 
		{
			plyr.darkness = 1;
		}

	    if (plyr.scenario==1) checkFixedEncounters();
		if (plyr.scenario==1) checkFixedTreasures();
		checkForItemsHere();


        dispMain();
        drawInfoPanels(); // Displayed here so as not to overwrite text from USE or an ENCOUNTER
		updateDisplay();

		    encounterCheckTime += dt;
		    if (encounterCheckTime >= sf::seconds(4.8f)) // 4.8f// was 0.8f
          {
				plyr.status_text = " ";
              checkEncounter();
              encounterCheckTime = sf::Time::Zero;
              addMinute();
          }


		if ( (plyr.special==1000) && (!smithyPlaying) ) { smithySound.play(); smithyPlaying = true; }
		if ( (smithyPlaying) && (plyr.special!=1000) ) { smithySound.stop(); smithyPlaying = false; }


		string key = readKey();
		if ( key=="left" ) { turnLeft(); }
		if ( key=="right" ) { turnRight(); }
		if ( key=="up" ) { moveForward(); }
		if ( key=="down" ) { moveBack(); }
		if ( key=="J" ) { turnLeft(); }
		if ( key=="L" ) { turnRight(); }
		if ( key=="I" ) { moveForward(); }
		if ( key=="K" ) { moveBack(); }
		if ( key=="U" ) { selectItem(1); }
		if ( key=="D" ) { selectItem(2); }
		if ( key=="C" ) { castSpells(); }
		if ( key=="B" ) { displayObjectBuffer(); }
		if ( key=="G" ) { getItems(); }
		if ( key=="P" ) { pauseGame(); }
		if ( key=="," ) { togglePanelsBackward();}
		if ( key=="." ) { togglePanelsForward(); }
		if ( key=="F12" ) { if (plyr.diagOn) {plyr.diagOn = false;} else {plyr.diagOn = true;} }
		if ( key=="F11" ) { tidyObjectBuffer(); }
		if ( key=="F1" ) plyr.infoPanel = 1;
		if ( key=="F2" ) plyr.infoPanel = 2;
		if ( key=="F3" ) plyr.infoPanel = 3;
		if ( key=="F4" ) plyr.infoPanel = 4;
		if ( key=="F5" ) plyr.infoPanel = 5;
		if ( key=="F6" ) plyr.infoPanel = 6;
		if ( key=="F7" ) plyr.infoPanel = 7;
		if ( key=="F8" ) plyr.infoPanel = 8;
		if ( key=="SPACE") plyr.infoPanel = 1;
		if ( key=="F" ) { if (plyr.fpsOn) {plyr.fpsOn = false;} else {plyr.fpsOn = true;} }
		if ( key=="A" ) { if (plyr.miniMapOn) {plyr.miniMapOn = false;} else {plyr.miniMapOn = true;} }
		if ( key=="M" ) { automap(); }

		if ( key=="W" ) { chooseEncounter(); }

		if ( key=="T" ) { if (AR_DEV.TELEPORT_OPTION) teleport(); }

		if ( key=="ESC" ) { optionsMenu(); }
		if ( key=="QUIT" ) { optionsMenu(); }
	}
		// Check smithy sounds and encounter music not playing
		if (!gameQuit) { smithySound.stop(); playerDies(); }
		Running=false;

	}
}


void optionsMenu()
{
		string str, key_value;
		bool keypressed = false;

        while ((!keypressed) && (Running))
		{
			clearDisplay();
			displayOptionsMenu();
			updateDisplay();

			key_value = getSingleKey();

			if (key_value == "ESC") { keypressed=true; plyr.status=1; }
			if (key_value == "S") { keypressed=true; displaySaveGame(); } // Needs updating to reflect slot
			if (key_value == "Q") { quitMenu(); }
        }
}


void quitMenu()
{
		string str, key_value;
		bool keypressed = false;

        while (!keypressed)
		{
			clearDisplay();
			displayQuitMenu();
			updateDisplay();

			key_value = getSingleKey();

			if (key_value == "N") { keypressed=true; }
			if (key_value == "Y") { keypressed=true; Running=false; gameQuit = true; }
			//if (key_value == "Y") { keypressed=true; smithySound.stop(); Running=false; gameQuit=true; }
        }

}


void playerDies()
{
	bool deathLooping = true;
	bool musicPlaying = false;
	plyr.fixedEncounter = false;
	plyr.miniMapOn = false;
	plyr.status = 5; // Dead

	if (plyr.musicStyle==0) deathMusic.openFromFile("data/audio/death.ogg");
	else deathMusic.openFromFile("data/audio/B/death.ogg");
	if (!musicPlaying) { deathMusic.play(); musicPlaying = true; }
//lyricstexture.clear(sf::Color::Green);
	loadLyrics("death.txt");
	while ( deathLooping )
	{
        clock1.restart();

        clearDisplay();
		drawStatsPanel();

		updateLyrics();
		iCounter++;

		cyText (3, "You're DEAD.");
		cyText (8, "(Press SPACE to continue)");
		updateDisplay();
		string key = getSingleKey();
		if (key=="SPACE") { deathLooping = false; }
		if (key=="F1") { deathMusic.stop(); loadLyrics("death.txt"); deathMusic.play(); }
	}
	if (musicPlaying) { deathMusic.stop(); }
}


void togglePanelsForward()
{
     plyr.infoPanel++;
     if (plyr.infoPanel == 9) { plyr.infoPanel = 1; }
}


void togglePanelsBackward()
{
     plyr.infoPanel--;
     if (plyr.infoPanel == 0) { plyr.infoPanel = 8; }
}


void barredDoor()
{
	//bool keynotpressed = true;
	string str, key;
	str = "";
	int doorType;
	if (plyr.movingForward) doorType = plyr.front;
	else doorType = plyr.back;
	int doorMenu = 0;
	bool doorNotExamined = true;
	while (doorMenu < 255) // closed
	{
		while (doorMenu == 0)
		{
			int useRef;
			doorNotExamined = true;
			dispMain();
			drawConsoleBackground();
			cyText(1, "The door won't open. You may:");
			bText(9, 3, "(1) Examine the door");
			bText(9, 4, "(2) Force the door");
			bText(9, 5, "(3) Use a key");
			bText(9, 6, "(4) Break an enchantment");
			bText(9, 8, "(0) Leave it");
			updateDisplay();
			key = getSingleKey();
			if (key == "0") { doorMenu = 255; }
			if (key == "1") { doorMenu = 1; }
			if (key == "2") { doorMenu = 2; }
			if (key == "3") { doorMenu = 3; }
			if (key == "4") { doorMenu = 4; }
			if (key == "U")
			{
				useRef = selectItem(1);
				if (useRef == 666) { doorMessage("@@@The door opens!@@@@<<< Press any key to continue >>>"); updateDoorDetails(); doorMenu = 255; moveThroughBarredDoor(); }
			}
		}
		while (doorMenu == 1)
		{
			if (doorNotExamined)
			{
				doorNotExamined = false;
				int doorIdentificationSuccess = rollDice(4, 6); // roll 3D6
				doorTimedMessage("@@@Examining...");
				str = "@You can't discern what bars the door.";
				if (doorIdentificationSuccess < plyr.wis)
				{
					if (doorType == 8) str = "@The door appears to need a key.";
					if (doorType == 9) str = "@The door appears to be bolted.";
					if (doorType == 10) str = "@The door appears to be enchanted.";
				}
			}
			dispMain();
			drawConsoleBackground();
			cText(str);
			updateDisplay();
			key = getSingleKey();
			if (key == "SPACE") { doorMenu = 0; }
		}

		while (doorMenu == 2)
		{
			doorMessage("@@@Wham!");
			int playerDamage = rollDice(1, 4);
			plyr.hp -= playerDamage;
			int doorRoll = rollDice(7, 6);
			bool doorOpenSuccess = false;
			if ((doorRoll <= plyr.str) && (doorType == 9)) doorOpenSuccess = true;
			if ((doorOpenSuccess) && (plyr.hp > -1)) { doorMessage("@@@The door opens!@@@@<<< Press any key to continue >>>"); updateDoorDetails(); doorMenu = 255; moveThroughBarredDoor(); }
			if ((!doorOpenSuccess) && (plyr.hp > -1)) { doorMessage("@@@The door remains shut.");  doorMenu = 0; }
			if (plyr.hp < 0) { doorMenu = 255; } // player dead
		}


		while (doorMenu == 3)
		{
			if (plyr.keys == 0) { doorMessage("@@@You have none."); doorMenu = 0; }
			else
			{
				if (doorType == 9) { doorMessage("@@@The door remains shut."); plyr.keys--; doorMenu = 0; }
				if (doorType == 10) { doorMessage("@@@The door remains shut."); plyr.keys--; doorMenu = 0; }
				if (doorType == 8) { doorMessage("@@@The door opens!@@@@<<< Press any key to continue >>>"); plyr.keys--; updateDoorDetails(); doorMenu = 255; moveThroughBarredDoor(); }
			}

		}

		while (doorMenu == 4)
		{
			doorTimedMessage("@@@Concentrating...");

			int doorRoll = rollDice(7, 6);
			// Add to fatigue?
			bool doorOpenSuccess = false;
			if ((doorRoll <= plyr.inte) && (doorType == 10)) doorOpenSuccess = true;
			if (doorOpenSuccess) { doorMessage("@@@The door opens!@@@@<<< Press any key to continue >>>"); updateDoorDetails(); doorMenu = 255; moveThroughBarredDoor(); }
			if (!doorOpenSuccess) { doorMessage("@@@The door remains shut.");  doorMenu = 0; }
		}




	}
}


void moveWest()
{
	if (plyr.facing==WEST) // west (forward)
	{

		if (plyr.z_offset>1.9f)
		{
			plyr.z_offset=0.0f;
			plyr.oldx = plyr.x;
			plyr.x--;
		}
		else
		{
			plyr.z_offset=plyr.z_offset+0.1f;
		}

	}

	if (plyr.facing==EAST) // east (back)
	{
		if (plyr.z_offset<0.1f)
		{
			plyr.z_offset=1.9f;
			plyr.oldx = plyr.x;
			plyr.x--;
		}
		else
		{
			plyr.z_offset=plyr.z_offset-0.1f;
		}
	}

}


void moveEast()
{
	if (plyr.facing==EAST) // east (forward)
	{
		if (plyr.z_offset>1.9f)
		{
			plyr.z_offset=0.0f;
			plyr.oldx = plyr.x;
			plyr.x++;
		}
		else
		{
			plyr.z_offset=plyr.z_offset+0.1f;
		}

	}

	if (plyr.facing==WEST) // west (back)
	{
		if (plyr.z_offset<0.1f)
		{
			plyr.z_offset=1.9f;
			plyr.oldx = plyr.x;
			plyr.x++;
		}
		else
		{
			plyr.z_offset=plyr.z_offset-0.1f;
		}
	}

}


void moveNorth()
{
	if (plyr.facing==NORTH) // north (forward)
	{
		if (plyr.z_offset>1.9f)
		{
			plyr.z_offset=0.0f;
			plyr.oldy = plyr.y;
			plyr.y--;
		}
		else
		{
			plyr.z_offset=plyr.z_offset+0.1f;
		}
	}

	if (plyr.facing==SOUTH) // south (back)
	{
		if (plyr.z_offset<0.1f)
		{
			plyr.z_offset=1.9f;
			plyr.oldy = plyr.y;
			plyr.y--;
		}
		else
		{
			plyr.z_offset=plyr.z_offset-0.1f;
		}
	}

}


void moveSouth()
{
	if (plyr.facing==SOUTH)
	{
		if (plyr.z_offset>1.9f)
		{
			plyr.z_offset=0.0f;
			plyr.oldy = plyr.y;
			plyr.y++;
		}
		else
		{
			plyr.z_offset=plyr.z_offset+0.1f;
		}
	}

	if (plyr.facing==NORTH)
	{
		if (plyr.z_offset<0.1f)
		{
			plyr.z_offset=1.9f;
			plyr.oldy = plyr.y;
			plyr.y++;
		}
		else
		{
			plyr.z_offset=plyr.z_offset-0.1f;
		}
	}

}


void moveForward()
{
	plyr.movingForward=true;
	string encText = checkEncumbrance();
	if (encText=="Encumbered") { sf::sleep(sf::milliseconds(50)); }
	if (encText=="Immobilized!") { sf::sleep(sf::milliseconds(200)); }

    if ( ((plyr.front != 13) && (plyr.front != 14) && (plyr.front != 37)) || (plyr.z_offset< 1.7))
	{
        switch(plyr.facing)
        {
        case WEST: // w
			moveWest();
			break;

        case NORTH: // n
			moveNorth();
            break;

        case EAST: // e
			moveEast();
			break;

        case SOUTH: // s
			moveSouth();
			break;
        }


        // Barred door
		if ( (( plyr.front == 8 ) || ( plyr.front == 9 ) || ( plyr.front == 10 )) && (plyr.z_offset > 1.7)) // barred door
		{
			plyr.z_offset-=0.1;
			bool doorAlreadyOpened = false;
			doorAlreadyOpened = checkBarredDoor();
			if (doorAlreadyOpened) {   doorMessage("@@@The door opens!@@@@<<< Press any key to continue >>>"); moveThroughBarredDoor(); }
			else { barredDoor(); }
		}

        if ( (( plyr.front == 5 ) || ( plyr.front == 6 )) && (plyr.z_offset > 1.8)) // secret door
        {
			plyr.z_offset=0.3f;


			switch(plyr.facing)
			{
				case WEST: // w
					plyr.oldx = plyr.x;
					plyr.x--;
					break;

				case NORTH: // n
					plyr.oldy = plyr.y;
					plyr.y--;
					break;

				case EAST: // e
					plyr.oldx = plyr.x;
					plyr.x++;
					break;
				case SOUTH:
					plyr.oldy = plyr.y;
					plyr.y++; // s
					break;
			}

			if (plyr.scenario==0) { citySecretSound.play(); } else { dungeonSecretSound.play(); }
        }

        if ( (( plyr.front == 3 ) || ( plyr.front == 4 )) && (plyr.z_offset > 1.8)) // door
        {
			plyr.z_offset=0.3f;

			switch(plyr.facing)
			{
				case WEST: // w
					plyr.oldx = plyr.x;
					plyr.x--;
					break;

				case NORTH: // n
					plyr.oldy = plyr.y;
					plyr.y--;
					break;

				case EAST: // e
					plyr.oldx = plyr.x;
					plyr.x++;
					break;

				case SOUTH:
					plyr.oldy = plyr.y;
					plyr.y++; // s
					break;
			}
			plyr.noticeability = plyr.noticeability + 0.1;
			if (plyr.scenario==0) { cityDoorSound.play(); } else { dungeonDoorSound.play(); }
		}

	 if ( ( plyr.front > 25 ) && ( plyr.front < 50 ) && (plyr.z_offset > 1.8)) // City doors with signs
     {
			plyr.z_offset=0.3f;

			switch(plyr.facing)
			{
				case WEST: // w
					plyr.oldx = plyr.x;
					plyr.x--;
					break;

				case NORTH: // n
					plyr.oldy = plyr.y;
					plyr.y--;
					break;

				case EAST: // e
					plyr.oldx = plyr.x;
					plyr.x++;
					break;

				case SOUTH:
					plyr.oldy = plyr.y;
					plyr.y++; // s
					break;

			}
			if (plyr.scenario==0) { cityDoorSound.play(); } else { dungeonDoorSound.play(); }
	  }

    }
	/*
	if (plyr.x == -1) { plyr.x = 63; } // city wrap around
	if (plyr.x == 64) { plyr.x = 0; } // city wrap around
	if (plyr.y == -1) { plyr.y = 63; } // city wrap around
	if (plyr.y == 64) { plyr.y = 0; } // city wrap around
	*/
	if ((plyr.x == -1) || (plyr.x == 64) || (plyr.y == -1) || (plyr.y == 64)) { scenarioEntrance(300); } // city wall edges leading to wilderness


}


void moveBack()
{
	plyr.movingForward=false;
	string encText = checkEncumbrance();
	if (encText=="Encumbered") { sf::sleep(sf::milliseconds(50)); }
	if (encText=="Immobilized!") { sf::sleep(sf::milliseconds(200)); }

	if ( ((plyr.back != 13)&&(plyr.back != 14) && (plyr.back != 37)) || (plyr.z_offset> 0.2))
	{

    switch(plyr.facing)
        {
        case WEST: // facing west
            moveEast(); // opposite of west
            break;

        case NORTH: // facing north
            moveSouth(); // opposite of north
            break;

        case EAST: // facing east
        	moveWest();
            break;

        case SOUTH: // facing south
        	moveNorth();
        	break;
        }


	        // Barred door
		if ( (( plyr.back == 8 ) || ( plyr.back == 9 ) || ( plyr.back == 10 )) && (plyr.z_offset < 0.3)) // barred door
		{
			plyr.z_offset+=0.1;
			bool doorAlreadyOpened = false;
			doorAlreadyOpened = checkBarredDoor();
			if (doorAlreadyOpened) {   doorMessage("@@@The door opens!@@@@<<< Press any key to continue >>>"); moveThroughBarredDoor(); }
			else { barredDoor(); }
		}


 if ( (( plyr.back == 5 ) || ( plyr.back == 6 )) && (plyr.z_offset < 0.3)) // secret door
        {
			plyr.z_offset=1.7f;


			switch(plyr.facing)
			{
				case WEST: // w
					plyr.oldx = plyr.x;
					plyr.x++;
					break;

				case NORTH: // n
					plyr.oldy = plyr.y;
					plyr.y++;
					break;

				case EAST: // e
					plyr.oldx = plyr.x;
					plyr.x--;
					break;
				case SOUTH:
					plyr.oldy = plyr.y;
					plyr.y--; // s
					break;
			}

			if (plyr.scenario==0) { citySecretSound.play(); } else { dungeonSecretSound.play(); }


        }

        if ( (( plyr.back == 3 ) || ( plyr.back == 4 )) && (plyr.z_offset < 0.3)) // door
        {
			plyr.z_offset=1.7f;

			switch(plyr.facing)
			{
				case WEST: // w
					plyr.oldx = plyr.x;
					plyr.x++;
					break;

				case NORTH: // n
					plyr.oldy = plyr.y;
					plyr.y++;
					break;

				case EAST: // e
					plyr.oldx = plyr.x;
					plyr.x--;
					break;

				case SOUTH:
					plyr.oldy = plyr.y;
					plyr.y--; // s
					break;
			}
			if (plyr.scenario==0) { cityDoorSound.play(); } else { dungeonDoorSound.play(); }
		}

	 if ( ( plyr.back > 25 ) && ( plyr.back < 50 ) && (plyr.z_offset < 0.3)) // City doors with signs
     {
			plyr.z_offset=1.7f;

			switch(plyr.facing)
			{
				case WEST: // w
					plyr.oldx = plyr.x;
					plyr.x++;
					break;

				case NORTH: // n
					plyr.oldy = plyr.y;
					plyr.y++;
					break;

				case EAST: // e
					plyr.oldx = plyr.x;
					plyr.x--;
					break;

				case SOUTH:
					plyr.oldy = plyr.y;
					plyr.y--; // s
					break;

			}
			if (plyr.scenario==0) { cityDoorSound.play(); } else { dungeonDoorSound.play(); }
	  }

}

}


void turnLeft()
{
	switch(plyr.facing)
    {
    case NORTH: // n

        plyr.facing = WEST;
		plyr.z_offset=1.0f;
        break;

    case WEST: //  facing w before turning

        plyr.facing = SOUTH;
		plyr.z_offset=1.0f;
        break;

    case EAST: // e

    	plyr.facing = NORTH;
		plyr.z_offset=1.0f;
        break;

    case SOUTH: // s

    	plyr.facing = EAST;
    	plyr.z_offset=1.0f;
		break;
    }
}


void turnRight()
{
    switch(plyr.facing)
    {
    case NORTH: // n

        plyr.facing = EAST;
        plyr.z_offset=1.0f;
		break;

    case WEST: // w

        plyr.facing = NORTH;
        plyr.z_offset=1.0f;
		break;

    case EAST: // e

    	plyr.facing = SOUTH;
        plyr.z_offset=1.0f;
		break;

    case SOUTH: // s
    	plyr.facing = WEST;
    	plyr.z_offset=1.0f;
		break;
    }
}


void checkTeleport()
{

	if ( (plyr.special>=0xE0) && (plyr.special<=0xFF) ) // 224 - 255
 	{
      for (int i=0 ; i<=31 ; i++) // Max number of teleport objects
	{
 		//int ind = ((plyr.special)-0xEF);
		if (plyr.special == teleports[i].ref)
		{
			if (teleports[i].new_map == 0)
			{
				plyr.x = (teleports[i].new_x);
				plyr.y = (teleports[i].new_y);
			}
			if (teleports[i].new_map == 1)
			{
				plyr.x = (teleports[i].new_x)+32;
				plyr.y = (teleports[i].new_y);
			}
			if (teleports[i].new_map == 2)
			{
				plyr.x = (teleports[i].new_x);
				plyr.y = (teleports[i].new_y)+32;
			}
			if (teleports[i].new_map == 3)
			{
				plyr.x = (teleports[i].new_x)+32;
				plyr.y = (teleports[i].new_y)+32;
			}
			int new_map;
			// Change map level to new_map value accounting for level 1 now being single 64x64 map
			if (teleports[i].new_map == 0) { new_map=1; } // was Dungeon level 1.1 32x32 segment
			if (teleports[i].new_map == 1) { new_map=1; } // was Dungeon level 1.2 32x32 segment
			if (teleports[i].new_map == 2) { new_map=1; } // was Dungeon level 1.3 32x32 segment
			if (teleports[i].new_map == 3) { new_map=1; } // was Dungeon level 1.4 32x32 segment
			if (teleports[i].new_map == 4) { new_map=2; } // was Dungeon level 2 32x32
			if (teleports[i].new_map == 5) { new_map=3; } // was Dungeon level 3 16x16

			if (plyr.map != new_map)
			{
				plyr.map = new_map;
				moveMapLevelTeleport();
			}

			// Display flashing sequence for teleport.
			plyr.teleporting = 20;

			//plyr.x++;
			//plyr.y++;

			int location_index = getMapIndex(plyr.x, plyr.y);
			transMapIndex (location_index);
		}
	  }
	}

}


void checkFixedTreasures()
{
	int treasureRef;
	switch(plyr.special)
    {
		case 0xAE: // Sword of the Adept
			treasureRef = plyr.special-128;
            if (plyr.fixedTreasures[treasureRef] == false)
			{
				treasureMessage("@A sword protrudes from a slab@of black marble.@@@");
				createWeapon(82);
				plyr.fixedTreasures[treasureRef] = true;
				getItems();
			}
			break;

		case 0xAF: // Razor Ice
			treasureRef = plyr.special-128;
            if (plyr.fixedTreasures[treasureRef] == false)
			{
				treasureMessage("A katana in a ribbed black lacquered@scabbard rests against a wall.@@A pattern of snowflakes has been@honed along its single edge.");
				createWeapon(81);
				plyr.fixedTreasures[treasureRef] = true;
				getItems();
			}
			break;

		case 0xB7: // Map Stone
			treasureRef = plyr.special-128;
            if (plyr.fixedTreasures[treasureRef] == false)
			{
				string gender = "man";
				if (plyr.gender==2) gender = "woman";
				string key = "";
				while (key!="SPACE")
				{
						dispMain();
						cyText(1,"A note reads:");
						string str = plyr.name+",";
						bText(1,2,str);
						str = "Congratulations my good "+gender+" on";
						bText(4,4,str);
						bText(1,5,"getting this far.");
						bText(33,7,"A friend");
						cyText(9,"<<< Press SPACE to continue >>>");
						updateDisplay();
						key = getSingleKey();
				}
				createQuestItem(4);
				plyr.fixedTreasures[treasureRef] = true;
				getItems();
			}
			break;

		case 0xB6: // Amethyst Rod
			treasureRef = plyr.special-128;
            if (plyr.fixedTreasures[treasureRef] == false)
			{
				treasureMessage("@A blue-violet quartz rod lies@on a purple dias.@@@");
				createQuestItem(5);
				plyr.fixedTreasures[treasureRef] = true;
				getItems();
			}
			break;
	}

}


void checkShop()
{
     switch(plyr.special)
     {
        case 0x0D: // damon
            shopDamon();
            break;
        case 0x0C: // guild
            shopGuild();
            break;
        case 0x0F: // retreat
            shopRetreat();
            break;
        case 11: // Palace Prison
            // Check for completed code already
            break;
        case 19: // Ferryman
            shopFerry();
            break;
        case 28: // lift
            shopLift();
            break;
        case 87: // king
            shopUndeadKing();
            break;
		case 0x1D: // rathskeller
            runModule(0x1D);
            break;
        case 21: // dwarvenSmithy
            runModule(21);
            break;
		case 0x50: // City Bank
			if (plyr.scenario==0) { shopBank(); }
			break;
		case 0x90: // City Smithy
			if (plyr.scenario==0) {shopSmithy(); }
			break;
		case 0x70: // City Shop
			if (plyr.scenario==0) { shopShop(); }
			break;
		case 0x10: // City Inn
			if (plyr.scenario==0) { shopInn(); }
			if (plyr.scenario==1) { shopOracle(); }
			break;
		case 0x30: // City Tavern
			if (plyr.scenario==0) { shopTavern(); }
			break;
		case 0xF0: // City Guild
		    if (plyr.scenario==0) { shopGuild(); }
			break;
		case 208: // City Healer
			if (plyr.scenario==0) { shopHealer(); }
			break;
		//case 176: // Closed by order of the palace
		//	if (plyr.scenario==0) { shopClosed(); }
		//	break;
		case 3:
			shopFountain();
			break;
		case 35:
			shopFountain();
			break;
		case 5:
			staircase();
			break;
		case 6:
			runModule(0x06);    // Vault
			break;
		case 7:
			if ((plyr.x==2) && (plyr.y==14) && (plyr.map==1)) shopGoblins();
			if ((plyr.x==56) && (plyr.y==57) && (plyr.map==1)) shopTrolls();
			break;
		case 10:
			shopChapel();
			break;
        case 4: // entrance to wilderness
			scenarioEntrance(300);
			break;
		case 300: // entrance to wilderness
			scenarioEntrance(300);
			break;
		case 301: // entrance to palace
			scenarioEntrance(301);
			break;
        case 302: // Entrance to arena
            arenaSouthernEntrance();
            break;
        case 303: // Entrance to arena
            arenaNorthernEntrance();
            break;
        case 304: // Entrance to arena
            arenaWesternEntrance();

            break;
     }
}


void leaveShop()
{
	//lyricstexture.clear(sf::Color::Black); // wipe the lyric strip
	if (plyr.facing==WEST) { plyr.x = plyr.oldx; }
	if (plyr.facing==EAST) { plyr.x = plyr.oldx; }
	if (plyr.facing==NORTH) { plyr.y = plyr.oldy; }
	if (plyr.facing==SOUTH) { plyr.y = plyr.oldy; }
	plyr.z_offset=1.6; // position player just outside door
	plyr.status = EXPLORE; // explore
}


void shopClosed()
{
	bool keynotpressed = true;
	while (keynotpressed) // closed
	{
		clearDisplay();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		string str, key;
		plyr.status = 2; // shopping
		//App.pushGLStates();
		drawStatsPanel();
		cyText (0, "Closed by@@Order of the Palace@@@@@@( Press space to continue )");
		updateDisplay();
		key = getSingleKey();
		if ( key != "" ) { keynotpressed = false; }
		if ( key == "up" ) { keynotpressed = true; }
	}
	leaveShop();
}


void scenarioEntrance(int scenarioNumber)
{
	bool keynotpressed = true;
	while (keynotpressed) // closed
	{
		clearDisplay();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		string str, key;
		plyr.status = 2; // shopping
		//App.pushGLStates();
		switch(scenarioNumber)
		{
			case 300: str="the Wilderness"; break;
			case 301: str="the Palace"; break;
		}

		drawStatsPanel();

		cyText (0, "You are at an entrance to");
		cyText (3, "Are you sure you want to enter this@scenario? (Y or N)");
		cyText (1, str);
		updateDisplay();
		key = getSingleKey();

		if ( key != "" ) { keynotpressed = false; }
		if ( key == "up" ) { keynotpressed = true; }
	}
	leaveShop();
}


void treasureMessage(string str)
{
	string txt;
	string key = "";
	while (key!="SPACE")
	{
			dispMain();
			txt = str + "@@<<< Press SPACE to continue >>>";
			cyText (1,txt);
			updateDisplay();
			key = getSingleKey();
	}
}


void pauseGame()
{
	string txt;
	string key = "";
	while (key!="SPACE")
	{
			dispMain();
			txt = "(Paused)@@@@@(Press SPACE to continue)";
			cText (txt);
			updateDisplay();
			key = getSingleKey();
	}
}


bool checkBarredDoor()
{
	// Assume moving forward for now
	bool doorAlreadyOpened = false;
	int doorFacing;
	if (plyr.movingForward) doorFacing = plyr.facing;
	else
	{
		if (plyr.facing==WEST) doorFacing = 3; // actually moving backwards east
		if (plyr.facing==NORTH) doorFacing = 4; // actually moving backwards south
		if (plyr.facing==EAST) doorFacing = 1; // actually moving backwards west
		if (plyr.facing==SOUTH) doorFacing = 2; // actually moving backwards north
	}
	for (int i=0 ; i<20 ; i++)
	{
		if ((plyr.doorDetails[i].x==plyr.x) && (plyr.doorDetails[i].y==plyr.y) && (plyr.doorDetails[i].level==plyr.map) && (plyr.doorDetails[i].direction == doorFacing)) { doorAlreadyOpened = true; }
	}
	return doorAlreadyOpened;
}


void updateDoorDetails()
{
	// Adds an entry about a door that has been successfully opened. The 1st entry is overwritten after 20 door openings.

	if (plyr.movingForward) plyr.z_offset=2.0f;
	else plyr.z_offset=0.0f;

	if (plyr.doorDetailIndex==19) plyr.doorDetailIndex = 0;
	plyr.doorDetails[plyr.doorDetailIndex].x=plyr.x;
	plyr.doorDetails[plyr.doorDetailIndex].y=plyr.y;
	plyr.doorDetails[plyr.doorDetailIndex].level=plyr.map;
	//int direction;
	if (plyr.movingForward) plyr.doorDetails[plyr.doorDetailIndex].direction = plyr.facing;
	if (!plyr.movingForward)
	{
		if (plyr.facing==WEST) plyr.doorDetails[plyr.doorDetailIndex].direction = 3; // actually moving east
		if (plyr.facing==NORTH) plyr.doorDetails[plyr.doorDetailIndex].direction = 4; // actually moving south
		if (plyr.facing==EAST) plyr.doorDetails[plyr.doorDetailIndex].direction = 1; // actually moving west
		if (plyr.facing==SOUTH) plyr.doorDetails[plyr.doorDetailIndex].direction = 2; // actually moving north
	}
	plyr.doorDetailIndex++;
}


void moveThroughBarredDoor()
{
	if (plyr.movingForward) plyr.z_offset = 2.0;
	else plyr.z_offset = 0.0;
	if ((plyr.facing==WEST) && (plyr.movingForward)) moveWest();
	if ((plyr.facing==NORTH) && (plyr.movingForward)) moveNorth();
	if ((plyr.facing==EAST) && (plyr.movingForward)) moveEast();
	if ((plyr.facing==SOUTH) && (plyr.movingForward)) moveSouth();
	if ((plyr.facing==WEST) && (!plyr.movingForward)) moveEast();
	if ((plyr.facing==NORTH) && (!plyr.movingForward)) moveSouth();
	if ((plyr.facing==EAST) && (!plyr.movingForward)) moveWest();
	if ((plyr.facing==SOUTH) && (!plyr.movingForward)) moveNorth();
}


void doorMessage(string str)
{

	string key;
	bool keyNotPressed = true;
	while (keyNotPressed)
	{
		dispMain();
		drawConsoleBackground();
		cyText (1, str);
		updateDisplay();
		key = getSingleKey();
		if ( key == "SPACE" ) { keyNotPressed = false; }
	}
}


void doorTimedMessage(string str)
{
	string key;
	dispMain();
	drawConsoleBackground();
	cyText (1, str);
	updateDisplay();
	sf::sleep(sf::milliseconds(4000));
}


void teleport()
{
	string typed_coords;
	bool teleportComplete = false;
	int name_length;
	while (!teleportComplete)
	{
		string str;
		string single_key;
		clearDisplay();
		drawText(2, 2, "To XX YY (followed by Enter)");
		str = "To: "+typed_coords+"_";
		drawText(2,5,str);
		updateDisplay();


		 single_key = getSingleKey();
		 if (single_key=="SPACE") { single_key=" "; }
		 if ((single_key=="RETURN") && (typed_coords!="")) { single_key = ""; teleportComplete = true; }
		 if (single_key=="BACKSPACE")
		 {
				  name_length = typed_coords.size();
				  if (name_length!=0)
				  {
					  int name_length = typed_coords.size();
					  typed_coords = typed_coords.substr(0,(name_length-1));
				  }
				  single_key = "";
		 }
		 else
		 {
				  if (single_key!="RETURN")
				  {
					  name_length = typed_coords.size();
					  if (name_length!=5) // check for limit of name length
					  {
							 typed_coords = typed_coords + single_key;
					  }
				   }
         }

	}
	string xtxt,ytxt,map;
	xtxt=typed_coords.substr(0,2);
	ytxt=typed_coords.substr(3,2);
	//map=typed_coords.substr(6,1);
	// if moving to new map then data files need to be loaded first
	int x = 0;
	int y = 0;
	x=atoi(xtxt.c_str());
	y=atoi(ytxt.c_str());

	if (!((x>=0) && (x<=63))) x = 0;
	if (!((y>=0) && (y<=63))) y = 0;
	plyr.x = x;
	plyr.y = y;
	//plyr.map=atoi(map.c_str());
	// load new content?
	// teleport sound
	// validate!
}


void initialiseNewGame()
{
    // COPY FROM ARX.CPP - Prepare shop stock etc...

    doorCityBuffer.loadFromFile("data/audio/cityDoor.wav");
    secretCityBuffer.loadFromFile("data/audio/citySecretDoor.wav");
    citySecretSound.setBuffer(secretCityBuffer);
    cityDoorSound.setBuffer(doorCityBuffer);

    doorDungeonBuffer.loadFromFile("data/audio/dungeonDoor.wav");
    secretDungeonBuffer.loadFromFile("data/audio/dungeonSecretDoor.wav");
    dungeonSecretSound.setBuffer(secretDungeonBuffer);
    dungeonDoorSound.setBuffer(doorDungeonBuffer);

	smithyBuffer.loadFromFile("data/audio/smithyHammer3.wav");
	smithySound.setBuffer(smithyBuffer);
	smithySound.setLoop(true);

	teleBlack.loadFromFile("data/images/teleport_black.png");
	teleGold.loadFromFile("data/images/teleport_gold.png");

	InitMap();

	//loadMonstersBinary(); duplicate
}


void readKeys()
{

	// Process events

	sf::RenderWindow App(sf::VideoMode(800, 600), "SFML Window");

	sf::Event Event;

	while (App.isOpen()) {
		while (App.pollEvent(Event)) {
			{
				if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Left)) turnLeft();
				if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Right)) turnRight();
				if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Up)) moveForward();
				//if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Down)) moveBack();

				// Close window : exit
				if (Event.type == sf::Event::Closed)
					Running = false;

				// Escape key : exit
				if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
					Running = false;

				//if (Event.Type == sf::Event::Resized)
			//glViewport(0, 0, Event.Size.Width, Event.Size.Height);

			}
		}
	}
}