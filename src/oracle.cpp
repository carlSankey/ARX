#include <string>
#include <iostream>
#include <sstream>

#include "player.h"
#include "font.h"
#include "display.h"
#include "game.h"
#include "misc.h"
#include "items.h"
#include "oracle.h"

// extern Player plyr;


inline std::string concat( int n, const char* str )
{
std::ostringstream ss;
ss << n;
ss << str;
return ss.str();
}

void shopOracle()
{
	// Module for Oracle

	//TODO Add penalties for Oracle on return visits and come back in month

	int oracleFailedOfferings = 0;
	int oracleMenu = 1; // high level menu
	string str,key;
	plyr.status = 2; // shopping
	//int oracleRandom = rand(0,3);

	loadShopImage(20);

	if (( plyr.oracleDay == plyr.days) && (plyr.oracleMonth == plyr.months) && (plyr.oracleYear == plyr.years )) oracleMenu = 5;

	while (oracleMenu > 0)
	{
		while (oracleMenu == 1) // main menu
		{
			clearShopDisplay();

			cyText (1, "You stand before the great and mighty");
			cyText (3, "Oracle. What will thee offer for");
			cyText (5, "words of wisdom?");
			bText (8,7, "Do you (1) make an offering");
			bText (12,8, "or (2) Leave?");

			//cyText (1, "Welcome to the Oracle.");
			//cyText (3, "What dost thou offer for my knowledge?");
			//bText (8,5, "Do you (1) make an offering");
			//bText (12,6, "or (2) Leave?");

			updateDisplay();

			key = getSingleKey();

			if ( key=="1" ) { oracleMenu = 2; }
			if ( key=="2" ) { oracleMenu = 0; }
			if ( key=="down" ) { oracleMenu = 0; }
		}

		while (oracleMenu == 2) // Offering
		{
			bool offerAccepted = false;

			int itemQuantity = 0;
			int itemRef = selectItem(3); // select an item in OFFER mode
			if (itemRef == 9999) oracleMenu=1;
			if ((itemRef > 999) && (itemRef < 1006))
			{
				itemQuantity = inputItemQuantity(3);
				// Check for item quantity!
				string offerText = "";
				switch(itemRef)
				{
					case 1000: offerText = "Food Packet(s)"; break;
					case 1001: offerText = "Water Flask(s)"; break;
					case 1002: offerText = "Torch(es)"; break;
					case 1003: offerText = "Timepiece(s)"; break;
					case 1004: offerText = "Compass(es)"; break;
					case 1005: offerText = "Key(s)"; break;
				}
				string str = "You toss the "+offerText+"@@into the flames beneath the Oracle.@@@<<< Press SPACE to continue >>>";
				oracleMessage(str);
				str = "The flames flare slightly as the@@"+offerText+" is@@consumed.@@<<< Press SPACE to continue >>>";
				oracleMessage(str);
				oracleMessage("I have no interest in your worthless@@offering!");
				oracleFailedOfferings++;
				if (oracleFailedOfferings==3) oracleMenu = 4; else oracleMenu = 1;
			}
			if ((itemRef > 1005) && (itemRef < 1012) && (itemRef!=1009))
			{
				itemQuantity = inputItemQuantity(3);
				// Check for item quantity!
				string offerText = "";
				switch(itemRef)
				{
					case 1006: offerText = "Crystal(s)"; break;
					case 1007: offerText = "Gem(s)"; break;
					case 1008: offerText = "Jewel(s)"; break;
					case 1010: offerText = "Silver"; break;
					case 1011: offerText = "Copper"; break;
				}
				string str = "You toss the "+offerText+"@@into the flames beneath the Oracle.@@@<<< Press SPACE to continue >>>";
				oracleMessage(str);
				str = "The flames flare slightly as the@@"+offerText+" is@@consumed.@@<<< Press SPACE to continue >>>";
				oracleMessage(str);
				oracleMessage("I only like gold!");
				oracleFailedOfferings++;
				if (oracleFailedOfferings==3) oracleMenu = 4; else oracleMenu = 1;
			}
			if (itemRef == 1009)
			{
				itemQuantity = inputItemQuantity(3);

				if ((itemQuantity > 4) && (plyr.gold>4))
				{
					plyr.gold-=itemQuantity;
					if (plyr.gold<0) plyr.gold = 0; // you can offer more than you have
					offerAccepted = true;
					oracleMessage("You hurl the Gold@@into the flaming Oracle pit.@@@<<< Press SPACE to continue >>>");

				}
				if (itemQuantity < 5)
				{
					plyr.gold-=itemQuantity;
					if (plyr.gold<0) plyr.gold = 0; // you can offer more than you have
					oracleMessage("You hurl the Gold@@into the flaming Oracle pit.@@@<<< Press SPACE to continue >>>");
					oracleMessage("I am sorry but your offering is not@@enough for my wisdom.@@@<<< Press SPACE to continue >>>");
					oracleFailedOfferings++;
					if (oracleFailedOfferings==3) oracleMenu = 4; else oracleMenu = 1;
				}
				if (offerAccepted) oracleMenu = 3;
			}

			if (itemRef < 100)
			{
				string itemDesc = getItemDesc(itemRef);
				if (itemDesc=="Reforged Ring")
				{
					str = "The flames flare fiercely as the@@"+itemDesc+" is@@consumed.@@<<< Press SPACE to continue >>>";
					oracleMessage(str);
					oracleMessage("You have just destroyed the ring of@@Evil!  For ridding the world of@@this evil artifact I shall reward you!");
					plyr.sta += 5; if (plyr.sta > 255) plyr.sta = 255;
					plyr.chr += 5; if (plyr.chr > 255) plyr.chr = 255;
					plyr.wis += 5; if (plyr.wis > 255) plyr.wis = 255;
					plyr.alignment += 5; if (plyr.alignment > 255) plyr.alignment = 255;
					moveItem(itemRef,0); // move the Reforged Ring to the void
					oracleMenu = 1;
				}
				else
				{
					string str = "You toss the "+itemDesc+"@@into the flames beneath the Oracle.@@@<<< Press SPACE to continue >>>";
					oracleMessage(str);
					str = "The flames flare slightly as the@@"+itemDesc+" is@@consumed.@@<<< Press SPACE to continue >>>";
					oracleMessage(str);
					oracleMessage("I have no interest in your worthless@@offering!");
					moveItem(itemRef,0); // move the inventory item to the void
					oracleFailedOfferings++;
					if (oracleFailedOfferings==3) oracleMenu = 4; else oracleMenu = 1;
				}

			}

		}

		while (oracleMenu == 3) // Give advice
		{
			if (plyr.oracleQuestNo==0) oracleMessage("The oracle says:@@Seek the prison under the palace.@@Free the prisoner with a silver key.@@<<< Press SPACE to continue >>>");
			if (plyr.oracleQuestNo==1) oracleMessage("Bring me the ring that the Goblins &@Trolls war over. The Goblins are to@the Northwest and the Trolls are to the@South. The Smithy on the second level@will reforge the ring halves. Be quick@returning the ring for it is evil.@@<<< Press SPACE to continue >>>");
			if (plyr.oracleQuestNo==2) oracleMessage("On the second level find the one in@the room of glass. Don't start off@without a touch of class.@@<<< Press SPACE to continue >>>");
			if (plyr.oracleQuestNo==3) oracleMessage("Cross the river Stonz at midnight. On@the south side of the river awaits@a king. Pass the seven who guard to@further your cause.@@<<< Press SPACE to continue >>>");
			if (plyr.oracleQuestNo==4) oracleMessage("A fearsome face lurks by the Dragon's@lair. Answer his queries for a@valuable ally. Remember \"Xebec's Demise\".@@<<< Press SPACE to continue >>>");
			if (plyr.oracleQuestNo==5) oracleMessage("Answer three of the Riddler's riddles@to help you find what the Dragon@seeks, for the Dragon is a mighty foe.@@<<< Press SPACE to continue >>>");
			if (plyr.oracleQuestNo==6) oracleMessage("Return all three staff pieces to@Acrinimiril to receive the key to@Death's Door.@@<<< Press SPACE to continue >>>");
			if (plyr.oracleQuestNo==7) oracleMessage("A mirror is proof against a stony stare@@@@<<< Press SPACE to continue >>>");
			if (plyr.oracleQuestNo==8) oracleMessage("Congratulations! The aliens no longer@gain amusement from The Dungeon! Keep up the good work and you shall@be able to return to your home world.@@<<< Press SPACE to continue >>>");
			if (plyr.oracleQuestNo==9) oracleMessage("Alas, I have told you all that I know@about your future. The future is in@your hands now.@@<<< Press SPACE to continue >>>");
			oracleMessage("May the gods be with you!");
			if (plyr.oracleQuestNo<10) plyr.oracleQuestNo++; // Up to maxiumum of x quests...
			oracleMenu = 0;
		}

		while (oracleMenu == 4) // Return tomorrow 3 failed offering attempts
		{
			oracleMessage("Return tomorrow you have tried my patience.@@<<< Press SPACE to continue >>>");
			plyr.oracleReturnTomorrow = true;
			plyr.oracleDay = plyr.days;
			plyr.oracleMonth = plyr.months;
			plyr.oracleYear = plyr.years;
			oracleMenu = 0;
		}

		while (oracleMenu == 5) // Return tomorrow no more to say
		{
			oracleMessage("I can give words of wisdom but once@@per day. Return tomorrow.@@@<<< Press SPACE to continue >>>");
			plyr.oracleReturnTomorrow = true;
			plyr.oracleDay = plyr.days;
			plyr.oracleMonth = plyr.months;
			plyr.oracleYear = plyr.years;
			oracleMenu = 0;
		}



	}
	leaveShop();
}


void oracleMessage(string str)
{
	string key;
	bool keyNotPressed = true;
	while (keyNotPressed)
	{
		clearShopDisplay();
		cyText (1, str);
		updateDisplay();
		key = getSingleKey();
		if ( key == "SPACE" ) { keyNotPressed = false; }
	}
}
