#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <iostream>

#include "game.h"
#include "globals.h"

#include "dev.h"
#include "player.h"
#include "font.h"
#include "display.h"
#include "level.h"
#include "misc.h"
#include "constants.h"

std::string getMapDescriptionByScenarioNumber(int scenarioNumber) {
	for (int i = 0; i < noOfMaps; ++i) {
		if (maps[i].scenarioNumber == scenarioNumber) {
			return maps[i].description;
		}
	}
	return "Scenario not found"; // Return a default message if no match is found
}


void scenarioEntrance(int scenarioNumber)
{



	// The doors between scenarios
	// ScenarioNumber is the going to Scenario (also the plyr.special)
	int scenarioMenu = 1;
	string str, key;

	str = getMapDescriptionByScenarioNumber(scenarioNumber);

	if (scenarioNumber >= 400 && scenarioNumber <= 499) { loadShopImage(30); }
	else { loadShopImage(31); }
	
	
	while (scenarioMenu > 0)
	{
		while (scenarioMenu == 1) // main menu
		{
			clearShopDisplay();
			//drawStatsPanel();

			cyText(0, "You are at an entrance to");
			cyText(3, "Are you sure you want to enter this@scenario?");
			cyText(1, str);
			cyText(5, "( es or  o)");
			SetFontColour(40, 96, 244, 255);
			cyText(5, " Y      N  ");
			SetFontColour(215, 215, 215, 255);
			updateDisplay();

			key = getSingleKey();

			//if ( key=="Y" ) { staircaseMenu = 2; }
			if (key == "N") { scenarioMenu = 0; leaveShop(); }
			if (key == "Y") { ; moveMapLevel(); plyr.status = 1; scenarioMenu = 0; };
		}
		
	}
	

}
