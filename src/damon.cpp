#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <stdio.h>

#include "constants.h"
#include "player.h"
#include "font.h"
#include "display.h"
#include "game.h"
#include "misc.h"
#include "damon.h"
#include "items.h"
#include "automap.h"
#include "module.h"
#include "spells.h"

unsigned char damonBinary[damonFileSize];
int itemNameOffset;

//extern buffer_item itemBuffer[100];


int gemsToSell = 0;
int jewelsToSell = 0;
int totalSilver = 0;

// provision variables
string provisionDescription, provMessage;
int quantity, total;
int damonStock[5];

extern bonusDamage itemBonus;


struct damonBattleGearItem
{

	//string name;
	int type; // 177 - armour, 178 - weapon
	int price;
	int itemRef;
};

struct newdamonBattleGearItem
{
	int itemRef;
};

damonBattleGearItem damonBattleGearWares[12] =
{
	{177,	127,	0x00},  // leather breastplate
	{177,	52,		0x29},  // leather gauntlets
	{177,	52,		0x50},  // leather leggings
	{177,	27,		0x76},  // leather helm
	{178,	22,		0x98},  // stiletto
	{178,	27,		0xBB},  // dagger
	{178,	47,		0x102}, // whip
	{178,	77,		0x167}, // battle axe
	{178,	102,	0x18C}, // short sword
	{178,	127,	0x1B2}, // long sword
	{178,	82,		0x1D7}, // small shield
	{178,	102,	0x1FE}  // large shield
};


newdamonBattleGearItem newdamonBattleGearWares[noOfDaemon];
newdamonBattleGearItem newsmithyWares[noOfDaemon];


/*
newdamonBattleGearItem newdamonBattleGearWares[12] =
{
	{134},  // leather breastplate
	{135},  // leather gauntlets
	{136},  // leather leggings
	{133},  // leather helm
	{238},  // stiletto
	{239},  // dagger
	{241}, // whip
	{244}, // battle axe
	{49}, // short sword
	{245}, // long sword
	{246}, // small shield
	{247}  // large shield
};
*/

struct damonClothingItem
{

	//string name;
	int type; // 180 - clothing?
	int price;
	int itemRef;
};

struct newdamonClothingItem
{
	int itemRef;
};




newdamonClothingItem newdamonClothingWares[noOfDaemonC];

/*
int readDaemonItems()
{
	const std::string filename = "daemon.csv"; // Replace with your CSV file name

	std::ifstream file("data/map/" + filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file " << filename << std::endl;
		return 1;
	}

	std::vector<newdamonBattleGearItem> items;
	int id;
	while (file >> id) {
		newdamonBattleGearItem newItem;
		newItem.itemRef = id;
		items.push_back(newItem);
	}

	file.close();

	// Assuming the size of newdamonBattleGearWares is fixed to 12
	//const size_t size = noOfDaemon;
	//newdamonBattleGearItem newdamonBattleGearWares[size];

	// Copy data from the vector to the array
	for (size_t i = 0; i < noOfDaemon && i < items.size(); ++i) {
		newdamonBattleGearWares[i] = items[i];
	}

	// Accessing the data in the newdamonBattleGearWares array
	

	return 0;
}

*/


void shopDamon()
{
	int itemChoice, itemCost, itemLowestCost, damonOffer, itemNo;
	int damonMenu = 1; // high level menu
	string str,key;
	plyr.status = 2; // shopping
	int menuStartItem = 0;
	int offerStatus = 0; // 0 is normal, 1 is demanding, 2 is bartering
	int offerRounds = 0;
	int maxClothingOnSale = 14;
	int maxBattleGearOnSale = 12;
	int maxMenuItems = 6;
	
	//readDaemonItems(); //read shop battlegear - needs to make this shop dependant at some point.

    stockDamon(); // Calculate provisions stock level - torches, food packets etc

	setAutoMapFlag(plyr.map, 45, 3);
	setAutoMapFlag(plyr.map, 46, 3);
	setAutoMapFlag(plyr.map, 45, 4);
	setAutoMapFlag(plyr.map, 46, 4);
	setAutoMapFlag(plyr.map, 45, 5);
	setAutoMapFlag(plyr.map, 46, 5);

	loadShopImage(3);

	


	while (damonMenu > 0)
	{
		while (damonMenu == 1) // main menu
		{
			clearShopDisplay();

			if (plyr.damonFriendship > 1)
			{
			bText (1,1, "Welcome to our shoppe. I am Omar, Jeff's");
			bText (4,2, "twin brother.  How can I help you?");
			}
			if (plyr.damonFriendship < 2)
			{
				cyText (1, "What are you here for, churl?");
			}
			bText (6,4, "(1) Stocking up on provisions");
			bText (6,5, "(2) Selecting battle gear");
			bText (6,6, "(3) Choosing some apparel");
			bText (6,7, "(4) Exchanging currency");
			bText (6,8, "(0) Leave");
			updateDisplay();

			key = getSingleKey();
			if ( key=="0" ) { damonMenu = 0; }
			if ( key=="down" ) { damonMenu = 0; }
			if ( key=="1" ) { damonMenu = 30; }
			if ( key=="2" ) { damonMenu = 3; }
			if ( key=="3" ) { damonMenu = 21; }
			if ( key=="4" ) { damonMenu = 7; }
		}


while (damonMenu == 30) // main menu
{
    clearShopDisplay();
    bText (1,1, "What provisions are you interested in?");
    bText (6,3, "(1) Nourishing food packets");
    bText (6,4, "(2) Delicious water flasks");
    bText (6,5, "(3) Bright torches");
    bText (6,6, "(4) Reliable compasses");
    bText (6,7, "(5) Accurate timepieces");
    bText (6,8, "(0) Buy something else");
    updateDisplay();

    key = getSingleKey();
    if ( key=="0" ) { damonMenu = 1; }
    if ( key=="1" ) { provisionDescription = "food packets"; damonMenu = 31; }
    if ( key=="2" ) { provisionDescription = "water flasks"; damonMenu = 31; }
    if ( key=="3" ) { provisionDescription = "torches"; damonMenu = 31; }
    if ( key=="4" ) { provisionDescription = "compasses"; damonMenu = 31; }
    if ( key=="5" ) { provisionDescription = "timepieces"; damonMenu = 31; }
}

while (damonMenu == 31) //Buy a specific provision item(s)
{
    if ((plyr.days==30) && (provisionDescription == "compasses")) { message("I'm sorry, but I seem to be out of@@stock in that particular item.  I@@expect my next shipment tomorrow."); damonMenu = 30; break; }
    if ((plyr.days==30) && (provisionDescription == "timepieces")) { message("I'm sorry, but I seem to be out of@@stock in that particular item.  I@@expect my next shipment tomorrow."); damonMenu = 30; break; }
    if ((damonStock[0]==0) && (provisionDescription == "food packets")) { message("I'm sorry, but I seem to be out of@@stock in that particular item.  I@@expect my next shipment tomorrow."); damonMenu = 30; break; }
    if ((damonStock[1]==0) && (provisionDescription == "water flasks")) { message("I'm sorry, but I seem to be out of@@stock in that particular item.  I@@expect my next shipment tomorrow."); damonMenu = 30; break; }
    if ((damonStock[2]==0) && (provisionDescription == "torches")) { message("I'm sorry, but I seem to be out of@@stock in that particular item.  I@@expect my next shipment tomorrow."); damonMenu = 30; break; }
    if ((damonStock[3]==0) && (provisionDescription == "timepieces")) { message("I'm sorry, but I seem to be out of@@stock in that particular item.  I@@expect my next shipment tomorrow."); damonMenu = 30; break; }
    if ((damonStock[4]==0) && (provisionDescription == "compasses")) { message("I'm sorry, but I seem to be out of@@stock in that particular item.  I@@expect my next shipment tomorrow."); damonMenu = 30; break; }

    // check for stock in other items
    int provisionCost = 5;
    int provisionStock = 0;
    if (provisionDescription=="food packets") { provisionCost = 6; provisionStock = damonStock[0]; }
    if (provisionDescription=="water flasks") { provisionCost = 4; provisionStock = damonStock[1]; }
    if (provisionDescription=="torches") { provisionCost = 4; provisionStock = damonStock[2]; }
    if (provisionDescription=="timepieces") { provisionCost = 15; provisionStock = damonStock[3]; }
    if (provisionDescription=="compasses") { provisionCost = 15; provisionStock = damonStock[4]; }

    // add look up prov cost here
    provMessage = "How many " + provisionDescription + " do you want@@at " + itos(provisionCost) + " silvers each?";
    string atHandMessage = "I only have " + itos(provisionStock) + " to hand.";
    quantity = inputNumber(provMessage);
    if (quantity > provisionStock) { message(atHandMessage); break; }

    if (quantity==0) { damonMenu = 30; break;}
    total = quantity * provisionCost;
    if (quantity==1) provMessage = "That will be " + itos(provisionCost) + " silvers for one of@my fine " + provisionDescription +".";
    if (quantity>1) provMessage = "Let's see..." + itos(quantity) + " " + provisionDescription +" at@" + itos(provisionCost) + " silvers each comes to a total@of " + itos(total) + " silvers.";
    bool keyNotPressed = true;
    while (keyNotPressed)
    {
        clearShopDisplay();
        cyText(1,provMessage);
        bText (12,6, "(1) Agree to sale");
        bText (12,7, "(2) Forget it");
        updateDisplay();
        key = getSingleKey();
        if ( key=="1" ) { keyNotPressed = false; damonMenu = 32; }
        if ( key=="2" ) { keyNotPressed = false; damonMenu = 30; }
    }
}


while (damonMenu == 32)
{
    bool fundsAvailable = checkCoins(0,total,0);
    if (!fundsAvailable) { message("Thou would be wise to check thy funds@@BEFORE purchasing!"); }
    else
    {
        message("A thousand blessings.");
        deductCoins(0,total,0);
        if (provisionDescription=="food packets") { plyr.food+=quantity; damonStock[0]-=quantity; }
        if (provisionDescription=="water flasks") { plyr.water+=quantity; damonStock[1]-=quantity; }
        if (provisionDescription=="torches") { plyr.torches+=quantity; damonStock[2]-=quantity; }
        if (provisionDescription=="timepieces") { plyr.timepieces+=quantity; damonStock[3]-=quantity; }
        if (provisionDescription=="compasses") { plyr.compasses+=quantity; damonStock[4]-=quantity; }
        // Add a weight check
    }
    damonMenu = 30;
}





while (damonMenu == 21)  //Clothing
{
    offerStatus = 0;
    offerRounds = 0;
	
    clearShopDisplay();

    cyText (0, "What would you like? (  to go back)");
    SetFontColour(40, 96, 244, 255);
    cyText (0, "                      0            ");
    SetFontColour(215, 215, 215, 255);

	//Populate newdamonClothing with random items

	 


	//Add items to the menu
    for (int i=0 ; i< maxMenuItems; i++)
    {
		int itemNo = menuStartItem + i;
		str = "( ) " + newItemArray[newdamonClothingWares[itemNo].itemRef].name;
        bText(1,(2+i), str); //was 4
        bText(1,(2+i), "                                 silvers");
    }
    displaySilverCoins();

	//Add item prices to the menu
    int itemCost, x;
    for (int i=0 ; i< maxMenuItems; i++) // Max number of item prices in this menu display
    {
        string itemCostDesc;

        int itemNo = menuStartItem+i;
        itemCost = newItemArray[newdamonClothingWares[itemNo].itemRef].cost;

        if (itemCost<1000) { x = 30;}
        if (itemCost<100) { x = 31;}

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
    if (menuStartItem!=6) { bText (2,8, "{"); } // tweak when number of purchase items changes
    SetFontColour(215, 215, 215, 255);

    updateDisplay();

    key = getSingleKey();
    if ( key=="1" ) { itemChoice = 0; damonMenu = 22; }
    if ( key=="2" ) { itemChoice = 1; damonMenu = 22; }
    if ( key=="3" ) { itemChoice = 2; damonMenu = 22; }
    if ( key=="4" ) { itemChoice = 3; damonMenu = 22; }
    if ( key=="5" ) { itemChoice = 4; damonMenu = 22; }
    if ( key=="6" ) { itemChoice = 5; damonMenu = 22; }
    if ( (key=="up") && (menuStartItem>0) ) { menuStartItem--; }
    if ( (key=="down") && (menuStartItem < maxClothingOnSale - maxMenuItems) ) { menuStartItem++; } // tweak when number of purchase items changes
    if ( key=="ESC" ) { damonMenu = 0; }
    if ( key=="0" ) { damonMenu = 1; }

}

while (damonMenu == 22) // buy item?
{
    itemNo = menuStartItem+itemChoice;
    itemCost = newItemArray[newdamonClothingWares[itemNo].itemRef].cost;
    float tempitemcost = static_cast<float>(newItemArray[newdamonClothingWares[itemNo].itemRef].cost);
    float temp = (tempitemcost/100)*75;
    itemLowestCost = static_cast<int>(temp);
    damonOffer = itemCost;
    damonMenu = 23;
}


		while (damonMenu == 23) // buy item?
		{
			clearShopDisplay();
			if (offerStatus==0)
			{
               // itemNameOffset = (damonClothingWares[itemNo].itemRef)+6;
                str = "The cost for "+ newItemArray[newdamonClothingWares[itemNo].itemRef].name;
				cyText (0,str);
				str = "is " + toCurrency(damonOffer) + " silvers. Agreed?";
				cyText (1,str);
			}
			if (offerStatus==1)
			{
				str = "I demand at least " + toCurrency(damonOffer) + " silvers!";
				cyText (1,str);
			}
			if (offerStatus==2)
			{
				str = "Would you consider " + toCurrency(damonOffer) + "?";
				cyText (1,str);
			}

			bText (11,3," ) Agree to price");
			bText (11,4," ) Make an offer");
			bText (11,5," ) Select other apparel");
			bText (11,6," ) Buy something else");
			displaySilverCoins();
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
				if (!checkCoins(0,damonOffer,0)) { damonMenu = 25; } // offended!
				else { damonMenu = 24; }
			}
			if ( key=="2" ) { damonMenu = 26; }
			if ( key=="3" ) { damonMenu = 21; }
			if ( key=="0" ) { damonMenu = 1; }

		}


		while (damonMenu == 24) // Agree to buy item and have funds
		{
			int itemNo = menuStartItem+itemChoice;
			clearShopDisplay();
			cText ("Excellent decision");
			updateDisplay();
			key = getSingleKey();


			if ( key!="" )

			{
				// Add a weight & inventory limit check prior to taking money

				//plyr.silver-=itemCost;
                deductCoins(0,damonOffer,0);
                int objectNumber = newItemArray[newdamonClothingWares[itemNo].itemRef].index; // ref within Weapons array
                newcreateInventoryItem(objectNumber);
				damonMenu = 21; // back to purchases
			}

		}




		while (damonMenu == 25) // insufficient funds!
		{
			message("Thou would be wise to check thy funds@@BEFORE purchasing!");
			damonMenu = 21; // back to clothing purchases
		}




		while (damonMenu == 26) // what is your offer
		{
			int silvers = inputValue("How many silvers do you offer?",3);

			// check offer
			if ( silvers==0 ) { damonMenu = 22; }

			if ( silvers >= itemCost )
			{
				damonOffer = silvers; // accepted the players offer
				offerStatus = 2;
				damonMenu = 27;
			}
			if (( silvers >= itemLowestCost ) && ( silvers < itemCost ))
			{

				offerStatus = 2;
				offerRounds++;
				if ( offerRounds > 2 ) { damonOffer = silvers; damonMenu = 27; }
				else
				{
					damonOffer = randn(silvers,itemCost);
					itemLowestCost = silvers;
					damonMenu = 23;
				}
			}
			if (( silvers < itemLowestCost ) && (silvers > 0))
			{
				offerStatus=1;
				offerRounds++;
				damonOffer = itemLowestCost;
				if ( offerRounds > 1 ) { damonMenu = 19; }
				else	{ damonMenu = 23; }
			}
		}

		while (damonMenu == 27) // Offer accepted (subject to funds check) for clothing
		{
			clearShopDisplay();
			cText ("I'll take it!");
			updateDisplay();
			key = getSingleKey();

			if ( key!="" )
			{
				if (!checkCoins(0,damonOffer,0)) { damonMenu = 25; }
				else {  plyr.damonFriendship++; if (plyr.damonFriendship > 4) { plyr.damonFriendship = 4; } damonMenu = 24; }
			}
		}





		while (damonMenu == 3)
		{
			offerStatus = 0;
			offerRounds = 0;
			

			clearShopDisplay();

			cyText (0, "What would you like? (  to go back)");
			SetFontColour(40, 96, 244, 255);
			cyText (0, "                      0            ");
			SetFontColour(215, 215, 215, 255);

		
			//Adding item menu
			for (int i=0 ; i< maxMenuItems; i++)
			{
				int itemNo = menuStartItem+i;

				str = "( ) " + newItemArray[newdamonBattleGearWares[itemNo].itemRef].name;
				bText(1,(2+i), str); //was 4
				bText(1,(2+i), "                                 silvers");
			}
			displaySilverCoins();

			//Adding price
			int itemCost, x;
			for (int i=0 ; i< maxMenuItems; i++) // Max number of item prices in this menu display
			{
				string itemCostDesc;

				int itemNo = menuStartItem+i;
				itemCost = newItemArray[newdamonBattleGearWares[itemNo].itemRef].cost;

				if (itemCost<1000) { x = 30;}
				if (itemCost<100) { x = 31;}

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
			if (menuStartItem!=6) { bText (2,8, "{"); } // tweak when number of purchase items changes
			SetFontColour(215, 215, 215, 255);

			updateDisplay();

			key = getSingleKey();
			if ( key=="1" ) { itemChoice = 0; damonMenu = 4; }
			if ( key=="2" ) { itemChoice = 1; damonMenu = 4; }
			if ( key=="3" ) { itemChoice = 2; damonMenu = 4; }
			if ( key=="4" ) { itemChoice = 3; damonMenu = 4; }
			if ( key=="5" ) { itemChoice = 4; damonMenu = 4; }
			if ( key=="6" ) { itemChoice = 5; damonMenu = 4; }
			if ( (key=="up") && (menuStartItem>0) ) { menuStartItem--; }
			if ( (key=="down") && (menuStartItem < maxBattleGearOnSale - maxMenuItems)){ menuStartItem++; } // tweak when number of purchase items changes
			if ( key=="ESC" ) { damonMenu = 0; }
			if ( key=="0" ) { damonMenu = 1; }
			
		}




		while (damonMenu == 4) // buy item?
		{
			itemNo = menuStartItem+itemChoice;
			itemCost = newItemArray[newdamonBattleGearWares[itemNo].itemRef].cost;
			float tempitemcost = newItemArray[newdamonBattleGearWares[itemNo].itemRef].cost;
			float temp = (tempitemcost/100)*75;
			itemLowestCost = temp;
			damonOffer = itemCost;
			damonMenu = 18;
		}


		while (damonMenu == 18) // buy item?
		{
			clearShopDisplay();
			if (offerStatus==0)
			{
               
                str = "The cost for "+ newItemArray[newdamonBattleGearWares[itemNo].itemRef].name;

				cyText (0,str);
				str = "is " + toCurrency(damonOffer) + " silvers. Agreed?";
				cyText (1,str);
			}
			if (offerStatus==1)
			{
				str = "I demand at least " + toCurrency(damonOffer) + " silvers!";
				cyText (1,str);
			}
			if (offerStatus==2)
			{
				str = "Would you consider " + toCurrency(damonOffer) + "?";
				cyText (1,str);
			}

			bText (11,3," ) Agree to price");
			bText (11,4," ) Make an offer");
			bText (11,5," ) Select other battle gear");
			bText (11,6," ) Buy something else");
			displaySilverCoins();
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
				if (!checkCoins(0,damonOffer,0)) { damonMenu = 6; } // offended!
				else { damonMenu = 5; }
			}
			if ( key=="2" ) { damonMenu = 16; }
			if ( key=="3" ) { damonMenu = 3; }
			if ( key=="0" ) { damonMenu = 1; }

		}




		while (damonMenu == 16) // what is your offer
		{
			int silvers = inputValue("How many silvers do you offer?",3);

			// check offer
			if ( silvers==0 ) { damonMenu = 4; }

			if ( silvers >= itemCost )
			{
				damonOffer = silvers; // accepted the players offer
				offerStatus = 2;
				damonMenu = 20;
			}
			if (( silvers >= itemLowestCost ) && ( silvers < itemCost ))
			{

				offerStatus = 2;
				offerRounds++;
				if ( offerRounds > 2 ) { damonOffer = silvers; damonMenu = 20; }
				else
				{
					damonOffer = randn(silvers,itemCost);
					itemLowestCost = silvers;
					damonMenu = 18;
				}
			}
			if (( silvers < itemLowestCost ) && (silvers > 0))
			{
				offerStatus=1;
				offerRounds++;
				damonOffer = itemLowestCost;
				if ( offerRounds > 1 ) { damonMenu = 19; }
				else	{ damonMenu = 18; }
			}
		}


		while (damonMenu == 19) // Leave my shop
		{
			clearShopDisplay();
			cText ("Leave my shoppe and don't return@@until you are ready to make a decent@@offer!");
			updateDisplay();
			key = getSingleKey();

			if ( key!="" ) { plyr.damonFriendship--; if (plyr.damonFriendship < 0) { plyr.damonFriendship = 0; } damonMenu = 0; } // Thrown out
		}

		while (damonMenu == 20) // Offer accepted (subject to funds check)
		{
			clearShopDisplay();
			cText ("I'll take it!");
			updateDisplay();
			key = getSingleKey();

			if ( key!="" )
			{
				if (!checkCoins(0,damonOffer,0)) { damonMenu = 6; }
				else {  plyr.damonFriendship++; if (plyr.damonFriendship > 4) { plyr.damonFriendship = 4; } damonMenu = 5; }
			}
		}






		while (damonMenu == 5) // Agree to buy item and have funds
		{
			int itemNo = menuStartItem+itemChoice;
			clearShopDisplay();
			cText ("It will serve you well!");
			cyText (9,"( Press a key )");
			updateDisplay();
			key = getSingleKey();

			if ( key!="" )

			{
				// Add a weight & inventory limit check prior to taking money

                deductCoins(0,damonOffer,0); 
                int objectNumber = newItemArray[newdamonBattleGearWares[itemNo].itemRef].index;; // ref within Weapons array

				newcreateInventoryItem(objectNumber);

				damonMenu = 3; // back to purchases
			}
		}




		while (damonMenu == 6) // insufficient funds!
		{
			clearShopDisplay();
			cText ("Thine eyes are bigger than thy purse!");
			cyText (9,"( Press a key )");
			updateDisplay();
			key = getSingleKey();

			if ( key!="" ) { damonMenu = 3; } // back to battle gear purchases
		}



		while (damonMenu == 7) // exchange currency menu
		{

			clearShopDisplay();
			cyText (1, "What would you like to exchange?");

			bText (6,4, "(1) Gems for coins");
			bText (6,5, "(2) Jewels for coins");
			bText (6,6, "(3) Silver & copper coins");
			bText (6,8, "(0) Done");
			updateDisplay();

			key = getSingleKey();
			if ( key=="1" ) { damonMenu = 8; }
			if ( key=="2" ) { damonMenu = 11; }
			if ( key=="3" ) { damonMenu = 14; }
			if ( key=="0" ) { damonMenu = 1; }
		}

		while (damonMenu == 8) // gems exchange currency menu
		{
			string str = "I will give you 22 silvers@for each gem, big or small.@@How many would you like to exchange?";
			gemsToSell = inputNumber(str);
			//clearShopDisplay();

			//updateDisplay();

			//key = pressKey();
			if ( gemsToSell==0 ) { damonMenu = 7; }
			if ( gemsToSell > plyr.gems ) { damonMenu = 9; }
			if (( gemsToSell > 0 ) && ( gemsToSell <= plyr.gems )) { damonMenu = 10; }
		}

		while (damonMenu == 9) // Insufficient gems!
		{


			str = "You have only " + itos(plyr.gems) + "!";
			clearShopDisplay();
			cyText (2, str);
			updateDisplay();
			key = getSingleKey();
			if ( key=="SPACE" ) { damonMenu = 7; }

		}

		while (damonMenu == 10) // Gems sold!
		{
			int totalSilvers = 22 * gemsToSell;
			int goldFromGems = 0;
			int silverFromGems = 0;
			silverFromGems = totalSilvers % 10;
			goldFromGems = (totalSilvers - silverFromGems) / 10;

			str = "Here is " + itos(goldFromGems) + " gold and " + itos(silverFromGems) + " silver coins.";
			clearShopDisplay();
			cyText (2, str);
			updateDisplay();
			key = getSingleKey();
			if ( key=="SPACE" )
			{
				plyr.gems -= gemsToSell;
				plyr.gold += goldFromGems;
				plyr.silver += silverFromGems;
				damonMenu = 7;
			}
		}

		while (damonMenu == 11) // jewels exchange currency menu
		{
			string str = "I can't tell one jewel from another@so I pay a flat rate of 32 per jewel.@@How many do you wish to sell?";
			jewelsToSell = inputNumber(str);
			//clearShopDisplay();

			//updateDisplay();

			//key = pressKey();
			if ( jewelsToSell==0 ) { damonMenu = 7; }
			if ( jewelsToSell > plyr.jewels ) { damonMenu = 12; }
			if (( jewelsToSell > 0 ) && ( jewelsToSell <= plyr.jewels )) { damonMenu = 13; }
		}

		while (damonMenu == 12) // Insufficient jewels!
		{


			str = "You have only " + itos(plyr.jewels) + "!";
			clearShopDisplay();
			cyText (2, str);
			updateDisplay();
			key = getSingleKey();
			if ( key=="SPACE" ) { damonMenu = 7; }

		}

		while (damonMenu == 13) // jewels sold!
		{
			int totalSilvers = 32 * jewelsToSell;
			int goldFromjewels = 0;
			int silverFromjewels = 0;
			silverFromjewels = totalSilvers % 10;
			goldFromjewels = (totalSilvers - silverFromjewels) / 10;
		//str = itos(totalSilvers);
			str = "Here is " + itos(goldFromjewels) + " gold and " + itos(silverFromjewels) + " silver coins.";
			clearShopDisplay();
			cyText (2, str);
			updateDisplay();
			key = getSingleKey();
			if ( key=="SPACE" )
			{
				plyr.jewels -= jewelsToSell;
				plyr.gold += goldFromjewels;
				plyr.silver += silverFromjewels;
				damonMenu = 7;
			}
		}


		while (damonMenu == 14) // exchange silver & copper for gold
		{

			clearShopDisplay();
			cyText (1, "For 11 silvers I will exchange all@of your silver & copper coins for gold.@This will lessen your load.@@ Would you like this? (Y or N)");

			updateDisplay();

			key = getSingleKey();
			if ( key=="Y" ) { totalSilver = plyr.silver + (plyr.gold*10) + ((plyr.copper - (plyr.copper % 10))/10); damonMenu = 15; }
			if ( key=="N" ) { damonMenu = 7; }
		}

		while (damonMenu == 15) // exchange silver & copper for gold
		{

			clearShopDisplay();
			if (totalSilver < 11) { str = "HA!  You haven't even the silver@to pay me!"; }
			else { str = "It's done."; }

			cyText (1, str);

			updateDisplay();

			key = getSingleKey();
			if ( key=="SPACE" )
			{
				if (totalSilver > 10)
				{
					deductCoins(0,11,0);
					totalSilver = plyr.silver + ((plyr.copper - (plyr.copper % 10))/10);
					plyr.gold += (totalSilver - totalSilver % 10) / 10;
					deductCoins(0,(totalSilver - totalSilver % 10),0);

				}
				damonMenu = 7;
			}
		}

	}
	leaveShop();
}


void message(string txt)
{
	string key = "";
	while (key!="SPACE")
	{
			clearShopDisplay();
			cText (txt);
			updateDisplay();
			key = getSingleKey();
	}
}


void stockDamon()
{
    for (int i=0 ; i<5 ; i++)
	{
	    damonStock[i] = randn(1,5) + 8;
	}
}


void loadDamonBinary() {
	FILE* fp = nullptr;       // Initialize fp to nullptr

	char tempString[100];     // temporary string
	int err = sprintf_s(tempString, sizeof(tempString), "%s%s", "data/map/core/", "DamonItems.bin");

	if (err > 0) {
		// Attempt to open the file for reading in binary mode
		err = fopen_s(&fp, tempString, "rb");
	}

	if (err == 0 && fp != nullptr) {
		// File opened successfully
		for (int i = 0; i < damonFileSize; i++) {
			damonBinary[i] = fgetc(fp);
		}
		fclose(fp);
	}
	else {
		// Handle file open error
		perror("Error opening file");
	}
}


string readNameString(int stringOffset)
{
    stringstream ss;
    int z = stringOffset;   // current location in the binary
    int c = 0;              // current byte
    string result = "";

   while (!(damonBinary[z]==0))
    {
        c = damonBinary[z];
        ss << (char) c;
        z++;
    }
    result = ss.str();
    return result;
}



/** @brief Take a binary offset within damonBinary and create a new inventory item from the binary data (weapon, armour or clothing)
/ @brief Item types:  03 - weapon, 04 - armour, 05 - clothing
*/
int createInventoryItem(int startByte)
{
    int index,alignment,weight,wAttributes,melee,ammo,blunt,sharp,earth,air,fire,water,power,magic,good,evil,cold,nature,acid,
        minStrength,minDexterity,hp,maxHP,flags,parry,useStrength, effect, cat, level, buffType;


	int offset = startByte;
    int itemType = damonBinary[offset];
    string itemName = readNameString((offset+6));

    if (itemType == 3)
    {
        itemType           = 178;       // ARX value for weapon
        index              = 0;         // No longer required
        useStrength        = 0;
        alignment          = damonBinary[offset+3];
        weight             = damonBinary[offset+4];

        wAttributes    = (offset + damonBinary[offset+1])-20; // Working out from the end of the weapon object

        melee              = damonBinary[wAttributes+1];
        ammo               = damonBinary[wAttributes+2];
        blunt              = damonBinary[wAttributes+3];
        sharp              = damonBinary[wAttributes+4];
        earth              = damonBinary[wAttributes+5];
        air                = damonBinary[wAttributes+6];
        fire               = damonBinary[wAttributes+7];
        water              = damonBinary[wAttributes+8];
        power              = damonBinary[wAttributes+9];
        magic              = damonBinary[wAttributes+10];
        good               = damonBinary[wAttributes+11];
        evil               = damonBinary[wAttributes+12];
        cold               = damonBinary[wAttributes+13];
		nature             = damonBinary[wAttributes+14];
		acid               = damonBinary[wAttributes+15];
        minStrength        = damonBinary[wAttributes+16];
        minDexterity       = damonBinary[wAttributes+17];
        hp                 = damonBinary[wAttributes+18];
        maxHP              = damonBinary[wAttributes+19];
        flags              = damonBinary[wAttributes+20];
        parry              = damonBinary[wAttributes+21];
		effect             = 0;
    }

    if (itemType == 4)
    {
        itemType           = 177;       // ARX value for armour
        index              = 0;         // No longer required
        useStrength        = 0;
        alignment          = damonBinary[offset+3];
        weight             = damonBinary[offset+4];

        wAttributes    = (offset + damonBinary[offset+1])-15; // Working out from the end of the weapon object

        melee              = damonBinary[wAttributes+1];    // Body part
        ammo               = 0;                             // Not used
        blunt              = damonBinary[wAttributes+2];    // ERROR ONWARDS
        sharp              = damonBinary[wAttributes+3];
        earth              = damonBinary[wAttributes+4];
        air                = damonBinary[wAttributes+5];
        fire               = damonBinary[wAttributes+6];
        water              = damonBinary[wAttributes+7];
        power              = damonBinary[wAttributes+8];
        magic              = damonBinary[wAttributes+9];
        good               = damonBinary[wAttributes+10];
        evil               = damonBinary[wAttributes+11];
        cold               = damonBinary[wAttributes+12];
		nature             = damonBinary[wAttributes+13];
		acid               = damonBinary[wAttributes+14];
        minStrength        = 0;
        minDexterity       = 0;
        hp                 = damonBinary[wAttributes+13];
        maxHP              = damonBinary[wAttributes+14];
        flags              = 0;
        parry              = 0;
		effect			   = 0;
    }

    if (itemType == 5)
    {
        itemType           = 180;       // ARX value for clothing
        index              = 0;         // No longer required
        useStrength        = 0;
        alignment          = damonBinary[offset+3];
        weight             = damonBinary[offset+4];

        wAttributes    = (offset + damonBinary[offset+1])-3; // Working out from the end of the weapon object

        melee              = damonBinary[wAttributes+1];    // clothing attribute
        ammo               = damonBinary[wAttributes+2];    // clothing attribute
        blunt              = damonBinary[wAttributes+3];    // clothing attribute
        sharp              = 0;                             // Set to 0 for non use
        earth              = 0;
        air                = 0;
        fire               = 0;
        water              = 0;
        power              = 0;
        magic              = 0;
        good               = 0;
        evil               = 0;
        cold               = 0;
		nature             = 0;
		acid               = 0;
        minStrength        = 0;
        minDexterity       = 0;
        hp                 = 0;
        maxHP              = 0;
        flags              = 0;
        parry              = 0;
		effect             = 0;
		cat				   = 0;
		level			   = 0;
		buffType		   = 0;
    }

    int newItemRef = createItem(itemType,index,itemName,hp,maxHP,flags,minStrength,minDexterity,useStrength,blunt,
                                sharp,earth,air,fire,water,power,magic,good,evil,cold,nature,acid,weight,alignment,melee,ammo,parry, effect );
    itemBuffer[newItemRef].location = 10; // Add to player inventory - 10
	return 1;
}

//just for now
int newcreateInventoryItem(int itemNo)
{
	int index, alignment, weight, wAttributes, melee, ammo, blunt, sharp, earth, air, fire, water, power, magic, good, evil, cold, nature, acid,
		minStrength, minDexterity, hp, maxHP, flags, parry, useStrength, effect, cat, iLevel, buffType;

	int offset = 6;
	
	int itemType = newItemArray[itemNo].itemType;
	string itemName = newItemArray[itemNo].name;

	
		
		index = itemNo;         // 
		useStrength = newItemArray[itemNo].useStrength;
		alignment = newItemArray[itemNo].alignment;
		weight = newItemArray[itemNo].weight;

		//wAttributes = newItemArray[newdamonClothingWares[itemNo].itemRef].at; // Working out from the end of the weapon object

		melee = newItemArray[itemNo].melee;
		ammo = newItemArray[itemNo].ammo;

		//std::bitset<13> binaryItemBuff(newItemArray[itemNo].elementType);

		//updateItemBuff(binaryItemBuff, (newItemArray[itemNo].positiveValue - newItemArray[itemNo].negativeValue));  //Old method no longer used

		blunt = newItemArray[itemNo].blunt + itemBonus.blunt;
		sharp = newItemArray[itemNo].sharp;
		earth = newItemArray[itemNo].earth;
		air = newItemArray[itemNo].air;
		fire = newItemArray[itemNo].fire;
		water = newItemArray[itemNo].water;
		power = newItemArray[itemNo].power;
		magic = newItemArray[itemNo].magic;
		good = newItemArray[itemNo].good;
		evil = newItemArray[itemNo].evil;
		cold = newItemArray[itemNo].cold;
		nature = newItemArray[itemNo].nature;
		acid = newItemArray[itemNo].acid;

		minStrength = newItemArray[itemNo].minStrength;
		minDexterity = newItemArray[itemNo].minDexterity;
		hp = newItemArray[itemNo].hp;
		maxHP = newItemArray[itemNo].maxHP;
		flags = newItemArray[itemNo].flags;
		parry = newItemArray[itemNo].parry;
		effect = newItemArray[itemNo].effect;
		cat = newItemArray[itemNo].cat; 
		iLevel = newItemArray[itemNo].iLevel;
		buffType = newItemArray[itemNo].buffType;

	

	int newItemRef = newcreateItem(itemType, index, itemName, hp, maxHP, flags, minStrength, minDexterity, useStrength, blunt,
		sharp, earth, air, fire, water, power, magic, good, evil, cold, nature, acid, weight, alignment, melee, ammo, parry, effect, cat,iLevel,buffType);
	itemBuffer[newItemRef].location = 10; // Add to player inventory - 10
	return 1;
}

void ImportItems()
{
	//int index, alignment, weight, wattributes, melee, ammo, blunt, sharp, earth, air, fire, water, power, magic, good, evil, cold, nature, acid,
	//	minstrength, mindexterity, hp, maxhp, flags, parry, usestrength, effect;



	//if (itemtype == 4)
	//{
	//	itemtype = 177;       // arx value for armour
	//	index = 0;         // no longer required
	//	usestrength = 0;
	//	alignment = damonbinary[offset + 3];
	//	weight = damonbinary[offset + 4];

	//	wattributes = (offset + damonbinary[offset + 1]) - 15; // working out from the end of the weapon object

	//	melee = damonbinary[wattributes + 1];    // body part
	//	ammo = 0;                             // not used
	//	blunt = damonbinary[wattributes + 2];    // error onwards
	//	sharp = damonbinary[wattributes + 3];
	//	earth = damonbinary[wattributes + 4];
	//	air = damonbinary[wattributes + 5];
	//	fire = damonbinary[wattributes + 6];
	//	water = damonbinary[wattributes + 7];
	//	power = damonbinary[wattributes + 8];
	//	magic = damonbinary[wattributes + 9];
	//	good = damonbinary[wattributes + 10];
	//	evil = damonbinary[wattributes + 11];
	//	cold = damonbinary[wattributes + 12];
	//	nature = damonbinary[wattributes + 13];
	//	acid = damonbinary[wattributes + 14];
	//	minstrength = 0;
	//	mindexterity = 0;
	//	hp = damonbinary[wattributes + 13];
	//	maxhp = damonbinary[wattributes + 14];
	//	flags = 0;
	//	parry = 0;
	//	effect = 0;
	//}

	//

	//int newitemref = createitem(itemtype, index, itemname, hp, maxhp, flags, minstrength, mindexterity, usestrength, blunt,
	//	sharp, earth, air, fire, water, power, magic, good, evil, cold, nature, acid, weight, alignment, melee, ammo, parry, effect);
	//itembuffer[newitemref].location = 10; // add to player inventory - 10
	//return 1;
}




// Function to check if an index already exists in the array
bool doesIndexExistBattleGear(int indexToCheck, const newdamonBattleGearItem* array, int size) {
	for (int i = 0; i < size; ++i) {
		if (array[i].itemRef == indexToCheck) {
			return true;
		}
	}
	return false;
}

// Function to generate unique indices and populate the array
int populateUniqueIndicesBG(newdamonBattleGearItem* newdamonBattleGearWares, int desiredNumberOfItems, int itemCatToMatch, int levelToMatch, int arraySize, int typeToMatch, bool clear) {
	// ... (Same as before)
	// Convert array of pointers to pointers to pointers
	int i = 0;
	if (!clear)
	{
	i = find_last_zero_index(0, newdamonBattleGearWares, arraySize);
	}
	desiredNumberOfItems = desiredNumberOfItems + i;

	for ( int x = 0; i < desiredNumberOfItems && x < arraySize; x++) {
		int newItemIndex = randomItemPicker(itemCatToMatch, levelToMatch, arraySize, typeToMatch);

		// Check if the index already exists in the array
		if (!doesIndexExistBattleGear(newItemIndex, newdamonBattleGearWares, arraySize)) {
			// If the index doesn't exist, add it to the array
			newdamonBattleGearItem newItem;
			newItem.itemRef = newItemIndex;
			// Assign other properties of newItem if needed
			newdamonBattleGearWares[i] = newItem;
			i++;
		}
		else {
			// If the index already exists, generate a new index or handle it as needed
			// Example: newItemIndex = randomItemPicker(0, 0, 12, someOtherParams);
		}
	}
	return i;
}

int  find_last_zero_index(int indexToCheck, const newdamonBattleGearItem* array, int size) {
	for (int i = 0; i < size; ++i) {
		if (array[i].itemRef == indexToCheck) {
			return i;
		}
	}
	return size;
}



bool doesIndexExistClothing(int indexToCheck, const newdamonClothingItem* array, int size) {
	for (int i = 0; i < size; ++i) {
		if (array[i].itemRef == indexToCheck) {
			return true;
		}
	}
	return false;
}

// Function to generate unique indices and populate the array
int  populateUniqueIndicesCL(newdamonClothingItem* newdamonClothingWares, int desiredNumberOfItems, int itemCatToMatch, int levelToMatch, int arraySize, int typeToMatch) {
	int i = 0;
	for (int x = 0; i < desiredNumberOfItems && x < arraySize; x++) {
		int newItemIndex = randomItemPicker(itemCatToMatch, levelToMatch, arraySize, typeToMatch);

		// Check if the index already exists in the array
		if (!doesIndexExistClothing(newItemIndex, newdamonClothingWares, arraySize)) {
			// If the index doesn't exist, add it to the array
			newdamonClothingItem newItem;
			newItem.itemRef = newItemIndex;
			// Assign other properties of newItem if needed
			newdamonClothingWares[i] = newItem;
			i++;
		}
		else {
			// If the index already exists, generate a new index or handle it as needed
			// Example: newItemIndex = randomItemPicker(0, 0, 12, someOtherParams);
		}
	}
	return i; //+1 to compensae 0 based index
}

void UpdateShopStock()
{

	int maxClothingOnSale = 14;
	int maxBattleGearOnSale = 12;
	std::map<std::pair<int, int>, int> itemCounts = countItemTypes(newItemArray, plyr.items_index);

	int WeaponQty = randn(1, maxBattleGearOnSale - 1);
	int ArmorQty = maxBattleGearOnSale - WeaponQty;

	maxBattleGearOnSale = populateUniqueIndicesBG(newdamonBattleGearWares, WeaponQty, 0, 0, plyr.items_index, 177, true);

	maxBattleGearOnSale = populateUniqueIndicesBG(newdamonBattleGearWares, ArmorQty, 0, 0, plyr.items_index, 178, false);

	maxClothingOnSale = populateUniqueIndicesCL(newdamonClothingWares, maxClothingOnSale, 0, 0, plyr.items_index, 180);
}


void UpdateSmithyStock()
{
	int maxBattleGearOnSale = 12;
	std::map<std::pair<int, int>, int> itemCounts = countItemTypes(newItemArray, plyr.items_index);
	int WeaponQty =  maxBattleGearOnSale;
	maxBattleGearOnSale = populateUniqueIndicesBG(newsmithyWares, WeaponQty, 0, 0, plyr.items_index, 177, true);
}
