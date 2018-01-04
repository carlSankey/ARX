
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <iostream>
#include <sstream>


#include "player.h"
#include "font.h"
#include "display.h"
#include "game.h"
#include "misc.h"
#include "retreat.h"
#include "automap.h"

//// extern Player plyr;
// extern sf::RenderWindow App;

inline std::string concat( int n, const char* str )
{
std::ostringstream ss;
ss << n;
ss << str;
return ss.str();
}




void shopRetreat()
{

	int retreatMenu = 1; // high level menu
	int hoursSlept = 0; // number of hours slept
	int roomChoice = 0; // spot by door, drafty spot or by fire
	string str,key;
	plyr.status = 2; // shopping

	setAutoMapFlag(plyr.map, 55, 3);
	setAutoMapFlag(plyr.map, 56, 3);
	setAutoMapFlag(plyr.map, 55, 2);
	setAutoMapFlag(plyr.map, 56, 2);

	loadShopImage(1);

	while (retreatMenu > 0)
	{
		bool firstVisit = false;
		if (plyr.retreatFriendship==5) { firstVisit = true; }


		int coppers = 0;
		while (retreatMenu == 1) // main menu
		{
			clearShopDisplay();
			if (firstVisit) { str = "Welcome, Stranger, to our meager hovel."; }
			else
			{
				if (plyr.retreatFriendship>1)  { str = "Greetings, Explorer. Welcome back!"; }
				if (plyr.retreatFriendship<2)  { str = "Art thou back again?"; }
			}
			cyText (1, str);
			cyText (3, "Woulds't thou like to");
			bText (15,5, "(1) Sleep");
			bText (15,6, "(0) Leave");
			updateDisplay();

			key = getSingleKey();

			if ( key=="1" ) { retreatMenu = 2; }
			if ( key=="0" ) { retreatMenu = 0; }
		}

		while (retreatMenu == 2) // how many coppers?
		{
			coppers = inputValue();

			if (coppers>0)
			{
				if (checkCoins(0,0,coppers))
				{
					deductCoins(0,0,coppers);
					retreatMenu = 4;
				}
				else
				{
					retreatMenu = 3; // insufficient funds
				}
			}

			if ( coppers==0 ) { retreatMenu = 1; }
		}

		while (retreatMenu == 3) // insufficient funds
		{
			clearShopDisplay();
			cyText (2, "Thou has offered more@@than thou hast!");
			updateDisplay();
			key = getSingleKey();
			if ( key!="" ) { retreatMenu = 1; }
		}

		while (retreatMenu == 4) // coppers offering message
		{
			clearShopDisplay();
			if (coppers < 15) { str = "That's not enough to@@cover the cost of delousing!@@Take the drafty spot near the door!"; roomChoice = 0; plyr.retreatFriendship--; if (plyr.retreatFriendship<0) plyr.retreatFriendship=0; }
			if ((coppers > 14) && (coppers < 31)) { str = "Pinching pennies, eh?@@Well, lie down in that corner!"; roomChoice = 1; }
			if (coppers > 30) { str = "Thou art generous to a fault!@@Take this place next to the fire!"; roomChoice = 2; plyr.retreatFriendship++; if (plyr.retreatFriendship>4) plyr.retreatFriendship=4;}
			cyText (2, str);
			updateDisplay();
			key = getSingleKey();
			if ( key!="" ) { retreatMenu = 5; }
		}

		while (retreatMenu == 5) // Sleeping
		{
			key="";
			while ((key=="") && (hoursSlept<8))
			{
				clearShopDisplay();
				cyText (3, "Thou sleepest.");
				cyText (6, "(Press SPACE when ready to awaken.)");
				updateDisplay();
				key = getSingleKey();
				//if ( key!="" ) {  }

				int roomProb = 0;
				if (roomChoice == 0) roomProb = 40;
				if (roomChoice == 1) roomProb = 60;
				if (roomChoice == 2) roomProb = 80;
				int actualSleepProb = randn(0,100);
				if (actualSleepProb <= roomProb )
				{
					plyr.hp = plyr.hp+randn(1,5);
					if (plyr.hp > plyr.maxhp) { plyr.hp = plyr.maxhp; }
				}
				addHour();
				hoursSlept++;
				sf::sleep(sf::seconds(1));
			}
			retreatMenu = 6;
		}

		while (retreatMenu == 6) // Dreams troubled...
		{
			clearShopDisplay();
			if (plyr.alignment<129) { str = "Thy sleep is troubled by evil dreams.@@Thou wakest in a cold sweat!"; }
			if (plyr.alignment>128) { str = "Thy dreams are warm and bright.@@Thou wakest rested and refreshed."; }
			cyText (2, str);
			updateDisplay();
			key = getSingleKey();
			if ( key!="" ) { retreatMenu = 7; }
		}

		while (retreatMenu == 7) // You slept for...
		{
			string monthDesc;
			switch(plyr.months)
			{
				case 1:
					monthDesc = "Rebirth"; break;
				case 2:
					monthDesc = "Awakening"; break;
				case 3:
					monthDesc = "Winds"; break;
				case 4:
					monthDesc = "Rains"; break;
				case 5:
					monthDesc = "Sowings"; break;
				case 6:
					monthDesc = "First Fruits"; break;
				case 7:
					monthDesc = "Harvest"; break;
				case 8:
					monthDesc = "Final Reaping"; break;
				case 9:
					monthDesc = "The Fall"; break;
				case 10:
					monthDesc = "Darkness"; break;
				case 11:
					monthDesc = "Cold Winds"; break;
				case 12:
					monthDesc = "Lights"; break;
			}

			clearShopDisplay();
			str = "Thou hast slept for "+itos(hoursSlept)+ " hours.";
			cyText (1, str);
			str = "It is day "+itos(plyr.days);
			cyText (3, str);
			str = "in the month of " + monthDesc;
			cyText (4, str);
			str = "year " + itos(plyr.years) + " since abduction.";
			cyText (5, str);
			updateDisplay();
			key = getSingleKey();
			if ( key!="" ) { retreatMenu = 0; }
		}

	}
	if (plyr.retreatFriendship==5) { plyr.retreatFriendship = 2; }
	leaveShop();
}


int inputValue()
{
	int itemQuantity = 0;

	string str, key;
	string inputText = "";
	int maxNumberSize = 6;
	bool enterKeyNotPressed = true;
	while ( enterKeyNotPressed )
	{

		clearShopDisplay();
		cyText (0, "Thou mayest have a spot on@the floor for a small donation.@@How many coppers woulds't thou give?");

		str = ">" + inputText + "_";
		bText(17,5, str);
		updateDisplay();
		key = getSingleKey();
		if ( (key=="0") || (key=="1") || (key=="2") || (key=="3") || (key=="4") || (key=="5") || (key=="6") || (key=="7") || (key=="8") || (key=="9") )
		{
			int numberLength = inputText.size();
			if (numberLength < maxNumberSize) { inputText = inputText + key; }
		}
		if (key=="BACKSPACE")
		{
			int numberLength = inputText.size();
			if (numberLength!=0)
			{
			  int numberLength = inputText.size();
			  inputText = inputText.substr(0,(numberLength-1));
			}
		}
		if (key=="RETURN") { enterKeyNotPressed = false; }
		if (key=="ESC") { itemQuantity = 0; enterKeyNotPressed = false; }
	}
	itemQuantity = atoi(inputText.c_str());

	return itemQuantity;
}
