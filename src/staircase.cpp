
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

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

// extern Player plyr;
// extern sf::RenderWindow App;


void staircase()
{
	//int staircaseNo = getstaircaseNo();
	int staircaseMenu = 1; // high level menu
	string str,key;
	plyr.status = 2; //shopping

	bool stairwayUp = true;
	if ((plyr.x==50)&&(plyr.y==3)&&(plyr.map==0)) { stairwayUp = false; }
	if ((plyr.x==59)&&(plyr.y==62)&&(plyr.map==0)) { stairwayUp = false; }
	if ((plyr.x==49)&&(plyr.y==17)&&(plyr.map==1)) { stairwayUp = false; }
	if ((plyr.x==16)&&(plyr.y==17)&&(plyr.map==1)) { stairwayUp = false; }
	if ((plyr.x==16)&&(plyr.y==48)&&(plyr.map==1)) { stairwayUp = false; }
	if ((plyr.x==48)&&(plyr.y==48)&&(plyr.map==1)) { stairwayUp = false; } //se
	if ((plyr.x==17)&&(plyr.y==12)&&(plyr.map==2)) { stairwayUp = false; } //level 2 to 3
	// Need to determine if up or down staircase
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (stairwayUp) { loadShopImage(6); }
	else { loadShopImage(7); }



	while (staircaseMenu > 0)
	{
		while (staircaseMenu == 1) // main menu
		{
			clearShopDisplay();
			//drawStatsPanel();

			if (stairwayUp) { cyText (1, "A stairway leads up, do you take it?"); }
			else { cyText (1, "A stairway leads down, do you take it?"); }
			cyText (3, "( es or  o)");
			SetFontColour(40, 96, 244, 255);
			cyText (3, " Y      N  ");
			SetFontColour(215, 215, 215, 255);
			updateDisplay();

			key = getSingleKey();

			//if ( key=="Y" ) { staircaseMenu = 2; }
			if ( key=="N" ) { staircaseMenu = 0; leaveShop();}
			if (key=="Y") { ; moveMapLevel(); plyr.status = 1; staircaseMenu = 0;  };
		}

	}

}
