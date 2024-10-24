
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
#include "inn.h"
#include "automap.h"

// extern Player plyr;
// extern sf::RenderWindow App;

struct innRoom
{
	string name;
	int baseCost;
    int fatigueRecoveryProbability;

};

innRoom Rooms[10] =
{
	{"the common area floor",	10,		30},
	{"a bed with no bath",		20,		45},
	{"a bed with common bath",	30,		60},
	{"a room with common bath",	40,		85},
	{"a room with bath",		50,		100},
	{"a Premium Room",			100,	140},
	{"a Deluxe Room",			200,	170},
	{"a Small Suite",			400,	200},
	{"a Suite",					800,	240},
	{"our BEST Suite",			1600,	255}
};

struct Inn
{
       string name;
       float costMultiplier;
       int jobProbability;
	   int x;
	   int y;
};

Inn Inns[7] =
{
	{"Green Boar Inn",		1,		64,		9,		44},
	{"Lazy Griffin Inn",	2,		154,	33,		11},
	{"Sleeping Dragon Inn",	0.5,	64,		60,		57},
	{"Traveller's Inn",		6,		179,	32,		39},
	{"Midnight Inn",		3,		56,		32,		40},
	{"Warrior's Retreat",	0.75,	77,		28,		9},
	{"Royal Resort Inn",	8,		102,	31,		60}
};

struct innJobOpening
{
	int jobNumber;
	int JobHoursRequired;
	int jobHourlyIncome;
};


innJobOpening innJobOpenings[7];

struct innJob
{
	string name;
	int minIncome;
	int maxIncome;
	string statRequirementName;
	int statRequirementValue;
	float fatigueRate;
	float minorWoundProbability;
	float majorWoundProbability;
};

innJob innJobs[3] =
{
	{"Bellhop",	10,	24,	"Strength",	18,	0.6875, 15.95, 3.97},
	{"Desk Clerk",	18,	20,	"Charm",16,	0.5625, 3.75, 0.15},
	{"Janitor",	12,	15,	"Stamina",	17,	0.6250, 8.50, 0.88 }
};

void checkDailyInnJobOpenings()
{
	// Run at the start of each new day
	int jobOpeningProbability = 0;
	for (int i=0 ; i<7 ; i++)
	{
		jobOpeningProbability  = randn(0,255);
		if (jobOpeningProbability <= Inns[i].jobProbability)
		{
			// Create a new job entry for the day
			int newJobNumber = randn(0,2);
			innJobOpenings[i].jobNumber = newJobNumber;
			innJobOpenings[i].JobHoursRequired = randn(0,5)+3;
			innJobOpenings[i].jobHourlyIncome = randn(innJobs[newJobNumber].minIncome,innJobs[newJobNumber].maxIncome);

		}
		else
		{
			// No job available today
			innJobOpenings[i].jobNumber = 255;
		}

	}
}



void shopInn()
{
	int InnNo = getInnNo();
	if (InnNo==0)
	{
		setAutoMapFlag(plyr.map, 8, 44);
		setAutoMapFlag(plyr.map, 9, 44);
		setAutoMapFlag(plyr.map, 10, 44);
		setAutoMapFlag(plyr.map, 8, 45);
	}
	if (InnNo==3)
	{
		setAutoMapFlag(plyr.map, 31, 38);
		setAutoMapFlag(plyr.map, 32, 39);
		setAutoMapFlag(plyr.map, 31, 39);
	}
	if (InnNo==4)
	{
		setAutoMapFlag(plyr.map, 31, 40);
		setAutoMapFlag(plyr.map, 32, 40);
		setAutoMapFlag(plyr.map, 33, 40);
		setAutoMapFlag(plyr.map, 31, 41);
	}

	int roomCost;
	int InnMenu = 1; // high level menu
	int roomChoice = 0;
	int sleepingHours = 1;
	int workingHours = 0;
	int hourlyRate = 0;
	int jobIncome = 0;

loadShopImage(11);

	//int InnLoc = 0; // bar, table or booth
	string str,key;
	plyr.status = 2; // shopping

	while (InnMenu > 0)
	{
		while (InnMenu == 1) // main menu
		{

			clearShopDisplay();
			bText (6,0, "You are at the Inn's counter");
			bText (10,2, "Do you wish to");
			bText (12,4, " ) Stay the night");
			bText (12,5, " ) Check the time");
			bText (12,6, " ) Apply for a job");
			bText (12,7, " ) Leave");
			displayCoins();
			SetFontColour(40, 96, 244, 255);
			bText (12,4, "1");
			bText (12,5, "2");
			bText (12,6, "3");
			bText (12,7, "0");
			SetFontColour(215, 215, 215, 255);
			updateDisplay();


			key = getSingleKey();

			if ( key=="1" ) { InnMenu = 2; }
			if ( key=="2" ) { InnMenu = 7; }
			if ( key=="3" ) { InnMenu = 8; }
			if ( key=="0" ) { InnMenu = 0; }
			if ( key=="down" ) { InnMenu = 0; }
		}

		while (InnMenu == 2) // first room booking menu
		{

			clearShopDisplay();
			bText (8,0, "Would you like to sleep in");
			bText (1,2, " ) the common area floor         coppers");
			bText (1,3, " ) a Bed with no bath            coppers");
			bText (1,4, " ) a Bed with common Bath        coppers");
			bText (1,5, " ) a Room, with common Bath      coppers");
			bText (1,6, " ) a Room with Bath              coppers");
			bText (1,7, " ) Something better");
			displayCoins();

			int room_cost, x;
			for (int i=0 ; i<5 ; i++) // Max number of room prices in this menu display
			{
				x = 30;
				room_cost = Rooms[i].baseCost * Inns[InnNo].costMultiplier;
				if (room_cost<100) { x = 31;}
				if (room_cost>999) { x = 29;}
				bText (x,(i+2),room_cost);
			}

			SetFontColour(40, 96, 244, 255);
			bText (1,2, "1");
			bText (1,3, "2");
			bText (1,4, "3");
			bText (1,5, "4");
			bText (1,6, "5");
			bText (1,7, "6");
			SetFontColour(215, 215, 215, 255);

			updateDisplay();

			//key = pressKey();
			key = getSingleKey();
			if ( key=="1" ) { roomChoice = 0; InnMenu = 4; }
			if ( key=="2" ) { roomChoice = 1; InnMenu = 4; }
			if ( key=="3" ) { roomChoice = 2; InnMenu = 4; }
			if ( key=="4" ) { roomChoice = 3; InnMenu = 4; }
			if ( key=="5" ) { roomChoice = 4; InnMenu = 4; }
			if ( key=="6" ) { InnMenu = 3; }
			if ( key=="0" ) { InnMenu = 0; }
		}

		while (InnMenu == 3) // Join Inn menu
		{


			clearShopDisplay();
			bText (8,0, "Would you like to sleep in");
			bText (1,2, " ) a Premium Room");
			bText (1,3, " ) a Deluxe Room");
			bText (1,4, " ) a Small Suite");
			bText (1,5, " ) a Suite");
			bText (1,6, " ) Our BEST Suite");
			bText (1,7, " ) Something cheaper");
			displayCoins();

			SetFontColour(40, 96, 244, 255);
			bText (1,2, "1");
			bText (1,3, "2");
			bText (1,4, "3");
			bText (1,5, "4");
			bText (1,6, "5");
			bText (1,7, "6");
			SetFontColour(215, 215, 215, 255);

			int room_cost, x;
			for (int i=5 ; i<10 ; i++) // Max number of room prices in this menu display
			{
				x = 28;
				room_cost = static_cast<int>(Rooms[i].baseCost * Inns[InnNo].costMultiplier);
				if (room_cost<1000) { x = 30; }
				bText (x,(i-3),toCurrency(room_cost));
				bText (34,(i-3),"coppers");
			}

			updateDisplay();

			key = getSingleKey();
			if ( key=="6" ) { InnMenu = 2; }
			if ( key=="1" ) { roomChoice = 5; InnMenu = 4; }
			if ( key=="2" ) { roomChoice = 6; InnMenu = 4; }
			if ( key=="3" ) { roomChoice = 7; InnMenu = 4; }
			if ( key=="4" ) { roomChoice = 8; InnMenu = 4; }
			if ( key=="5" ) { roomChoice = 9; InnMenu = 4; }
			if ( key=="0" ) { InnMenu = 0; }
		}

		while (InnMenu == 4) // Confirm room choice and rate
		{

			clearShopDisplay();
			cyText (0, "Our rate for sleeping in");
			str = Rooms[roomChoice].name + " is " + toCurrency((Rooms[roomChoice].baseCost * Inns[InnNo].costMultiplier)) + " coppers.";
			cyText (1, str);
			cyText (3,"Do you wish to sign in?");
			cyText (5,"( es or  o)");
			displayCoins();
			SetFontColour(40, 96, 244, 255);
			cyText (5," Y      N  ");
			SetFontColour(215, 215, 215, 255);

			updateDisplay();

			key = getSingleKey();
			if ( key=="Y" ) { InnMenu = 5; }
			if ( key=="N" ) { InnMenu = 1; }
		}

		while (InnMenu == 5) // Check funds and display sleeping time choices
		{

			clearShopDisplay();
			roomCost = static_cast<int>(Rooms[roomChoice].baseCost * Inns[InnNo].costMultiplier);
			if (!checkCoins(0,0,roomCost))
			{
				cText ("I'm sorry, you have not the funds.");
				cyText (9,"( Press a key )");
				updateDisplay();
				key = getSingleKey();
				//if ( key=="SPACE" ) { InnMenu = 1; }
				if ( key!="" ) { InnMenu = 1; }
			}
			else
			{
				cyText (0,"How many hours from now do you wish");
				cyText (1,"your wake-up call to be set for");
				cyText (2,"(Maximum of 12 hours)?");
				bText (3,4," ) 1 hour    ) 2 hours   ) 3 hours");
				bText (3,5," ) 4 hours   ) 5 hours   ) 6 hours");
				bText (3,6," ) 7 hours   ) 8 hours   ) 9 hours");
				bText (3,7," ) 10 hours  ) 11 hours  ) 12 hours");
				SetFontColour(40, 96, 244, 255);
				bText (3,4,"1           2           3");
				bText (3,5,"4           5           6");
				bText (3,6,"7           8           9");
				bText (3,7,"A           B           C");
				SetFontColour(215, 215, 215, 255);
				updateDisplay();
				key = getSingleKey();

				if ( key=="1" ) { sleepingHours = 1; InnMenu = 6; }
				if ( key=="2" ) { sleepingHours = 2; InnMenu = 6; }
				if ( key=="3" ) { sleepingHours = 3; InnMenu = 6; }
				if ( key=="4" ) { sleepingHours = 4; InnMenu = 6; }
				if ( key=="5" ) { sleepingHours = 5; InnMenu = 6; }
				if ( key=="6" ) { sleepingHours = 6; InnMenu = 6; }
				if ( key=="7" ) { sleepingHours = 7; InnMenu = 6; }
				if ( key=="8" ) { sleepingHours = 8; InnMenu = 6; }
				if ( key=="9" ) { sleepingHours = 9; InnMenu = 6; }
				if ( key=="A" ) { sleepingHours = 10; InnMenu = 6; }
				if ( key=="B" ) { sleepingHours = 11; InnMenu = 6; }
				if ( key=="C" ) { sleepingHours = 12; InnMenu = 6; }
			}
		}

		while (InnMenu == 6) // Display sleeping message and wake up message
		{

			//sleepingHours = 100;
			while (sleepingHours>0)
			{

				clearShopDisplay();
				//bText (1,1,sleepingHours);
				cyText (2,"You are sleeping in");
				str = Rooms[roomChoice].name + ".";
				cyText (3,str);
				updateDisplay();
				sf::sleep(sf::seconds(1));
				//for (int i=0 ; i<sleepingHours ; i++) // loop round for x number of hours
				//{
//				sf::sleep(0.8f);
					// check for diseases
					// modify fatigue

					// sleepProbabability for each room determines whether 6 hp per hour will be recovered
					int roomProb = 0;
					if (roomChoice == 0) roomProb = 30;
					if (roomChoice == 1) roomProb = 45;
					if (roomChoice == 2) roomProb = 60;
					if (roomChoice == 3) roomProb = 85;
					if (roomChoice == 4) roomProb = 100;
					if (roomChoice == 5) roomProb = 140;
					if (roomChoice == 6) roomProb = 170;
					if (roomChoice == 7) roomProb = 200;
					if (roomChoice == 8) roomProb = 240;
					if (roomChoice == 9) roomProb = 255;
					int actualSleepProb = randn(0,255);
					if (actualSleepProb <= roomProb )
					{
						plyr.hp = plyr.hp+6;
						if (plyr.hp > plyr.maxhp) { plyr.hp = plyr.maxhp; }
					}


					// modify temporary magic bonuses
				//}
				addHour();
				sleepingHours--;
			}


		clearShopDisplay();
			cyText (2,"Brave adventurer...");
			cyText (3,"It is time to wake up.");
			cyText (4,"I hope you rested well.");
			cyText (9,"( Press a key )");
			updateDisplay();
			//key = pressKey();
			key = getSingleKey();
			if (key!="") { deductCoins(0,0,roomCost); InnMenu = 1; }

		}


		while (InnMenu == 7) // Check time menu
		{

			clearShopDisplay();
			str = "Hour " + itos(plyr.hours) + " of day " + itos(plyr.days);
			bText (7,2, str);

			string monthDesc;
			switch(plyr.months)
			{
				case 1:
					monthDesc = "Rebirth";
					break;
				case 2:
					monthDesc = "Awakening";
					break;
				case 3:
					monthDesc = "Winds";
					break;
				case 4:
					monthDesc = "Rains";
					break;
				case 5:
					monthDesc = "Sowings";
					break;
				case 6:
					monthDesc = "First Fruits";
					break;
				case 7:
					monthDesc = "Harvest";
					break;
				case 8:
					monthDesc = "Final Reaping";
					break;
				case 9:
					monthDesc = "The Fall";
					break;
				case 10:
					monthDesc = "Darkness";
					break;
				case 11:
					monthDesc = "Cold Winds";
					break;
				case 12:
					monthDesc = "Lights";
					break;
			}
			str = "In the month of " + monthDesc;
			bText (7,4, str);
			str = "In year " + itos(plyr.years) + " since abduction";
			bText (7,6, str);
			cyText (9,"( Press a key )");
			updateDisplay();

			key = getSingleKey();
			if (key!="") { InnMenu = 1; }
		}

		while (InnMenu == 8) // apply for job
		{
			int jobNumber = innJobOpenings[InnNo].jobNumber;

			clearShopDisplay();
			if ( jobNumber == 255)
			{
				bText (7,0, "I'm sorry but there are no");
				cyText (1, "job openings at the moment.");
				cyText (9,"( Press a key )");
				updateDisplay();

				key = getSingleKey();
				//if ( key=="Y" ) { InnMenu = 9; }
				if ( key!="" ) { InnMenu = 1; }

			}
			else
			{	str = "We have an opening for a " + innJobs[jobNumber].name;
				cyText (0, str);
				str = "for " + itos(innJobOpenings[InnNo].JobHoursRequired) + " hours at " + itos(innJobOpenings[InnNo].jobHourlyIncome) + " coppers per hour.";
				cyText (1, str);
				cyText (3, "Would you like to apply?");
				cyText (5, "( es or  o)");
				SetFontColour(40, 96, 244, 255);
				cyText (5, " Y      N  ");
				SetFontColour(215, 215, 215, 255);
				updateDisplay();

				key = getSingleKey();
				if ( key=="Y" ) { InnMenu = 9; }
				if ( key=="N" ) { InnMenu = 1; }

			}




		}


		while (InnMenu == 9) // Check job stat requirements
		{
			int jobNumber = innJobOpenings[InnNo].jobNumber;
			string statRequirementName = innJobs[jobNumber].statRequirementName;
			int statRequirement = innJobs[jobNumber].statRequirementValue;
			bool jobStatMet = false;

			// Check stat requirement met
			if ((statRequirementName == "Strength") && (statRequirement<=plyr.str)) { jobStatMet = true; }
			if ((statRequirementName == "Charm") &&	(statRequirement<=plyr.chr)) { jobStatMet = true; }
			if ((statRequirementName == "Stamina") && (statRequirement<=plyr.sta)) { jobStatMet = true; }



			if (!jobStatMet)
			{
				clearShopDisplay();
				str = "You will need more " + statRequirementName;
				cyText (0, str);
				cyText (1, "to get the job.");
				cyText (9,"( Press a key )");
				updateDisplay();

				key = getSingleKey();

				if ( key!="" ) { InnMenu = 1; }

			}
			else
			{
				workingHours = innJobOpenings[InnNo].JobHoursRequired;
				hourlyRate = innJobOpenings[InnNo].jobHourlyIncome;
				jobIncome = workingHours*hourlyRate;
				InnMenu = 10;
			}
		}

		while (InnMenu == 10) // Display sleeping message and wake up message
		{


			while (workingHours>0)
			{

				clearShopDisplay();
				cyText (2,"WORKING");
				updateDisplay();
				sf::sleep(sf::seconds(1));
				for (int i=0 ; i<60 ; i++) // 60 minutes
				{

					// check for diseases
					// modify fatigue
					// modify hitpoints
					// modify temporary magic bonuses
				}
				//sf::sleep(sf::Seconds(1));
				addHour();
				workingHours--;
			}


			clearShopDisplay();
			// CHECK FOR INJURY
			cyText (2,"The job is completed.");
			str = "You have earned " + itos(jobIncome) + " coppers.";
			cyText (3,str);
			cyText (9,"( Press a key )");
			updateDisplay();
			//key = pressKey();
			key = getSingleKey();
			if (key!="") { plyr.copper+=jobIncome; innJobOpenings[InnNo].jobNumber=255; InnMenu = 1; }

		}


	}
	leaveShop();
}





int getInnNo()
{
	int Inn_no;
	for (int i=0 ; i<=6 ; i++) // Max number of Inn objects
	{
		if ((Inns[i].x == plyr.x) && (Inns[i].y == plyr.y))
		{
			Inn_no = i; // The number of the Inn you have entered
		}
	}
	if ((plyr.x == 31) && (plyr.y == 38)) { Inn_no = 3; } //  Traveller's Inn second entrance
	return Inn_no;
}
