#include "game.h"
#include "player.h"
#include "font.h"
#include "display.h"
#include "misc.h"
#include "arena.h"

void arenaSouthernEntrance()
{
    int menu = 1; // high level menu
	string str,key;
	plyr.status = 2; // special module

	loadShopImage(25);

	while (menu > 0)
	{
		while (menu == 1) // main menu
		{
			clearShopDisplay();
			cyText (1, "You are at the southern entrance");
            cyText (3, " to the Arena of Xebec's Demise.");
			cyText (5, "Dost thou wish to?");
			bText (9,7, "  (1) Enter the Arena");
			bText (9,8, "  (0) Leave");
			updateDisplay();

			key = getSingleKey();
			if ( key== "0" ) { menu = 0; }
			if ( key== "1" ) { menu = 2; }
		}

		while (menu == 2) // Enter the Arena attempt
        {
            key="";
            while (key=="")
            {
                clearShopDisplay();
                if (plyr.gender==1) str = "Away with you knave!";
                else str = "Away with you scullion!";
                cyText (2,str);
                cyText (4,"Only those of the great houses");
                cyText (6,"may enter here.");
                updateDisplay();
                key = getSingleKey();
                if ( key!="" ) { menu = 0; } // Thrown out
            }
        }

	} // end main while loop

	leaveShop();
}


void arenaNorthernEntrance()
{
    int menu = 1; // high level menu
	string key;
	plyr.status = 2; // special module

	//loadShopImage(25);

	while (menu > 0)
	{
		while (menu == 1) // main menu
		{
			clearShopDisplay();
			cyText (1, "You are at the northern entrance");
            cyText (3, " to the Arena of Xebec's Demise.");
			cyText (5, "Dost thou wish to?");
			bText (9,7, "  (1) Enter the Arena");
			bText (9,8, "  (0) Leave the Arena");
			updateDisplay();

			key = getSingleKey();
			if ( key== "0" )
            {
                plyr.x = 16;
                plyr.y = 1;
                plyr.z_offset = 1.0;
                plyr.scenario = 0;
                plyr.status = 1; // explore
                menu = 0;
            }

			if ( key== "1" )
            {
                plyr.x = 16;
                plyr.y = 3;
                plyr.z_offset = 1.0;
                plyr.scenario = 2;
                plyr.status = 1; // explore
                menu = 0;
            }
		}

	} // end main while loop

}


    void arenaWesternEntrance()
    {
        string key="";
        while (key!="0")
        {
            clearShopDisplay();
            cyText (1, "You are at the western entrance");
            cyText (3, "to the Arena. A heavy portcullis");
			cyText (5, "blocks the entrance.");
			cyText (8, "(0) Leave");
            updateDisplay();
            key = getSingleKey();
        }
        leaveShop();
}
