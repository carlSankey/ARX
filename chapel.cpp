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

// Per-frame state machine. Called each frame by checkShop while plyr.status==2.
void shopChapel()
{
	static int chapelMenu = 0;

	if (chapelMenu == 0) {
		plyr.status = 2;
		loadShopImage(18);
		chapelMenu = 1;
		return;
	}

	if (chapelMenu == 1)
	{
		clearShopDisplay();
		cyText(1,"Welcome to the Dungeon Chapel.");
		cyText(3,"What would you like to do?");
		bText(8,5,"(1) Pray");
		bText(8,6,"(2) Listen to a sermon");
		bText(8,7,"(3) Consult with a priest");
		bText(8,8,"(4) Make a donation");
		bText(8,9,"(0) Leave");
		updateDisplay();
		playShopMusic(3);
		string key = getSingleKey();
		if (key=="0"||key=="down") { chapelMenu=-1; }
		// options 1-4 not yet implemented
	}

	if (chapelMenu == -1) { chapelMenu=0; stopShopMusic(); leaveShop(); }
}
