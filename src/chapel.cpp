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
#include "chapel.h"

// extern Player plyr;


inline std::string concat( int n, const char* str )
{
std::ostringstream ss;
ss << n;
ss << str;
return ss.str();
}

void shopChapel()
{
	// Module for Dungeon Chapel

	int chapelMenu = 1; // high level menu
	string str,key;
	plyr.status = 2; // shopping


	loadShopImage(18);

	while (chapelMenu > 0)
	{
		while (chapelMenu == 1) // main menu
		{
			clearShopDisplay();
			cyText (1, "Welcome to the Dungeon Chapel.");
			cyText (3, "What would you like to do?");
			bText (8,5, "(1) Pray");
			bText (8,6, "(2) Listen to a sermon");
			bText (8,7, "(3) Consult with a priest");
			bText (8,8, "(4) Make a donation");
			bText (8,9, "(0) Leave");
			updateDisplay();
			playShopMusic(3);

			key = getSingleKey();

			if ( key=="0" ) { chapelMenu = 0; }
			if ( key=="down" ) { chapelMenu = 0; }
		}



	}
	stopShopMusic();
	leaveShop();
}

