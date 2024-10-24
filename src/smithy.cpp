
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <stdio.h>
#include <iomanip>

#include <string>
#include <iostream>
#include <sstream>
//#include <algorithm>

#include "constants.h"
#include "player.h"
#include "font.h"
#include "display.h"
#include "game.h"
#include "encounter.h"
#include "misc.h"
#include "smithy.h"
#include "items.h"
#include "lyrics.h"
#include "actor.h" // For weapons

using namespace std;

unsigned char citySmithyBinary[noOfCitySmithyFile];

extern sf::Clock        clock1;
extern int              iCounter;

sf::Music smithyMusic;

int itemChoice;
int itemNo, itemCost;
int menuStartItem;
int maxMenuItems = 6;
int smithyNo;

struct Smithy
{
   string name;
   float minimumPriceFactor;
   float initialPriceFactor;
   int location; // match with location text description number
   int openingHour;
   int closingHour;
};

Smithy Smithies[4] =
{
	{"Sharp Weaponsmiths",		1.25f,		1.65f,	55,		4,	20},
	{"Occum's Weaponsmiths",	1.10f,		1.35f,	56,		5,	21},
	{"Best Armorers",			1.50f,		2.40f,	57,		8,	19},
	{"Knight's Armorers",		1.60f,		2.35f,	58,		11,	15}

};

bool smithyWaresCheck[4][23]; // markers used to check for duplicate items

extern int smithyDailyWares[4][10];

struct smithyItem
{

	string name;
	int type; // 177 - armour, 178 - weapon
	int basePrice;
	int itemRef;
};

smithyItem smithyWares[23] =
{
	{"a Stiletto",		178,113,	0xAA},
	{"a Dagger",		178,129,	0xCB},
	{"a Whip",			178,396,	0xE9},
	{"a War Net",		178,908,	0x24},
	{"Padded Armor",	177,2200,	1},
	{"a Small Shield",	178,2460,	0x86},
	{"a Shortsword",	178,3146,	0x105},
	{"a Shield",		178,4290,	0x68},
	{"a Flail",			178,4620,	0x128},
	{"Leather Armor",	177,4840,	2},
	{"a Spiked Shield",	178,6160,	0x43},
	{"a Battle Axe",	178,16930,	0x145},
	{"Studded Armor",	177,7260,	3},
	{"a Sword",			178,7680,	0x167},
	{"a Tower Shield",	178,9488,	0x0},
	{"Ring Mail",		177,10010,	4},
	{"a Battle Hammer",	178,10285,	0x184},
	{"a Longsword",		178,11193,	0x1A9},
	{"Scale Mail",		177,14245,	5},
	{"Splint Mail",		177,18975,	6},
	{"Chain Mail",		177,24640,	7},
	{"Banded Armor",	177,32000,	8},
	{"Plate Armor",		177,41500,	9}
};

/*
smithyItem smithyWares[23] =
{
	{"a Stiletto",		178,113,	61},
	{"a Dagger",		178,129,	62},
	{"a Whip",			178,396,	63},
	{"a War Net",		178,908,	57},
	{"Padded Armor",	177,2200,	1},
	{"a Small Shield",	178,2460,	60},
	{"a Shortsword",	178,3146,	64},
	{"a Shield",		178,4290,	59},
	{"a Flail",			178,4620,	65},
	{"Leather Armor",	177,4840,	2},
	{"a Spiked Shield",	178,6160,	58},
	{"a Battle Axe",	178,16930,	66},
	{"Studded Armor",	177,7260,	3},
	{"a Sword",			178,7680,	67},
	{"a Tower Shield",	178,9488,	56},
	{"Ring Mail",		177,10010,	4},
	{"a Battle Hammer",	178,10285,	68},
	{"a Longsword",		178,11193,	69},
	{"Scale Mail",		177,14245,	5},
	{"Splint Mail",		177,18975,	6},
	{"Chain Mail",		177,24640,	7},
	{"Banded Armor",	177,32000,	8},
	{"Plate Armor",		177,41500,	9}
};
*/


void shopSmithy()
{
	if (plyr.timeOfDay == 1) { loadShopImage(8); } else { loadShopImage(9); }

	int offerStatus = 0; // 0 is normal, 1 is demanding, 2 is bartering
	int offerRounds = 0;
	int  itemLowestCost, smithyOffer;

	if (plyr.timeOfDay == 1) { loadShopImage(8); } else { loadShopImage(9); }

	smithyNo = getSmithyNo();
	bool musicPlaying = false;
	int smithyMenu = 1; // high level menu
	string str,key;
	plyr.status = 2; // shopping
	menuStartItem = 0; // menu starts at item 0
	if ((Smithies[smithyNo].closingHour<=plyr.hours) || (Smithies[smithyNo].openingHour>plyr.hours)) {smithyMenu = 5;}


	while (smithyMenu > 0)
	{
		while (smithyMenu == 5) // closed
		{
			smithyDisplayUpdate();
			cyText (1, "Sorry, we are closed. Come back@during our working hours.");
			str = "We are open from " + itos(Smithies[smithyNo].openingHour) + ":00 in the morning@to " + itos(Smithies[smithyNo].closingHour) + ":00 in the evening.";
			if (Smithies[smithyNo].closingHour == 15) { str = "We are open from " + itos(Smithies[smithyNo].openingHour) + ":00 in the morning@to " + itos(Smithies[smithyNo].closingHour) + ":00 in the afternoon."; }
			cyText (4, str);
			cyText (9,"( Press a key )");
			updateDisplay();

			key = getSingleKey();
			if ((key!="") && (key!="up")) { smithyMenu = 0; }
		}

		while (smithyMenu == 1) // main menu
		{
			smithyDisplayUpdate();
			bText (13,0, "Welcome Stranger!");
			bText (7,3, "Do you wish to see our wares?");
			cyText (5, "( es or  o)");
			SetFontColour(40, 96, 244, 255);
			cyText (5, " Y      N  ");
			SetFontColour(215, 215, 215, 255);
			displayCoins();
			updateDisplay();

			if (!musicPlaying)
			{
				//int Random = randn(0, 2);
				if (plyr.musicStyle==0)
				{ smithyMusic.openFromFile("data/audio/armor.ogg"); }
				else
				{ smithyMusic.openFromFile("data/audio/B/armor.ogg"); }
				loadLyrics("armor.txt");
				smithyMusic.play();
				musicPlaying = true;
			}

			key = getSingleKey();
			if ( key=="Y" ) { smithyMenu = 2; }
			if ( key=="N" ) { smithyMenu = 0; }
			if ( key=="down" ) { smithyMenu = 0; }
		}

		while (smithyMenu == 2)
		{
			offerStatus = 0;
			offerRounds = 0;
			smithyDisplayUpdate();
			cyText (0, "What would you like? (  to leave)");
			SetFontColour(40, 96, 244, 255);
			cyText (0, "                      0          ");
			SetFontColour(215, 215, 215, 255);

			smithyNo = getSmithyNo();
			for (int i=0 ; i<maxMenuItems ; i++)
			{
				int itemNo = smithyDailyWares[smithyNo][menuStartItem+i];
				str = ") " + smithyWares[itemNo].name;
				//if ((smithyWares[itemNo].itemRef) > 10) { str = ") " + Weapons[smithyWares[itemNo].itemRef].name; }
				bText(3,(2+i), str); //was 4
				bText(1,(2+i), "                                 coppers");
			}
			displayCoins();

			int itemCost, x;
			for (int i=0 ; i<maxMenuItems ; i++) // Max number of item prices in this menu display
			{
				string itemCostDesc;
				x = 28;
				int itemNo = smithyDailyWares[smithyNo][menuStartItem+i];
				itemCost = static_cast<int>(Smithies[smithyNo].initialPriceFactor * smithyWares[itemNo].basePrice);
				//itemCost = smithyWares[itemNo].basePrice;
				if (itemCost<1000) { x = 30;}
				if (itemCost>999) { x = 28;}
				if (itemCost>9999) { x = 27;}
				itemCostDesc = toCurrency(itemCost);
				bText (x,(i+2),itemCostDesc);
			}

			SetFontColour(40, 96, 244, 255);
			bText (2,2, "1");
			bText (2,3, "2");
			bText (2,4, "3");
			bText (2,5, "4");
			bText (2,6, "5");
			bText (2,7, "6");
			if (menuStartItem!=0) { bText (2,1, "}"); }
			if (menuStartItem!=4) { bText (2,8, "{"); }
			SetFontColour(215, 215, 215, 255);

			updateDisplay();

			key = getSingleKey();
			if ( key=="1" ) { itemChoice = 0; smithyMenu = 20; }
			if ( key=="2" ) { itemChoice = 1; smithyMenu = 20; }
			if ( key=="3" ) { itemChoice = 2; smithyMenu = 20; }
			if ( key=="4" ) { itemChoice = 3; smithyMenu = 20; }
			if ( key=="5" ) { itemChoice = 4; smithyMenu = 20; }
			if ( key=="6" ) { itemChoice = 5; smithyMenu = 20; }
			if ( (key=="up") && (menuStartItem>0) ) { menuStartItem--; }
			if ( (key=="down") && (menuStartItem<4) ) { menuStartItem++; }
			if ( key=="ESC" ) { smithyMenu = 0; }
			if ( key=="0" ) { smithyMenu = 0; }

		}


		while (smithyMenu == 20) // buy item?
		{
			smithyNo = getSmithyNo();
			itemNo = smithyDailyWares[smithyNo][menuStartItem+itemChoice];
			itemCost = static_cast<int>(Smithies[smithyNo].initialPriceFactor * smithyWares[itemNo].basePrice);
			float tempitemcost = Smithies[smithyNo].initialPriceFactor * smithyWares[itemNo].basePrice;
			float temp = (tempitemcost/100)*75;
			itemLowestCost = static_cast<int>(temp);
			smithyOffer = itemCost;
			smithyMenu = 3;
		}


		while (smithyMenu == 3) // buy item
		{
			smithyDisplayUpdate();
			if (offerStatus==0)
			{
				str = "The cost for " + smithyWares[itemNo].name;
				cyText (0,str);
				str = "is " + toCurrency(smithyOffer) + " coppers. Agreed?";
				cyText (1,str);
			}
			if (offerStatus==1)
			{
				str = "I demand at least " + toCurrency(smithyOffer) + " silvers!";
				cyText (1,str);
			}
			if (offerStatus==2)
			{
				str = "Would you consider " + toCurrency(smithyOffer) + "?";
				cyText (1,str);
			}

			bText (11,3," ) Agree to price");
			bText (11,4," ) Make an offer");
			bText (11,5," ) No sale");
			bText (11,6," ) Leave");
			displayCoins();
			SetFontColour(40, 96, 244, 255);
			bText (11,3,"1");
			bText (11,4,"2");
			bText (11,5,"3");
			bText (11,6,"0");
			SetFontColour(215, 215, 215, 255);

			updateDisplay();

			key = getSingleKey();
			if ( key=="1" )
			{

				if (!checkCoins(0,0,smithyOffer)) { smithyMenu = 5; } // offended!
				else { smithyMenu = 4; }
			}

			if ( key=="2" ) { smithyMenu = 16; }
			if ( key=="3" ) { smithyMenu = 2; }
			if ( key=="0" ) { smithyMenu = 0; }

		}


		while (smithyMenu == 16) // what is your offer
		{
			int coppers = inputValue("What is your offer? (in coppers)",9);

			// check offer
			if ( coppers==0 ) { smithyMenu = 2; }

			if ( coppers >= itemCost )
			{
				smithyOffer = coppers; // accepted the players offer
				offerStatus = 2;
				smithyMenu = 20;
			}
			if (( coppers >= itemLowestCost ) && ( coppers < itemCost ))
			{

				offerStatus = 2;
				offerRounds++;
				if ( offerRounds > 2 ) { smithyOffer = coppers; smithyMenu = 20; }
				else
				{
					smithyOffer = randn(coppers,itemCost);
					itemLowestCost = coppers;
					smithyMenu = 3;
				}
			}
			if (( coppers < itemLowestCost ) && (coppers > 0))
			{
				offerStatus=1;
				offerRounds++;
				smithyOffer = itemLowestCost;
				if ( offerRounds > 1 ) { smithyMenu = 19; }
				else	{ smithyMenu = 3; }
			}
		}




		while (smithyMenu == 20) // Offer accepted (subject to funds check)
		{
			smithyDisplayUpdate();
			cText ("Agreed!");
			updateDisplay();
			key = getSingleKey();

			if ( key!="" )
			{
				if (!checkCoins(0,0,smithyOffer)) { smithyMenu = 5; }
				else {  plyr.smithyFriendships[smithyNo]++; if (plyr.smithyFriendships[smithyNo] > 4) { plyr.smithyFriendships[smithyNo] = 4; } smithyMenu = 4; }
			}
		}

		while (smithyMenu == 19) // Leave my shop
		{
			smithyDisplayUpdate();
			cText ("Leave my shoppe and don't return@@until you are ready to make a decent@@offer!");
			updateDisplay();
			key = getSingleKey();

			if ( key!="" ) { plyr.smithyFriendships[smithyNo]--; if (plyr.smithyFriendships[smithyNo] < 0) { plyr.smithyFriendships[smithyNo] = 0; } smithyMenu = 0; } // Thrown out
		}



		while (smithyMenu == 5) // insufficient funds!
		{
			smithyDisplayUpdate();
			cText ("THAT OFFENDS ME DEEPLY!@Why don't you get serious and only@agree to something that you can afford!");
			cyText (9,"( Press a key )");
			updateDisplay();
			key = getSingleKey();

			if ( key!="" ) { smithyMenu = 2; } // back to purchases
		}

		while (smithyMenu == 4) // Agree to buy item and have funds
		{
			smithyDisplayUpdate();
			cText ("An excellent choice!");
			cyText (9,"( Press a key )");
			updateDisplay();
			key = getSingleKey();

			if ( key!="" )
			{
				// Add a weight & inventory limit check prior to taking money
				deductCoins(0,0,smithyOffer);
				int objectNumber = smithyWares[itemNo].itemRef; // ref within Weapons array
				//int weaponNumber = smithyWares[itemNo].itemRef; // ref within Weapons array

				if ((objectNumber>10) || (objectNumber == 0))
				{
					// Weapon item
                    createCitySmithyInventoryItem(objectNumber);
				}

				// Create an armour set
				if (objectNumber==1)
				{
					// Padded armor set - buying group of items
					createCitySmithyInventoryItem(0x1CA);
					createCitySmithyInventoryItem(0x1EE);
					createCitySmithyInventoryItem(0x217);
					createCitySmithyInventoryItem(0x23E);
                }
				if (objectNumber==2)
				{
					// Leather armor set - buying group of items
					createCitySmithyInventoryItem(0x263);
					createCitySmithyInventoryItem(0x288);
					createCitySmithyInventoryItem(0x2B2);
					createCitySmithyInventoryItem(0x2DA);
                }
				if (objectNumber==3)
				{
					// Studded armor set - buying group of items
					createCitySmithyInventoryItem(0x300);
					createCitySmithyInventoryItem(0x325);
					createCitySmithyInventoryItem(0x34F);
					createCitySmithyInventoryItem(0x377);
				}
				if (objectNumber==4)
				{
					// Ring mail set - buying group of items
					createCitySmithyInventoryItem(0x39D);
					createCitySmithyInventoryItem(0x3C1);
					createCitySmithyInventoryItem(0x3E5);
				}
				if (objectNumber==5)
				{
					// Scale mail set - buying group of items
					createCitySmithyInventoryItem(0x40D);
					createCitySmithyInventoryItem(0x432);
					createCitySmithyInventoryItem(0x457);
				}
				if (objectNumber==6)
				{
					// Splint mail set - buying group of items
					createCitySmithyInventoryItem(0x480);
					createCitySmithyInventoryItem(0x4A6);
					createCitySmithyInventoryItem(0x4CC);
				}
				if (objectNumber==7)
				{
					// Chain mail set - buying group of items
					createCitySmithyInventoryItem(0x4F6);
					createCitySmithyInventoryItem(0x51B);
					createCitySmithyInventoryItem(0x540);
				}
				if (objectNumber==8)
				{
					// Banded mail set - buying group of items
					createCitySmithyInventoryItem(0x569);
					createCitySmithyInventoryItem(0x58D);
					createCitySmithyInventoryItem(0x5B6);
					createCitySmithyInventoryItem(0x5DD);
				}
				if (objectNumber==9)
				{
					// Plate mail set - buying group of items
					createCitySmithyInventoryItem(0x602);
					createCitySmithyInventoryItem(0x626);
					createCitySmithyInventoryItem(0x64F);
					createCitySmithyInventoryItem(0x676);
				}

				smithyMenu = 2; // back to purchases
			}
		}
	}
	smithyMusic.stop();
	leaveShop();
}




int getSmithyNo()
{
	int smithy_no;
	for (int i=0 ; i<4 ; i++) // Max number of smithy objects
	{
		if (Smithies[i].location == plyr.location)
		{
				smithy_no = i; // The number of the smithy you have entered
		}
	}
	return smithy_no;
}


void stockSmithyWares()
{
	// Run each day to randomly pick 10 items for sale at each of the 4 smithies
	// Check for duplicates using smithyWaresCheck array of bools

	// Set bools for duplicate items check to false
	int itemNo = 0;

	for (int x=0; x<4; x++)
	{
		for (int y=0; y<23; y++)
		{
			smithyWaresCheck[x][y] = false;
		}
	}

	for (int smithyNo=0 ; smithyNo<4 ; smithyNo++)
	{
		for (int waresNo=0 ; waresNo<10 ; waresNo++)
		{
			// Current code may create duplicate items in each smithy
			bool uniqueItem = false;
			while (!uniqueItem)
			{
				itemNo = randn(0,22);

				if (!smithyWaresCheck[smithyNo][itemNo])
				{
					smithyDailyWares[smithyNo][waresNo] = itemNo; // its not a duplicate
					smithyWaresCheck[smithyNo][itemNo] = true;
					uniqueItem = true;
				}
			}
		}
	}

	// Simple sort of items in numeric order
    sort(smithyDailyWares[0], smithyDailyWares[0]+10);
	sort(smithyDailyWares[1], smithyDailyWares[1]+10);
	sort(smithyDailyWares[2], smithyDailyWares[2]+10);
	sort(smithyDailyWares[3], smithyDailyWares[3]+10);

	// Always make sure a stiletto will be available
	smithyDailyWares[0][0] = 0;
	smithyDailyWares[1][0] = 0;
	smithyDailyWares[2][0] = 0;
	smithyDailyWares[3][0] = 0;
}


void smithyDisplayUpdate()
{
    clock1.restart();
	clearShopDisplay();
	updateLyrics();
	iCounter++;
}

string readSmithyItemString(int stringOffset)
{
    stringstream ss;
    int z = stringOffset;   // current location in the binary
    int c = 0;              // current byte
    string result = "";

   while (!(citySmithyBinary[z]==0))
    {
        c = citySmithyBinary[z];
        ss << (char) c;
        z++;
    }
    result = ss.str();
    return result;
}

#include <cstdio> // Include the necessary header for fopen

void loadCitySmithyBinary() {
	FILE* fp;               // file pointer - used when reading files
	char tempString[100];   // temporary string
	sprintf_s(tempString, sizeof(tempString), "%s%s", "data/map/core/", "smithyItems.bin");

	// Use fopen_s for improved error handling
	if (fopen_s(&fp, tempString, "rb") == 0 && fp != NULL) {
		// File opened successfully
		for (int i = 0; i < noOfCitySmithyFile; i++) {
			citySmithyBinary[i] = fgetc(fp);
		}
		fclose(fp); // Close the file when done
	}
	else {
		// Handle file open error
		// You can print an error message or take appropriate action
		perror("Error opening file");
	}
}






// Take a binary offset within citySmithyBinary and create a new inventory item from the binary data (weapon or armour)
// Item types:  0x83 - weapon, 0x84 - armour

int createCitySmithyInventoryItem(int startByte)
{
    int index,alignment,weight,wAttributes,melee,ammo,blunt,sharp,earth,air,fire,water,power,magic,good,evil,cold,nature,acid,
        minStrength,minDexterity,hp,maxHP,flags,parry,useStrength, effect;

    int offset = startByte;
    int itemType = citySmithyBinary[offset];
    string itemName = readSmithyItemString((offset+6));

    if (itemType == 0x83)
    {
        itemType           = 178;       // ARX value for weapon
        index              = 0;         // No longer required
        useStrength        = 0;
        alignment          = citySmithyBinary[offset+3];
        weight             = citySmithyBinary[offset+4];

        wAttributes    = (offset + citySmithyBinary[offset+1])-19; // Working out from the end of the weapon object

        melee              = 0xFF;                              //citySmithyBinary[wAttributes+1];
        ammo               = 0;                                 //citySmithyBinary[wAttributes+2];
        blunt              = citySmithyBinary[wAttributes+3];
        sharp              = citySmithyBinary[wAttributes+4];
        earth              = citySmithyBinary[wAttributes+5];
        air                = citySmithyBinary[wAttributes+6];
        fire               = citySmithyBinary[wAttributes+7];
        water              = citySmithyBinary[wAttributes+8];
        power              = citySmithyBinary[wAttributes+9];
        magic              = citySmithyBinary[wAttributes+10];
        good               = citySmithyBinary[wAttributes+11];
        evil               = citySmithyBinary[wAttributes+12];
        cold               = 0; // No cold damage for City items
		nature = 0;// No cold damage for City items
		acid = 0; // No cold damage for City items
        citySmithyBinary[wAttributes+13];
        minStrength        = citySmithyBinary[wAttributes+13];
        minDexterity       = citySmithyBinary[wAttributes+14];
        hp                 = 44; //citySmithyBinary[wAttributes+15];
        maxHP              = 44; //citySmithyBinary[wAttributes+16];
        flags              = citySmithyBinary[wAttributes+17];
        //flags = 1;
        parry              = citySmithyBinary[wAttributes+18];
		effect = 0;
    }

    if (itemType == 0x84)
    {
        itemType           = 177;       // ARX value for armour
        index              = 0;         // No longer required
        useStrength        = 0;
        alignment          = citySmithyBinary[offset+3];
        weight             = citySmithyBinary[offset+4];

        wAttributes    = (offset + citySmithyBinary[offset+1])-17; // Working out from the end of the weapon object

        melee              = citySmithyBinary[wAttributes+13];    // Body part
        if (melee == 1) melee = 0;
        if (melee == 2) melee = 1;
        if (melee == 4) melee = 2;
        if (melee == 8) melee = 3;
        if (melee == 6) melee = 1;

        ammo               = 0;                             // Not used
        blunt              = citySmithyBinary[wAttributes+2];    // ERROR ONWARDS
        sharp              = citySmithyBinary[wAttributes+3];
        earth              = citySmithyBinary[wAttributes+4];
        air                = citySmithyBinary[wAttributes+5];
        fire               = citySmithyBinary[wAttributes+6];
        water              = citySmithyBinary[wAttributes+7];
        power              = citySmithyBinary[wAttributes+8];
        magic              = citySmithyBinary[wAttributes+9];
        good               = citySmithyBinary[wAttributes+10];
        evil               = citySmithyBinary[wAttributes+11];
        cold               = 0;
		nature = 0;

        minStrength        = 0;
        minDexterity       = 0;
        hp                 = 56; //citySmithyBinary[wAttributes+12];
        maxHP              = 56; //255;
        flags              = 0;
        parry              = 0;
		effect             = 0;
    }
//cout << itemName << " " << std::hex << "HP:" << hp << " Bl:" << blunt << " " << "Sh:" << sharp << "\n";
    int newItemRef = createItem(itemType,index,itemName,hp,maxHP,flags,minStrength,minDexterity,useStrength,blunt,
                                sharp,earth,air,fire,water,power,magic,good,evil,cold,nature,acid,weight,alignment,melee,ammo,parry, effect);
    itemBuffer[newItemRef].location = 10; // Add to player inventory - 10
	return 1;
}
