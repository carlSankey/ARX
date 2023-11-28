
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <utility> // For std::pair
#include <map>

#include "spells.h"
#include "effects.h"
#include "items.h"
#include "player.h"
#include "display.h"
#include "encounter.h"
#include "font.h"
#include "misc.h"
#include "audio.h"
#include "effects.h"

using namespace std;

unsigned char  spellItems[SpellList];
extern struct monster;
extern  monster Opponents[];

spellItem spellBuffer[76]; // learnt spells that can be cast

effectItem effectBuffer[1000]; // Actual definition with memory allocation

//extern std::map<int, int*> weaponBonusMap;

spellRecord spells[150] =
{
{"Bewilder",0,2,10,10,0,0,0,0,0,0,6,1,0.16,255,99,16383},  //0
{"Blinding",1,2,10,10,0,0,0,0,0,0,3,1,0.15,255,99,16383},  //1
{"Charisma",2,0,10,10,0,0,2,0,25,8,12,1,0.15,255,99,16383},  //2
{"Cold Blast",3,2,10,10,0,1024,0,35,0,0,2,1,0.17,255,99,16383},  //3
{"Conjure Food",4,3,10,10,0,0,0,0,1,0,2,1,0.14,255,99,512},  //4
{"Conjure Key",5,3,10,10,2,0,0,0,1,0,9,1,0.06,255,99,256},  //5
{"Defeat Evil",6,2,10,10,0,512,0,35,0,0,2,1,0.12,255,99,2816},  //6
{"Defeat Good",7,2,10,10,0,256,0,35,0,0,4,1,0.09,255,99,162},  //7
{"Dexterity",8,0,10,10,0,0,32,0,25,8,7,1,0.07,255,99,265},  //8
{"Fear",9,2,10,10,0,0,0,0,0,0,3,1,0.12,255,99,512},  //9
{"Fireballs",10,2,10,10,0,16,0,60,0,0,4,1,0.11,255,99,16383},  //10
{"Fireblade",11,4,10,10,0,16,0,0,30,1,12,1,0.16,255,99,16383},  //11
{"Fury",12,2,10,10,0,0,0,0,0,0,2,1,0.06,255,99,16383},  //12
{"Healing",13,5,10,10,0,0,0,0,30,0,1,1,0.14,255,99,768},  //13
{"Light",14,3,10,10,3,0,0,0,1,0,10,1,0.14,255,99,768},  //14
{"Lightning Bolts",15,2,10,10,8,64,0,40,0,0,6,1,0.17,255,99,16383},  //15
{"Location",16,5,10,10,0,0,0,0,0,0,4,1,0.13,255,99,768},  //16
{"Luck",17,3,10,10,6,0,0,0,60,8,5,1,0.16,255,99,16383},  //17
{"Magic Darts",18,2,10,10,9,130,0,25,0,0,10,1,0.07,255,99,16383},  //18
{"Night Vision",19,3,10,10,3,0,0,0,1,8,11,1,0.17,255,99,16383},  //19
{"Paralysis",20,2,10,10,0,0,0,0,0,0,2,1,0.12,255,99,2464},  //20
{"Prism",21,2,10,10,0,4094,0,0,0,0,2,1,0.13,255,99,16383},  //21
{"Protect from Evil",22,1,10,10,0,512,0,0,255,8,3,1,0.13,255,99,768},  //22
{"Protect from Good",23,1,10,10,0,256,0,0,255,8,5,1,0.12,255,99,2208},  //23
{"Protection",24,1,10,10,0,3,0,0,255,8,10,1,0.11,255,99,16383},  //24
{"Razoredge",25,4,10,10,0,2,0,0,30,1,3,1,0.1,255,99,4243},  //25
{"Repair",26,5,10,10,0,0,0,0,0,0,11,1,0.17,255,99,0},  //26
{"Shadowmeld",27,1,10,10,0,0,0,0,0,8,10,1,0.14,255,99,2218},  //27
{"Shield",28,1,10,10,0,3,0,0,255,8,11,1,0.12,255,99,768},  //28
{"Slay Evil",29,2,10,10,1,512,0,60,0,0,9,1,0.11,255,99,9024},  //29
{"Slay Good",30,2,10,10,1,256,0,60,0,0,10,1,0.16,255,99,160},  //30
{"Speed",31,3,10,10,12,0,64,0,25,8,1,1,0.07,255,99,69},  //31
{"Strength",32,0,10,10,0,0,4,0,25,8,5,1,0.14,255,99,1288},  //32
{"Super Vision",33,3,10,10,4,0,0,0,1,8,7,1,0.13,255,99,16383},  //33
{"Vigor",34,0,10,10,0,0,0,0,0,8,6,1,0.17,255,99,16383}, //34
{"Noticeability",35,3,0,1,7,0,0,0,25,8,1,8,0.1133,30,91,5}, //Decreases your unnoticeability by 5 (Increases your noticeability by 5).
{"Continual Light",36,3,0,1,3,0,0,0,1,12,1,5,0.1758,30,88,533}, //No effect.
{"Healing Minor Wounds",37,5,0,3,0,0,0,0,20,0,3,7,0.1211,10,85,1}, //Restores 5 hit points.
{"Stormy Weather",38,5,0,12,0,0,0,0,0,0,12,8,0.1133,15,91,130}, //Changes the weather to type P (fully stormy) with a new duration (depending on the current month).
{"Slowness",39,2,0,4,0,0,0,25,0,0,4,10,0.0742,40,74,132}, //Decreases the encounter speed by 5.
{"The Water Hammer",40,2,0,11,1,32,0,0,8,0,11,11,0.0742,20,82,2}, //Inflicts 8 water damage to the encounter.
{"The Ice Shield",41,1,0,6,0,16,0,0,255,0.75,6,11,0.0664,40,73,2}, //Immunizes to fire damage (bugged).
{"Numbness",42,2,0,5,1,0,0,25,0,0,5,8,0.1055,37,84,2}, //Decreases the encounter speed and skill by 5.
{"Blindness",43,2,0,8,0,0,0,0,0,0,8,7,0.1016,33,71,4}, //Sets encounter skill to 0.
{"Clear Vision",44,5,0,3,0,0,0,0,0,0,3,7,0.1055,255,74,4}, //Removes delusion.
{"The Energy Shield",45,1,0,10,1,67,0,0,255,0.75,10,10,0.0781,38,78,2084}, //Immunizes to power damage. Increases resistance by 50% to sharp and blunt damage.
{"Curing Hunger",46,3,0,7,8,0,0,0,0,0,7,6,0.1484,8,89,8}, //Removes hunger.
{"Curing Thirst",47,3,0,7,9,0,0,0,0,0,7,6,0.1484,11,89,8}, //Removes thirst.
{"Stone Flesh",48,1,0,9,1,2,0,0,255,0.75,9,11,0.0742,43,82,8}, //Immunizes to sharp damage.
{"Healing Moderate Wounds",49,5,0,4,1,0,0,0,40,0,4,9,0.0977,9,88,8}, //Restores 8 hit points.
{"Endurance",50,0,0,3,1,0,0,0,0,48,3,10,0.0977,16,98,8}, //Decreases fatigue rate by 0.25 (-50%).
{"The Hot Foot",51,2,0,7,1,16,0,20,0,0,7,11,0.0625,55,69,16}, //Inflicts 4 fire damage to the encounter.
{"Flame",52,2,0,7,1,16,0,40,0,0,7,10,0.0781,60,78,16}, //Inflicts 8 fire damage to the encounter.
{"The Fireball",53,2,0,11,8,16,0,60,0,0,11,12,0.0703,20,84,80}, //Inflicts 12 fire damage to the encounter.
{"Heat",54,2,0,7,8,16,0,0,0,0,7,9,0.0859,77,77,16}, //Sets weather temperature to 75°F (23.9°C).
{"The Flame Shield",55,1,0,9,1,1056,0,0,255,0.75,9,10,0.0742,109,74,16}, //Immunizes to water damage.
{"Paralysis",56,2,0,11,0,0,0,0,0,0.0016,11,12,0.0625,50,75,2464}, //Paralyses the encounter.
{"Inflicting Serious Wounds",57,2,0,13,1,2048,0,60,0,0,13,13,0.0625,17,81,2080}, //Inflicts 12 'evil' damage to the encounter.
{"Fear",58,2,0,7,1,0,0,0,0,0,7,10,0.0859,117,86,32}, //Frightens the encounter.
{"Luck",59,3,0,5,6,0,0,0,75,12,5,8,0.1211,13,97,64}, //Increases treasure finding by 20 (removes 15 when ended! bug?).
{"Glamour",60,0,0,3,1,0,0,0,25,1.5,3,12,0.0742,10,89,64}, //Increases the charisma bonus by 10.
{"Deflecting Weapons",61,1,0,9,1,3,0,0,255,0.75,9,13,0.0547,44,71,64}, //Immunizes to sharp and blunt damage.
{"Confusion",62,2,0,5,1,0,0,50,0,0,5,10,0.0781,73,78,384}, //Decreases encounter intelligence by 10.
{"Poisoning",63,2,0,7,1,0,0,10,0,0.33,7,11,0.0625,45,69,2176}, //Removes 2 hit points to the encounter every minutes (probability of 0.3% each minute to naturally cure).
{"Inflicting Moderate Wounds",64,2,0,9,1,2048,0,40,0,0,9,13,0.0586,22,76,128}, //Inflicts 8 'evil' damage to the encounter.
{"Imprisonment",65,2,0,6,1,0,0,0,0,0,6,14,0.0586,99,82,256}, //Sets encounter speed to 0.
{"Healing Major Wounds",66,0,0,5,1,0,0,0,80,0,5,12,0.0742,7,89,3840}, //Restores 16 hit points.
{"Iron Flesh",67,1,0,9,1,3,0,0,255,0.75,9,14,0.0508,46,71,512}, //Immunizes to sharp and blunt damage.
{"Fair Weather",68,5,0,12,1,0,0,0,0,0,12,6,0.1484,21,89,512}, //Changes the weather to type A (fully sunny) with a new duration (depending on the current month).
{"Wisdom",69,0,0,5,1,0,16,0,25,1.5,5,15,0.0586,10,88,512}, //Increases the wisdom bonus by 5.
{"Intelligence",70,0,0,5,1,0,8,0,25,1.5,5,15,0.0586,10,88,528}, //Increases the intelligence bonus by 5.
{"Purification",71,3,0,3,10,0,0,0,0,0,3,9,0.0977,5,88,1024}, //Cures diseases (in incubation or not).
{"Removing Poison",72,3,0,3,11,0,0,0,0,0,3,9,0.1016,6,91,1024}, //Cures poisons (in incubation or not).
{"Removing Alcohol",73,3,0,7,5,0,0,0,0,0,7,9,0.1016,12,91,1024}, //Cures inebriation. Clears alcohol blood and intestine concentration, digestion rate and blackouts.
{"Stamina",74,0,0,3,1,0,0,0,25,1.5,3,10,0.0898,10,90,1024}, //Increases the stamina bonus by 5.
{"The Force Field",75,1,0,9,1,1,0,0,255,1.5,9,14,0.0547,36,77,1024}, //Immunizes to sharp a
{"RFE",76,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",77,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",78,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",79,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",80,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",81,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",82,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",83,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",84,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",85,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",86,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",87,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",88,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",89,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",90,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",91,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",92,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",93,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",94,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",95,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",96,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",97,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",98,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"RFE",99,0,0,0,0,0,0,0,0,0,0,0,0,255,99,0}, //RFE
{"Permananent Light",100,3,10,10,3,0,0,0,1,62365,11,1,0.17,255,99,0}, //Item effect only not castable
{"Speed",101,10,0,0,1,0,64,1,0,62365,0,0,0,255,99,0}, //Item effect only not castable
{"Dexterity",102,10,0,0,1,0,32,1,0,62365,0,0,0,255,99,0}, //Item effect only not castable
{"Wisdom",103,10,0,0,1,0,16,1,0,62365,0,0,0,255,99,0}, //Item effect only not castable
{"Intelligence",104,10,0,0,1,0,8,1,0,62365,0,0,0,255,99,0}, //Item effect only not castable
{"Strength",105,10,0,0,1,0,4,1,0,62365,0,0,0,255,99,0}, //Item effect only not castable
{"Charisma",106,10,0,0,1,0,2,1,0,62365,0,0,0,255,99,0}, //Item effect only not castable
{"Stamina",107,10,0,0,1,0,1,1,0,62365,0,0,0,255,99,0}, //Item effect only not castable
    
};

// Define a mapping between bit positions and player stats



extern bonusDamage weaponBonus;

std::map<int, int*> weaponBonusMap = {
	{0, &weaponBonus.blunt},
	 {1 , &weaponBonus.sharp },
     {2 , &weaponBonus.earth },
     {3 , &weaponBonus.air },
     {4 , &weaponBonus.fire },
     {5 , &weaponBonus.water },
     {6 , &weaponBonus.power },
     {7 , &weaponBonus.magic },
     {8 , &weaponBonus.good },
     {9 , &weaponBonus.evil },
     {10 , &weaponBonus.cold },
     {11 , &weaponBonus.nature },
     {12 , &weaponBonus.acid }

};

void loadSpells() {
	FILE* fp;               // file pointer - used when reading files
	char tempString[100];   // temporary string
	sprintf_s(tempString, sizeof(tempString), "%s%s", "data/map/", "Spells.bin");

	// Use fopen_s for improved error handling
	if (fopen_s(&fp, tempString, "rb") == 0 && fp != NULL) {
		// File opened successfully
		for (int i = 0; i < SpellList; i++) {
			spellItems[i] = fgetc(fp);
		}
		fclose(fp); // Close the file when done
	}
	else {
		// Handle file open error
		// You can print an error message or take appropriate action
		perror("Error opening file");
	}
}


void castSpells()
{
	// Based on SelectItem code using "pages" of spells hence reference to "pages > 2" etc

	int itemRef = 9999; // Nothing selected
	std::string str, selectDesc;
	selectDesc = "CAST";

    int menuitem1 = 255; // 255 is used here as nil
    int menuitem2 = 255;
    int menuitem3 = 255;
    int menuitem4 = 255;
    bool selectDone = false;

    int no_items = plyr.spellIndex; // Number of spells in players inventory
    int cur_idx = 0;
    int pages = 0;
    int page = 3;
    int page_item = 0;
	pages = 0;

	int noPages = no_items / 4; // based on 4 oncreen items per page
	pages += noPages;
	int tempRemainder = no_items % 4;
	if (tempRemainder != 0) { pages++; }

    while ( !selectDone )
    {
        if (page > 2) // Variable items
		{
			bool keypressed = false;
			while (!keypressed)
			{
				if (plyr.status == 3) drawEncounterView();
				if (plyr.status == 1) dispMain();
				if (plyr.status == 0) dispMain();
				cyText(1, selectDesc);
				bText (5, 3, "(1)");
				bText (5, 4, "(2)");
				bText (5, 5, "(3)");
				bText (5, 6, "(4)");
				bText (2, 8,"Item #, Forward, Back, or ESC to exit");
				SetFontColour(40, 96, 244, 255);
				bText (2, 8,"     #  F        B        ESC");
				SetFontColour(215, 215, 215, 255);

				page_item = 1;
				cur_idx = ((page-3)*4);
				menuitem1 = 9999; // 9999 is used as nil
				menuitem2 = 9999;
				menuitem3 = 9999;
				menuitem4 = 9999;

				while ((cur_idx < plyr.spellIndex) && (page_item<5))
				{
					str = spells[(spellBuffer[cur_idx].no)].name + " " + itos(spellBuffer[cur_idx].percentage) + "%";
					bText (9, (page_item+2), str);
					switch (page_item)
					{
						 case 1:
							  menuitem1 = cur_idx;
							  break;
						 case 2:
							  menuitem2 = cur_idx;
							  break;
						 case 3:
							  menuitem3 = cur_idx;
							  break;
						 case 4:
							  menuitem4 = cur_idx;
							  break;
					}
					page_item++;
					cur_idx++;
				}
				updateDisplay();

				string key_value;
				key_value = getSingleKey();
				if ((key_value == "1") && (menuitem1!=9999))
				{
					itemRef = menuitem1;
					keypressed=true;
					selectDone = true;
				}
				if ((key_value == "2") && (menuitem2!=9999))
				{
					itemRef = menuitem2;
					keypressed=true;
					selectDone = true;
				}
				if ((key_value == "3") && (menuitem3!=9999))
				{
					itemRef = menuitem3;
					keypressed=true;
					selectDone = true;
				}
				if ((key_value == "4") && (menuitem4!=9999))
				{
					itemRef = menuitem4;
					keypressed=true;
					selectDone = true;
				}

				if (key_value == "ESC") { keypressed=true; selectDone = true; }
				if ((key_value == "B") && (page>3)) { keypressed=true; page--; }
				if ((key_value == "up") && (page>3)) { keypressed=true; page--; }
				if ((key_value == "F") && (pages>(page-2))) { keypressed=true; page++;}
				if ((key_value == "down") && (pages>(page-2))) { keypressed=true; page++;}
			}
        } // page > 0 loop


    } // while cast not done

	if (itemRef != 9999) { attemptSpell(itemRef); } // Attempt to cast the selected spell
}


void attemptSpell(int spellRef)
{
	// Attempt to cast the selected spell from "castSpells()"

	// spellRef = index within spellBuffer foe selected spell
	int spellNo = spellBuffer[spellRef].no;
	int spellPercentage = spellBuffer[spellRef].percentage;
	string spellDesc = spells[spellBuffer[spellRef].no].name;
	int spellPoints = randn(1,5);

	int castProb = randn(0, 100);
	if (castProb < spellPercentage)
	{

		castSpellMessage(spellDesc);
		plyr.ringCharges -= spellPoints;
		if ( plyr.ringCharges < 0 ) { 
			plyr.ringCharges = 0; 
			plyr.fatigue = plyr.fatigue + spells[spellNo].cost;
			}
		int spellincrease = randn(0, 100);
		if (spellincrease <= (spells[spellNo].lessonboost * 100))
		{
			spellBuffer[spellRef].percentage += 1;
		}

		
		if (spells[spellNo].spelltype == 2)  //We are in combat
		{
			playerCast(spellNo);
		}
		else
		{
			castSpellAction(spellNo);
		}	
	}
	else
	{
		spellBackfiredMessage(spellPoints);
		plyr.hp -= spellPoints;
		// NEED  NEW METHOD OF CHECKING HP REDUCTION TO CATCH DEATH!
	}

}


void castSpellMessage(string spellDesc)
{
	string str, key; // for message text
	bool keynotpressed = true;
	playSpellSound();
	while ( keynotpressed )
	{
		if (plyr.status == 3) drawEncounterView();
		if (plyr.status == 1) dispMain();
		str = "You cast the spell of@@$ " + spellDesc + " $";
		cyText(3, str);
		updateDisplay();
		key = getSingleKey();
		if (key!="") { keynotpressed = false; }
	}
}


void spellBackfiredMessage(int spellPoints)
{
	string str, key; // for message text
	bool keynotpressed = true;
	while ( keynotpressed )
	{
		if (plyr.status == 3) drawEncounterView();
		if (plyr.status == 1) dispMain();
		if (plyr.status == 0) dispMain();
		str = "The spell failed@@and backfired for " +itos(spellPoints)+"!";
		cyText(3, str);
		updateDisplay();
		key = getSingleKey();
		if (key!="") { keynotpressed = false; }
	}
	// update ring charges and hp
}

/** @brief the purpose of this function is to split out which type of spells do what
* @brief If the spell has a duration then add it to the effectBuffer[].name and to the active spell list
* @brief effectbuufer includes all buffs positive and negative including potions,spells, clothing curses etc. upto 50
* @brief after adding it to the effectBuffer the call the updateSpell to action the effect passing the index to the effectBuffer
* @brief to tell the updateSpell which spell it is being actioned. using 0 as the hour means new spell
* @brief udapteSpell(hour) is the same but used when time passes to reduce all effects
* @brief call cast
* @param spellNo as supplied in the spells[] array
*
* Spelltype 0 = stat increase
* Spelltype 1 = Buff
* Spelltype 2 = Combat spell
* Spelltype 3 = hidden stat increase
* Spelltype 4 = Weapon enchant
* Spelltype 5 = Misc
*
*/
void castSpellAction(int spellNo)
{




#include <iostream>
#include <string>

	SearchResult effectIndexloc;
	SearchResult spellSlotloc;

	if (spells[spellNo].spelltype != 2)  //If the spell that was cast is not a combat spell
	{
		//search for the spell if it already active
 		effectIndexloc = findEffectByNoOrLastFree(spells[spellNo].no, sizeof(effectBuffer));
		spellSlotloc = findActiveSpellNoLastFree(spells[spellNo].no, sizeof(plyr.ActiveSpell));
	}

	if (spells[spellNo].duration > 0 && spellNo < 100)
	{
		plyr.ActiveSpell[spellSlotloc.index] = spells[spellNo].no;
	}

	switch (spells[spellNo].spelltype)
	{

	case 0:  //stat increase
		if (spells[spellNo].duration > 0 && spellNo<100)
		{
			plyr.ActiveSpell[spellSlotloc.index] = spells[spellNo].no;
		}
		
			effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
			effectBuffer[effectIndexloc.index].effectNo = spells[spellNo].no; //don't forget its 1 based
			effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
		
		if (!effectIndexloc.found)
		{
			applyEffect(spells[spellNo].duration, effectIndexloc.index);
		}
		break;

	case 1:  //Buff
		//Not implemented yet
		if (spells[spellNo].duration > 0 && spellNo < 100)
		{
			plyr.ActiveSpell[spellSlotloc.index] = spells[spellNo].no;
		}
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effectNo = spells[spellNo].no; //don't forget its 1 based
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
		if (!effectIndexloc.found)
		{
			applyEffect(effectBuffer[effectIndexloc.index].duration, effectIndexloc.index);
		}
		break;

	case 2:  //Combat spell
		playerCast(spellNo);
		break;

	case 3: //hidden stat increase
		if (spells[spellNo].duration > 0 && spellNo < 100)
		{
			plyr.ActiveSpell[spellSlotloc.index] = spells[spellNo].no;
		}
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effectNo = spells[spellNo].no;// don't forget its 1 based
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
		if (!effectIndexloc.found)
		{
			applyEffect(effectBuffer[effectIndexloc.index].duration, effectIndexloc.index);
		}
		break;
	
	case 4: // Weapon enchant
		if (spells[spellNo].duration > 0 && spellNo < 100)
		{
			plyr.ActiveSpell[spellSlotloc.index] = spells[spellNo].no;
		}
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effectNo = spells[spellNo].no;// don't forget its 1 based
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
		if (!effectIndexloc.found)
		{
			applyEffect(effectBuffer[effectIndexloc.index].duration, effectIndexloc.index);
		}
		break;
	
	case 5: //Misc
		
		//Not implmented yet

		break;
	case 10:  //stat increase
		if (spells[spellNo].duration > 0 && spellNo < 100)
		{
			plyr.ActiveSpell[spellSlotloc.index] = spells[spellNo].no;
		}

		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effectNo = spells[spellNo].no; //don't forget its 1 based
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours

		if (!effectIndexloc.found)
		{
			applyEffect(spells[spellNo].duration, effectIndexloc.index);
		}
		break;
	}

}


void castSpellActionold(int spellNo)
{

	/** @brief the purpose of this function is to split out which type of spells do what
	* @brief If the spell has a duration then add it to the effectBuffer[].name
	* @brief after adding it to the effectBuffer the call the updateSpell to action the effect passing the index to the effectBuffer
	* @brief to tell the updateSpell which spell it is being actioned. using 0 as the hour means new spell
	* @brief udapteSpell(hour) is the same but used when time passes to reduce all effects
	* @brief call cast
	* @param spellNo as supplied in the spells[] array
	*
	* Spelltype 0 = stat increase
	* Spelltype 1 = Buff
	* Spelltype 2 = Combat spell
	* Spelltype 3 = hidden stat increase
	* Spelltype 4 = Weapon enchant
	* Spelltype 5 = Misc
	*
	


#include <iostream>
#include <string>

	SearchResult effectIndexloc;
	SearchResult spellSlotloc;

	if (spells[spellNo].spelltype != 2)  //If the spell that was cast is not a combat spell
	{
		//search for the spell if it already active
		effectIndexloc = findEffectByNameOrLastFree(spells[spellNo].name, sizeof(effectBuffer));
		spellSlotloc = findEffectByNoOrLastFree(spells[spellNo].no, sizeof(plyr.ActiveSpell));
	}



	// Check for specific spells and their effects
	if (spellNo == 0)
	{
		plyr.food++;

	}  //Combat
	if (spellNo == 1) { plyr.food++; }  //Combat
	if (spellNo == 2)
	{
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no;// Charisma  //don't forget its 1 based
		effectBuffer[effectIndexloc.index].negativeValue = spells[spellNo].negativeValue;
		effectBuffer[effectIndexloc.index].positiveValue = spells[spellNo].positiveValue; // +30 to plyr.dex
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
	}


	if (spellNo == 4) {
		//effectsMap[effectBuffer[effectIndexloc.index].effect] = effectsMap;
		*effectsMap[effectBuffer[effectIndexloc.index].effect] = effectBuffer[effectIndexloc.index].positiveValue;
	}
	if (spellNo == 5) {
		*effectsMap[effectBuffer[effectIndexloc.index].effect] = effectBuffer[effectIndexloc.index].positiveValue;
	}
	if (spellNo == 6) { plyr.food++; }  //Combat
	if (spellNo == 7) { plyr.food++; }  //Combat
	if (spellNo == 8)  // Dexterity
	{
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no; // Charisma  //don't forget its 1 based
		effectBuffer[effectIndexloc.index].negativeValue = spells[spellNo].negativeValue;
		effectBuffer[effectIndexloc.index].positiveValue = spells[spellNo].positiveValue; // +30 to plyr.dex
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
	}
	if (spellNo == 9) { plyr.food++; } //Combat
	if (spellNo == 10) { plyr.food++; } //Combat
	if (spellNo == 11) { plyr.food++; } //Weapon Enchant
	if (spellNo == 12) { plyr.food++; } //Combat
	if (spellNo == 13)
	{
		plyr.hp += randn(1, 10);
		if (plyr.hp > plyr.maxhp) { plyr.hp = plyr.maxhp; }
	}
	if (spellNo == 14) {
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no;
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
	} //Light
	if (spellNo == 15) { plyr.food++; }  //Combat
	if (spellNo == 16) { displayLocation(); }
	if (spellNo == 17)
	{
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no;// Charisma  //don't forget its 1 based
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
	}
	if (spellNo == 18)
	{


	} //Combat
	if (spellNo == 19) {
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no;
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
	} //same as 14
	if (spellNo == 20) { plyr.food++; } //Combat
	if (spellNo == 21) { plyr.food++; } //Combat
	if (spellNo == 22) {
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no; // Charisma  //don't forget its 1 based
		effectBuffer[effectIndexloc.index].negativeValue = spells[spellNo].negativeValue;
		effectBuffer[effectIndexloc.index].positiveValue = spells[spellNo].positiveValue; // +30 to plyr.dex
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
	} //Buff
	if (spellNo == 23) {

		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no; // Charisma  //don't forget its 1 based
		effectBuffer[effectIndexloc.index].negativeValue = spells[spellNo].negativeValue;
		effectBuffer[effectIndexloc.index].positiveValue = spells[spellNo].positiveValue; // +30 to plyr.dex
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
	} //Buff

	if (spellNo == 24) {
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no;// Charisma  //don't forget its 1 based
		effectBuffer[effectIndexloc.index].negativeValue = spells[spellNo].negativeValue;
		effectBuffer[effectIndexloc.index].positiveValue = spells[spellNo].positiveValue; // +30 to plyr.dex
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
	} //Buff
	if (spellNo == 25) { plyr.food++; } //weapon Enchant
	if (spellNo == 26) { plyr.food++; } //Repair
	if (spellNo == 27) { plyr.food++; } //Buff
	if (spellNo == 28)
	{

		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no;
		effectBuffer[effectIndexloc.index].negativeValue = spells[spellNo].negativeValue;
		effectBuffer[effectIndexloc.index].positiveValue = spells[spellNo].positiveValue; // 
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // 
	} //Buff
	if (spellNo == 29) { plyr.food++; } //Combat
	if (spellNo == 30) { plyr.food++; } //Combat
	if (spellNo == 31)
	{
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no;// Charisma  //don't forget its 1 based
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
	} //Buff
	if (spellNo == 32)
	{
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no; // Charisma  //don't forget its 1 based
		effectBuffer[effectIndexloc.index].negativeValue = spells[spellNo].negativeValue;
		effectBuffer[effectIndexloc.index].positiveValue = spells[spellNo].positiveValue; // +30 to plyr.dex
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
	}
	if (spellNo == 33) {
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no;
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
	} //Buff
	if (spellNo == 34) {
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no; // Charisma  //don't forget its 1 based
		effectBuffer[effectIndexloc.index].negativeValue = spells[spellNo].negativeValue;
		effectBuffer[effectIndexloc.index].positiveValue = spells[spellNo].positiveValue; // +30 to plyr.dex
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
	} //
	if (spellNo == 36) { //Light
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours
	}
	if (spellNo == 46) {
		*effectsMap[effectBuffer[effectIndexloc.index].effect] = effectBuffer[effectIndexloc.index].positiveValue;

	}
	if (spellNo == 47) {
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no;

	}
	if (spellNo == 59) {
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no;

	}
	if (spellNo == 71) {
		effectBuffer[effectIndexloc.index].effect = spells[spellNo].no;
		effectBuffer[effectIndexloc.index].name = spells[spellNo].name;
		effectBuffer[effectIndexloc.index].duration = spells[spellNo].duration; // hours

	}
	if (spellNo == 72) {
		*effectsMap[effectBuffer[effectIndexloc.index].effect] = effectBuffer[effectIndexloc.index].positiveValue;

	}
	if (spellNo == 73) {
		*effectsMap[effectBuffer[effectIndexloc.index].effect] = effectBuffer[effectIndexloc.index].positiveValue;

	}

	if (spells[spellNo].duration > 0)
	{
		plyr.ActiveSpell[spellSlotloc.index] = spells[spellNo].no;
	}

	if (spells[spellNo].spelltype == 0)  //Atrribute changing spells
	{
		if (!effectIndexloc.found)
		{
			updateSpell(0, effectIndexloc.index);
		}
	}
	else if (spells[spellNo].spelltype == 3)
	{
		if (!effectIndexloc.found)
		{
			updateSpell(0, effectIndexloc.index);
		}

	}
	else if (spells[spellNo].spelltype == 2)  //This is a combat type
	{
		playerCast(spellNo);



	}
	else if (spells[spellNo].spelltype == 3)
	{
		if (!effectIndexloc.found)
		{
			updateSpell(0, effectIndexloc.index);
		}

	}
	*/
}