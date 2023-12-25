
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

spellRecord* newSpellArray = nullptr;

//extern std::map<int, int*> weaponBonusMap;


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

std::vector<spellRecord> readSpellsCSV(const std::string& filename) {
	std::vector<spellRecord> data;

	// Open the CSV file
	std::ifstream file("data/map/" + filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return data; // Return empty vector if file couldn't be opened
	}

	std::string line;
	while (std::getline(file, line)) {
		std::stringstream lineStream(line);
		spellRecord newSpell;
		std::string cell;

		// Parsing CSV fields into the struct members
		std::getline(lineStream, cell, ','); // Assuming the index is the first column
		newSpell.no = std::stoi(cell);

		std::getline(lineStream, newSpell.name, ',');

		std::getline(lineStream, cell, ',');
		newSpell.spelltype = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newSpell.percentage = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newSpell.cost = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newSpell.effect = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newSpell.elementtype = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newSpell.stattype = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newSpell.negativeValue = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newSpell.positiveValue = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newSpell.duration = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newSpell.damage = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newSpell.lessonsno = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newSpell.lessonboost = std::stof(cell);

		std::getline(lineStream, cell, ',');
		newSpell.uses = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newSpell.maxpercent = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newSpell.guild = std::stoi(cell);

		std::getline(lineStream, newSpell.castText, ',');
		

		data.push_back(newSpell);
	}

	file.close();
	return data;
}

spellRecord* spells = nullptr;

void readSpellData()
{
	std::string filename = "Spells.csv";
	std::vector<spellRecord>  csvData = readSpellsCSV(filename);

	// Convert vector to a dynamically allocated array of NewItem structs
	size_t itemCount = csvData.size();
	spells = new spellRecord[itemCount];

	// Copy data from vector to the dynamically allocated array
	for (size_t i = 0; i < itemCount; ++i) {
		spells[i] = csvData[i];
		plyr.spell_index = i;
	}
	
}

int createSpell(
	int spelltype,
	int no,
	std::string name,
	int percentage,
	int cost,
	int flags,
	int elementtype,
	int stattype,
	int negativeValue,
	int positiveValue,
	float duration,
	int damage,
	int lessonsno,
	float lessonboost,
	int uses,
	int maxpercent,
	int effect,
	int guild,
	std::string castText)
{

	//tidyObjectBuffer();

	// Create a new item
	spellRecord new_spell;

	SearchResult results = findSpellByNameOrLastFree(name, plyr.spell_index);
	int newSpellIndex = 0;
	if (results.found) //If already in the spells list (we must ahve cast this before
	{
		newSpellIndex = results.index;
	}
	else
	{
		newSpellIndex = plyr.spell_index;
	}
	// Set item attributes

	new_spell.name = name ;
	new_spell.no = no;
	new_spell.spelltype = spelltype;	//Type 0 = Attribute Change, 1 = inventory change, 2 = Buff, 3 = Damage
	new_spell.percentage = percentage;		//base
	new_spell.cost = cost;			// ring cost
	new_spell.effect = effect;			//0 = Blind,1 = paralyse......
	new_spell.elementtype= elementtype;    //Hex value
	new_spell.stattype= stattype;       //Hex value		
	new_spell.negativeValue= negativeValue;
	new_spell. positiveValue=positiveValue;
	new_spell.duration=duration;		//Duration
	new_spell.damage=damage;			//Failure Damage
	new_spell. lessonsno=lessonsno;
	new_spell.lessonboost=lessonboost;
	new_spell.uses=uses;
	new_spell.maxpercent=maxpercent;
	new_spell.guild=guild;
	new_spell.castText = castText;


	// Update buffer and buffer references
		spells[newSpellIndex] = new_spell;
	
	plyr.spell_index = newSpellIndex;
	return newSpellIndex; // what was the new items index in the object buffer
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

	while (!selectDone)
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
				bText(5, 3, "(1)");
				bText(5, 4, "(2)");
				bText(5, 5, "(3)");
				bText(5, 6, "(4)");
				bText(2, 8, "Item #, Forward, Back, or ESC to exit");
				SetFontColour(40, 96, 244, 255);
				bText(2, 8, "     #  F        B        ESC");
				SetFontColour(215, 215, 215, 255);

				page_item = 1;
				cur_idx = ((page - 3) * 4);
				menuitem1 = 9999; // 9999 is used as nil
				menuitem2 = 9999;
				menuitem3 = 9999;
				menuitem4 = 9999;

				while ((cur_idx < plyr.spellIndex) && (page_item < 5))
				{
					str = spells[(spellBuffer[cur_idx].no)].name + " " + itos(spellBuffer[cur_idx].percentage) + "%";
					bText(9, (page_item + 2), str);
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
				if ((key_value == "1") && (menuitem1 != 9999))
				{
					itemRef = menuitem1;
					keypressed = true;
					selectDone = true;
				}
				if ((key_value == "2") && (menuitem2 != 9999))
				{
					itemRef = menuitem2;
					keypressed = true;
					selectDone = true;
				}
				if ((key_value == "3") && (menuitem3 != 9999))
				{
					itemRef = menuitem3;
					keypressed = true;
					selectDone = true;
				}
				if ((key_value == "4") && (menuitem4 != 9999))
				{
					itemRef = menuitem4;
					keypressed = true;
					selectDone = true;
				}

				if (key_value == "ESC") { keypressed = true; selectDone = true; }
				if ((key_value == "B") && (page > 3)) { keypressed = true; page--; }
				if ((key_value == "up") && (page > 3)) { keypressed = true; page--; }
				if ((key_value == "F") && (pages > (page - 2))) { keypressed = true; page++; }
				if ((key_value == "down") && (pages > (page - 2))) { keypressed = true; page++; }
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
	int spellPoints = spells[spellBuffer[spellRef].no].cost;

	int castProb = randn(0, 100-plyr.luck);  //if player has luck then then get a better chance of casting a spell
	if (castProb < spellPercentage)
	{

		castSpellMessage(spellDesc);
		plyr.ringCharges -= spellPoints;
		if (plyr.ringCharges < 0) {
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
		spellBackfiredMessage(spells[spellBuffer[spellRef].no].damage);
 		plyr.hp -= spells[spellBuffer[spellRef].no].damage;
		// NEED  NEW METHOD OF CHECKING HP REDUCTION TO CATCH DEATH!
	}

}


void castSpellMessage(string spellDesc)
{
	string str, key; // for message text
	bool keynotpressed = true;
	playSpellSound();
	while (keynotpressed)
	{
		if (plyr.status == 3) drawEncounterView();
		if (plyr.status == 1) dispMain();
		str = "You cast the spell of@@$ " + spellDesc + " $";
		cyText(3, str);
		updateDisplay();
		key = getSingleKey();
		if (key != "") { keynotpressed = false; }
	}
}


void spellBackfiredMessage(int spellPoints)
{
	string str, key; // for message text
	bool keynotpressed = true;
	while (keynotpressed)
	{
		if (plyr.status == 3) drawEncounterView();
		if (plyr.status == 1) dispMain();
		if (plyr.status == 0) dispMain();
		str = "The spell failed@@and backfired for " + itos(spellPoints) + "!";
		cyText(3, str);
		updateDisplay();
		key = getSingleKey();
		if (key != "") { keynotpressed = false; }
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
* Spelltype 10 = wearble buffs (armor or clothing)
* Spelltype 11 = Wands/eyes/scrolls etc damaging effects
*/
void castSpellAction(int spellNo)
{




#include <iostream>
#include <string>

	SearchResult effectIndexloc;
	SearchResult spellSlotloc;

	if (spells[spellNo].spelltype != 2 || spells[spellNo].duration == 0)  //If the spell that was cast is not a combat spell
	{
		//search for the spell if it already active
		effectIndexloc = findEffectByNoOrLastFree(spells[spellNo].no, sizeof(effectBuffer));
		spellSlotloc = findActiveSpellNoLastFree(spells[spellNo].no, sizeof(plyr.ActiveSpell));
	}

	if (spells[spellNo].duration > 0 )
	{
		plyr.ActiveSpell[spellSlotloc.index] = spells[spellNo].no;
	}

	switch (spells[spellNo].spelltype)
	{

	case 0:  //stat increase
		if (spells[spellNo].duration > 0)
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

		switch (spells[spellNo].effect)
		{

		case 0:
			displayLocation();
		break;

		case 1:
			repairAll();

		break;

		case 2:
			changeWeather(spells[spellNo].elementtype);

		break;


		}
		//Not implmented yet

		break;
	case 10:  //stat increase used for items
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
	case 11: // Item action
		//wands or eyes or scrolls or Silver Horn or certain cards



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


