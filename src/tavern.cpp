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
#include "tavern.h"
#include "automap.h"
#include "lyrics.h"

//==============================================================================
//                          Declarations
//==============================================================================
//extern Player           plyr;
// extern sf::RenderWindow App;
extern int              tavernDailyFoods[14][6];
extern int              tavernDailyDrinks[14][6];
extern sf::Clock               clock1;
sf::Music               tavernMusic;
bool                    tavernDrinksCheck[14][12];          // markers used to check for duplicate items - 14 taverns, 12 potential drinks
bool                    tavernFoodsCheck[14][32];           // markers used to check for duplicate items
int                     tavernNo;
int                     drinkChoice, drinkNo, drinkCost;
int                     foodChoice, foodNo, foodCost;
int                     descriptionPointer = 0;

extern int                     iCounter;

string                  eatDrinkDescriptions[4];
string                  openingText, closingText;
string                  lyricsFilename;


//==============================================================================
//                           Structures & Instances
//==============================================================================
struct tavern
{
       string           name;
       float            priceFactor;
	   int              location;                           // match with location text description number
	   int              openingHour;
	   int              closingHour;
	   int              jobProbability;
	   int              membershipFee;                      // usually 0
	   bool             classy;
};

tavern Taverns[14] =
{
//	  name						price		loc		op	cl	job		member	classy
	{"Flaming Dragon",			1.2,		23,		0,	23,	51,		0,		0},
	{"Misty Mountain",			3.5,		24,		11,	2,	51,		3000,	1},
	{"Screaming Siren Bar",		1.2,		25,		16,	3,	64,		0,		0},
	{"Happy Hunter Rest Stop",	1.4,		26,		0,	7,	77,		0,		0},
	{"Dancing Nymph",			1.6,		27,		16,	7,	72,		0,		0},
	{"Club",					3.5,		28,		18,	22,	182,	1500,	1},
	{"Black Devil",				1.4,		29,		0,	5,	64,		0,		1},
	{"Lost Oasis",				0.8,		30,		0,	23,	26,		0,		1},
	{"Last Stop",				1.2,		31,		0,	23,	38,		0,		0},
	{"Tail of the Dog",			2.0,		32,		0,	23,	192,	0,		0},
	{"Club Babylon",			5.0,		33,		10,	4,	77,		50000,	1},
	{"Lost Tears",				1.8,		34,		0,	23,	56,		0,		1},
	{"Mom's Bar",				1.6,		35,		0,	23,	128,	0,		0},
	{"Lusty Lloyds",			1.0,		36,		0,	23,	46,		0,		0}
};

struct tavernDrinkItem
{
	string              name;
	int                 basePrice;
	int                 alcoholAdded;
	int                 thirstRemoved;
	int                 waterFlaskAdded;
};

tavernDrinkItem tavernDrinks[12] =
{
	{"Water Flask",		5,	0,	0,	1},
	{"Milk",			2,	0,	6,	0},
	{"Beer",			2,	1,	2,	0},
	{"Ale",				3,	1,	3,	0},
	{"Wine",			4,	2,	2,	0},
	{"Grog",			2,	2,	2,	0},
	{"Spirits",			5,	2,	2,	0},
	{"Water",			1,	0,	8,	0},
	{"Sarsaparila",		3,	0,	5,	0},
	{"Orange Juice",	3,	0,	5,	0},
	{"Grape Juice",		3,	0,	5,	0},
	{"Mineral Water",	2,	0,	8,	0}
};

struct tavernFoodItem
{
	string              name;
	int                 basePrice;
	int                 hungerRemoved;
	int                 foodPacketAdded;
};

tavernFoodItem tavernFoods[32] =
{
	{"Rack of Lamb",		40,	16,	0},
	{"Roast Beef",			50,	18,	0},
	{"Roast Chicken",		25,	16,	0},
	{"Roast Dragon",		150,22,	1},
	{"Pork Ribs",			30,	12,	0},
	{"Leg of Lamb",			80,	19,	0},
	{"Fried Chicken",		50,	15,	0},
	{"Leg of Dragon",		300,22,	2},
	{"Ham",					60,	14,	0},
	{"Lamb",				56,	17,	0},
	{"Filet of Beef",		70,	19,	0},
	{"Vegetable Soup",		5,	8,	0},
	{"Ragout of Beef",		33,	10,	0},
	{"Ragout of Dragon",	50,	11,	1},
	{"Bowl of Fruit",		25,	7,	0},
	{"Plate of Greens",	    18,	6,	0},
	{"Smoked Fish",			30,	12,	0},
	{"Apple Pie",			12,	4,	0},
	{"Lemon Pie",			12,	4,	0},
	{"Chocolate Cake",		10,	4,	0},
	{"Loaf of Bread",		8,	6,	0},
	{"Block of Cheese",		15,	9,	0},
	{"Food Packet",			25, 0,	1},
	{"Gruel",				4,	8,	0},
	{"Bagel",				6,	5,	0},
	{"Pemmican",			20,	4,	1},
	{"Bowl of Chili",		10,	6,	0},
	{"Pasta",				30,	10,	0},
	{"Lasagna",				30,	10,	0},
	{"Crayfish",			40,	9,	0},
	{"Lobster",				100,14,	0},
	{"Sandwich",			10,	8,	0}
};

struct tavernJobOpening
{
	int                 jobNumber;
	int                 JobHoursRequired;
	int                 jobHourlyIncome;
};

tavernJobOpening tavernJobOpenings[14];

struct tavernJob
{
	string              name;
	int                 minIncome;
	int                 maxIncome;
	string              statRequirementName;
	int                 statRequirementValue;
	float               fatigueRate;
	float               minorWoundProbability;
	float               majorWoundProbability;
};

tavernJob tavernJobs[3] =
{
	{"Bouncer",	    40,	44,	"Strength",	22,	0.75,	19.69, 7.26},
	{"Host",	    20,	24,	"Charm",	12,	0.5625, 0.77,  0.01},
	{"Dish Washer",	8,	12,	"Skill",    9,	0.5625, 12.64, 2.20}
};


//==============================================================================
//                                Functions
//==============================================================================
void checkDailyTavernJobOpenings()
{
	// Run at the start of each new day
	int jobOpeningProbability = 0;
	for (int i=0 ; i<14 ; i++) // 14 taverns in total
	{
		jobOpeningProbability  = randn(0,255);
		if (jobOpeningProbability <= Taverns[i].jobProbability)
		{
			// Create a new job entry for the day
			int newJobNumber = randn(0,2);
			tavernJobOpenings[i].jobNumber = newJobNumber;
			tavernJobOpenings[i].JobHoursRequired = randn(0,5)+3;
			tavernJobOpenings[i].jobHourlyIncome = randn(tavernJobs[newJobNumber].minIncome,tavernJobs[newJobNumber].maxIncome);

		}
		else
		{
			// No job available today
			tavernJobOpenings[i].jobNumber = 255; // 255 for none
		}

	}
}

/*
inline std::string concat( int n, const char* str )
{
std::ostringstream ss;
ss << n;
ss << str;
return ss.str();
}
*/


void shopTavern()
{
	int tavernNo = getTavernNo();

	loadShopImage(10);

	if (tavernNo==3)
	{
		setAutoMapFlag(plyr.map, 7, 39);
		setAutoMapFlag(plyr.map, 8, 39);
		setAutoMapFlag(plyr.map, 9, 39);
		setAutoMapFlag(plyr.map, 10,39);
	}
	if (tavernNo==6)
	{
		setAutoMapFlag(plyr.map, 32, 44);
		setAutoMapFlag(plyr.map, 31, 44);
		setAutoMapFlag(plyr.map, 33, 44);
		setAutoMapFlag(plyr.map, 31, 43);
	}
	if (tavernNo==9)
	{
		setAutoMapFlag(plyr.map, 39, 34);
		setAutoMapFlag(plyr.map, 39, 33);
	}
	if (tavernNo==12)
	{
		setAutoMapFlag(plyr.map, 5, 28);
		setAutoMapFlag(plyr.map, 6, 28);

	}

	int workingHours, hourlyRate, jobIncome;
	descriptionPointer = 0;
	eatDrinkDescriptions[0] = "";
	eatDrinkDescriptions[1] = "";
	eatDrinkDescriptions[2] = "";
	eatDrinkDescriptions[3] = "";

	tavernNo = getTavernNo();

	tavernMusic.setLoop(false);
	bool musicPlaying = false;
	int tavernMenu = 1; // high level menu
	int tavernLoc = 0; // bar, table or booth
	string str,key;
	plyr.status = 2; // shopping


	if (Taverns[tavernNo].closingHour < Taverns[tavernNo].openingHour )
	{
		//We have hours that wrap into the next day.
		if (Taverns[tavernNo].openingHour > plyr.hours )
			tavernMenu = 20;
		if (Taverns[tavernNo].closingHour < plyr.hours && plyr.hours < Taverns[tavernNo].openingHour)
			tavernMenu = 20;
	} else
	if ((Taverns[tavernNo].closingHour<=plyr.hours) || (Taverns[tavernNo].openingHour>plyr.hours)) {tavernMenu = 20;}
	
	if (Taverns[tavernNo].membershipFee > 0) { tavernMenu = 21; }

	while (tavernMenu > 0)
	{

		while (tavernMenu == 20) // closed
		{
			int openHour = Taverns[tavernNo].openingHour;
			int closeHour = Taverns[tavernNo].closingHour;
			if ((openHour>=0) && (openHour<12)) openingText = "morning";
			if ((openHour>=12) && (openHour<18)) openingText = "afternoon";
			if ((openHour>=18) && (openHour<=23)) openingText = "evening";
			if ((closeHour>=0) && (closeHour<12)) closingText = "morning";
			if ((closeHour>=12) && (closeHour<18)) closingText = "afternoon";
			if ((closeHour>=18) && (closeHour<=23)) closingText = "evening";


			tavernDisplayUpdate();
			cyText (1, "Sorry, we are closed. Come back@during our working hours.");
			str = "We are open from " + itos(Taverns[tavernNo].openingHour) + ":00 in the "+openingText+"@to " + itos(Taverns[tavernNo].closingHour) + ":00 in the "+closingText+".";
			cyText (4, str);
			cyText (9,"( Press a key )");
			updateDisplay();

			key = getSingleKey();
			if ( key == "SPACE" ) { tavernMenu = 0; }
		}

		while (tavernMenu == 21) // membership fee required
		{
				//clearShopDisplay();
				tavernDisplayUpdate();
				cyText (1, "To enter you must become a member.");
				str = "Dues are " + toCurrency(Taverns[tavernNo].membershipFee) + " copper coins.";
				cyText (3, str);
				//cyText (3, "Would you like to apply?");
				cyText (5, "( es or  o)");
				SetFontColour(40, 96, 244, 255);
				cyText (5, " Y      N  ");
				SetFontColour(215, 215, 215, 255);
				displayCoins();
				updateDisplay();

			key = getSingleKey();
			if ( key == "N" ) { tavernMenu = 0; }
			if ( key == "Y" ) { tavernMenu = 22; }
		}

while (tavernMenu == 22) // Attempt to buy a club membership
		{
			tavernNo = getTavernNo();
			//drinkNo = tavernDailyDrinks[tavernNo][drinkChoice];
			int membershipCost = Taverns[tavernNo].membershipFee;
			if (!checkCoins(0,0,membershipCost)) { tavernMenu = 23; } // offended!
			else
			{
				deductCoins(0,0,membershipCost);
				Taverns[tavernNo].membershipFee = 0; // needs reseting on new game!
				tavernMenu = 1;
			}
		}

		while (tavernMenu == 23) // Insufficient funds for club membership
		{
			//clearShopDisplay();
			tavernDisplayUpdate();
			cyText (3, "I'm sorry... You have not the funds.");
			updateDisplay();
			key = getSingleKey();
			if ( key=="SPACE" ) { tavernMenu = 0; }
		}



		while (tavernMenu == 1) // main menu
		{
			// check whether player requires food and drink based on tavern friendship level
			int countedCoppers = (plyr.gold*100) + (plyr.silver*10) + plyr.copper;
			if ((plyr.tavernFriendships[tavernNo] >= 4) && (plyr.water==0) && (countedCoppers==0) && (plyr.thirst>56))
            {
                plyr.thirst = 0; plyr.water = 1; tavernMessage("Friend, you thirst. Let me get you a drink.");
            }
            if ((plyr.tavernFriendships[tavernNo] >= 4) && (plyr.food==0) && (countedCoppers==0) && (plyr.hunger>96))
            {
                plyr.hunger = 0; plyr.food = 1; tavernMessage("Friend, you hunger. Let me get you some food.");
            }

			tavernDisplayUpdate();

			bText (7,0, "Welcome Stranger! You are at ");
			bText (7,1, "the entrance. Do you wish to");
			bText (9,3, " ) Go to the bar");
			bText (9,4, " ) Get a table");
			bText (9,5, " ) Sit in a private booth");
			bText (9,6, " ) Apply for a job");
			bText (9,7, " ) Leave");
			SetFontColour(40, 96, 244, 255);
			bText (9,3, "1");
			bText (9,4, "2");
			bText (9,5, "3");
			bText (9,6, "4");
			bText (9,7, "0");
			SetFontColour(215, 215, 215, 255);
			displayCoins();

			updateDisplay();

			if (!musicPlaying)
			{
				//int Random = randn(0, 2);

				int Random = randn(1,5);
				if (plyr.musicStyle==0)
				{
					if (Random ==1) { tavernMusic.openFromFile("data/audio/dwarfdance.ogg"); lyricsFilename = "dwarfdance.txt"; }
					if (Random ==2) { tavernMusic.openFromFile("data/audio/thoreandan.ogg"); lyricsFilename = "thoreandan.txt"; }
					if (Random ==3) { tavernMusic.openFromFile("data/audio/waves.ogg"); lyricsFilename = "waves.txt"; }
					if (Random ==4) { tavernMusic.openFromFile("data/audio/moments.ogg"); lyricsFilename = "moments.txt"; }
					if (Random ==5) { tavernMusic.openFromFile("data/audio/B/TheNightstalker.ogg"); lyricsFilename = "TheNightstalker.txt"; }
				}
				if (plyr.musicStyle==1)
				{
					if (Random ==1) { tavernMusic.openFromFile("data/audio/B/dwarfdance.ogg"); lyricsFilename = "dwarfdance.txt"; }
					if (Random ==2) { tavernMusic.openFromFile("data/audio/B/thoreandan.ogg"); lyricsFilename = "thoreandan.txt"; }
					if (Random ==3) { tavernMusic.openFromFile("data/audio/B/waves.ogg"); lyricsFilename = "waves.txt"; }
					if (Random ==4) { tavernMusic.openFromFile("data/audio/B/LetInTheLight.ogg"); lyricsFilename = "LetInTheLight.txt"; }
					if (Random ==5) { tavernMusic.openFromFile("data/audio/B/TheNightstalker.ogg"); lyricsFilename = "TheNightstalker.txt"; }
				}

				loadLyrics(lyricsFilename);
				tavernMusic.play();
				musicPlaying = true;
			}

			//key = pressKey();
			key = getSingleKey();

			if ( key=="1" ) { tavernMenu = 2; tavernLoc = 1; }
			if ( key=="2" ) { tavernMenu = 2; tavernLoc = 2;}
			if ( key=="3" ) { tavernMenu = 2; tavernLoc = 3;}
			if ( key=="4" ) { tavernMenu = 11; }
			if ( key=="0" ) { tavernMenu = 0; }
			if ( key=="down" ) { tavernMenu = 0; }
			if (key=="F1") { tavernMusic.stop(); loadLyrics(lyricsFilename); tavernMusic.play(); }
		}

		while (tavernMenu == 2) // at bar, table or booth menu
		{
			//clearShopDisplay();
			tavernDisplayUpdate();
			if ( tavernLoc == 1) { bText (7,0, "You are sitting at the bar."); bText (23,3, "A few nuts"); }
			if ( tavernLoc == 2) { bText (7,0, "You are at your table."); bText (26,3, "Popcorn"); }
			if ( tavernLoc == 3) { bText (7,0, "You are in a private booth."); bText (7,3, "A smokey torch  A few nuts"); tavernMusic.stop(); musicPlaying = false; }
			bText (7,4, " ) Hail the Barkeeper");
			bText (7,5, " ) Hail the Waitress");
			bText (7,6, " ) Buy a round for the house");
			bText (7,7, " ) Leave");
			SetFontColour(40, 96, 244, 255);
			bText (7,4, "1");
			bText (7,5, "2");
			bText (7,6, "3");
			bText (7,7, "0");
			SetFontColour(215, 215, 215, 255);
			displayCoins();

			SetFontColour(208, 178, 2,255);
			bText (7,1,(eatDrinkDescriptions[0]));
			bText (23,1,(eatDrinkDescriptions[1]));
			bText (7,2,(eatDrinkDescriptions[2]));
			bText (23,2,(eatDrinkDescriptions[3]));
			SetFontColour(215, 215, 215, 255);

			updateDisplay();

			key = getSingleKey();
			if ( key=="0" ) { tavernMenu = 0; }
			if ( key=="1" ) { tavernMenu = 3; }
			if ( key=="2" ) { tavernMenu = 7; }
			if ( key=="3" ) { tavernMenu = 30; } // Buy a round


		}

		while (tavernMenu == 3) // Order a drink
		{

			tavernDisplayUpdate();
			cyText (0, "What would you like? (  to go back)");
			SetFontColour(40, 96, 244, 255);
			cyText (0, "                      0            ");
			SetFontColour(215, 215, 215, 255);

			tavernNo = getTavernNo();
			for (int i=0 ; i<6 ; i++)
			{
				int itemNo = tavernDailyDrinks[tavernNo][i];
				str = ") " + tavernDrinks[itemNo].name;
			//	if ((tavernDrinks[itemNo].itemRef) > 10)
			//	{ str = ") " + tavernDrinks[itemNo].name; }
				bText(3,(2+i), str); //was 4
				bText(1,(2+i), "                                 coppers");
			}
			displayCoins();

			int itemCost, x;
			for (int i=0 ; i<6 ; i++) // Max 6 drink items on menu each day
			{
				string itemCostDesc;
				x = 33;
				int itemNo = tavernDailyDrinks[tavernNo][i];
				itemCost = Taverns[tavernNo].priceFactor * tavernDrinks[itemNo].basePrice;
				//itemCost = tavernWares[itemNo].basePrice;
				if (itemCost<10) { x = 34;}
				if ((itemCost>9) && (itemCost < 100)) { x = 32;}
				if (itemCost<1000) { x = 30;}
				//if (itemCost>999) { x = 29;}
				//if (itemCost>9999) { x = 27;}
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
			//if (menuStartItem!=0) { bText (2,1, "}"); }
			//if (menuStartItem!=4) { bText (2,8, "{"); }
			SetFontColour(215, 215, 215, 255);

			updateDisplay();

			key = getSingleKey();
			if ( key=="1" ) { drinkChoice = 0; tavernMenu = 4; }
			if ( key=="2" ) { drinkChoice = 1; tavernMenu = 4; }
			if ( key=="3" ) { drinkChoice = 2; tavernMenu = 4; }
			if ( key=="4" ) { drinkChoice = 3; tavernMenu = 4; }
			if ( key=="5" ) { drinkChoice = 4; tavernMenu = 4; }
			if ( key=="6" ) { drinkChoice = 5; tavernMenu = 4; }
			//if ( (key=="up") && (menuStartItem>0) ) { menuStartItem--; }
			//if ( (key=="down") && (menuStartItem<4) ) { menuStartItem++; }
			if ( key=="ESC" ) { tavernMenu = 0; }
			if ( key=="0" ) { tavernMenu = 2; }

		}


		while (tavernMenu == 4) // Attempt to buy a drink
		{
			tavernNo = getTavernNo();
			drinkNo = tavernDailyDrinks[tavernNo][drinkChoice];
			drinkCost = Taverns[tavernNo].priceFactor * tavernDrinks[drinkNo].basePrice;
			if (!checkCoins(0,0,drinkCost)) { tavernMenu = 5; } // offended!
			else { tavernMenu = 6; }
		}

		while (tavernMenu == 5) // Insufficient funds
		{
			tavernDisplayUpdate();
			cyText (3, "I'm sorry... You have not the funds.");
			updateDisplay();
			key = getSingleKey();
			if ( key=="SPACE" ) { tavernMenu = 3; }
		}

		while (tavernMenu == 6) // Successful purchase
		{
			tavernDisplayUpdate();
			cyText (3, "Right away!");
			updateDisplay();
			key = getSingleKey();
			if ( key=="SPACE" )
			{
				tavernMenu = 2;
				//int drinkCost = Taverns[tavernNo].priceFactor * tavernDrinks[drinkNo].basePrice;
				deductCoins(0,0,drinkCost);
				plyr.thirst -= tavernDrinks[drinkNo].thirstRemoved;
				if (plyr.thirst < 0) { plyr.thirst = 0; }
				plyr.alcohol += tavernDrinks[drinkNo].alcoholAdded;
				plyr.water += tavernDrinks[drinkNo].waterFlaskAdded;
				eatDrinkDescriptions[descriptionPointer] = tavernDrinks[drinkNo].name;
				if ( descriptionPointer== 3 ) { descriptionPointer = 0; } else { descriptionPointer++; }

			}
		}


		while (tavernMenu == 7) // Order food
		{

			tavernDisplayUpdate();
			cyText (0, "What would you like? (  to go back)");
			SetFontColour(40, 96, 244, 255);
			cyText (0, "                      0            ");
			SetFontColour(215, 215, 215, 255);

			tavernNo = getTavernNo();
			for (int i=0 ; i<6 ; i++)
			{
				int itemNo = tavernDailyFoods[tavernNo][i];
				str = ") " + tavernFoods[itemNo].name;
			//	if ((tavernDrinks[itemNo].itemRef) > 10)
			//	{ str = ") " + tavernDrinks[itemNo].name; }
				bText(3,(2+i), str); //was 4
				bText(1,(2+i), "                                 coppers");
			}
			displayCoins();

			int itemCost, x;
			for (int i=0 ; i<6 ; i++) // Max 6 drink items on menu each day
			{
				string itemCostDesc;
				x = 33;
				int itemNo = tavernDailyFoods[tavernNo][i];
				itemCost = Taverns[tavernNo].priceFactor * tavernFoods[itemNo].basePrice;
				//itemCost = tavernWares[itemNo].basePrice;
				if (itemCost<10) { x = 37;}
				if ((itemCost>9) && (itemCost < 100)) { x = 34;}
				if (itemCost<1000) { x = 30;}
				//if (itemCost>999) { x = 28;}
				//if (itemCost>9999) { x = 27;}
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
			//if (menuStartItem!=0) { bText (2,1, "}"); }
			//if (menuStartItem!=4) { bText (2,8, "{"); }
			SetFontColour(215, 215, 215, 255);

			updateDisplay();

			key = getSingleKey();
			if ( key=="1" ) { foodChoice = 0; tavernMenu = 8; }
			if ( key=="2" ) { foodChoice = 1; tavernMenu = 8; }
			if ( key=="3" ) { foodChoice = 2; tavernMenu = 8; }
			if ( key=="4" ) { foodChoice = 3; tavernMenu = 8; }
			if ( key=="5" ) { foodChoice = 4; tavernMenu = 8; }
			if ( key=="6" ) { foodChoice = 5; tavernMenu = 8; }
			//if ( (key=="up") && (menuStartItem>0) ) { menuStartItem--; }
			//if ( (key=="down") && (menuStartItem<4) ) { menuStartItem++; }
			if ( key=="ESC" ) { tavernMenu = 0; }
			if ( key=="0" ) { tavernMenu = 2; }

		}




		while (tavernMenu == 8) // Attempt to buy food
		{
			tavernNo = getTavernNo();
			foodNo = tavernDailyFoods[tavernNo][foodChoice];
			foodCost = Taverns[tavernNo].priceFactor * tavernFoods[foodNo].basePrice;
			if (!checkCoins(0,0,foodCost)) { tavernMenu = 9; } // offended!
			else { tavernMenu = 10; }
		}

		while (tavernMenu == 9) // Insufficient funds
		{
			tavernDisplayUpdate();
			cyText (3, "I'm sorry... You have not the funds.");
			updateDisplay();
			key = getSingleKey();
			if ( key=="SPACE" ) { tavernMenu = 7; }
		}

		while (tavernMenu == 10) // Successful purchase
		{
			tavernDisplayUpdate();
			cyText (3, "Right away!");
			updateDisplay();
			key = getSingleKey();
			if ( key=="SPACE" )
			{
				tavernMenu = 2;
				//int drinkCost = Taverns[tavernNo].priceFactor * tavernFoods[drinkNo].basePrice;
				deductCoins(0,0,foodCost);
				plyr.hunger -= tavernFoods[foodNo].hungerRemoved;
				if (plyr.hunger < 0) { plyr.hunger = 0; }
				//plyr.alcohol += tavernFoods[drinkNo].alcoholAdded;
				plyr.digestion += (tavernFoods[foodNo].hungerRemoved)*2;
				plyr.food += tavernFoods[foodNo].foodPacketAdded;
				eatDrinkDescriptions[descriptionPointer] = tavernFoods[foodNo].name;
				if ( descriptionPointer== 3 ) { descriptionPointer = 0; } else { descriptionPointer++; }

			}
		}





		while (tavernMenu == 11) // apply for job
		{
			int jobNumber = tavernJobOpenings[tavernNo].jobNumber;

			tavernDisplayUpdate();
			if ( jobNumber == 255)
			{
				bText (7,0, "I'm sorry but there are no");
				cyText (1, "job openings at the moment.");
				cyText (9,"( Press a key )");
				updateDisplay();

				key = getSingleKey();
				//if ( key=="Y" ) { tavernMenu = 9; }
				if ( key!="" ) { tavernMenu = 1; }

			}
			else
			{	str = "We have an opening for a " + tavernJobs[jobNumber].name;
				cyText (0, str);
				str = "for " + itos(tavernJobOpenings[tavernNo].JobHoursRequired) + " hours at " + itos(tavernJobOpenings[tavernNo].jobHourlyIncome) + " coppers per hour.";
				cyText (1, str);
				cyText (3, "Would you like to apply?");
				cyText (5, "( es or  o)");
				SetFontColour(40, 96, 244, 255);
				cyText (5, " Y      N  ");
				SetFontColour(215, 215, 215, 255);
				updateDisplay();

				key = getSingleKey();
				if ( key=="Y" ) { tavernMenu = 12; }
				if ( key=="N" ) { tavernMenu = 1; }

			}

		}


		while (tavernMenu == 12) // Check job stat requirements
		{
			int jobNumber = tavernJobOpenings[tavernNo].jobNumber;
			string statRequirementName = tavernJobs[jobNumber].statRequirementName;
			int statRequirement = tavernJobs[jobNumber].statRequirementValue;
			bool jobStatMet = false;

			// Check stat requirement met
			if ((statRequirementName == "Strength") && (statRequirement<=plyr.str)) { jobStatMet = true; }
			if ((statRequirementName == "Charm") &&	(statRequirement<=plyr.chr)) { jobStatMet = true; }
			if ((statRequirementName == "Skill") && (statRequirement<=plyr.skl)) { jobStatMet = true; }



			if (!jobStatMet)
			{
				tavernDisplayUpdate();
				str = "You will need more " + statRequirementName;
				cyText (0, str);
				cyText (1, "to get the job.");
				cyText (9,"( Press a key )");
				updateDisplay();

				key = getSingleKey();

				if ( key=="SPACE" ) { tavernMenu = 1; }

			}
			else
			{
				workingHours = tavernJobOpenings[tavernNo].JobHoursRequired;
				hourlyRate = tavernJobOpenings[tavernNo].jobHourlyIncome;
				jobIncome = workingHours*hourlyRate;
				tavernMenu = 13;
			}
		}




		while (tavernMenu == 13) // Display working message
		{


			while (workingHours>0)
			{

				tavernDisplayUpdate();
				//bText (1,1,workingHours);
				cyText (2,"WORKING");
				updateDisplay();
				sf::sleep(sf::seconds(1));
				for (int i=0 ; i<60 ; i++) // 60 minutes
				{
					//sf::sleep(0.01f);
					// check for diseases
					// modify fatigue
					// modify hitpoints
					// modify temporary magic bonuses
				}
				addHour();

				workingHours--;
			}


			tavernDisplayUpdate();
			// CHECK FOR INJURY
			cyText (2,"The job is completed.");
			str = "You have earned " + itos(jobIncome) + " coppers.";
			cyText (3,str);
			cyText (9,"( Press a key )");
			updateDisplay();
			//key = pressKey();
			key = getSingleKey();
			if (key=="SPACE") { plyr.copper+=jobIncome; tavernJobOpenings[tavernNo].jobNumber=255; tavernMenu = 1; }

		}



        while (tavernMenu == 30) // Buy a round
		{
		    int roundCost = 80 * Taverns[tavernNo].priceFactor;
		    tavernDisplayUpdate();
			str = "A round for the house will cost@@" + itos(roundCost) + " coppers.@@@Dost thou still wish to buy? (Y or N)";
            cyText (0, str);
            updateDisplay();
            key = getSingleKey();
            if ( key=="Y" )
            {
                bool affordRound = checkCoins(0,0,roundCost);
                if (affordRound) { deductCoins(0,0,roundCost); plyr.tavernFriendships[tavernNo]+=1; tavernMenu = 31; }
                else { plyr.tavernFriendships[tavernNo]-=1; tavernMessage("I'm sorry you have not the funds."); tavernMenu = 2; }
            }
            if ( key=="N" ) { tavernMenu = 2; }
		}

		while (tavernMenu == 31) // Round successfully bought messages
		{
		    int tf = plyr.tavernFriendships[tavernNo];
		    str = "The patrons go up to the bar.";
		    if ((tf >= 0) && (tf < 3)) { str = "A few people take up your offer."; }
			if ((tf > 3) && (tf < 6)) { str = "You have won yourself some friends, Adventurer."; }
			if ((tf > 5) && (tf < 7)) { str = "All the patrons applaud your generosity!"; }
			if (tf > 6) { str = "A toast to our friend, " + plyr.name +"."; }
            tavernMessage(str);
            tavernMenu = 2;
		}



	}
	if (musicPlaying) { tavernMusic.stop(); }
	leaveShop();
}



void tavernDisplayUpdate()
{
    clock1.restart();
	clearShopDisplay();
	updateLyrics();
	iCounter++;
}


int getTavernNo()
{
	int tavern_no;
	for (int i=0 ; i<14 ; i++) // Max number of tavern objects
	{
		if (Taverns[i].location == plyr.location)
		{
				tavern_no = i; // The number of the tavern you have entered
		}
	}
	return tavern_no;
}

void stockTavernDrinks()
{
	// Run each hour to randomly pick 10 items for sale at each of the 4 smithies
	// Check for duplicates using smithyWaresCheck array of bools

	// Set bools for duplicate items check to false
	int itemNo = 0;

	for (int x=0; x<14; x++)
	{
		for (int y=0; y<12; y++)
		{
			tavernDrinksCheck[x][y] = false;
		}
	}

	for (int tavernNo=0 ; tavernNo<14 ; tavernNo++)
	{
		for (int waresNo=0 ; waresNo<6 ; waresNo++)
		{
			// Current code may create duplicate items in each tavern
			bool uniqueItem = false;
			while (!uniqueItem)
			{
				itemNo = randn(0,11); // was 12


				if (!tavernDrinksCheck[tavernNo][itemNo])
				{
					tavernDailyDrinks[tavernNo][waresNo] = itemNo; // its not a duplicate
					tavernDrinksCheck[tavernNo][itemNo] = true;
					uniqueItem = true;
				}
			}
		}
	}

	// Simple sort of items in numeric order
//	for (int tavernNo=0 ; tavernNo<14 ; tavernNo++)
//	{
//		sort(tavernDailyDrinks[tavernNo], tavernDailyDrinks[tavernNo]+6); // 6 max no drinks available on menu
//	}

}


void stockTavernFoods()
{
	// Run each hour to randomly pick 10 items for sale at each of the 4 smithies
	// Check for duplicates using smithyWaresCheck array of bools

	// Set bools for duplicate items check to false
	int itemNo = 0;

	for (int x=0; x<14; x++)
	{
		for (int y=0; y<12; y++)
		{
			tavernFoodsCheck[x][y] = false;
		}
	}

	for (int tavernNo=0 ; tavernNo<14 ; tavernNo++)
	{
		for (int waresNo=0 ; waresNo<6 ; waresNo++)
		{
			// Current code may create duplicate items in each tavern
			bool uniqueItem = false;
			while (!uniqueItem)
			{
				itemNo = randn(0,31); // was 32

				if (!tavernFoodsCheck[tavernNo][itemNo])
				{
					tavernDailyFoods[tavernNo][waresNo] = itemNo; // its not a duplicate
					tavernFoodsCheck[tavernNo][itemNo] = true;
					uniqueItem = true;
				}
			}
		}
	}

	// Simple sort of items in numeric order
//	for (int tavernNo=0 ; tavernNo<14 ; tavernNo++)
//	{
//		sort(tavernDailyDrinks[tavernNo], tavernDailyDrinks[tavernNo]+6); // 6 max no drinks available on menu
//	}

}


void tavernMessage(string txt)
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
