
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "items.h"
#include "spells.h"
#include "player.h"
#include "display.h"
#include "encounter.h"
#include "font.h"
#include "misc.h"
#include "audio.h"

spellItem spellBuffer[35]; // learnt spells that can be cast

spellRecord spells[35] =
{
	//											T   L     D   C L O     M P
	{"Bewilder", 50, 42, 10, 10, 0, 1,			1,0,0,0,0,0,0,1,0,1,0,0,1,0}, //0
	{"Blinding",50, 42, 10, 10, 0, 1,			0,0,1,0,0,0,0,0,0,0,0,0,0,0}, //1
	{"Charisma", 50, 42, 10, 10, 0, 1,			1,0,0,0,0,1,0,1,0,0,0,0,1,0}, //2
	{"Cold Blast",50, 42, 10, 10, 0, 1,			0,0,0,0,0,1,0,0,1,1,0,0,0,0}, //3
	{"Conjure Food", 50, 42, 10, 10, 0, 1,		0,0,0,0,0,0,0,0,0,1,0,0,1,0}, //4
	{"Conjure Key",50, 42, 10, 10, 0, 1,		1,0,0,0,0,0,0,0,1,0,0,0,1,0}, //5
	{"Defeat Evil", 50, 42, 10, 10, 0, 1,		0,0,0,0,0,0,0,0,1,1,0,0,0,0}, //6
	{"Defeat Good",50, 42, 10, 10, 0, 1,		0,0,0,0,0,1,0,1,0,0,0,0,0,0}, //7
	{"Dexterity", 50, 42, 10, 10, 0, 1,			1,0,1,0,0,0,0,0,0,0,0,0,0,0}, //8
	{"Fear",50, 42, 10, 10, 0, 1,				0,0,1,0,0,1,0,1,0,1,0,0,0,0}, //9
	{"Fireballs", 50, 42, 10, 10, 0, 1,			0,0,0,0,0,0,0,1,1,0,0,0,0,0}, //10
	{"Fireblade",50, 42, 10, 10, 0, 1,			0,0,0,0,0,0,0,1,0,0,0,0,0,1}, //11
	{"Fury", 50, 42, 10, 10, 0, 1,				0,0,0,0,0,0,0,0,0,0,0,0,0,1}, //12
	{"Healing",50, 42, 10, 10, 0, 1,			1,1,1,1,1,1,1,1,1,1,1,1,0,1}, //13
	{"Light", 50, 42, 10, 10, 0, 1,				0,0,1,0,0,0,0,0,1,1,0,0,0,1}, //14
	{"Lightning Bolts",50, 42, 10, 10, 0, 1,	0,0,1,0,0,1,0,0,0,0,0,0,0,0}, //15
	{"Location", 50, 42, 10, 10, 0, 1,			0,1,1,1,1,1,1,1,1,1,1,1,1,0}, //16
	{"Luck",50, 42, 10, 10, 0, 1,				1,0,0,0,0,0,0,0,0,0,0,0,1,0}, //17
	{"Magic Darts", 50, 42, 10, 10, 0, 1,		0,0,1,0,0,1,0,0,0,0,0,0,0,0}, //18
	{"Night Vision",50, 42, 10, 10, 0, 1,		1,0,0,0,0,1,0,1,0,0,0,0,1,0}, //19
	{"Paralysis", 50, 42, 10, 10, 0, 1,			0,0,1,0,0,0,0,1,0,0,0,0,1,0}, //20
	{"Prism",50, 42, 10, 10, 0, 1,				0,0,1,0,0,0,0,0,0,0,0,0,0,0}, //21
	{"Protect from Evil", 50, 42, 10, 10, 0, 1, 0,0,1,0,0,0,0,0,1,1,0,0,0,0}, //22
	{"Protect from Good",50, 42, 10, 10, 0, 1,	0,0,0,0,0,1,0,1,0,0,0,0,0,0}, //23
	{"Protection", 50, 42, 10, 10, 0, 1,		1,0,1,0,0,1,0,0,0,0,0,0,0,1}, //24
	{"Razoredge",50, 42, 10, 10, 0, 1,			0,0,0,0,0,0,0,0,0,0,0,0,1,1}, //25
	{"Repair", 50, 42, 10, 10, 0, 1,			1,0,0,0,0,0,0,0,0,0,0,0,1,1}, //26
	{"Shadowmeld",50, 42, 10, 10, 0, 1,			1,0,0,0,0,1,0,0,0,0,0,0,0,0}, //27
	{"Shield", 50, 42, 10, 10, 0, 1,			1,1,1,1,1,1,1,1,1,1,1,1,1,1}, //28
	{"Slay Evil",50, 42, 10, 10, 0, 1,			0,0,0,0,0,0,0,0,0,1,0,0,0,0}, //29
	{"Slay Good",50, 42, 10, 10, 0, 1,			0,0,0,0,0,0,0,1,0,0,0,0,0,0}, //30
	{"Speed", 50, 42, 10, 10, 0, 1,				1,0,1,0,0,0,0,0,0,0,0,0,0,0}, //31
	{"Strength",50, 42, 10, 10, 0, 1,			0,0,0,0,0,0,0,1,1,0,0,0,0,1}, //32
	{"Super Vision", 50, 42, 10, 10, 0, 1,		1,0,1,0,0,0,0,1,0,1,0,0,1,0}, //33
	{"Vigor",50, 42, 10, 10, 0, 1,				0,0,0,0,0,0,0,1,1,0,0,0,0,1} //34

};


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
		if ( plyr.ringCharges < 0 ) { plyr.ringCharges = 0; }

		// Check for specific spells and their effects
		if ( spellNo == 4 ) { plyr.food++; }
		if ( spellNo == 5 ) { plyr.keys++; }
		if ( spellNo == 13)
		{
			plyr.hp+=randn(1,10);
			if (plyr.hp > plyr.maxhp) { plyr.hp = plyr.maxhp; }
		}
		if ( spellNo == 16) { displayLocation(); }
		if ( spellNo == 8)  // Dexterity
		{
			effectBuffer[plyr.effectIndex].effect = 1; // Dexterity
			effectBuffer[plyr.effectIndex].negativeValue = 0;
			effectBuffer[plyr.effectIndex].positiveValue = 30; // +30 to plyr.dex
			effectBuffer[plyr.effectIndex].duration = 8; // hours
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
