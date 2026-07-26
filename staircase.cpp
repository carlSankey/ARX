#include <string>
#include <iostream>
#include <sstream>

#include "player.h"
#include "font.h"
#include "display.h"
#include "game.h"
#include "misc.h"
#include "staircase.h"
#include "level.h"

// Per-frame state machine. Called each frame by checkShop while plyr.status==2.
void staircase()
{
	static int staircaseMenu = 0;
	static bool stairwayUp = true;

	// Entry: initialise on first call
	if (staircaseMenu == 0) {
		stairwayUp = true;
		if ((plyr.x==50)&&(plyr.y==3)&&(plyr.map==0)) stairwayUp=false;
		if ((plyr.x==59)&&(plyr.y==62)&&(plyr.map==0)) stairwayUp=false;
		if ((plyr.x==49)&&(plyr.y==17)&&(plyr.map==1)) stairwayUp=false;
		if ((plyr.x==16)&&(plyr.y==17)&&(plyr.map==1)) stairwayUp=false;
		if ((plyr.x==16)&&(plyr.y==48)&&(plyr.map==1)) stairwayUp=false;
		if ((plyr.x==48)&&(plyr.y==48)&&(plyr.map==1)) stairwayUp=false;
		if ((plyr.x==17)&&(plyr.y==12)&&(plyr.map==2)) stairwayUp=false;
		plyr.status = 2;
		if (stairwayUp) loadShopImage(6); else loadShopImage(7);
		staircaseMenu = 1;
		return;
	}

	if (staircaseMenu == 1)
	{
		clearShopDisplay();
		if (stairwayUp) cyText(1,"A stairway leads up, do you take it?");
		else            cyText(1,"A stairway leads down, do you take it?");
		cyText(3,"( es or  o)");
		SetFontColour(40,96,244,255); cyText(3," Y      N  "); SetFontColour(215,215,215,255);
		updateDisplay();
		string key = getSingleKey();
		if (key=="N") { staircaseMenu=-1; }
		if (key=="Y") { moveMapLevel(); plyr.status=1; staircaseMenu=-1; }
	}

	if (staircaseMenu == -1) { staircaseMenu=0; leaveShop(); }
}
