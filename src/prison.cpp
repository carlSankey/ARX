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
#include "items.h"
#include "prison.h"
#include "lyrics.h"
#include "automap.h"




void shopPrison()
{
	int prisonerEscaped = 28;
	bool prisonerChecked = false;
	bool checkKeys = false;
	//fills all the gaps in the box for the auto map
	//these need changing to the prison XY or map special or something need updating
	setAutoMapFlag(plyr.map, 59, 27);
	/*
	setAutoMapFlag(plyr.map, 2, 14);
	setAutoMapFlag(plyr.map, 2, 15);
	setAutoMapFlag(plyr.map, 1, 13);
	setAutoMapFlag(plyr.map, 1, 14);
	setAutoMapFlag(plyr.map, 1, 15);
	*/
	plyr.status = 2; // shopping
	int prisonMenu = 1; // high level menu
	switch (plyr.prisonRelease)
	{
	case 0: //Never been to the prison
		prisonMenu = 1;
		prisonerEscaped = 28;  //Image with ozob still chained
		break;

	case 1: //Been but didnt release
		prisonMenu = 1;
		prisonerEscaped = 28;  //Image with ozob still chained
		break;

	case 2: //Been and released prisoner
		prisonMenu = 3;
		prisonerEscaped = 29;  //Image with ozob still chained
		break;
	}
	loadShopImage(prisonerEscaped ); // 28 is the Prison 29 is Prison released cast converts bool to 0,1
	
	//determine the menu to go to first 

	string str, key;
	while (prisonMenu > 0)
	{
		while (prisonMenu == 1) // main menu
		{
			
			while (!prisonerChecked)
			{
				clearShopDisplay();
				if (plyr.prisonRelease == 0)
				{
					cyText(1, "Hail, Friend. I am Ozob, Acrinimiril's.");
					cyText(3, "former Apprentisce. I was imprisoned here.");
					cyText(5, "by my Master's killers. Unlock my chains");
					cyText(7, "and i shall greatly reward thee.");

				}
				else
				{
					cyText(1, plyr.name + ", hast thou");
					cyText(3, "returned to rescue me");
				}
				cyText(9, "< Press any key to continue >");
				updateDisplay();
				key = getSingleKey();
				 if ((key != "") && (key != "up") && (key != "down") && (key != "I") && (key != "K")) { prisonerChecked = true; }
			}
			//updateDisplay();

			prisonMenu = 2;
		} //Full Prison
		
		while (prisonMenu == 2) // main menu
		{
			clearShopDisplay();
			cyText(1, "There is a prisoner chained here.");
			cyText(3, "What do you do");

			bText(8, 7, "(1) Unlock his chains with a key");
			bText(12, 8, "(0) Leave");


			updateDisplay();

			key = getSingleKey();

			if (key == "1") {
				checkKeys = true;
			}
			if (key == "0") { prisonMenu = 0; }
			if (key == "down") { prisonMenu = 0; }

			while (checkKeys)
			{
				clearShopDisplay();
				if (plyr.keys == 0 && !checkForQuestItem(187)) //Need to do something better here 187 is the silverkey
				{
					str = "You have no keys";
				}
				else if (plyr.keys > 0 && !checkForQuestItem(187))
				{
					str = "None of your keys fit the lock";
				}
				cyText(1, str);
				updateDisplay();
				key = getSingleKey();
				if ((key != "") && (key != "up") && (key != "down") && (key != "I") && (key != "K")) {
					checkKeys = false; prisonMenu = 2;
				}
			}

		}
		while (prisonMenu == 3) // main menu
		{
			
			

			clearShopDisplay();
			
		}
		while (prisonMenu == 4) // main menu
		{
			clearShopDisplay();
			if (plyr.prisonRelease )
			{
				str = "This is an empty cell." ;
				cyText(2, str);
				bText(8, 7, "(0) Leave");
				updateDisplay();
				key = getSingleKey();
				if (key == "0") { prisonMenu = 0; }
				if (key == "down") { prisonMenu = 0; }
			}
			updateDisplay();
		} //Empty Prison
	}

	//plyr.prisonVisited = true;
	if (plyr.prisonRelease == 0) { plyr.prisonRelease = 1; }
	leaveShop();
}


void prisonMessage(string str)
{
	string key;
	bool keyNotPressed = true;
	while (keyNotPressed)
	{
		
		cyText(1, str);
		updateDisplay();
		key = getSingleKey();
		if (key > "") { keyNotPressed = false; }
	}
}