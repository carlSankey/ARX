#include <string>
#include <iostream>
#include <sstream>

#include "player.h"
#include "font.h"
#include "display.h"
#include "game.h"
#include "misc.h"
#include "lift.h"
#include "automap.h"

// Per-frame state machine. Called each frame by checkShop while plyr.status==2.
void shopLift()
{
	static int liftMenu = 0;

	if (liftMenu == 0) {
		plyr.status = 2;
		loadShopImage(22);
		liftMenu = 1;
		return;
	}

	if (liftMenu == 1)
	{
		clearShopDisplay();
		cyText(1,"In the elevator you find three buttons.");
		cyText(3,"Which do you press?");
		bText(11,5,"(1) The red button");
		bText(11,6,"(2) The green button");
		bText(11,7,"(3) The blue button");
		updateDisplay();
		string key = getSingleKey();
		if (key=="0"||key=="down") { liftMenu=-1; }
		// buttons 1,2,3 have no effect yet - placeholder
	}

	if (liftMenu == -1) { liftMenu=0; leaveShop(); }
}
