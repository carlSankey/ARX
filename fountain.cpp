#include <string>
#include <iostream>
#include <sstream>

#include "player.h"
#include "font.h"
#include "display.h"
#include "game.h"
#include "misc.h"
#include "fountain.h"
#include "automap.h"

// Per-frame state machine. Called each frame by checkShop while plyr.status==2.
void shopFountain()
{
	static int fountainMenu = 0;

	if (fountainMenu == 0) {
		plyr.status = 2;
		loadShopImage(19);
		if (plyr.map==1) {
			setAutoMapFlag(plyr.map,13,52); setAutoMapFlag(plyr.map,14,52); setAutoMapFlag(plyr.map,15,52);
			setAutoMapFlag(plyr.map,13,53); setAutoMapFlag(plyr.map,14,53); setAutoMapFlag(plyr.map,15,53);
			setAutoMapFlag(plyr.map,13,54); setAutoMapFlag(plyr.map,14,54); setAutoMapFlag(plyr.map,15,54);
		}
		if (plyr.map==2) {
			setAutoMapFlag(plyr.map,24,16); setAutoMapFlag(plyr.map,25,16);
			setAutoMapFlag(plyr.map,24,15); setAutoMapFlag(plyr.map,25,15);
		}
		fountainMenu = 1;
		return;
	}

	string str;

	if (fountainMenu == 1)
	{
		clearShopDisplay();
		cyText(1,"You're facing a crystal clear fountain.");
		bText(6,3,"Do you (1) Take a drink or");
		bText(13,4,"(0) Leave?");
		updateDisplay();
		string key = getSingleKey();
		if (key=="0"||key=="down") { fountainMenu=-1; }
		if (key=="1") { fountainMenu=2; }
	}
	else if (fountainMenu == 2)
	{
		clearShopDisplay();
		if (plyr.map==1) { str="The water tastes delicious!"; plyr.hp=plyr.maxhp; plyr.thirst=0; }
		if (plyr.map==2) { str="Every cell in your body seems purified!"; plyr.thirst=0; }
		if (plyr.map==3) { str="The water tastes delicious!"; plyr.thirst=0; }
		cyText(3,str);
		updateDisplay();
		string key = getSingleKey();
		if (key=="SPACE") { fountainMenu=1; }
	}

	if (fountainMenu == -1) { fountainMenu=0; leaveShop(); }
}
