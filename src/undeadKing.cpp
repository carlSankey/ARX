
#include <SFML/Graphics.hpp>

//#include <string>
//#include <iostream>
//#include <sstream>

#include "player.h"
//#include "font.h"
#include "display.h"
#include "game.h"
#include "misc.h"
#include "undeadKing.h"
#include "automap.h"
#include "items.h"


void shopUndeadKing()
{
	string str,key;
	plyr.status = 2; // shopping

	setAutoMapFlag(plyr.map, 13, 26);
	setAutoMapFlag(plyr.map, 14, 26);
	setAutoMapFlag(plyr.map, 13, 27);
	setAutoMapFlag(plyr.map, 14, 27);

	loadShopImage(24);

    moduleMessage("As you gaze at the wall of the crypt@@the ghostly figure of King Danjor@@appears.  The apparition speaks:");
    if (plyr.undeadKingVisited)
    {
        moduleMessage("I can tell you no more@@than you already know.");
    }
    else
    {
        plyr.undeadKingVisited = true;
        moduleMessage("Long ago, on a distant soil,@@The Keepers came. What became of@@my home, I know not. I awoke in");
        moduleMessage("a strange world. In it were some@@like myself, but most were strange@@creatures. Many among us forsook the");
        moduleMessage("Way of Knowledge and became sloth,@@turning to thievery and murder for@@their daily bread. Only I and seven");
        moduleMessage("others remained true. We learned to@@look behind the mask of this world,@@and to see The Keepers, who brought");
        moduleMessage("us here to quarrel and fight amongst@@ourselves for their amusement. We@@taught others our knowledge and stole");
        moduleMessage("the weapons of light from the Keepers.@@A great battle was made; but, in the@@end, we were defeated.");
        moduleMessage("Lest our dreams of home and freedom@@die with us on this alien soil,@@The Seven took an oath so strong");
        moduleMessage("that it bound them beyond death:@@We shall await one that will have@@the strength to carry on our hope.");
        string mesText = "You are that one,@@" + plyr.name + ".@@Other than the gifts that you have";
        moduleMessage(mesText);
        moduleMessage("received from The Seven, I can only@@offer you a portion of the staff of@@Acrinimiril. The Keepers consider");
        moduleMessage("Acrinimiril mad and are not@@aware of his true knowledge.@@Seek this knowledge!");
        // Create the staff piece if it's not already in players inventory
        if (!checkForQuestItem(6)) { int itemRef = createQuestItem(6); itemBuffer[itemRef].location = 10; }
    }


	// Move the player outside the Undead Palace
	plyr.x = 14;
	plyr.y = 28;
	plyr.z_offset=1.6f; // position player just outside door
	plyr.status = 1; // explore
}

