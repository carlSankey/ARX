
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <bitset>
#include <map>


#include "constants.h"
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
#include "effects.h"
#include "spells.h"
#include "automap.h"
#include "guild.h"
#include "damon.h"


using namespace std;

Player plyr;




bool autoMapExplored[5][4096]; // 5 levels of 4096 on/off values

int shopDailyWares[15][12]; //15 shops with 12 items each a day for sale
int smithyDailyWares[4][10]; // 4 smithies with 10 items each a day for sale
int tavernDailyFoods[14][6]; // 14 taverns with 6 food items each day for sale
int tavernDailyDrinks[14][6]; // 14 taverns with 6 drink items each day for sale

extern std::map<int, int(*)> effectsMap;

extern std::map<int, int(*)> weaponBonusMap;
// Define a mapping between bit positions and player stats
std::map<int, int*> statMap = {
    {0, &plyr.sta},
    {1, &plyr.chr},
    {2, &plyr.str},
    {3, &plyr.inte},
    {4, &plyr.wis},
    {5, &plyr.skl},
    {6, &plyr.speed},
    {7, &plyr.hp}
};


std::map<int, int*> resistanceMap = {
    {0, &plyr.invulnerability[0]},  // Blunt
    {1, &plyr.invulnerability[1]},  // Sharp
    {2, &plyr.invulnerability[2]},  // Earth
    {3, &plyr.invulnerability[3]},  // Air
    {4, &plyr.invulnerability[4]},  // Fire
    {5, &plyr.invulnerability[5]},  // Water
    {6, &plyr.invulnerability[6]},  // Power
    {7, &plyr.invulnerability[7]},  // Magic
    {8, &plyr.invulnerability[8]},  // Good
    {9, &plyr.invulnerability[9]},  // Evit
    {10, &plyr.invulnerability[10]}, // Cold
    {11, &plyr.invulnerability[11]}, // Nature
    {12, &plyr.invulnerability[12]}  // Acid
};



//void updateWeaponBuff(std::bitset<13> binaryWeaponBuff, int positiveValue);


extern struct bonusDamage;

extern int weaponBonus;



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
	plyr.sta	+= statBonuses[0]; // 1
	plyr.chr	+= statBonuses[1]; // 2
	plyr.str	+= statBonuses[2]; // 4
	plyr.inte	+= statBonuses[3]; // 8
	plyr.wis	+= statBonuses[4]; // 16
	plyr.skl	+= statBonuses[5]; // 32
	plyr.speed	+= statBonuses[6]; // 64
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
	

	//update diseases every 15mins.  This will not increase incubation period.  Only once it is active.	
    if (plyr.minutes == 0 || plyr.minutes == 15 || plyr.minutes == 30 || plyr.minutes == 45)
    {
        updateDisease(0);
        updateNoticability();
        updateFatigue();
    }
}


void addHour()
{
	if (plyr.hours == 23)
	{
		plyr.hours = 0; // 12 midnight important for Ferry crossing
        updateHunger();
        updateThirst();
    
		if (plyr.alcohol > 0) { plyr.alcohol--; }
		addDay();
	}
	else
	{
		plyr.hours++;
		// perform hourly actions / checks
        updateHunger();
        updateThirst();
		if (plyr.alcohol > 0) { plyr.alcohol--; }
	}
	checkActiveMagic();
	updateDisease(1);
	updatePoison();
	checkBackgroundTime();
    updateWeapon();
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
    checkGuildApplications();
    UpdateShopStock();



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
    if ((plyr.hunger > -100) && (plyr.hunger < -49)) { hungerDesc = "Bloated!"; }
    if ((plyr.hunger > -48) && (plyr.hunger < -17)) { hungerDesc = "Stuffed"; }
    if ((plyr.hunger > -16) && (plyr.hunger < -6)) { hungerDesc = "Full"; }
    if ((plyr.hunger > -5) && (plyr.hunger < 15)) { hungerDesc = "       "; }
	if ((plyr.hunger > 16) && (plyr.hunger < 49)) { hungerDesc = "Hungry"; }
	if ((plyr.hunger > 48) && (plyr.hunger < 97)) { hungerDesc = "Famished"; }
	if (plyr.hunger > 96 ) { hungerDesc = "Starving"; }

	return hungerDesc;
}


string checkFatigue()
{
    string fatigueDesc;
    fatigueDesc = ""; // alcohol level is 0
    if ((plyr.fatigue > 16) && (plyr.fatigue < 49)) { fatigueDesc = "Weary"; }
    if ((plyr.fatigue > 48) && (plyr.fatigue < 95)) { fatigueDesc = "Tired"; }
    if (plyr.fatigue > 95) { fatigueDesc = "Exhausted!"; }
    return fatigueDesc;
}


string checkTemprature()
{
    string tempDesc;
    tempDesc = ""; // 
    if ((plyr.temperature >= -255) && (plyr.temperature < -156)) { tempDesc = "Freezing!"; }
    if ((plyr.temperature >= -155) && (plyr.temperature < -51)) { tempDesc = "Cold"; }
    if ((plyr.temperature > -50) && (plyr.temperature < 0)) { tempDesc = "Chilly"; }
    if ((plyr.temperature > 0 ) && (plyr.temperature < 25)) { tempDesc = "    "; }
    if ((plyr.temperature > 25) && (plyr.temperature < 50)) { tempDesc = "Warm";  }
    if ((plyr.temperature > 51) && (plyr.temperature < 154)) { tempDesc = "Hot"; plyr.thirstRate += 0.08; }
    if (plyr.temperature > 155) { tempDesc = "Roasting!"; plyr.thirstRate += 0.16; }
    return tempDesc;
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
    if ((plyr.diseases[0]>0) || (plyr.diseases[1]>0) || (plyr.diseases[1]>0) || (plyr.diseases[1]>0))  { diseaseDesc = "Diseased!"; }
    return diseaseDesc;
}


void updateNoticability()
{
    if (plyr.noticeability > 0.024) { plyr.noticeability -= 0.025; }  
}


void updateDigestion()
{

    plyr.digestion -= 3;
    if (plyr.digestion < 0) { plyr.digestion = 0; }

}


void updateThirst()
{
    plyr.thirst += plyr.thirstRate;
}


void updateHunger()
{
    plyr.hunger += plyr.hungerRate;

}


void updateFatigue()
{
    std::cout << "UpdateFatigue\n";
    plyr.fatigue += plyr.fatigueRate;
   
    if (plyr.fatigue > 96 && plyr.fatigueRate>0)
    {
     // create a random number
        int tempFatigue = randn(0, 100);
     
         if (tempFatigue < 50)
         {
             castSpellAction(randn(101, 107));  //Add a negative buff to the effectbuff
         }
    }
    else if (plyr.fatigueRate > 0)
    {
        int initialIndex = 10;
        
        int bufferSize = sizeof(effectBuffer) / sizeof(effectBuffer[0]);
        SearchArray result = findAllEffectByNameOrLastFree(initialIndex, bufferSize );

        // Determine the number of elements in the dynamic array
       

        // Use std::sort to sort the dynamic array
        SearchResult* arrayResult = result.resultArray;
        int arrSize = result.arrSize;

        for (int i = 0; i < arrSize; ++i) {
            applyEffect(0, result.resultArray->index);
        }
    }
    
    if (plyr.fatigue > 255) { plyr.fatigue = 255; }
  
}


void updateDisease(int hour)
{
	std::cout << "UpdateDisease\n";
 	  //for now treat all of the diseases the same.  Change in the future
 	  for(int i = 0;i<4;i++)
	  {
	    int incurbation = plyr.diseases[i];
	    if (incurbation > 0)
	    {
	        // 0 - no rabies, 1-14 in incubation, 15 - active and identified
	        if (incurbation<15 && i == 0 && hour == 1) { plyr.diseases[i]++; }
	        if (incurbation<48 && i == 1 && hour == 1) { plyr.diseases[i]++; }
	        if (incurbation<48 && i == 2 && hour == 1) { plyr.diseases[i]++; }
	        else if (hour == 0)
	        {
	        	// hp effect is every 15mins once incubation period is over.
        	  		std::string disease = "Rabies";
        	  		switch(i)
        	  		{
        	  			case 1:
        	  				disease = "Mold";
        	  				break;
  	  				   case 2:
  	  				   	disease = "Fungus";
  	  				   	break;
				   }
					plyr.status_text = disease + " has taken hold of you.";        	
	            plyr.hp -= 2; // temporary penalty.
	            if (i == 2)		//Fungus - infexted by SLIME
					{
						if (plyr.food > 0)
						{
						 	plyr.food = 0;
						 	plyr.status_text = ("Fungus has fouled all your food.");
						}
					}	
	        }
	    }
	  }


}


void updateWeaponBuff(std::bitset<13> binaryWeaponBuff, int positiveValue) {
    for (int i = 0; i < 13; i++) {
        if (binaryWeaponBuff.test(i)) {
            *(weaponBonusMap[i]) += positiveValue;
        }
    }
}


void updateSpell(int hour )
{
    std::cout << "UpdateSpell\n";
    //for now treat all of the spells the same.  Change in the future
   

    for (int i = 0; i < 4; i++)
    {
        int SpellDuraction = effectBuffer[i].duration;
        
        std::bitset<8> binaryStat(spells[effectBuffer[i].effectNo].stattype);
        std::bitset<13> binaryElems(spells[effectBuffer[i].effectNo].elementtype);
            if (spells[effectBuffer[i].effectNo].spelltype == 0)  //Stat adjustment
            {
                if (SpellDuraction > 0)
                {
                    
                    updateStats(binaryStat, effectBuffer[i].positiveValue);
                }
                else if (hour == 0)
                {
                    //remove the effects
                    updateStats(binaryStat, -effectBuffer[i].positiveValue);
                    effectBuffer[plyr.effectIndex].effectNo = 0; // Dexterity
                    effectBuffer[plyr.effectIndex].negativeValue = 0;
                    effectBuffer[plyr.effectIndex].positiveValue = 0; // +30 to plyr.dex
                    effectBuffer[plyr.effectIndex].duration = 0; // hours
                }

            }
            else if (spells[effectBuffer[i].effectNo].spelltype == 1)  //Buff
            {
                if (SpellDuraction > 0)
                {
                   
                    updateInvuls(binaryElems, effectBuffer[i].positiveValue);
                }
                else if (hour == 0)
                {
                //remove the effects
                    updateInvuls(binaryElems, -effectBuffer[i].positiveValue);
                    effectBuffer[plyr.effectIndex].effectNo = 0; // 
                    effectBuffer[plyr.effectIndex].negativeValue = 0;
                    effectBuffer[plyr.effectIndex].positiveValue = 0; // 
                    effectBuffer[plyr.effectIndex].duration = 0; // hours
                }
            }
            effectBuffer[i].duration -= hour;
    }
}


/**
* 
* 
* Spelltype 0 = stat increase
* Spelltype 1 = Buff
* Spelltype 2 = Combat spell  - these should use castspell
* Spelltype 3 = hidden stat increase
* Spelltype 4 = Weapon enchant
* Spelltype 5 = Misc
*/
void updateSpell(int hour, int effectid)
{
    /*
    std::cout << "UpdateSpell\n";
    //for now treat all of the spells the same.  Change in the future
    int SpellDuraction = effectBuffer[effectid].duration;

    std::bitset<7> binaryStat(spells[effectBuffer[effectid].effectNo].stattype);
    std::bitset<13> binaryElems(spells[effectBuffer[effectid].effectNo].elementtype);
    if (spells[effectBuffer[effectid].effectNo].spelltype == 0)  //Stat adjustment
    {
        if (SpellDuraction > 0)
        {

            updateStats(binaryStat, effectBuffer[effectid].positiveValue);
        }
        else if (hour == 0)
        {
            //remove the effects
            updateStats(binaryStat, -effectBuffer[effectid].positiveValue);
            effectBuffer[plyr.effectIndex].effect = 0; // Dexterity
            effectBuffer[plyr.effectIndex].negativeValue = 0;
            effectBuffer[plyr.effectIndex].positiveValue = 0; // +30 to plyr.dex
            effectBuffer[plyr.effectIndex].duration = 0; // hours
        }

    }
    else if (spells[effectBuffer[effectid].effectNo].spelltype == 1)  //Buff
    {
        if (SpellDuraction > 0)
        {

            updateInvuls(binaryElems, effectBuffer[effectid].positiveValue);
        }
        else if (hour == 0)
        {
            //remove the effects
            updateInvuls(binaryElems, -effectBuffer[effectid].positiveValue);
            effectBuffer[plyr.effectIndex].effect = 0; // Dexterity
            effectBuffer[plyr.effectIndex].negativeValue = 0;
            effectBuffer[plyr.effectIndex].positiveValue = 0; // +30 to plyr.dex
            effectBuffer[plyr.effectIndex].duration = 0; // hours
        }
    }
    else if (spells[effectBuffer[effectid].effectNo].spelltype == 3)
    {
        if (SpellDuraction > 0)
        {
            if (spells[effectBuffer[effectid].effect].effect == 3)  //for any boolean spells like Light where its on or off
            {
                *effectsMap[spells[effectBuffer[effectid].effect].effect] = spells[effectBuffer[effectid].effect].positiveValue;

                
            }
            else
            {
                *effectsMap[spells[effectBuffer[effectid].effect].effect] = spells[effectBuffer[effectid].effect].positiveValue;
            }
            checkplayerLight();
           
        }
        else if (hour == 0)
        {
            //remove the effects
            if (spells[effectBuffer[effectid].effect].effect == 3)
            {
                *effectsMap[effectBuffer[effectIndexloc.index].effect] = -spells[effectBuffer[effectid].effect].positiveValue;
            }

            *effectsMap[effectBuffer[effectIndexloc.index].effect] = -spells[effectBuffer[effectid].effect].positiveValue;
            
            effectBuffer[plyr.effectIndex].effect = 0; // Dexterity
            effectBuffer[plyr.effectIndex].negativeValue = 0;
            effectBuffer[plyr.effectIndex].positiveValue = 0; // +30 to plyr.dex
            effectBuffer[plyr.effectIndex].duration = 0; // hours
        }
    }
    else if (spells[effectBuffer[effectid].effectNo].spelltype == 4) //Not implemented yet
    {
        if (SpellDuraction > 0)
        {
            if (spells[effectBuffer[effectid].effect].effect == 3)  
            {
                *effectsMap[spells[effectBuffer[effectid].effect].effect] = spells[effectBuffer[effectid].effect].positiveValue;
            }
            else
            {
                *effectsMap[spells[effectBuffer[effectid].effect].effect] = spells[effectBuffer[effectid].effect].positiveValue;
            }
        }
        else if (hour == 0)
        {
            //remove the effects
            if (spells[effectBuffer[effectid].effect].effect == 3)
            {
                *effectsMap[effectBuffer[effectIndexloc.index].effect] = -spells[effectBuffer[effectid].effect].positiveValue;
            }

            *effectsMap[effectBuffer[effectIndexloc.index].effect] = -spells[effectBuffer[effectid].effect].positiveValue;

            effectBuffer[plyr.effectIndex].effect = 0; // Dexterity
            effectBuffer[plyr.effectIndex].negativeValue = 0;
            effectBuffer[plyr.effectIndex].positiveValue = 0; // +30 to plyr.dex
            effectBuffer[plyr.effectIndex].duration = 0; // hours
        }
    }
    */
}


void updatePoison()
{
    if (plyr.poison[0] > 0) { plyr.hp -= 2; }
    if (plyr.poison[1] > 0) { plyr.hp -= 5; }
    if (plyr.poison[2] > 0) { plyr.hp -= 7; }
    if (plyr.poison[3] > 0) { plyr.hp -= 10; }
}


void updateWeapon()
{
    
        if(itemBuffer[plyr.priWeapon].alignment >128 and plyr.alignment < 128)
        {
            int evilWeaponDamage = randn(0, 100);
            if (randn(0, 100) < 5)
            {
                string str = "You are to good for this ";
                itemMessage(str);
                plyr.hp -= evilWeaponDamage;
            }    
    }
 
}


void checkActiveMagic()
{
    std::cout << "checkActiveMagic\n";

    for (int i = 0; i < sizeof(effectBuffer) / sizeof(effectBuffer[0]); ++i)
    {
        if (effectBuffer[i].duration > 0) { effectBuffer[i].duration = effectBuffer[i].duration - 1; }
        if (effectBuffer[i].effectNo != 0 && effectBuffer[i].duration == 0) 
        { 
            applyEffect(0, effectBuffer[i].effectNo); 
            SearchResult ExpiredSpell = findActiveSpellNoLastFree(effectBuffer[i].effectNo, sizeof(plyr.ActiveSpell));
            plyr.ActiveSpell[ExpiredSpell.index] = 0;
            effectBuffer[i].name = "";
            effectBuffer[i].effectNo = 0;
            effectBuffer[i].positiveValue = 0;
            effectBuffer[i].negativeValue = 0;
        }
    }
    
    
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
    plyr.facing = NORTH; // 2 equals north?
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

    plyr.clothing[0] = createClothing(0,10);              // Put "Cheap Robe" into itemBuffer[1]
    plyr.clothing[1] = 255;         // Slot 2 empty
    plyr.clothing[2] = 255;         // Slot 3 empty
    plyr.clothing[3] = 255;         // Slot 4 empty


    plyr.retreatFriendship = 5;
    plyr.damonFriendship = 2;
    plyr.smithyFriendships[0] = 2;
    plyr.smithyFriendships[1] = 2;
    plyr.smithyFriendships[2] = 2;
    plyr.smithyFriendships[3] = 2;

    for (int i=0 ; i<=noOfSpellBuffer ; i++) { spellBuffer[i].no = 255; } // Set buffer to 255 to stop confusion about learning spell no. 0
    for (int i = 0; i <= noOfEffects; i++) { effectBuffer[i].effectNo = 0; } // Set buffer to 255 to stop confusion about learning spell no. 0
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
    for (int i=0 ; i< noOfElements; i++) { plyr.invulnerability[i] = 0; }
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
    plyr.darkness = 1;   //Is it dark outside?
    plyr.light = 0;     //Do i have light source?

    plyr.hungerRate = 0.5;
    plyr.thirstRate = 0.5;
    plyr.fatigueRate = 0.5;
    plyr.supervision = 0;
    plyr.temperature = 13;
    plyr.fatigue = 0;

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


void checkplayerLight()
{
    plyr.light = 0;
    //Reasons for player light to be true
    //Torch (could change this to a spell effect
     if (itemBuffer[plyr.priWeapon].name == "Lit Torch" || itemBuffer[plyr.secWeapon].name == "Lit Torch")
    {
        plyr.light = 1;
       
        return;
    }

    for  (int i = 0; i < sizeof(effectBuffer) / sizeof(effectBuffer[0]); i++)
    {
        if (spells[effectBuffer[i].effectNo].spelltype == 3 && spells[effectBuffer[i].effectNo].stattype == 8)
        {
            plyr.light = 1;
            
            break;
        }

    }
   
}


std::string setGenderString(int gender) {
    if (gender == 1) {
        return "man";
    }
    else if (gender == 2) {
        return "woman";
    }
    else {
        return "unknown";
    }
}