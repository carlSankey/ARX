//#include <SFML/Graphics.hpp>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "saveGame.h"

#include "globals.h"
#include "dev.h"

#include "display.h"
#include "misc.h"

#include "player.h"
#include "items.h"
#include "spells.h"
#include "guild.h"
#include "effects.h"


//using namespace std;

// Each character save game file is made up of a 28540 element string array.
// Each array element can hold a number or text string (e.g. no. of torches carried or an item name)

// NOTES:
// job openings not currently part of saved game

string character[saveGameSize];
string saveGameDescriptions[10];
int damageTypes = 13;

void displayLoadGame()
{
	string str;
	drawText (1,3,"(0)");
	drawText (1,5,"(1)");
	drawText (1,7,"(2)");
	drawText (1,9,"(3)");
	drawText (1,11,"(4)");
	drawText (1,13,"(5)");
	drawText (1,15,"(6)");
	drawText (1,17,"(7)");
	drawText (1,19,"(8)");
	drawText (1,21,"(9)");

	drawText(8,23,"Select 0-9 or ESC to cancel");

	for (int a = 0; a < 10; ++a) // number of save game slots 0 - 9
	{
        str	= saveGameDescriptions[a];
        drawText(5,((a*2)+3),str);
	}

}


void initSaveGameDescriptions()
{
	std::ifstream instream;
	instream.open("data/saves/saveGames.txt");
	string text;

	for (int a = 0; a < 10; a++) // number of save game slots 0 - 9
	{
		getline(instream, text);
		saveGameDescriptions[a] = text;
	}
	instream.close();
}


void updateSaveGameDescriptions()
{
	ofstream outdata; // outdata is like cin
	outdata.open("data/saves/saveGames.txt"); // opens the file

	for(int y = 0; y < 10; ++y)
	{
		outdata << saveGameDescriptions[y] << endl;
	}
	outdata.close();
}



void initcharacter()
{
	for (int i=0 ; i<saveGameSize ; i++)
	{
		character[i] = "<BLANK>";
	}
}



void displaySaveGame()
{
	string key;
	plyr.status = 0;
	{
			int savegameMenu = 255; // high level menu



			while (savegameMenu < 256)
			{
				while (savegameMenu == 255) // main menu
				{
					clearDisplay();
					displayLoadGame();
					drawText(12,0,"Save a character");
					updateDisplay();

					key = getSingleKey();
					if ( key=="0" ) { savegameMenu = 0;  }
					if ( key=="1" ) { savegameMenu = 1; }
					if ( key=="2" ) { savegameMenu = 2; }
					if ( key=="3" ) { savegameMenu = 3; }
					if ( key=="4" ) { savegameMenu = 4; }
					if ( key=="5" ) { savegameMenu = 5; }
					if ( key=="6" ) { savegameMenu = 6; }
					if ( key=="7" ) { savegameMenu = 7; }
					if ( key=="8" ) { savegameMenu = 8; }
					if ( key=="9" ) { savegameMenu = 9; }
					if ( key=="ESC" ) { savegameMenu = 256; plyr.status=1; }
				}
				while (savegameMenu < 10) // attempt to save a character
				{
                    saveCharacter(savegameMenu);
					plyr.status=1; // for display canvas
					savegameMenu = 256;
				}
			}

		}

}



bool loadCharacter(int saveSlot)
{

	std::ifstream instream;
	std::string junk,junk2, data;

	string filename = "data/saves/save"+itos(saveSlot)+".txt";
	instream.open(filename.c_str()); // opens the file
	//instream.open("data/saves/save0.txt");
	if( !instream )
	{
		cerr << "No save game in this slot" << endl;
		return false;
	}
	string text;

	for (int a = 0; a < saveGameSize; ++a)
	{
		getline(instream, text);
		character[a] = text;
		//atoi(character[a].c_str()) = atoi(text.c_str());
	}


	//getline(instream, text); // read first line from the file for the character name
	//plyr.name = text;
	//getline(instream, text); // read second line from the file for the z_offset
	//plyr.z_offset = atof(text.c_str());

	instream.close();

	plyr.status = EXPLORE;
	// Copy atoi(character[24576].c_str()) int block values back into the atoi(character object
	//cout << "loaded gender = " << plyr.gender << "\n";

	plyr.gender = atoi(character[0].c_str());
	plyr.hp = atoi(character[1].c_str());
	plyr.maxhp = atoi(character[2].c_str());

	plyr.scenario = atoi(character[3].c_str());
	plyr.map = atoi(character[4].c_str());
	plyr.mapWidth = atoi(character[5].c_str());
	plyr.mapHeight = atoi(character[6].c_str());
	plyr.x = atoi(character[7].c_str());
	plyr.oldx = atoi(character[8].c_str());
	plyr.y = atoi(character[9].c_str());
	plyr.oldy = atoi(character[10].c_str());
	plyr.facing = atoi(character[11].c_str());

	plyr.front = atoi(character[12].c_str());
	plyr.back = atoi(character[13].c_str());
	plyr.left = atoi(character[14].c_str());
	plyr.right = atoi(character[15].c_str());
	plyr.frontheight = atoi(character[16].c_str());
	plyr.leftheight = atoi(character[17].c_str());
	plyr.rightheight = atoi(character[18].c_str());
	plyr.floorTexture = atoi(character[19].c_str());
	plyr.ceiling = atoi(character[20].c_str());
	plyr.location = atoi(character[21].c_str());
	plyr.special = atoi(character[22].c_str());

	plyr.alive = atoi(character[23].c_str());

	plyr.teleporting = atoi(character[24].c_str());
      plyr.buffer_index = atoi(character[25].c_str());
      plyr.infoPanel = atoi(character[26].c_str());
      plyr.priWeapon = atoi(character[27].c_str());
      plyr.secWeapon = atoi(character[28].c_str());
      plyr.headArmour = atoi(character[29].c_str());
      plyr.bodyArmour = atoi(character[30].c_str());
      plyr.legsArmour = atoi(character[31].c_str());
      plyr.armsArmour = atoi(character[32].c_str());
      plyr.timeOfDay = atoi(character[33].c_str());
      plyr.minutes = atoi(character[34].c_str());
      plyr.hours = atoi(character[35].c_str());
      plyr.days = atoi(character[36].c_str());
      plyr.months = atoi(character[37].c_str());
      plyr.years = atoi(character[38].c_str());

	plyr.sta = atoi(character[39].c_str());
  plyr.chr = atoi(character[40].c_str());
  plyr.str = atoi(character[41].c_str());
  plyr.inte = atoi(character[42].c_str());
  plyr.wis = atoi(character[43].c_str());
  plyr.skl = atoi(character[44].c_str());
  plyr.maxhp = atoi(character[45].c_str());
  plyr.hp = atoi(character[46].c_str());
  plyr.xp = atoi(character[47].c_str());
  plyr.level = atoi(character[48].c_str()); // xp level
  plyr.chrPartials = atoi(character[49].c_str());
  plyr.intPartials = atoi(character[50].c_str());
  plyr.strPartials = atoi(character[51].c_str());
  plyr.speed = atoi(character[52].c_str());
  plyr.stealth = atoi(character[53].c_str());
  plyr.diagOn = atoi(character[54].c_str());
  plyr.mapOn = atoi(character[55].c_str());
  plyr.fpsOn = atoi(character[56].c_str());
  plyr.miniMapOn = atoi(character[57].c_str());
  plyr.silver = atoi(character[58].c_str());
  plyr.gold = atoi(character[59].c_str());
  plyr.copper = atoi(character[60].c_str());
  plyr.food = atoi(character[61].c_str());
  plyr.torches = atoi(character[62].c_str());
  plyr.water = atoi(character[63].c_str());
  plyr.timepieces = atoi(character[64].c_str());
  plyr.crystals = atoi(character[65].c_str());
  plyr.jewels = atoi(character[66].c_str());
  plyr.gems = atoi(character[67].c_str());
  plyr.compasses = atoi(character[68].c_str());
  plyr.keys = atoi(character[69].c_str());
  plyr.encounter_done = atoi(character[70].c_str());
  plyr.game_on = atoi(character[71].c_str());
  plyr.gender = atoi(character[72].c_str());
  plyr.zone = atoi(character[73].c_str());
  plyr.zoneSet = atoi(character[74].c_str());
  plyr.current_zone = atoi(character[75].c_str()); // used by drawing function
  plyr.status = atoi(character[76].c_str());
  plyr.specialwall = atoi(character[77].c_str());
//  plyr.windowSize = atoi(character[78].c_str());
  //plyr.graphicStyle = atoi(character[79].c_str());
  plyr.fixedEncounter = atoi(character[80].c_str());
  plyr.fixedEncounterRef = atoi(character[81].c_str());
  plyr.thirst = atoi(character[82].c_str());
  plyr.hunger = atoi(character[83].c_str());
  plyr.digestion = atoi(character[84].c_str());
  plyr.alcohol = atoi(character[85].c_str());


 for(int y = 0; y < 12; ++y) { plyr.guildAwards[y] = atoi(character[86+y].c_str()); }
 for(int y = 0; y < 32; ++y) { plyr.fixedEncounters[y] = atoi(character[98+y].c_str()); }
 for(int y = 0; y < 14; ++y) { plyr.guildMemberships[y] = atoi(character[130+y].c_str()); }

  plyr.ringCharges = atoi( character[144].c_str());
  plyr.alignment = atoi( character[145].c_str());
  plyr.lfood = atoi( character[146].c_str());
  plyr.lwater = atoi( character[147].c_str());
  plyr.ltorches = atoi( character[148].c_str());
  plyr.ltimepieces = atoi( character[149].c_str());
  plyr.lcompasses = atoi( character[150].c_str());
  plyr.lkeys = atoi( character[151].c_str());
  plyr.lcrystals = atoi( character[152].c_str());
  plyr.lgems = atoi( character[153].c_str());
  plyr.ljewels = atoi( character[154].c_str());
  plyr.lgold = atoi( character[155].c_str());
  plyr.lsilver = atoi( character[156].c_str());
  plyr.lcopper = atoi( character[157].c_str());
  plyr.spellIndex = atoi( character[158].c_str());
  plyr.effectIndex = atoi( character[159].c_str());
  plyr.retreatFriendship = atoi( character[160].c_str());
  plyr.damonFriendship = atoi( character[161].c_str());

  plyr.smithyFriendships[0] = atoi( character[162].c_str());
  plyr.smithyFriendships[1] = atoi(	character[163].c_str());
  plyr.smithyFriendships[2] = atoi(	character[164].c_str());
  plyr.smithyFriendships[3] = atoi(	character[165].c_str());

plyr.bankAccountStatuses[0] = atoi( character[166].c_str());
plyr.bankAccountStatuses[1] = atoi( character[167].c_str());
plyr.bankAccountStatuses[2] = atoi( character[168].c_str());
plyr.bankAccountStatuses[3] = atoi( character[169].c_str());
plyr.bankAccountStatuses[4] = atoi( character[170].c_str());
plyr.bankAccountStatuses[5] = atoi( character[171].c_str());
plyr.bankAccountStatuses[6] = atoi( character[172].c_str());
plyr.bankAccountStatuses[7] = atoi( character[173].c_str());
plyr.bankAccountStatuses[8] = atoi( character[174].c_str());

plyr.bankAccountBalances[0] = atoi( character[175].c_str());
plyr.bankAccountBalances[1] = atoi( character[176].c_str());
plyr.bankAccountBalances[2] = atoi( character[177].c_str());
plyr.bankAccountBalances[3] = atoi( character[178].c_str());
plyr.bankAccountBalances[4] = atoi( character[179].c_str());
plyr.bankAccountBalances[5] = atoi( character[180].c_str());
plyr.bankAccountBalances[6] = atoi( character[181].c_str());
plyr.bankAccountBalances[7] = atoi( character[182].c_str());
plyr.bankAccountBalances[8] = atoi( character[183].c_str());

plyr.clothing[0] = atoi( character[184].c_str());
plyr.clothing[1] = atoi( character[185].c_str());
plyr.clothing[2] = atoi( character[186].c_str());
plyr.clothing[3] = atoi( character[187].c_str());

plyr.goblinsVisited = atoi( character[188].c_str());
plyr.goblinsChallenged = atoi( character[189].c_str());
plyr.goblinsDefeated = atoi( character[190].c_str());
plyr.goblinsCombat = atoi( character[191].c_str());
plyr.goblinsReforged = atoi( character[192].c_str());
plyr.trollsVisited = atoi( character[193].c_str());
plyr.trollsChallenged = atoi( character[194].c_str());
plyr.trollsDefeated = atoi( character[195].c_str());
plyr.trollsCombat = atoi( character[196].c_str());
plyr.trollsReforged = atoi( character[197].c_str());

plyr.oracleReturnTomorrow = atoi( character[198].c_str());
plyr.oracleDay = atoi( character[199].c_str());
plyr.oracleMonth = atoi( character[200].c_str());
plyr.oracleYear = atoi( character[201].c_str());
plyr.oracleQuestNo = atoi( character[202].c_str());
plyr.healerDays[0] = atoi( character[203].c_str());
plyr.healerDays[1] = atoi( character[204].c_str());
plyr.healerHours[0] = atoi( character[205].c_str());
plyr.healerHours[1] = atoi( character[206].c_str());
plyr.healerMinutes[0] = atoi( character[207].c_str());
plyr.healerMinutes[1] = atoi( character[208].c_str());
plyr.treasureFinding = atoi( character[209].c_str());
plyr.invisibility = atoi( character[210].c_str());
plyr.diseases[0] = atoi( character[211].c_str());
plyr.diseases[1] = atoi( character[212].c_str());
plyr.diseases[2] = atoi( character[213].c_str());
plyr.diseases[3] = atoi( character[214].c_str());
plyr.poison[0] = atoi( character[215].c_str());

plyr.poison[1] = atoi( character[216].c_str());
plyr.poison[2] = atoi( character[217].c_str());
plyr.poison[3] = atoi( character[218].c_str());
plyr.delusion = atoi( character[219].c_str());
for(int y = 0; y < damageTypes; ++y) { plyr.invulnerability[y] = atoi(character[220+y].c_str()); }
plyr.noticeability = atoi(character[229].c_str());
plyr.protection1 = atoi(character[230].c_str());
plyr.protection2 = atoi(character[231].c_str());

plyr.forgeDays  = atoi(character[232].c_str());
plyr.forgeType  = atoi(character[233].c_str());
plyr.forgeBonus = atoi(character[234].c_str());
plyr.forgeName  = character[235];
plyr.stolenFromVault = atoi(character[236].c_str());

for(int y = 0; y < 14; ++y) 
{ 
	plyr.guildAccepting[y] = atoi(character[237+y].c_str()); 
}


int loadGameIndex = 400; // start location for object buffer items
for(int z = 0; z < itemBufferSize; ++z)
{
	itemBuffer[z].hp = atoi(character[loadGameIndex].c_str());
	itemBuffer[z].index = atoi(character[loadGameIndex+1].c_str());
	itemBuffer[z].level = atoi(character[loadGameIndex+2].c_str());
	itemBuffer[z].location = atoi(character[loadGameIndex+3].c_str());
	itemBuffer[z].type = atoi(character[loadGameIndex+4].c_str());
	itemBuffer[z].x = atoi(character[loadGameIndex+5].c_str());
	itemBuffer[z].y = atoi(character[loadGameIndex+6].c_str());


	itemBuffer[z].name = character[loadGameIndex+7];
	itemBuffer[z].maxHP = atoi( character[loadGameIndex+8].c_str());
	itemBuffer[z].flags = atoi( character[loadGameIndex+9].c_str());
	itemBuffer[z].minStrength = atoi( character[loadGameIndex+10].c_str());
	itemBuffer[z].minDexterity = atoi( character[loadGameIndex+11].c_str());
	itemBuffer[z].useStrength = atoi( character[loadGameIndex+12].c_str());
	itemBuffer[z].blunt = atoi( character[loadGameIndex+13].c_str());
	itemBuffer[z].sharp = atoi( character[loadGameIndex+14].c_str());

	itemBuffer[z].earth = atoi( character[loadGameIndex+15].c_str());
	itemBuffer[z].air = atoi( character[loadGameIndex+16].c_str());
	itemBuffer[z].fire = atoi( character[loadGameIndex+17].c_str());
	itemBuffer[z].water = atoi( character[loadGameIndex+18].c_str());
	itemBuffer[z].power = atoi( character[loadGameIndex+19].c_str());
	itemBuffer[z].magic = atoi( character[loadGameIndex+20].c_str()); // mental
	itemBuffer[z].good = atoi( character[loadGameIndex+21].c_str()); // cleric
	itemBuffer[z].evil = atoi( character[loadGameIndex+22].c_str());
	itemBuffer[z].cold = atoi( character[loadGameIndex+23].c_str());
	itemBuffer[z].weight = atoi( character[loadGameIndex+24].c_str());
	itemBuffer[z].alignment = atoi( character[loadGameIndex+25].c_str());
	itemBuffer[z].melee = atoi( character[loadGameIndex+26].c_str());
	itemBuffer[z].ammo = atoi( character[loadGameIndex+27].c_str());
	itemBuffer[z].parry = atoi( character[loadGameIndex+28].c_str());

	loadGameIndex = loadGameIndex + 28;
}


// Copy spell buffer
loadGameIndex = 7400; // start location for spell buffer items (70 bytes)
for(int z = 0; z < 35; ++z)
{
	spellBuffer[z].no = atoi(character[loadGameIndex].c_str());
	spellBuffer[z].percentage = atoi(character[loadGameIndex+1].c_str());
	loadGameIndex = loadGameIndex + 2;
}

// Copy effect buffer
loadGameIndex = 7470; // start location for effect buffer items (200 bytes)
for(int z = 0; z < 50; ++z)
{
	effectBuffer[z].effectNo = atoi(character[loadGameIndex].c_str());
	effectBuffer[z].negativeValue = atoi(character[loadGameIndex+1].c_str());
	effectBuffer[z].positiveValue = atoi(character[loadGameIndex+2].c_str());
	effectBuffer[z].duration = atoi(character[loadGameIndex+3].c_str());
	loadGameIndex = loadGameIndex + 4;
}

// Smithy daily wares - int smithyDailyWares[4][10];
loadGameIndex = 7670;
for(int z = 0; z < 4; ++z)
{
	for(int x = 0; x < 10; ++x)
	{
		smithyDailyWares[z][x] = atoi(character[loadGameIndex].c_str());
		loadGameIndex++;
	}
}

//int tavernDailyFoods[14][6];
loadGameIndex = 7710;
for(int z = 0; z < 14; ++z)
{
	for(int x = 0; x < 6; ++x)
	{
		tavernDailyFoods[z][x] = atoi(character[loadGameIndex].c_str());
		loadGameIndex++;
	}
}

//int tavernDailyDrinks[14][6];
loadGameIndex = 7794;
for(int z = 0; z < 14; ++z)
{
	for(int x = 0; x < 6; ++x)
	{
		tavernDailyDrinks[z][x] = atoi(character[loadGameIndex].c_str());
		loadGameIndex++;
	}
}

// int shopDailyWares[15][12];
loadGameIndex = 7878;
for(int z = 0; z < 15; ++z)
{
	for(int x = 0; x < 12; ++x)
	{
		shopDailyWares[z][x] = atoi(character[loadGameIndex].c_str());
		loadGameIndex++;
	}
}

// Currently inn and tavern job openings are not part of saved game

// load automap flags
loadGameIndex = 8058; // start location for object buffer items
for(int z = 0; z < 5; ++z)
{
	for(int x = 0; x < 4096; ++x)
	{
		autoMapExplored[z][x] = atoi(character[loadGameIndex].c_str());
		loadGameIndex++;
	}
}

plyr.name = character[28538];
plyr.z_offset = atof(character[28539].c_str());
string saveGameReleaseNo = character[28540];

  return true;
}






bool saveCharacter(int saveSlot)
{
    ofstream outdata;

    saveGameDescriptions[saveSlot] = plyr.name;
    updateSaveGameDescriptions();

    initcharacter(); // Clear out string array

    // Copy character object data (except name) into the character[4096] int block
    character[0] = itos(plyr.gender);
    character[1] = itos(plyr.hp);
    character[2] = itos(plyr.maxhp);
    character[3] = itos( plyr.scenario);
    character[4] = itos( plyr.map);
    character[5] = itos( plyr.mapWidth);
    character[6] = itos( plyr.mapHeight);
    character[7] = itos( plyr.x);
    character[8] = itos( plyr.oldx);
    character[9] = itos( plyr.y);
    character[10] = itos( plyr.oldy);
    character[11] = itos( plyr.facing);
    character[12] = itos( plyr.front);
    character[13] = itos( plyr.back);
    character[14] = itos( plyr.left);
    character[15] = itos( plyr.right);
    character[16] = itos( plyr.frontheight);
    character[17] = itos( plyr.leftheight);
    character[18] = itos( plyr.rightheight);
    character[19] = itos( plyr.floorTexture);
    character[20] = itos( plyr.ceiling);
    character[21] = itos( plyr.location);
    character[22] = itos( plyr.special);
    character[23] = itos( plyr.alive);
    character[24] = itos( plyr.teleporting);
    character[25] = itos( plyr.buffer_index);
    character[26] = itos( plyr.infoPanel);
    character[27] = itos( plyr.priWeapon);
    character[28] = itos( plyr.secWeapon);
    character[29] = itos( plyr.headArmour);
    character[30] = itos( plyr.bodyArmour);
    character[31] = itos( plyr.legsArmour);
    character[32] = itos( plyr.armsArmour);
    character[33] = itos( plyr.timeOfDay);
    character[34] = itos( plyr.minutes);
    character[35] = itos( plyr.hours);
    character[36] = itos( plyr.days);
    character[37] = itos( plyr.months);
    character[38] = itos( plyr.years);

    character[39] = itos( plyr.sta);
      character[40] = itos( plyr.chr);
      character[41] = itos( plyr.str );
      character[42] = itos( plyr.inte);
      character[43] = itos( plyr.wis);
      character[44] = itos( plyr.skl);
      character[45] = itos( plyr.maxhp);
      character[46] = itos( plyr.hp);
      character[47] = itos( plyr.xp);
      character[48] = itos( plyr.level); // xp level
      character[49] = itos( plyr.chrPartials);
      character[50] = itos( plyr.intPartials);
      character[51] = itos( plyr.strPartials);
      character[52] = itos( plyr.speed);
     character[53] = itos( plyr.stealth);
      character[54] = itos( plyr.diagOn);
      character[55] = itos( plyr.mapOn);
     character[56] = itos( plyr.fpsOn );
      character[57] = itos( plyr.miniMapOn);
      character[58] = itos( plyr.silver);
      character[59] = itos( plyr.gold);
      character[60] = itos( plyr.copper);
      character[61] = itos( plyr.food);
      character[62] = itos( plyr.torches);
      character[63] = itos( plyr.water);
      character[64] = itos( plyr.timepieces);
      character[65] = itos( plyr.crystals);
      character[66] = itos( plyr.jewels);
     character[67] = itos( plyr.gems );
     character[68] = itos( plyr.compasses );
      character[69] = itos(plyr.keys);

      character[70] = itos( plyr.encounter_done);
      character[71] = itos( plyr.game_on);
      character[72] = itos( plyr.gender);

      character[73] = itos( plyr.zone);
      character[74] = itos( plyr.zoneSet);
      character[75] = itos( plyr.current_zone); // used by drawing function
      character[76] = itos( plyr.status);
      character[77] = itos( plyr.specialwall);
//					  character[78] = itos( plyr.windowSize);
//					  character[79] = itos( plyr.graphicStyle);
      character[80] = itos( plyr.fixedEncounter);
      character[81] = itos( plyr.fixedEncounterRef);
      character[82] = itos( plyr.thirst);
      character[83] = itos( plyr.hunger);
     character[84] = itos( plyr.digestion );
      character[85] = itos( plyr.alcohol);

    for(int y = 0; y < 12; ++y) { character[86+y] = itos(plyr.guildAwards[y]); }
    for(int y = 0; y < 32; ++y) { character[98+y] = itos(plyr.fixedEncounters[y]); }
    for(int y = 0; y < 14; ++y) { character[130+y] = itos(plyr.guildMemberships[y]); }

      character[144] = itos(plyr.ringCharges);
      character[145] = itos(plyr.alignment);
      character[146]= itos(plyr.lfood);
      character[147]= itos(plyr.lwater);
      character[148]= itos(plyr.ltorches);
      character[149]= itos(plyr.ltimepieces);
      character[150]= itos(plyr.lcompasses);
      character[151]= itos(plyr.lkeys);
      character[152]= itos(plyr.lcrystals);
      character[153]= itos(plyr.lgems);
      character[154]= itos(plyr.ljewels);
      character[155]= itos(plyr.lgold);
      character[156]= itos(plyr.lsilver);
      character[157]= itos(plyr.lcopper);
      character[158]= itos(plyr.spellIndex);
      character[159]= itos(plyr.effectIndex);
      character[160]= itos(plyr.retreatFriendship);
      character[161]= itos(plyr.damonFriendship);

      character[162]= itos(plyr.smithyFriendships[0]);
      character[163]= itos(plyr.smithyFriendships[1]);
      character[164]= itos(plyr.smithyFriendships[2]);
      character[165]= itos(plyr.smithyFriendships[3]);

        character[166]= itos(plyr.bankAccountStatuses[0]);
        character[167]= itos(plyr.bankAccountStatuses[1]);
        character[168]= itos(plyr.bankAccountStatuses[2]);
        character[169]= itos(plyr.bankAccountStatuses[3]);
        character[170]= itos(plyr.bankAccountStatuses[4]);
        character[171]= itos(plyr.bankAccountStatuses[5]);
        character[172]= itos(plyr.bankAccountStatuses[6]);
        character[173]= itos(plyr.bankAccountStatuses[7]);
        character[174]= itos(plyr.bankAccountStatuses[8]);

        character[175]= itos(plyr.bankAccountBalances[0]);
        character[176]= itos(plyr.bankAccountBalances[1]);
        character[177]= itos(plyr.bankAccountBalances[2]);
        character[178]= itos(plyr.bankAccountBalances[3]);
        character[179]= itos(plyr.bankAccountBalances[4]);
        character[180]= itos(plyr.bankAccountBalances[5]);
        character[181]= itos(plyr.bankAccountBalances[6]);
        character[182]= itos(plyr.bankAccountBalances[7]);
        character[183]= itos(plyr.bankAccountBalances[8]);

        character[184] = itos( plyr.clothing[0]);
        character[185] = itos( plyr.clothing[1]);
        character[186] = itos( plyr.clothing[2]);
        character[187] = itos( plyr.clothing[3]);

        character[188] = itos( plyr.goblinsVisited);
        character[189] = itos( plyr.goblinsChallenged);
        character[190] = itos( plyr.goblinsDefeated);
        character[191] = itos( plyr.goblinsCombat);
        character[192] = itos( plyr.goblinsReforged);
        character[193] = itos( plyr.trollsVisited);
        character[194] = itos( plyr.trollsChallenged);
        character[195] = itos( plyr.trollsDefeated);
        character[196] = itos( plyr.trollsCombat);
        character[197] = itos( plyr.trollsReforged);

        character[198] = itos(  plyr.oracleReturnTomorrow);
        character[199] = itos(  plyr.oracleDay);
        character[200] = itos(  plyr.oracleMonth);
        character[201] = itos(  plyr.oracleYear);
        character[202] = itos(  plyr.oracleQuestNo);
        character[203] = itos(  plyr.healerDays[0]);
        character[204] = itos(  plyr.healerDays[1]);
        character[205] = itos(  plyr.healerHours[0]);
        character[206] = itos(  plyr.healerHours[1]);
        character[207] = itos(  plyr.healerMinutes[0]);
        character[208] = itos(  plyr.healerMinutes[1]);
        character[209] = itos(  plyr.treasureFinding);
        character[210] = itos(  plyr.invisibility);
        character[211] = itos( plyr.diseases[0]);
        character[212] = itos( plyr.diseases[1]);
        character[213] = itos( plyr.diseases[2]);
        character[214] = itos( plyr.diseases[3]);
        character[215] = itos( plyr.poison[0]);
        character[216] = itos( plyr.poison[1]);
        character[217] = itos( plyr.poison[2]);
        character[218] = itos( plyr.poison[3]);
        character[219] = itos( plyr.delusion);

        for(int y = 0; y < damageTypes; ++y) { character[220+y] = itos(plyr.invulnerability[y]); }
        character[229] = itos(plyr.noticeability);
        character[230] = itos(plyr.protection1);
        character[231] = itos(plyr.protection2);

        character[232] = itos(plyr.forgeDays);
        character[233] = itos(plyr.forgeType);
        character[234] = itos(plyr.forgeBonus);
        character[235] = plyr.forgeName;
        character[236] = itos(plyr.stolenFromVault);

    for(int y = 0; y < 14; ++y) 
	 { 
	 	character[237+y] = itos(plyr.guildAccepting[y]); 
	 }


		character[399] = "Line 400: Item Buffer follows";

 // Copy item buffer

    int saveGameIndex = 400; // start location for object buffer items
    for(int z = 0; z < itemBufferSize; ++z)
    {
        character[saveGameIndex] = itos(itemBuffer[z].hp);
        character[saveGameIndex+1] = itos(itemBuffer[z].index);
        character[saveGameIndex+2] = itos(itemBuffer[z].level);
        character[saveGameIndex+3] = itos(itemBuffer[z].location);
        character[saveGameIndex+4] = itos(itemBuffer[z].type);
        character[saveGameIndex+5] = itos(itemBuffer[z].x);
        character[saveGameIndex+6] = itos(itemBuffer[z].y);

		character[saveGameIndex+7] = itemBuffer[z].name;
		character[saveGameIndex+8] = itos(itemBuffer[z].maxHP);
		character[saveGameIndex+9] = itos(itemBuffer[z].flags);
		character[saveGameIndex+10] = itos(itemBuffer[z].minStrength);
		character[saveGameIndex+11] = itos(itemBuffer[z].minDexterity);
		character[saveGameIndex+12] = itos(itemBuffer[z].useStrength);
		character[saveGameIndex+13] = itos(itemBuffer[z].blunt);
		character[saveGameIndex+14] = itos(itemBuffer[z].sharp);
		character[saveGameIndex+15] = itos(itemBuffer[z].earth);
		character[saveGameIndex+16] = itos(itemBuffer[z].air);
		character[saveGameIndex+17] = itos(itemBuffer[z].fire);
		character[saveGameIndex+18] = itos(itemBuffer[z].water);
		character[saveGameIndex+19] = itos(itemBuffer[z].power);
		character[saveGameIndex+20] = itos(itemBuffer[z].magic); // mental
		character[saveGameIndex+21] = itos(itemBuffer[z].good); // cleric
		character[saveGameIndex+22] = itos(itemBuffer[z].evil);
		character[saveGameIndex+23] = itos(itemBuffer[z].cold);
		character[saveGameIndex+24] = itos(itemBuffer[z].weight);
		character[saveGameIndex+25] = itos(itemBuffer[z].alignment);
		character[saveGameIndex+26] = itos(itemBuffer[z].melee);
		character[saveGameIndex+27] = itos(itemBuffer[z].ammo);
		character[saveGameIndex+28] = itos(itemBuffer[z].parry);

        saveGameIndex = saveGameIndex + 28;
    }
//cout << saveGameIndex << "\n";


    // Copy spell buffer
    saveGameIndex = 7400; // start location for spell buffer items (70 bytes)
    for(int z = 0; z < 35; ++z)
    {
        character[saveGameIndex] = itos(spellBuffer[z].no);
        character[saveGameIndex+1] = itos(spellBuffer[z].percentage);
        saveGameIndex = saveGameIndex + 2;
    }
//cout << saveGameIndex << "\n";
    // Copy effect buffer
    saveGameIndex = 7470; // start location for effect buffer items (200 bytes)
    for(int z = 0; z < 50; ++z)
    {
        character[saveGameIndex] = itos(effectBuffer[z].effectNo);
        character[saveGameIndex+1] = itos(effectBuffer[z].negativeValue);
        character[saveGameIndex+2] = itos(effectBuffer[z].positiveValue);
        character[saveGameIndex+3] = itos(effectBuffer[z].duration);
        saveGameIndex = saveGameIndex + 4;
    }
//cout << saveGameIndex << "\n";
    // Smithy daily wares - int smithyDailyWares[4][10];
    saveGameIndex = 7670; // start location for object buffer items
    for(int z = 0; z < 4; ++z)
    {
        for(int x = 0; x < 10; ++x)
        {
            character[saveGameIndex] = itos(smithyDailyWares[z][x]);
            saveGameIndex++;
        }
    }
//cout << saveGameIndex << "\n";
    //int tavernDailyFoods[14][6];
    saveGameIndex = 7710; // start location for object buffer items
    for(int z = 0; z < 14; ++z)
    {
        for(int x = 0; x < 6; ++x)
        {
            character[saveGameIndex] = itos(tavernDailyFoods[z][x]);
            saveGameIndex++;
        }
    }
//cout << saveGameIndex << "\n";
    //int tavernDailyDrinks[14][6];
    saveGameIndex = 7794; // start location for object buffer items
    for(int z = 0; z < 14; ++z)
    {
        for(int x = 0; x < 6; ++x)
        {
            character[saveGameIndex] = itos(tavernDailyDrinks[z][x]);
            saveGameIndex++;
        }
    }
//cout << saveGameIndex << "\n";
    // int shopDailyWares[15][12];
    saveGameIndex = 7878; // start location for object buffer items
    for(int z = 0; z < 15; ++z)
    {
        for(int x = 0; x < 12; ++x)
        {
            character[saveGameIndex] = itos(shopDailyWares[z][x]);
            saveGameIndex++;
        }
    }

    // Currently inn and tavern job openings are not part of saved game
//cout << saveGameIndex << "\n";
    //bool autoMapExplored[5][4096];
    saveGameIndex = 8058; // start location for automapexplored
    for(int z = 0; z < 5; ++z)
    {
        for(int x = 0; x < 4096; ++x)
        {
            character[saveGameIndex] = itos(autoMapExplored[z][x]);
            saveGameIndex++;
        }
    }
//cout << saveGameIndex << "\n";

character[28538] = plyr.name;
character[28539] = ftos(plyr.z_offset);
character[28540] = "Release 0.80";

    string filename = "data/saves/save"+itos(saveSlot)+".txt";
    outdata.open(filename.c_str()); // opens the file
    if( !outdata )
    {
        cerr << "Error: character file could not be saved" << endl;
    }


    for(int y = 0; y < saveGameSize; ++y)
    {
        outdata << character[y] << endl;
    }


   // outdata << plyr.name << endl;
   // outdata << plyr.z_offset << endl;

    outdata.close();
    return (1); // Added by me
}






std::string itos(int i)	// convert int to string
{
	std::stringstream s;
	s << i;
	return s.str();
}


std::string ftos(float i)	// convert float to string
{
	std::stringstream s;
	s << i;
	return s.str();
}



