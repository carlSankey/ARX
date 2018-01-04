
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
#include "items.h"
#include "shop.h"

// extern Player plyr;
// extern sf::RenderWindow App;
extern clothingItem clothingItems[12];

// There are 16 possible items that could be in stock but only 12 unique items per day per shop
bool shopWaresCheck[15][16]; // markers used to check for duplicate items created during daily stocking up process
extern int shopDailyWares[15][12]; //15 shops with 12 items each a day for sale

struct shopClothingItem
{
	//string name;
	int type; // 180 - clothing?
	int price;
	int itemRef;
};

shopClothingItem shopClothingWares[12] =
{
	{180,	112,		13},
	{180,	117,		14},
	{180,	117,		15},
	{180,	1252,	    16},
	{180,	162,		17},
	{180,	112,		18},
	{180,	117,		19},
	{180,	122,		20},
	{180,	1222,		21},
	{180,	162,		22},
	{180,	162,		23},
	{180,	182,		24}
};

struct Shop
{
       string name;
       float minimumPriceFactor;
	   float initialPriceFactor;
	   int location; // match with location text description number
	   int openingHour;
	   int closingHour;
};

Shop Shops[15] =
{
	{"Smiley's Shop",		    1.00,		1.53,	40,		8,	20},
	{"Honest Trader",	        1.10,		1.75,	41,		8,	21},
	{"Adventurer's Outfitters",	0.95,		1.35,	42,		8,	19},
	{"Warrior's Supplies",		0.80,		1.10,	43,		5,	17},
    {"General Store",		    0.90,		1.29,	44,		5,	23},
	{"Exclusive Outfitters",	1.10,		1.85,	45,		10,	15},
	{"Rocky's Emporium",		1.00,		1.53,	46,		9,	17},
	{"Best Bargain Store",		1.10,		1.85,	47,		9,	21},
    {"Special Imports Store",	0.90,		1.55,	48,		10,	14},
	{"Betelgeuse Sales",	    0.90,		1.43,	49,		3,	22},
	{"Merchant's Grotto",		1.00,		1.64,	50,		4,	19},
	{"Sunset Market",		    1.00,		1.53,	51,		9,	19},
    {"Pauline's Emporium",		0.95,		1.35,	52,		11,	16},
	{"Da Place!",	            0.82,		1.12,	53,		8,	17},
	{"Trade Winds",			    0.95,		1.70,	54,		8,	17}
};


void shopShop()
{
    int shopNo = getShopNo();
    // copies wares into local data structure for easier display as list
    for (int i=0 ; i<12 ; i++)
    {
        shopClothingWares[i].itemRef = shopDailyWares[shopNo][i];
        int clothingRef = shopDailyWares[shopNo][i];
        shopClothingWares[i].price= clothingItems[clothingRef].quality; // quality used for price just now
    }


	int shopMenu = 1; // high level menu
	string str,key;
	plyr.status = 2; // shopping

    int itemChoice, itemCost, itemLowestCost, shopOffer, itemNo;
	int menuStartItem = 0;
	int offerStatus = 0; // 0 is normal, 1 is demanding, 2 is bartering
	int offerRounds = 0;

    loadShopImage(12);

    if ((Shops[shopNo].closingHour<=plyr.hours) || (Shops[shopNo].openingHour>plyr.hours)) {shopMenu = 50;}
    //cout << Shops[shopNo].closingHour << " , " << Shops[shopNo].openingHour;

	while (shopMenu > 0)
	{
		while (shopMenu == 1) // main menu
		{

			clearShopDisplay();
			bText (13,0, "Welcome Stranger!");
			bText (7,3, "Do you wish to see our wares?");
			cyText (5, "( es or  o)");
			SetFontColour(40, 96, 244, 255);
			cyText (5, " Y      N  ");
			SetFontColour(215, 215, 215, 255);
			displayCoins();
			updateDisplay();

			key = getSingleKey();

			//if ( key=="Y" ) { shopMenu = 3; }
			if ( key=="N" ) { shopMenu = 2; }
			if ( key=="Y" ) { shopMenu = 3; }
			if ( key=="down" ) { shopMenu = 2; }
		}

		while (shopMenu == 2) // Buy a compass?
		{
			clearShopDisplay();
			cyText (1, "Well then, How about a compass?");
			cyText (3, "I charge only 5 silvers for it.");
			cyText (5, "( es or  o)");
			SetFontColour(40, 96, 244, 255);
			cyText (5, " Y      N  ");
			SetFontColour(215, 215, 215, 255);
			displayCoins();
			updateDisplay();

			key = getSingleKey();
			if ( key=="Y" )
			{
				if (checkCoins(0,5,0))
				{
					shopMessage("Right away!");
					plyr.compasses++;
					deductCoins(0,5,0);
					shopMenu = 0;
				}
				else
				{
					shopMessage("THAT OFFENDS ME DEEPLY!@Why don't you get serious@and only agree for something@you can afford!");
					// Adjust friendship?
					shopMenu = 0;
				}
			}
			if ( key=="N" ) { shopMenu = 0; }
		}


while (shopMenu == 3)
{
    offerStatus = 0;
    offerRounds = 0;
    int maxMenuItems = 6;

    clearShopDisplay();

    cyText (0, "What would you like? (  to leave)");
    SetFontColour(40, 96, 244, 255);
    cyText (0, "                      0          ");
    SetFontColour(215, 215, 215, 255);

    for (int i=0 ; i<maxMenuItems ; i++)
    {
        int itemNo = menuStartItem+i;

        str = "( ) " + clothingItems[shopClothingWares[itemNo].itemRef].name;

        bText(1,(2+i), str); //was 4
        //bText(1,(2+i), "                                      coppers");
    }
    displayCoins();

    int itemCost, x;
    for (int i=0 ; i<maxMenuItems ; i++) // Max number of item prices in this menu display
    {
        string itemCostDesc;

        int itemNo = menuStartItem+i;
        itemCost = shopClothingWares[itemNo].price;

        if (itemCost<10000) { x = 34;}
        if (itemCost<1000) { x = 36;}
        if (itemCost<100) { x = 37;}

        itemCostDesc = toCurrency(itemCost);
        bText (x+2,(i+2),itemCostDesc);
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
    if ( key=="1" ) { itemChoice = 0; shopMenu = 22; }
    if ( key=="2" ) { itemChoice = 1; shopMenu = 22; }
    if ( key=="3" ) { itemChoice = 2; shopMenu = 22; }
    if ( key=="4" ) { itemChoice = 3; shopMenu = 22; }
    if ( key=="5" ) { itemChoice = 4; shopMenu = 22; }
    if ( key=="6" ) { itemChoice = 5; shopMenu = 22; }
    if ( (key=="up") && (menuStartItem>0) ) { menuStartItem--; }
    if ( (key=="down") && (menuStartItem<6) ) { menuStartItem++; } // tweak when number of purchase items changes
    if ( key=="ESC" ) { shopMenu = 0; }
    if ( key=="0" ) { shopMenu = 2; }

}


while (shopMenu == 22) // buy item?
{
    itemNo = menuStartItem+itemChoice;
    itemCost = shopClothingWares[itemNo].price;
    float tempitemcost = shopClothingWares[itemNo].price;
    float temp = (tempitemcost/100)*75;
    itemLowestCost = temp;
    shopOffer = itemCost;
    shopMenu = 23;
}


while (shopMenu == 23) // buy item?
{
    clearShopDisplay();
    if (offerStatus==0)
    {
        str = "The cost for " + clothingItems[shopClothingWares[itemNo].itemRef].name;
        cyText (0,str);
        str = "is " + toCurrency(shopOffer) + " coppers. Agreed?";
        cyText (1,str);
    }
    if (offerStatus==1)
    {
        str = "I demand at least " + toCurrency(shopOffer) + " coppers!";
        cyText (1,str);
    }
    if (offerStatus==2)
    {
        str = "Would you consider " + toCurrency(shopOffer) + "?";
        cyText (1,str);
    }

    bText (11,3," ) Agree to price");
    bText (11,4," ) Make an offer");
    bText (11,5," ) Select other apparel");
    bText (11,6," ) Buy something else");
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
        if (!checkCoins(0,0,shopOffer)) { shopMenu = 25; } // offended!
        else { shopMenu = 24; }
    }
    if ( key=="2" ) { shopMenu = 26; }
    if ( key=="3" ) { shopMenu = 21; }
    if ( key=="0" ) { shopMenu = 1; }

}


while (shopMenu == 24) // Agree to buy item and have funds
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
        deductCoins(0,0,shopOffer);
        int objectNumber = shopClothingWares[itemNo].itemRef; // ref within Weapons array
        int itemHandle = createClothing(objectNumber); // create a new weapon or armour item(s)
        itemBuffer[itemHandle].location = 10; // Add to player inventory - 10
        shopMenu = 3; // back to purchases
    }

}




while (shopMenu == 25) // insufficient funds!
{
    shopMessage("Thou would be wise to check thy funds@@BEFORE purchasing!");
    shopMenu = 3; // back to clothing purchases
}




while (shopMenu == 26) // what is your offer
{
    int coppers = inputValue("How many coppers do you offer?",3);

    // check offer
    if ( coppers==0 ) { shopMenu = 22; }

    if ( coppers >= itemCost )
    {
        shopOffer = coppers; // accepted the players offer
        offerStatus = 2;
        shopMenu = 27;
    }
    if (( coppers >= itemLowestCost ) && ( coppers < itemCost ))
    {

        offerStatus = 2;
        offerRounds++;
        if ( offerRounds > 2 ) { shopOffer = coppers; shopMenu = 27; }
        else
        {
            shopOffer = randn(coppers,itemCost);
            itemLowestCost = coppers;
            shopMenu = 23;
        }
    }
    if (( coppers < itemLowestCost ) && (coppers > 0))
    {
        offerStatus=1;
        offerRounds++;
        shopOffer = itemLowestCost;
        if ( offerRounds > 1 ) { shopMenu = 19; }
        else	{ shopMenu = 23; }
    }
}

while (shopMenu == 27) // Offer accepted (subject to funds check) for clothing
{
    clearShopDisplay();
    cText ("I'll take it!");
    updateDisplay();
    key = getSingleKey();

    if ( key!="" )
    {
        if (!checkCoins(0,0,shopOffer)) { shopMenu = 25; }
        else {  plyr.shopFriendships[shopNo]++; if (plyr.shopFriendships[shopNo] > 4) { plyr.shopFriendships[shopNo] = 4; } shopMenu = 24; }
    }
}



		while (shopMenu == 50) // closed
		{
		    string openingText, closingText;
			int openHour = Shops[shopNo].openingHour;
			int closeHour = Shops[shopNo].closingHour;
			if ((openHour>=0) && (openHour<12)) openingText = "morning";
			if ((openHour>=12) && (openHour<18)) openingText = "afternoon";
			if ((openHour>=18) && (openHour<=23)) openingText = "evening";
			if ((closeHour>=0) && (closeHour<12)) closingText = "morning";
			if ((closeHour>=12) && (closeHour<18)) closingText = "afternoon";
			if ((closeHour>=18) && (closeHour<=23)) closingText = "evening";
			clearShopDisplay();
			cyText (1, "Sorry, we are closed. Come back@during our working hours.");
			str = "We are open from " + itos(Shops[shopNo].openingHour) + ":00 in the "+openingText+"@to " + itos(Shops[shopNo].closingHour) + ":00 in the "+closingText+".";
			cyText (4, str);
			cyText (9,"( Press a key )");
			updateDisplay();
			key = getSingleKey();
			if ( key == "SPACE" ) { shopMenu = 0; }
		}



	}
	leaveShop();
}

int getShopNo()
{
	int shop_no = 255;
	for (int i=0 ; i<15 ; i++) // Max number of smithy objects
	{
		if (Shops[i].location == plyr.location)
		{
				shop_no = i; // The number of the shop you have entered
		}
	}
	return shop_no;
}


void shopMessage(string txt)
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


void stockShopWares()
{
	// Run each day to randomly pick 12 items for sale at each of the 15 shops
	// Check for duplicates using smithyWaresCheck array of bools

	// Set bools for duplicate items check to false
	int itemNo = 0;
	for (int x=0; x<15; x++)
	{
		for (int y=0; y<16; y++)
		{
			shopWaresCheck[x][y] = false;
		}
	}

	for (int shopNo=0 ; shopNo<15 ; shopNo++)
	{
		for (int waresNo=0 ; waresNo<12 ; waresNo++)
		{
			bool uniqueItem = false;
			while (!uniqueItem)
			{
				itemNo = randn(0,15); // to exclude damon clothing items
				    int itemIndex = 12 + itemNo; // to exclude the Damon items
				    if (itemIndex==12) itemIndex = 13;
				if (!shopWaresCheck[shopNo][itemNo])
				{
				    // problem

                    // problem duplicates
					shopDailyWares[shopNo][waresNo] = itemIndex; // its not a duplicate
					shopWaresCheck[shopNo][itemNo] = true;
					uniqueItem = true;
				}
			}
		}
	}

	// Simple sort of items in cost numeric order
	// Requires items arranged in the array in ascending price order to work!

    sort(shopDailyWares[0], shopDailyWares[0]+12);
	sort(shopDailyWares[1], shopDailyWares[1]+12);
	sort(shopDailyWares[2], shopDailyWares[2]+12);
	sort(shopDailyWares[3], shopDailyWares[3]+12);
    sort(shopDailyWares[4], shopDailyWares[4]+12);
	sort(shopDailyWares[5], shopDailyWares[5]+12);
	sort(shopDailyWares[6], shopDailyWares[6]+12);
	sort(shopDailyWares[7], shopDailyWares[7]+12);
    sort(shopDailyWares[8], shopDailyWares[8]+12);
	sort(shopDailyWares[9], shopDailyWares[9]+12);
	sort(shopDailyWares[10], shopDailyWares[10]+12);
	sort(shopDailyWares[11], shopDailyWares[11]+12);
    sort(shopDailyWares[12], shopDailyWares[12]+12);
	sort(shopDailyWares[13], shopDailyWares[13]+12);
	sort(shopDailyWares[14], shopDailyWares[14]+12);
}
