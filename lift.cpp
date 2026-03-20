
#include <SFML/Graphics.hpp>

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

// extern Player plyr;
// extern sf::RenderWindow App;




void shopLift()
{
	int liftMenu = 1; // high level menu
	string str,key;
	plyr.status = 2; // shopping

	//setAutoMapFlag(plyr.map, 58, 3);


	loadShopImage(22);

	while (liftMenu > 0)
	{
		while (liftMenu == 1) // main menu
		{
			clearShopDisplay();
			cyText (1, "In the elevator you find three buttons.");
			cyText (3, "Which do you press?");
			bText (11,5, "(1) The red button");
			bText (11,6, "(2) The green button");
			bText (11,7, "(3) The blue button");
			updateDisplay();


			key = getSingleKey();


			if ( key=="0" ) { liftMenu = 0; }
			if ( key=="down" ) { liftMenu = 0; }
		}



	}
	leaveShop();
}

