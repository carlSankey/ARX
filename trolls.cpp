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
#include "trolls.h"
#include "lyrics.h"
#include "automap.h"

string trollLyricsFilename;




void shopTrolls()
{
	setAutoMapFlag(plyr.map, 56, 56);
	setAutoMapFlag(plyr.map, 56, 57);
	setAutoMapFlag(plyr.map, 56, 58);
	setAutoMapFlag(plyr.map, 57, 56);
	setAutoMapFlag(plyr.map, 57, 57);
	setAutoMapFlag(plyr.map, 57, 58);

	plyr.status = 2; // shopping
	int trollsMenu = 1; // high level menu
	loadShopImage(16); // trolls
	if (checkForQuestItem(1)) { trollsMenu = 2; }

	string str,key;
	while (trollsMenu > 0)
	{



		while (trollsMenu == 1) // main menu
		{
			clearShopDisplay();

			// Need to check for possession of ring halves!

			if (plyr.trollsDefeated)
			{
				cyText (2, "Thief! Thou hast dared to steal from the");
				cyText (3, "Trolls! Now thou must fight the horde!");
			}

			if (plyr.trollsVisited == false)
			{
				bText (2,1, "Welcome Brave Quester! I need thy help.");
				bText (2,2, "Goblins have taken half the magic ring");
				bText (2,3, "my father got from the Dwarf. Great");
				bText (2,4, "reward is thine for bringing it back.");

			}
			if ((plyr.trollsVisited) && (plyr.trollsChallenged == false)&& (plyr.trollsReforged == false))
			{
				cyText (2, "Why art thou back emptyhanded?");
				cyText (4, "Go get the ring half!");
			}


			if ((plyr.trollsVisited) && (plyr.trollsChallenged == false) && (plyr.trollsReforged))
			{
				str = "Welcome "+ plyr.name + ".";
				cyText (2,str);
				cyText (4, "Hast thou more business here?");
			}



			if ((plyr.trollsVisited) && (plyr.trollsChallenged) && (plyr.trollsDefeated == false))
			{
				cyText (2, "Art thou back again, Goblin-face?");
				cyText (4, "Thou'll not escape this time!");
			}

			updateDisplay();

			if (plyr.musicStyle==0) { playShopMusic(1); trollLyricsFilename="trolls.txt"; }
			if (plyr.musicStyle==1) { playShopMusic(4); trollLyricsFilename="trolls.txt"; }
			loadLyrics(trollLyricsFilename);

			key = getSingleKey();

			if (( key=="SPACE" ) && ( plyr.trollsDefeated )) { trollsMenu = 0; plyr.trollsCombat = true; } // leave shop and start combat!
			if (( key=="SPACE" ) && ( plyr.trollsDefeated == false )) { trollsMenu = 2; } // move to actual menu
			//if (plyr.trollsVisited) && (plyr.trollsChallenged) && (plyr.trollsDefeated == false)) { trollsMenu = 3; }
		}





		while (trollsMenu == 2) // secondary menu
		{
			if (plyr.trollsChallenged) { trollsMenu = 3; }
			clearShopDisplay();

			if ((plyr.trollsChallenged == false)  && (checkForQuestItem(1) == false))
			{
				cyText (1, "What shall you do?");
				bText (6,3, "(1) Demand the Troll's ring or");
				bText (6,4, "(0) Leave?");
			}

			if ((plyr.trollsChallenged == false) && (checkForQuestItem(1)))
			{
				cyText (1, "Thou hast brought my treasure!");
				cyText (2, "Give me the ring half!");

				bText (6,4, "(1) Give up the ring or");
				bText (6,5, "(2) Refuse to give it up");
			}

			updateDisplay();
			key = getSingleKey();

			if ((plyr.trollsChallenged == false) && (checkForQuestItem(1) == false))
			{
				if (key=="0") { trollsMenu = 0; }
				if (key=="1") { plyr.trollsChallenged = true; trollsMenu = 3; }
			}
			if ((plyr.trollsChallenged == false) && (checkForQuestItem(1)))
			{
				if (key=="1") { trollsMenu = 4; }
				if (key=="2") { plyr.trollsChallenged = true; trollsMenu = 3; }
			}



		}












		while (trollsMenu == 3) // third menu
		{
			 clearShopDisplay();
			 updateLyrics();

			if ((plyr.trollsChallenged) && (checkForQuestItem(1) == false))
			{
				cyText (2, "I'll not give it up so easily!");
				cyText (4, "Prepare to do battle!");
			}

			if ((plyr.trollsChallenged) && (checkForQuestItem(1)))
			{
				cyText (2, "Foul friend to Goblins!");
				cyText (4, "Prepare to die.");
			}

			updateDisplay();
			key = getSingleKey();
			if ( key=="SPACE" ) { trollsMenu = 0; plyr.trollsCombat = true; }// leave shop and start combat!
		}




		while (trollsMenu == 4) // fourth menu
		{
			clearShopDisplay();
			updateLyrics();
			cyText (2, "Thou art wise not to risk my wrath.");
			cyText (4, "Thy reward is my thanks.");
			updateDisplay();

			key = getSingleKey();

			if ( key=="SPACE" )
			{
				trollsMenu = 2;
				plyr.trollsReforged = true;
				int ringRef = getQuestItemRef(1);
				itemBuffer[ringRef].location = 0; // move this item to the void
			}

		}




	}
	plyr.trollsVisited = true;
	stopShopMusic();
	leaveShop();
}


