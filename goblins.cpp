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
#include "goblins.h"
#include "lyrics.h"
#include "automap.h"


string goblinLyricsFilename;


void shopGoblins()
{
	setAutoMapFlag(plyr.map, 2, 13);
	setAutoMapFlag(plyr.map, 2, 14);
	setAutoMapFlag(plyr.map, 2, 15);
	setAutoMapFlag(plyr.map, 1, 13);
	setAutoMapFlag(plyr.map, 1, 14);
	setAutoMapFlag(plyr.map, 1, 15);

	plyr.status = 2; // shopping
	int goblinsMenu = 1; // high level menu
	loadShopImage(17); // goblins
	if (checkForQuestItem(0)) { goblinsMenu = 2; }
	string str,key;
	while (goblinsMenu > 0)
	{
		while (goblinsMenu == 1) // main menu
		{
			clearShopDisplay();

			// Need to check for possession of ring halves!

			if (plyr.goblinsDefeated)
			{
				cyText (2, "Thief! Thou hast dared to steal from the");
				cyText (3, "goblins! Now thou must fight the horde!");
			}

			if (plyr.goblinsVisited == false)
			{
				bText (2,1, "Welcome, Sir Traveller! Lend me thine");
				bText (2,2, "assistance. Trolls have stolen half ");
				bText (2,3, "the magic ring The Dwarf made for me.");
				bText (2,4, "Bring it back and I'll reward thee. Go!");

			}
			if ((plyr.goblinsVisited) && (plyr.goblinsChallenged == false) && (plyr.goblinsReforged == false))
			{
				cyText (2, "Kind Knight, where is the ring? Don't");
				cyText (4, "darken this hall again without it!");
			}

			if ((plyr.goblinsVisited) && (plyr.goblinsChallenged == false) && (plyr.goblinsReforged))
			{
				str = "Greetings "+ plyr.name + ".";
				cyText (2,str);
				cyText (4, "To what do we owe this pleasure?");
			}

			if ((plyr.goblinsVisited) && (plyr.goblinsChallenged) && (plyr.goblinsDefeated == false))
			{
				cyText (2, "Art thou tired of living?");
				cyText (4, "Stand and fight!");
			}

			updateDisplay();

			if (plyr.musicStyle==0) { playShopMusic(2); goblinLyricsFilename="goblins.txt"; }
			if (plyr.musicStyle==1) { playShopMusic(5); goblinLyricsFilename="goblins.txt"; }
			loadLyrics(goblinLyricsFilename);

			key = getSingleKey();

			if ((key=="SPACE") && ( plyr.goblinsDefeated )) { goblinsMenu = 0; plyr.goblinsCombat = true; } // leave shop and start combat!
			if ((key=="SPACE") && ( plyr.goblinsDefeated == false )) { goblinsMenu = 2; } // move to actual menu
			//if (plyr.goblinsVisited) && (plyr.goblinsChallenged) && (plyr.goblinsDefeated == false)) { goblinsMenu = 3; }
		}




		while (goblinsMenu == 2) // secondary menu
		{
			if (plyr.goblinsChallenged) { goblinsMenu = 3; }
			clearShopDisplay();

			if ((plyr.goblinsChallenged == false) && (checkForQuestItem(0) == false))
			{
				cyText (1, "What shall you do?");
				bText (6,3, "(1) Demand the Goblin's ring or");
				bText (6,4, "(0) Leave?");
			}

			if ((plyr.goblinsChallenged == false) && (checkForQuestItem(0)))
			{
				cyText (1, "Thou hast brought my treasure!");
				cyText (2, "Give me the ring half!");

				bText (6,4, "(1) Give up the ring or");
				bText (6,5, "(2) Refuse to give it up");
			}

			updateDisplay();
			key = getSingleKey();

			if ((plyr.goblinsChallenged == false) && (checkForQuestItem(0) == false))
			{
				if (key=="0") { goblinsMenu = 0; }
				if (key=="1") { plyr.goblinsChallenged = true; goblinsMenu = 3; }
			}
			if ((plyr.goblinsChallenged == false) && (checkForQuestItem(0)))
			{
				if (key=="1") { goblinsMenu = 4; }
				if (key=="2") { plyr.goblinsChallenged = true; goblinsMenu = 3; }
			}
		}




		while (goblinsMenu == 3) // third menu
		{
			 clearShopDisplay();
			 updateLyrics();

			if ((plyr.goblinsChallenged) && (checkForQuestItem(0) == false))
			{
				cyText (2, "Thou cans't have the ring only by");
				cyText (4, "defeating me! Prepare to duel!");
			}

			if ((plyr.goblinsChallenged) && (checkForQuestItem(0)))
			{
				cyText (2, "Foul friend to Trolls!");
				cyText (4, "Prepare to die.");
			}

			updateDisplay();
			key = getSingleKey();
			if ( key=="SPACE" ) { goblinsMenu = 0; plyr.goblinsCombat = true; }// leave shop and start combat!
		}




		while (goblinsMenu == 4) // fourth menu
		{
			clearShopDisplay();
			updateLyrics();
			cyText (2, "Thou art wise not to risk my wrath.");
			cyText (4, "Thy reward is my thanks.");
			updateDisplay();

			key = getSingleKey();

			if ( key=="SPACE" )
			{
				goblinsMenu = 2;
				plyr.goblinsReforged = true;
				int ringRef = getQuestItemRef(0);
				itemBuffer[ringRef].location = 0; // move this item to the void
			}

		}



	}
	plyr.goblinsVisited = true;
	stopShopMusic();
	leaveShop();
}
