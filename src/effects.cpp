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

using namespace std;

extern effectItem effectBuffer[50]; // Actual definition with memory allocation

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
	{12,&plyr.speed}
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





// Pass the array size as a parameter
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


SearchResult findEffectByNoOrLastFree(int targetNo, int size) {
	SearchResult spellSlot = { -1, false }; // Initialize result
	for (int i = 0; i < size; i++) {
		if (plyr.ActiveSpell[i] == targetNo) {
			// Match found, set the result and return
			spellSlot.index = i;
			spellSlot.found = true;
			
			break;
		}

		if (plyr.ActiveSpell[i] == 0) {
			// Empty name indicates a free slot, store the index
			spellSlot.index = i;
			break;
		}
		
	}
	return spellSlot;
	// No match found, return the result indicating it's the last free index
	
}

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


void updateStats(std::bitset<7> binaryStats, int positiveValue) {
	for (int i = 0; i < 7; i++) {
		if (binaryStats.test(i)) {
			*(statMap[i]) += positiveValue;
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


void applyEffect(int hour, int effectid)
{
	int spellActivated = 1;
	if (hour == 0) { spellActivated = -1; }


	std::cout << "UpdateSpell\n";
	//for now treat all of the spells the same.  Change in the future
	int SpellDuraction = effectBuffer[effectid].duration;

	std::bitset<7> binaryStat(spells[effectBuffer[effectid].effectNo].stattype);
	std::bitset<13> binaryElems(spells[effectBuffer[effectid].effectNo].elementtype);
	std::bitset<13> binaryWeaponBuff(spells[effectBuffer[effectid].effectNo].elementtype);

	switch (spells[effectBuffer[effectid].effectNo].spelltype)
	{
	case 0:

		updateStats(binaryStat, spells[effectBuffer[effectid].effectNo].positiveValue * spellActivated);
		break;
	case 1:
		updateInvuls(binaryElems, spells[effectBuffer[effectid].effectNo].positiveValue * spellActivated);
		break;
	case 2:
		//There is no effect for a combat spell
		break;
	case 3:
		*effectsMap[spells[effectBuffer[effectid].effectNo].effect] = spells[effectBuffer[effectid].effectNo].positiveValue * spellActivated;
		break;
	case 4:  //Weapon Buff
		updateWeaponBuff(binaryWeaponBuff, spells[effectBuffer[effectid].effectNo].positiveValue * spellActivated);

		break;
	case 5:
		//Not implemented yet
		break;
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

