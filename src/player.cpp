
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "globals.h"
#include "dev.h"

#include "createCharacter.h"
#include "player.h"
#include "misc.h"
#include "display.h"
#include "font.h"
#include "smithy.h"
#include "bank.h"
#include "tavern.h"
#include "shop.h"
#include "items.h"
#include "spells.h"
#include "automap.h"


using namespace std;


Player plyr;


effectItem effectBuffer[50]; // active time limited effects from spells, scrolls, eyes

bool autoMapExplored[5][4096]; // 5 levels of 4096 on/off values

int shopDailyWares[15][12]; //15 shops with 12 items each a day for sale
int smithyDailyWares[4][10]; // 4 smithies with 10 items each a day for sale
int tavernDailyFoods[14][6]; // 14 taverns with 6 food items each day for sale
int tavernDailyDrinks[14][6]; // 14 taverns with 6 drink items each day for sale


void increaseExperience(int xpIncrease)
{
	double levelBottom, levelTop, basePower;
	basePower = 2;
	if (plyr.level==0) {levelBottom = 200; } else { levelBottom = 200 * pow(basePower,plyr.level); }
	levelTop = levelBottom*2;
	levelBottom--;
	plyr.xp+=xpIncrease;
	if ((plyr.xp > levelBottom) && (plyr.xp < levelTop)) increaseLevel();
}



void increaseLevel()
{
	int statBonuses[7];
	int statBonus;
	plyr.level++;
	for(int x = 0; x < 7; ++x) { statBonuses[x] = 0; }

	// Increase hit points
	int hpIncrease= randn(0,plyr.sta);
	plyr.hp+=hpIncrease;
	plyr.maxhp+=hpIncrease;

	// Increase stats
	for(int x = 0; x < 7; ++x)
	{
		statBonus = randn(1,100); // roll to see if this stat will receive a bonus
		if (statBonus > 30) { statBonuses[x] = randn(0,3); } // Award a stat bonus
	}

	// Add the bonuses (which might be 0)
	plyr.sta	+= statBonuses[0];
	plyr.chr	+= statBonuses[1];
	plyr.str	+= statBonuses[2];
	plyr.inte	+= statBonuses[3];
	plyr.wis	+= statBonuses[4];
	plyr.skl	+= statBonuses[5];
	plyr.speed	+= statBonuses[6];
}



void checkBackgroundTime()
{
	plyr.timeOfDay = 0;

	if ((plyr.hours > 18) || (plyr.hours < 4)) { plyr.timeOfDay = 1;} // 6pm to 3am dark hours
	if ((plyr.hours == 4) && (plyr.minutes>=0) && (plyr.minutes<30)) { plyr.timeOfDay = 1;} // 4am - 4:29 dark hours

	if ((plyr.hours==4) && (plyr.minutes>29) && (plyr.minutes<=59)) { plyr.timeOfDay = 2; }
	if ((plyr.hours==5) && (plyr.minutes<=29)) { plyr.timeOfDay = 3; }
	if ((plyr.hours==5) && (plyr.minutes>29) && (plyr.minutes<=59)) { plyr.timeOfDay = 4; }
	if ((plyr.hours==6) && (plyr.minutes<=29)) { plyr.timeOfDay = 5; }
	if ((plyr.hours==6) && (plyr.minutes>29) && (plyr.minutes<=59)) { plyr.timeOfDay = 6; }
	if ((plyr.hours==7) && (plyr.minutes<=29)) { plyr.timeOfDay = 7; }

	if ((plyr.hours==16) && (plyr.minutes<=29)) { plyr.timeOfDay = 7; }
	if ((plyr.hours==16) && (plyr.minutes>29) && (plyr.minutes<=59)) { plyr.timeOfDay = 6; }
	if ((plyr.hours==17) && (plyr.minutes<=29)) { plyr.timeOfDay = 5; }
	if ((plyr.hours==17) && (plyr.minutes>29) && (plyr.minutes<=59)) { plyr.timeOfDay = 4; }
	if ((plyr.hours==18) && (plyr.minutes<=29)) { plyr.timeOfDay = 3; }
	if ((plyr.hours==18) && (plyr.minutes>29) && (plyr.minutes<=59)) { plyr.timeOfDay = 2; }

	//if ((plyr.hours == 17) || (plyr.hours == 5)) { plyr.timeOfDay = 2;}
}


void addMinute()
{
	if (plyr.minutes==59)
	{
		plyr.minutes=0;
		addHour();
	}
	else
	{
		plyr.minutes++;
	}
	// perform minute actions - disease, hp point loss, check for weather change etc
	checkBackgroundTime();
}

void addHour()
{
	if (plyr.hours == 23)
	{
		plyr.hours = 0; // 12 midnight important for Ferry crossing
		plyr.hunger += 2;
		plyr.thirst += 2;
		if (plyr.alcohol > 0) { plyr.alcohol--; }
		addDay();
	}
	else
	{
		plyr.hours++;
		// perform hourly actions / checks
		plyr.hunger += 2;
		plyr.thirst += 2;
		if (plyr.alcohol > 0) { plyr.alcohol--; }
	}
	checkActiveMagic();
	updateDisease();
	updatePoison();
	checkBackgroundTime();

}


void addDay()
{
	if (plyr.days == 31)
	{
		plyr.days = 1; // first day of a new month
		addMonth();
	}
	else
	{
		plyr.days++;
	}

    // perform daily actions / checks / shop changes
    stockSmithyWares();
    stockShopWares();
    stockTavernDrinks();
    stockTavernFoods();
    checkDailybankInterest();
    if (plyr.forgeDays > 0) plyr.forgeDays--;
}


void addMonth()
{
	if (plyr.months == 12)
	{
		plyr.months = 1; // first month of a new year
		addYear();
	}
	else
	{
		plyr.months++;
		// perform monthly actions - are there any? Weather changes?
	}
}


void addYear()
{
	plyr.years++;
	// perform yearly actions - bank interest?
}



string checkThirst()
{
	string thirstDesc;
	thirstDesc = ""; // alcohol level is 0
	if ((plyr.thirst > 16) && (plyr.thirst < 33)) { thirstDesc = "Thirsty"; }
	if ((plyr.thirst > 32) && (plyr.thirst < 57)) { thirstDesc = "Very Thirsty"; }
	if (plyr.thirst > 56 ) { thirstDesc = "Parched"; }
	return thirstDesc;
}


string checkHunger()
{
	string hungerDesc;
	hungerDesc = ""; // alcohol level is 0
	if ((plyr.hunger > 16) && (plyr.hunger < 49)) { hungerDesc = "Hungry"; }
	if ((plyr.hunger > 48) && (plyr.hunger < 97)) { hungerDesc = "Famished"; }
	if (plyr.hunger > 96 ) { hungerDesc = "Starving"; }
	return hungerDesc;
}

string checkAlcohol()
{
	string alcoholDesc;
	alcoholDesc = ""; // alcohol level is 0
	if ((plyr.alcohol > 0) && (plyr.alcohol < plyr.sta)) { alcoholDesc = "Tipsy"; }
	if ((plyr.alcohol >= plyr.sta) && (plyr.alcohol < (plyr.sta*2))) { alcoholDesc = "Drunk"; }
	if (plyr.alcohol >= (plyr.sta*2)) { alcoholDesc = "Very Drunk"; }
	return alcoholDesc;
}


string checkEncumbrance()
{
	string weightDesc ="";
	int weight = returnCarriedWeight();
	int encumbrance = weight - (plyr.str + 224);
	if ((encumbrance >= 0) && (encumbrance < 16)) { weightDesc = "Burdened"; }
	if ((encumbrance >= 16) && (encumbrance < 33)) { weightDesc = "Encumbered"; }
	if (encumbrance >= 33) { weightDesc = "Immobilized!"; }
	return weightDesc;
}

string checkPoison()
{
    string poisonDesc ="";
    if ((plyr.poison[0]>0) || (plyr.poison[0]>0) || (plyr.poison[0]>0) || (plyr.poison[0]>0))  { poisonDesc = "Poisoned!"; }
    return poisonDesc;
}

string checkDisease()
{
    string diseaseDesc ="";
    if ((plyr.diseases[0]>0) || (plyr.diseases[0]>0) || (plyr.diseases[0]>0) || (plyr.diseases[0]>0))  { diseaseDesc = "Diseased!"; }
    return diseaseDesc;
}

void updateDisease()
{
    int rabiesStatus = plyr.diseases[0];
    if (rabiesStatus > 0)
    {
        // 0 - no rabies, 1-14 in incubation, 15 - active and identified
        if (rabiesStatus<15) { plyr.diseases[0]++; }
        else
        {
            plyr.hp -= 5; // temporary penalty.
        }
    }
}

void updatePoison()
{
    if (plyr.poison[0] > 0) { plyr.hp -= 2; }
    if (plyr.poison[1] > 0) { plyr.hp -= 5; }
    if (plyr.poison[2] > 0) { plyr.hp -= 7; }
    if (plyr.poison[3] > 0) { plyr.hp -= 10; }
}

void checkActiveMagic()
{
        if (plyr.protection1 > 0) { plyr.protection1--; }
        if (plyr.protection2 > 0) { plyr.protection2--; }
        if (plyr.invulnerability[0] > 0) { plyr.invulnerability[0]--; }
        if (plyr.invulnerability[1] > 0) { plyr.invulnerability[1]--; }
        if (plyr.invulnerability[2] > 0) { plyr.invulnerability[2]--; }
        if (plyr.invulnerability[3] > 0) { plyr.invulnerability[3]--; }
        if (plyr.invulnerability[4] > 0) { plyr.invulnerability[4]--; }
        if (plyr.invulnerability[5] > 0) { plyr.invulnerability[5]--; }
        if (plyr.invulnerability[6] > 0) { plyr.invulnerability[6]--; }
        if (plyr.invulnerability[7] > 0) { plyr.invulnerability[7]--; }
        if (plyr.invulnerability[8] > 0) { plyr.invulnerability[8]--; }
}

void initStats()
{
    for (int i=0 ; i<14 ; i++) plyr.guildMemberships[i] = 0;
    for (int i=0 ; i<=11 ; i++) plyr.guildAwards[i] = false;
    for (int i=0 ; i<32 ; i++) plyr.fixedEncounters[i] = false;
    for (int i=0 ; i<9 ; i++) plyr.bankAccountStatuses[i] = 0;
    for (int i=0 ; i<9 ; i++) plyr.bankAccountBalances[i] = 0;
    for (int i=0 ; i<14 ; i++) plyr.tavernFriendships[i] = 2;
    for (int i=0 ; i<15 ; i++) plyr.shopFriendships[i] = 2;

    plyr.healerDays[0] = 0;
    plyr.healerHours[0] = 0;
    plyr.healerMinutes[0] = 0;
    plyr.healerDays[1] = 0;
    plyr.healerHours[1] = 0;
    plyr.healerMinutes[1] = 0;
    plyr.teleporting = 0; // counter that is used for flashing teleport sequence
    plyr.z_offset = 1.0f;
    plyr.scenario = 255; // city 0, dungeon 1 - 255 means main menu for font choice
    plyr.map = 0; // city
    plyr.x = 63; // 49
    plyr.y = 63; // 3
    plyr.facing = 2; // 2 equals north?
    plyr.sta = 16;
    plyr.chr = 13;
    plyr.str = 17;
    plyr.inte = 14;
    plyr.wis = 15;
    plyr.skl = 18;
    plyr.hp = 15;
    plyr.maxhp = 15;
    plyr.name = "Xebec";
    plyr.xp = 0;
    plyr.level = 0;
    plyr.alive = true;
    plyr.encounter_done = true;
    plyr.gender = 1;
    plyr.zone = 1;
    plyr.status = 1; // Exploring
    plyr.torches = 3;
    plyr.food = 3;
    plyr.water = 3;
    plyr.timepieces = 0;
    plyr.compasses = 0;
    plyr.gems = 0;
    plyr.jewels = 0;
    plyr.crystals = 0;
    plyr.copper = 0;
    plyr.silver = 0;
    plyr.gold = 0;
    plyr.keys = 0;

    plyr.infoPanel = 1;
    plyr.stealth = 4; //was 4
    plyr.speed = 10;
    plyr.diagOn = false;
    plyr.fpsOn = false;
    plyr.miniMapOn = false;

    plyr.chrPartials = 0;
    plyr.intPartials = 0;
    plyr.strPartials = 0;
    plyr.buffer_index = 0;
    plyr.zone = 0;
    plyr.zoneSet = 0;
    plyr.specialwall = 0;

    plyr.minutes = 0;
    plyr.hours = 12; // Atari 8bit time
    plyr.days = 30;
    plyr.months = 4;
    plyr.years = 0;
    plyr.timeOfDay=0;


    // Quest flags
    plyr.goblinsVisited = false;
    plyr.goblinsChallenged = false;
    plyr.trollsVisited = false;
    plyr.trollsChallenged = false;
    plyr.trollsDefeated = false;
    plyr.trollsCombat = false;
    plyr.goblinsDefeated = false;
    plyr.goblinsCombat = false;
    plyr.goblinsReforged = false;
    plyr.trollsReforged = false;

    plyr.thirst = 0;
    plyr.hunger = 0;
    plyr.digestion = 0;
    plyr.alcohol = 0;

    plyr.alignment = 128; // was 128

    plyr.spellIndex = 0;

    // Create starting items and set weapon, armour and clothing slots to initial values
    // Do not set these values manually - must be set by USE command in game! They hold array references within inventory

    plyr.buffer_index = 0;

    createBareHands();              // Put "bare hand" into itemBuffer[0]
    plyr.priWeapon = 0;             // bare hand should always be itemBuffer[0]
    plyr.secWeapon = 0;             // bare hand should always be itemBuffer[0]
    plyr.headArmour = 255;          // none
    plyr.bodyArmour = 255;          // none
    plyr.legsArmour = 255;          // none
    plyr.armsArmour = 255;          // none

    plyr.clothing[0] = createClothing(0);              // Put "Cheap Robe" into itemBuffer[1]
    plyr.clothing[1] = 255;         // Slot 2 empty
    plyr.clothing[2] = 255;         // Slot 3 empty
    plyr.clothing[3] = 255;         // Slot 4 empty


    plyr.retreatFriendship = 5;
    plyr.damonFriendship = 2;
    plyr.smithyFriendships[0] = 2;
    plyr.smithyFriendships[1] = 2;
    plyr.smithyFriendships[2] = 2;
    plyr.smithyFriendships[3] = 2;

    for (int i=0 ; i<=35 ; i++) { spellBuffer[i].no = 255; } // Set buffer to 255 to stop confusion about learning spell no. 0
    for (int i=0 ; i<20 ; i++) { plyr.doorDetails[i].direction=0; plyr.doorDetails[i].x=0; plyr.doorDetails[i].y=0; plyr.doorDetails[i].level=0; } // Initialise door buffer
    plyr.doorDetailIndex=0;

    plyr.oracleReturnTomorrow = false;
    plyr.oracleDay = 255; // unset
    plyr.oracleMonth = 255;
    plyr.oracleYear = 255;
    plyr.oracleQuestNo = 0; // First quest from Oracle is silver key & palace prison

    plyr.treasureFinding = 15;
    plyr.invisibility = 0;
    plyr.diseases[0] = 0;
    plyr.diseases[1] = 0;
    plyr.diseases[2] = 0;
    plyr.diseases[3] = 0;
    plyr.poison[0] = 0;
    plyr.poison[1] = 0;
    plyr.poison[2] = 0;
    plyr.poison[3] = 0;
    plyr.delusion = 0;
    for (int i=0 ; i<9 ; i++) { plyr.invulnerability[i] = 0; }
    plyr.noticeability = 0;
    plyr.protection1 = 0;
    plyr.protection2 = 0;

    plyr.fixedEncounter = false;

    plyr.undeadKingVisited = false;
    plyr.special = 0;
    plyr.mapWidth=64;
    plyr.mapHeight=64;

    plyr.forgeDays = 0;
    plyr.forgeType = 0;
    plyr.forgeBonus = 0;
    plyr.forgeName = "";

    plyr.stolenFromVault = 0;

}

void createNewCharacter(int scenario)
{
	initStats();

	switch(scenario)
    {
        case CITY:
            plyr.x = 35;
            plyr.y = 36;
            plyr.scenario = CITY;
            plyr.map=0;
            plyr.facing = NORTH;
            break;
        case DUNGEON:
            plyr.scenario = DUNGEON;
            plyr.map = 1;
            plyr.x = 49;
            plyr.y = 3;
            plyr.facing = WEST;
// TESTING DATA
            //plyr.x = 60;
            //plyr.y = 4;
            //plyr.silver = 972;
            //plyr.maxhp = 225;
            //plyr.hp = 225;
            //plyr.inte = 18;
            //plyrC:\Files\AR_Env\AR 0.82 18April2017\player.cpp.level = 4;
            //plyr.keys= 10;
// END OF TESTING DATA
            break;
    }


    stockSmithyWares();
    stockTavernDrinks();
    stockTavernFoods();
    stockShopWares();



    if (AR_DEV.CHARACTER_CREATION==ON)
    {
        getPlayerName();
        switch(scenario)
        {
            case CITY:
                cityGate();
                break;
            case DUNGEON:
                dungeonGate();
                break;
        }

    }
}
