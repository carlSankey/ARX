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

// extern Player plyr;


void shopFountain()
{

	int fountainMenu = 1; // high level menu
	string str,key;
	plyr.status = 2; // shopping
	loadShopImage(19);

	if (plyr.map==1)
	{
		setAutoMapFlag(plyr.map, 13, 52);
		setAutoMapFlag(plyr.map, 14, 52);
		setAutoMapFlag(plyr.map, 15, 52);
		setAutoMapFlag(plyr.map, 13, 53);
		setAutoMapFlag(plyr.map, 14, 53);
		setAutoMapFlag(plyr.map, 15, 53);
		setAutoMapFlag(plyr.map, 13, 54);
		setAutoMapFlag(plyr.map, 14, 54);
		setAutoMapFlag(plyr.map, 15, 54);
	}

	if (plyr.map==2)
	{
		setAutoMapFlag(plyr.map, 24, 16);
		setAutoMapFlag(plyr.map, 25, 16);
		setAutoMapFlag(plyr.map, 24, 15);
		setAutoMapFlag(plyr.map, 25, 15);
	}

	if (plyr.map==3)
	{
		//TODO Add level 3 fountain!
	}

	while (fountainMenu > 0)
	{
		while (fountainMenu == 1) // main menu
		{
			clearShopDisplay();
			cyText (1, "You're facing a crystal clear fountain.");
			bText (6,3, "Do you (1) Take a drink or");
			bText (13,4, "(0) Leave?");
			updateDisplay();

			key = getSingleKey();
			if ( key=="0" ) { fountainMenu = 0; }
			if ( key=="down" ) { fountainMenu = 0; }
			if ( key=="1" ) { fountainMenu = 2; }
		}


		while (fountainMenu == 2) // main menu
		{
			clearShopDisplay();

			if ( plyr.map == 1 )
			{
				int strNo = 1;
				if (strNo == 1) str = "The water tastes delicious!";
				if (strNo == 2) str = "Ahh! The water tastes@@absolutely marvelous!";
				if (strNo == 3) str = "The water is cool, clear and@@really hits the spot!";
				if (strNo == 4) str = "You feel rejuvenated!";
				if (strNo == 5) str = "Ahh! Now you feel much better!";
				plyr.hp = plyr.maxhp;
				plyr.thirst = 0;
				//Increase consumption FULL and too bloated to drink message
			}

			if ( plyr.map == 2)
			{
				int strNo = 2;
				if (strNo == 1) str = "Your health improves rapidly!";
				if (strNo == 2) str = "Every cell in your body seems purified!";
				if (strNo == 3) str = "You feel a wave of relief@@sweep over you!";
				//TODO Diseases cleansed!
				//Increase consumption FULL
				plyr.thirst = 0;
			}

			if ( plyr.map == 3 )
			{
				str = "The water tastes delicious!";
				//TODO Fatigue removed!
				//Increase consumption FULL
				plyr.thirst = 0;
			}
			cyText (3, str);
			updateDisplay();

			key = getSingleKey();
			if ( key=="SPACE" ) { fountainMenu = 1; }
		}

	}

	leaveShop();
}

