#include <string>
#include <iostream>
#include <sstream>

#include "player.h"
#include "font.h"
#include "display.h"
#include "game.h"
#include "misc.h"
#include "retreat.h"
#include "audio.h"
#include "items.h"
#include "prison.h"
#include "lyrics.h"
#include "automap.h"




void shopPrison()
{

	//fills all the gaps in the box for the auto map
	setAutoMapFlag(plyr.map, 2, 13);
	setAutoMapFlag(plyr.map, 2, 14);
	setAutoMapFlag(plyr.map, 2, 15);
	setAutoMapFlag(plyr.map, 1, 13);
	setAutoMapFlag(plyr.map, 1, 14);
	setAutoMapFlag(plyr.map, 1, 15);

	plyr.status = 2; // shopping
	int prisonMenu = 1; // high level menu
	loadShopImage(28); // goblins
	if (checkForQuestItem(187)) { prisonMenu = 2; }
	string str, key;
	while (prisonMenu > 0)
	{
		while (prisonMenu == 1) // main menu
		{
			clearShopDisplay();

			// Need to check for possession of ring halves!

			
			if ((plyr.goblinsVisited) && (plyr.goblinsChallenged == false) && (plyr.goblinsReforged))
			{
				str = "Greetings " + plyr.name + ".";
				cyText(2, str);
				cyText(4, "To what do we owe this pleasure?");
			}

			

			updateDisplay();

			/*if (plyr.musicStyle == 0) { playShopMusic(2); goblinLyricsFilename = "goblins.txt"; }
			if (plyr.musicStyle == 1) { playShopMusic(5); goblinLyricsFilename = "goblins.txt"; }
			loadLyrics(goblinLyricsFilename);

			key = getSingleKey();

			if ((key == "SPACE") && (plyr.goblinsDefeated)) { goblinsMenu = 0; plyr.goblinsCombat = true; } // leave shop and start combat!
			if ((key == "SPACE") && (plyr.goblinsDefeated == false)) { goblinsMenu = 2; } // move to actual menu
			//if (plyr.goblinsVisited) && (plyr.goblinsChallenged) && (plyr.goblinsDefeated == false)) { goblinsMenu = 3; }

*/
		}




		while (prisonMenu == 2) // secondary menu
		{
			
		}




		while (prisonMenu == 3) // third menu
		{
			clearShopDisplay();
			
		}




		while (prisonMenu == 4) // fourth menu
		{
			clearShopDisplay();
			
		}



	}
	//plyr.prisonVisited = true;
	stopShopMusic();
	leaveShop();
}
