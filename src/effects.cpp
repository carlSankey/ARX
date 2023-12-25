#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <utility> // For std::pair
#include <bitset>
#include <map>

#include "effects.h"
#include "player.h"
#include "spells.h"
#include "misc.h"
#include "items.h"

using namespace std;

extern effectItem effectBuffer[1000]; // Actual definition with memory allocation

extern std::map<int, int*> statMap;

extern std::map<int, int*> resistanceMap;

struct effectIndex effectIndexloc;



std::map<int, int(*)> effectsMap = {
	{0, &plyr.food},
	{1, &plyr.water},
	{2, &plyr.keys},
	{3, &plyr.light},
	{4, &plyr.supervision},
	{5, &plyr.alcohol },
	{6, &plyr.treasureFinding },
	{7, &plyr.noticeability},
	{8, &plyr.hunger},
	{9, &plyr.thirst},
	{10,&plyr.diseases[0]},
	{11,&plyr.poison[0]},
	{12,&plyr.crystals},
	{13,&plyr.luck}
	
};

struct spellSlot spellSlotloc;


// Blunt damage - 1 byte = 0000000000000000
// Sharp damage - 1 byte
// Earth damage - 1 byte
// Air damage - 1 byte
// Fire damage - 1 byte
// Water damage - 1 byte
// Power damage - 1 byte
// Magic damage - 1 byte
// Good damage - 1 byte
// Evil damage - 1 byte
// Cold damage - 1 byte





/**
* @brief find the effect by name
* @brief or returns the found = false if not found
* @param targetname is the name of the effect
*
*/
SearchResult  findEffectByNameOrLastFree(const std::string& targetName, int size) {
	SearchResult effectIndex = { -1, false }; // Initialize result

	for (int i = 0; i < size; i++) {
		if (effectBuffer[i].name == targetName) {
			// Match found, set the result and return
			effectIndex.index = i;
			effectIndex.found = true;
			//return effectIndex;
			break;
			}

		if (effectBuffer[i].name.empty()) {
			// Empty name indicates a free slot, store the index
			effectIndex.index = i;
			break;
			//return effectIndex;
		}
	}
	return effectIndex;
	// No match found, return the result indicating it's the last free index

}

/**
* @brief find all the effect by type and retunr an array
* @brief or returns the found = false if not found
* @param targetname is the name of the effect
*
*/
SearchArray findAllEffectByNameOrLastFree(int targetNo, int size) {
	SearchResult* resultArray = new SearchResult[size];
	int arrSize = 0;
	for (int i = 0; i < size; ++i) {
				
		if (spells[effectBuffer[i].effectNo].spelltype == targetNo) {
			resultArray[arrSize].index = i;
			resultArray[arrSize].found = true;
			arrSize++;
		}
		else {
			resultArray[i].index = -1;  // or any other default value
			resultArray[i].found = false;
			}
	}
	SearchArray result;
	result.resultArray = resultArray;
	result.arrSize = arrSize;

	return result;
	// No match found, return the result indicating it's the last free index

}



/**
* @brief find the Effect by spell No  
* @brief or returns the found = false if not found
* @param targetname is the no of the effect
*/
SearchResult findEffectByNoOrLastFree(int targetNo, int size) {
	SearchResult EffectIndex = { -1, false }; // Initialize result
	for (int i = 0; i < size; i++) {
		if (effectBuffer[i].effectNo == targetNo) {
			// Match found, set the result and return
			EffectIndex.index = i;
			EffectIndex.found = true;
			
			break;
		}

		if (effectBuffer[i].effectNo == 0) {
			// Empty name indicates a free slot, store the index
			EffectIndex.index = i;
			break;
		}
		
	}
	return EffectIndex;
	// No match found, return the result indicating it's the last free index
	
}

/**
* @brief find the spell type
* @brief or returns the found = false if not found
* @param targetname is the type of the effect
*/
SearchResult  findEffectBySpellTypeOrLastFree(int targetType, int size) {
	SearchResult effectIndex = { -1, false }; // Initialize result

	for (int i = 0; i < size; i++) {
		if (spells[effectBuffer[i].effectNo].spelltype == targetType) {
			// Match found, set the result and return
			effectIndex.index = i;
			effectIndex.found = true;
			//return effectIndex;
			break;
		}

		if (effectBuffer[i].effectNo == 0) {
			// 0 effectNo indicates a free slot, store the index
			effectIndex.index = i;
			break;
			//return effectIndex;
		}
	}
	return effectIndex;
	// No match found, return the result indicating it's the last free index

}

/**
* @brief find the active spell
* @ or returns the found = false if not found
* @param targetname is the no of the active spell
*/
SearchResult  findActiveSpellNoLastFree(int targetNo, int size) {
	SearchResult effectIndex = { -1, false }; // Initialize result

	for (int i = 0; i < size; i++) {
		if (plyr.ActiveSpell[i] == targetNo) {
			// Match found, set the result and return
			effectIndex.index = i;
			effectIndex.found = true;
			//return effectIndex;
			break;
		}

		if (plyr.ActiveSpell[i] == 0) {
			// 0 effectNo indicates a free slot, store the index
			effectIndex.index = i;
			break;
			//return effectIndex;
		}
	}
	return effectIndex;
	// No match found, return the result indicating it's the last free index

}


SearchResult  findSpellByNameOrLastFree(const std::string& targetName, int size) {
	SearchResult effectIndex = { -1, false }; // Initialize result

	for (int i = 0; i < size; i++) {
		if (spells[i].name == targetName) {
			// Match found, set the result and return
			effectIndex.index = i;
			effectIndex.found = true;
			//return effectIndex;
			break;
		}

		if (spells[i].name.empty()) {
			// Empty name indicates a free slot, store the index
			effectIndex.index = i;
			break;
			//return effectIndex;
		}
	}
	return effectIndex;
	// No match found, return the result indicating it's the last free index

}

void updateStats(std::bitset<8> binaryStats, int positiveValue) {
	for (int i = 0; i < 8; i++) {
		if (binaryStats.test(i)) {
			*(statMap[i]) += positiveValue;
		}
	}
}


void updateHStats(std::bitset<14> binaryEffects, int positiveValue) {
	for (int i = 0; i < 8; i++) {
		if (binaryEffects.test(i)) {
			*(effectsMap[i]) += positiveValue;
		}
	}
}



void updateInvuls(std::bitset<13> binaryElems, int positiveValue) {
	for (int i = 0; i < 13; i++) {
		if (binaryElems.test(i)) {
			*(resistanceMap[i]) += positiveValue;
		}
	}
}

/**
* @brief applys buffs to the player  
* @brief and leave room for expansion
* @param hour = 0 then remove affect only (not active spell) as this could be a buff by clothing/armour etc
* @param effectid  = The spell affect (some spells are not castable but hold the same effect types/etc
*/
void applyEffect(int hour, int effectid)
{

	int spellActivated = 1;
	if (hour == 0) { spellActivated = -1; }


	std::cout << "UpdateSpell\n";
	
	int SpellDuraction = effectBuffer[effectid].duration;

	std::bitset<8> binaryStat(spells[effectBuffer[effectid].effectNo].stattype);
	std::bitset<13> binaryElems(spells[effectBuffer[effectid].effectNo].elementtype);
	std::bitset<13> binaryWeaponBuff(spells[effectBuffer[effectid].effectNo].elementtype);
	std::bitset<14> binaryEffects(spells[effectBuffer[effectid].effectNo].stattype);

	switch (spells[effectBuffer[effectid].effectNo].spelltype)
	{
	case 0: //Update stats
		if (spells[effectBuffer[effectid].effectNo].effect == 0) { spellActivated = 1; }  //bit of a frig for single use spells like Conjure Food
		updateStats(binaryStat, (spells[effectBuffer[effectid].effectNo].positiveValue - spells[effectBuffer[effectid].effectNo].negativeValue) * spellActivated);
		if (spells[effectBuffer[effectid].effectNo].effect == 0) { spellActivated = -1; }
		break;
	case 1: //Update vulnarabilities
		updateInvuls(binaryElems, (spells[effectBuffer[effectid].effectNo].positiveValue - spells[effectBuffer[effectid].effectNo].negativeValue) * spellActivated);
		break;
	case 2:  //combat spell
		//There is no effect for a combat spell
		break;
	case 3:  //
		if (spells[effectBuffer[effectid].effectNo].effect == 0) { spellActivated = 1; }  //bit of a frig for single use spells like Conjure Food
		updateHStats(binaryEffects, (spells[effectBuffer[effectid].effectNo].positiveValue - spells[effectBuffer[effectid].effectNo].negativeValue) * spellActivated);
		if (spells[effectBuffer[effectid].effectNo].effect == 0) { spellActivated = -1; }
		break;
	case 4:  //Weapon Buff
		updateWeaponBuff(binaryWeaponBuff, (spells[effectBuffer[effectid].effectNo].positiveValue - spells[effectBuffer[effectid].effectNo].negativeValue) * spellActivated);

		break;
	case 5:
		//Not implemented yet
		break;
	case 10: //Update stats

		updateStats(binaryStat, (spells[effectBuffer[effectid].effectNo].positiveValue - spells[effectBuffer[effectid].effectNo].negativeValue) * spellActivated);
		break;
	}

	//If removing effect then remove it from Effectbuffer
	if (spellActivated == -1) { 
		effectBuffer[effectid].name = "";
	effectBuffer[effectid].effectNo = 0;
	effectBuffer[effectid].negativeValue = 0;
	effectBuffer[effectid].positiveValue = 0;
	effectBuffer[effectid].duration = 0;
	}

}


void spellEffect(int spellId)
{

	switch (spells[spellId].effect)
	{
	case 0:  //Healing
		
		plyr.hp += randn(1, spells[spellId].positiveValue);
		if (plyr.hp > plyr.maxhp) { plyr.hp = plyr.maxhp; }

		break;
	case 1:  //Weather
		//not implemented graphically yet
		
		break;

	case 2:  //Weapon repair

		break;
	case 3: //
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		break;


	}



}

void repairAll()
{

	for (int i = 0; i < plyr.items_index - 1; ++i) {

		if (itemBuffer[i].location == 10)
		{
			itemBuffer[i].hp = itemBuffer[i].maxHP;

		}

	}

}

void changeWeather(int WeatherType)
{
	//not implemented yet



}