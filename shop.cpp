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
	{"Smiley's Shop",		    1.00f,		1.53f,	40,		8,	20},
	{"Honest Trader",	        1.10f,		1.75f,	41,		8,	21},
	{"Adventurer's Outfitters",	0.95f,		1.35f,	42,		8,	19},
	{"Warrior's Supplies",		0.80f,		1.10f,	43,		5,	17},
    {"General Store",		    0.90f,		1.29f,	44,		5,	23},
	{"Exclusive Outfitters",	1.10f,		1.85f,	45,		10,	15},
	{"Rocky's Emporium",		1.00f,		1.53f,	46,		9,	17},
	{"Best Bargain Store",		1.10f,		1.85f,	47,		9,	21},
    {"Special Imports Store",	0.90f,		1.55f,	48,		10,	14},
	{"Betelgeuse Sales",	    0.90f,		1.43f,	49,		3,	22},
	{"Merchant's Grotto",		1.00f,		1.64f,	50,		4,	19},
	{"Sunset Market",		    1.00f,		1.53f,	51,		9,	19},
    {"Pauline's Emporium",		0.95f,		1.35f,	52,		11,	16},
	{"Da Place!",	            0.82f,		1.12f,	53,		8,	17},
	{"Trade Winds",			    0.95f,		1.70f,	54,		8,	17}
};


void shopShop()
{
	static int shopMenu = 0;
	static int shopNo_s = 0;
	static int itemChoice_s = 0;
	static int itemCost_s = 0;
	static int itemLowestCost_s = 0;
	static int shopOffer_s = 0;
	static int itemNo_s = 0;
	static int menuStart_s = 0;
	static int offerStatus_s = 0;
	static int offerRounds_s = 0;
	string str, key;

	if (shopMenu == 0) {
		shopNo_s=getShopNo(); menuStart_s=0; offerStatus_s=0; offerRounds_s=0;
		for(int i=0;i<12;i++){shopClothingWares[i].itemRef=shopDailyWares[shopNo_s][i]; shopClothingWares[i].price=clothingItems[shopDailyWares[shopNo_s][i]].quality;}
		plyr.status=2; loadShopImage(12);
		shopMenu=((Shops[shopNo_s].closingHour<=plyr.hours)||(Shops[shopNo_s].openingHour>plyr.hours))?50:1;
		return;
	}

	if (shopMenu == 1)
	{
		clearShopDisplay(); bText(13,0,"Welcome Stranger!"); bText(7,3,"Do you wish to see our wares?");
		cyText(5,"( es or  o)"); SetFontColour(40,96,244,255); cyText(5," Y      N  "); SetFontColour(215,215,215,255); displayCoins(); updateDisplay();
		key=getSingleKey(); if(key=="Y")shopMenu=3; if(key=="N")shopMenu=2; if(key=="down")shopMenu=2;
	}
	else if (shopMenu == 2) // compass offer
	{
		clearShopDisplay(); cyText(1,"Well then, How about a compass?"); cyText(3,"I charge only 5 silvers for it.");
		cyText(5,"( es or  o)"); SetFontColour(40,96,244,255); cyText(5," Y      N  "); SetFontColour(215,215,215,255); displayCoins(); updateDisplay();
		key=getSingleKey();
		if(key=="Y"){if(checkCoins(0,5,0)){shopMessage("Right away!");plyr.compasses++;deductCoins(0,5,0);}else{shopMessage("THAT OFFENDS ME DEEPLY!@Why don't you get serious@and only agree for something@you can afford!");}shopMenu=-1;}
		if(key=="N") shopMenu=-1;
	}
	else if (shopMenu == 3) // browse wares
	{
		offerStatus_s=0; offerRounds_s=0; int maxM=6;
		clearShopDisplay(); cyText(0,"What would you like? (  to leave)"); SetFontColour(40,96,244,255); cyText(0,"                      0          "); SetFontColour(215,215,215,255);
		for(int i=0;i<maxM;i++){int n=menuStart_s+i; str="( ) "+clothingItems[shopClothingWares[n].itemRef].name; bText(1,2+i,str);}
		displayCoins();
		for(int i=0;i<maxM;i++){int n=menuStart_s+i; int c2=shopClothingWares[n].price; int x=34; if(c2<1000)x=36; if(c2<100)x=37; bText(x+2,i+2,toCurrency(c2));}
		SetFontColour(40,96,244,255); bText(2,2,"1");bText(2,3,"2");bText(2,4,"3");bText(2,5,"4");bText(2,6,"5");bText(2,7,"6");
		if(menuStart_s!=0)bText(2,1,"}"); if(menuStart_s!=6)bText(2,8,"{"); SetFontColour(215,215,215,255);
		updateDisplay();
		key=getSingleKey();
		if(key=="1"){itemChoice_s=0;shopMenu=22;} if(key=="2"){itemChoice_s=1;shopMenu=22;} if(key=="3"){itemChoice_s=2;shopMenu=22;}
		if(key=="4"){itemChoice_s=3;shopMenu=22;} if(key=="5"){itemChoice_s=4;shopMenu=22;} if(key=="6"){itemChoice_s=5;shopMenu=22;}
		if((key=="up")&&(menuStart_s>0))menuStart_s--; if((key=="down")&&(menuStart_s<6))menuStart_s++;
		if(key=="ESC"||key=="0") shopMenu=2;
	}
	else if (shopMenu == 22) // calc price
	{
		itemNo_s=menuStart_s+itemChoice_s; itemCost_s=shopClothingWares[itemNo_s].price;
		itemLowestCost_s=static_cast<int>((float(itemCost_s)/100)*75); shopOffer_s=itemCost_s; shopMenu=23;
	}
	else if (shopMenu == 23) // negotiate
	{
		clearShopDisplay();
		if(offerStatus_s==0){str="The cost for "+clothingItems[shopClothingWares[itemNo_s].itemRef].name;cyText(0,str);str="is "+toCurrency(shopOffer_s)+" coppers. Agreed?";cyText(1,str);}
		if(offerStatus_s==1){str="I demand at least "+toCurrency(shopOffer_s)+" coppers!";cyText(1,str);}
		if(offerStatus_s==2){str="Would you consider "+toCurrency(shopOffer_s)+"?";cyText(1,str);}
		bText(11,3," ) Agree to price");bText(11,4," ) Make an offer");bText(11,5," ) Select other apparel");bText(11,6," ) Buy something else");
		displayCoins();SetFontColour(40,96,244,255);bText(11,3,"1");bText(11,4,"2");bText(11,5,"3");bText(11,6,"0");SetFontColour(215,215,215,255);
		updateDisplay();
		key=getSingleKey();
		if(key=="1"){shopMenu=checkCoins(0,0,shopOffer_s)?24:25;} if(key=="2")shopMenu=26; if(key=="3")shopMenu=21; if(key=="0")shopMenu=1;
	}
	else if (shopMenu == 21) // back to list (menu label used in original for back)
	{ shopMenu=3; }
	else if (shopMenu == 24) // confirmed buy
	{
		int n=menuStart_s+itemChoice_s; clearShopDisplay(); cText("Excellent decision"); updateDisplay();
		key=getSingleKey();
		if(key!=""){deductCoins(0,0,shopOffer_s); int on=shopClothingWares[n].itemRef; int h=createClothing(on,10); itemBuffer[h].location=10; shopMenu=3;}
	}
	else if (shopMenu == 25) // insufficient funds
	{
		shopMessage("Thou would be wise to check thy funds@@BEFORE purchasing!"); shopMenu=3;
	}
	else if (shopMenu == 26) // counter offer
	{
		int coppers=inputValue("How many coppers do you offer?",3);
		if(coppers==0)shopMenu=22;
		else if(coppers>=itemCost_s){shopOffer_s=coppers;offerStatus_s=2;shopMenu=27;}
		else if(coppers>=itemLowestCost_s){offerStatus_s=2;offerRounds_s++;if(offerRounds_s>2){shopOffer_s=coppers;shopMenu=27;}else{shopOffer_s=randn(coppers,itemCost_s);itemLowestCost_s=coppers;shopMenu=23;}}
		else{offerStatus_s=1;offerRounds_s++;shopOffer_s=itemLowestCost_s;if(offerRounds_s>1)shopMenu=19;else shopMenu=23;}
	}
	else if (shopMenu == 19) // thrown out
	{ shopMessage("THAT OFFENDS ME DEEPLY!@Why don't you get serious@and only agree for something@you can afford!"); shopMenu=-1; }
	else if (shopMenu == 27) // offer accepted
	{
		clearShopDisplay(); cText("I'll take it!"); updateDisplay();
		key=getSingleKey();
		if(key!=""){if(!checkCoins(0,0,shopOffer_s))shopMenu=25;else{plyr.shopFriendships[shopNo_s]++;if(plyr.shopFriendships[shopNo_s]>4)plyr.shopFriendships[shopNo_s]=4;shopMenu=24;}}
	}
	else if (shopMenu == 50) // closed
	{
		string ot,ct; int oh=Shops[shopNo_s].openingHour,ch=Shops[shopNo_s].closingHour;
		if(oh<12)ot="morning"; else if(oh<18)ot="afternoon"; else ot="evening";
		if(ch<12)ct="morning"; else if(ch<18)ct="afternoon"; else ct="evening";
		clearShopDisplay(); cyText(1,"Sorry, we are closed. Come back@during our working hours.");
		str="We are open from "+itos(oh)+":00 in the "+ot+"@to "+itos(ch)+":00 in the "+ct+"."; cyText(4,str); cyText(9,"( Press a key )"); updateDisplay();
		key=getSingleKey(); if(key=="SPACE")shopMenu=-1;
	}

	if (shopMenu == -1) { shopMenu=0; leaveShop(); }
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
	// Non-blocking: draw once, key consumed on next frame.
	// For simple one-shot messages still called from non-web code this is fine.
	clearShopDisplay(); cText(txt); updateDisplay(); getSingleKey();
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
