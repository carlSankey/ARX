//#include <SFML/Graphics.hpp>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>



#include "constants.h"
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


enum PlayerDataFields {
    gender,
    hp,
    maxhp,
    scenario,
    map,
    mapWidth,
    mapHeight,
    x,
    oldx,
    y,
    oldy,
    facing,
    front,
    back,
    left,
    right,
    frontheight,
    leftheight,
    rightheight,
    floorTexture,
    ceiling,
    location,
    special,
    alive,
    teleporting,
    buffer_index,
    infoPanel,
    priWeapon,
    secWeapon,
    headArmour,
    bodyArmour,
    legsArmour,
    armsArmour,
    timeOfDay,
    minutes,
    hours,
    days,
    months,
    years,
    sta,
    chr,
    str,
    inte,
    wis,
    skl,
    xp,
    level, // xp level
    chrPartials,
    intPartials,
    strPartials,
    speed,
    stealth,
    diagOn,
    mapOn,
    fpsOn,
    miniMapOn,
    silver,
    gold,
    copper,
    food,
    torches,
    water,
    timepieces,
    crystals,
    jewels,
    gems,
    compasses,
    keys,
    encounter_done,
    game_on,
    zone,
    zoneSet,
    current_zone, // used by drawing function
    status,
    specialwall,
    fixedEncounter,
    fixedEncounterRef,
    thirst,
    hunger,
    digestion,
    alcohol,
    base_guildAwards, //Contains 12 elements
    base_fixedEncounters = base_guildAwards + noOfguildAwards,  //Contains 32 elements
    base_guildMemberships = base_fixedEncounters + noOfFixedEncounters, //Contains 14 elments
    ringCharges = base_guildMemberships + noOfGuilds,
    alignment,
    lfood,
    lwater,
    ltorches,
    ltimepieces,
    lcompasses,
    lkeys,
    lcrystals,
    lgems,
    ljewels,
    lgold,
    lsilver,
    lcopper,
    spellIndex,
    effectIndex,
    retreatFriendship,
    damonFriendship,
    base_smithyFriendships, //Contains 4 elements
    base_bankAccountStatuses = base_smithyFriendships + noOfSmithyFriendships, //Contains 9 elements
    base_bankAccountBalances = base_bankAccountStatuses + noOfBankAccountStatuses, //Contains 9 elements
    base_clothing = base_bankAccountBalances + noOfBankAccountBalances, //Contains 4 elements
    goblinsVisited = base_clothing + noOfClothing,
    goblinsChallenged,
    goblinsDefeated,
    goblinsCombat,
    goblinsReforged,
    trollsVisited,
    trollsChallenged,
    trollsDefeated,
    trollsCombat,
    trollsReforged,
    oracleReturnTomorrow,
    oracleDay,
    oracleMonth,
    oracleYear,
    oracleQuestNo,
    base_healerDays, //Contains 2 elements
    base_healerHours = base_healerDays + noOfHealerDays, //Contains 2 elements
    base_healerMinutes = base_healerHours + noOfHealerHours, //Contains 2 elements
    treasureFinding = base_healerMinutes + noOfHealerMinutes,
    invisibility,
    base_diseases, //Contains 4  elements
    base_poison = base_diseases + noOfDiseases, //Contains 4 elements
    delusion = base_poison + noOfPoison,
    base_noOfElements, //Contains 13 elements
    noticeability = base_noOfElements + noOfElements,
    protection1,
    protection2,
    forgeDays,
    forgeType,
    forgeBonus,
    forgeName,
    stolenFromVault,
    base_guildAccepting, //Contains 14 elements
    light = base_guildAccepting + noOfGuilds,
    supervision,
    fatigue,
    fatigueRate,
    spell_index,
    temperature,
    hungerRate,
    thirstRate,
    items_index,
    luck,
    TemporalAdjustment,
    base_fixedTreasures, //Contains 64 elements
    itemBufferStart = 400, // "Item Buffer follows";
    base_itemBufferhp,  //Contains 250 elements
    base_itemBufferindex = base_itemBufferhp + noOfItemBuffer, //Contains 250 elements
    base_itemBufferlevel = base_itemBufferindex + noOfItemBuffer, //Contains 250 elements
    base_itemBufferlocation = base_itemBufferlevel + noOfItemBuffer,
    base_itemBuffertype = base_itemBufferlocation + noOfItemBuffer,
    base_itemBufferx = base_itemBuffertype + noOfItemBuffer,
    base_itemBuffery = base_itemBufferx + noOfItemBuffer,
    base_itemBuffername = base_itemBuffery + noOfItemBuffer,
    base_itemBuffermaxHP = base_itemBuffername + noOfItemBuffer,
    base_itemBufferflags = base_itemBuffermaxHP + noOfItemBuffer,
    base_itemBufferminStrength = base_itemBufferflags + noOfItemBuffer,
    base_itemBufferminDexterity = base_itemBufferminStrength + noOfItemBuffer,
    base_itemBufferuseStrength = base_itemBufferminDexterity + noOfItemBuffer,
    base_itemBufferblunt = base_itemBufferuseStrength + noOfItemBuffer,
    base_itemBuffersharp = base_itemBufferblunt + noOfItemBuffer,
    base_itemBufferearth = base_itemBuffersharp + noOfItemBuffer,
    base_itemBufferair = base_itemBufferearth + noOfItemBuffer,
    base_itemBufferfire = base_itemBufferair + noOfItemBuffer,
    base_itemBufferwater = base_itemBufferfire + noOfItemBuffer,
    base_itemBufferpower = base_itemBufferwater + noOfItemBuffer,
    base_itemBuffermagic = base_itemBufferpower + noOfItemBuffer, // mental
    base_itemBuffergood = base_itemBuffermagic + noOfItemBuffer, // cleric
    base_itemBufferevil = base_itemBuffergood + noOfItemBuffer,
    base_itemBuffercold = base_itemBufferevil + noOfItemBuffer,
    base_itemBufferacid = base_itemBuffercold + noOfItemBuffer,
    base_itemBuffernature = base_itemBufferacid + noOfItemBuffer,
    base_itemBufferweight = base_itemBuffernature + noOfItemBuffer,
    base_itemBufferalignment = base_itemBufferweight + noOfItemBuffer,
    base_itemBuffermelee = base_itemBufferalignment + noOfItemBuffer,
    base_itemBufferammo = base_itemBuffermelee + noOfItemBuffer,
    base_itemBufferparry = base_itemBufferammo + noOfItemBuffer,
    base_itemBuffercat = base_itemBufferparry + noOfItemBuffer,
    base_itemBufferiLevel = base_itemBuffercat + noOfItemBuffer,
    base_itemBufferbuffType = base_itemBufferiLevel + noOfItemBuffer,
    spellBufferStart = 9000,
    base_spellBufferno,  //Contains 35 elements
    base_spellBufferpercentage = base_spellBufferno + noOfSpellBuffer, //Contains 35 elements
    effectBufferStart = 9100,
    base_effectBuffereffectNo, //Contains 50 elements
    base_effectBuffernegativeValue = base_effectBuffereffectNo + noOfEffects,
    base_effectBufferpositiveValue = base_effectBuffernegativeValue + noOfEffects,
    base_effectBufferduration = base_effectBufferpositiveValue + noOfEffects,
    smithyDailyWaresStart = 9300,
    base_smithyDailyWares1,
    base_smithyDailyWares2 = base_smithyDailyWares1 + noOfSmithDailyWares, //contains 10 elements
    base_smithyDailyWares3 = base_smithyDailyWares2 + noOfSmithDailyWares,
    base_smithyDailyWares4 = base_smithyDailyWares3 + noOfSmithDailyWares,
    tavernDailyFoodsStart = 9400,
    savetavernDailyFoods,  //Move to Random
    savetavernDailyDrinks, //Move to Random
    saveshopDailyWares, //Moved to Random
    saveGameIndex = 9600, // start location for automapexplored
    // Currently inn and tavern job openings are not part of saved game
    base_autoMapExplored1,
    base_autoMapExplored2 = base_autoMapExplored1 + noOfAutoMapExplored,
    base_autoMapExplored3 = base_autoMapExplored2 + noOfAutoMapExplored,
    base_autoMapExplored4 = base_autoMapExplored3 + noOfAutoMapExplored,
    base_autoMapExplored5 = base_autoMapExplored4 + noOfAutoMapExplored,
    name = base_autoMapExplored5 + noOfAutoMapExplored,
    z_offset,
    progrelease,
    FIELD_COUNT // This should be the last element to automatically get the count of fields
};



// NOTES:
// job openings not currently part of saved game

string character[noOfSaveGame];
string saveGameDescriptions[10];


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
	for (int i=0 ; i<noOfSaveGame ; i++)
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
                    newsaveCharacter(savegameMenu,plyr);
					plyr.status=1; // for display canvas
					savegameMenu = 256;
				}
			}

		}

}


/*
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

	for (int a = 0; a < noOfSaveGame; ++a)
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
for(int y = 0; y < noOfElements; ++y) { plyr.invulnerability[y] = atoi(character[220+y].c_str()); }
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


 for (int y = 0; y < 64; ++y)
 {
     plyr.fixedTreasures[y] = atoi(character[251 + y].c_str());
 }

 plyr.light = atoi(character[251].c_str());
 plyr.supervision = atoi(character[252].c_str());
 plyr.fatigue = atoi(character[253].c_str());
 plyr.fatigueRate = atoi(character[254].c_str());
 plyr.spell_index = atoi(character[255].c_str());
 plyr.temperature = atoi(character[256].c_str());
 plyr.hungerRate = atoi(character[257].c_str());
 plyr.thirstRate = atoi(character[258].c_str());
 plyr.items_index = atoi(character[259].c_str());
 plyr.luck = atoi(character[260].c_str());
 plyr.TemporalAdjustment = atoi(character[261].c_str());
 int PrisonRelease;

int loadGameIndex = 400; // start location for object buffer items
for(int z = 0; z < noOfItemBuffer; ++z)
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
    itemBuffer[z].cat = atoi(character[loadGameIndex + 29].c_str());
    itemBuffer[z].iLevel = atoi(character[loadGameIndex + 30].c_str());
    itemBuffer[z].buffType = atoi(character[loadGameIndex + 31].c_str());

	loadGameIndex = loadGameIndex + 32;
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
*/

bool newloadCharacter(int saveSlot, Player& plyr) {
    string filename = "data/saves/save" + to_string(saveSlot) + ".txt";
    ifstream instream(filename.c_str());

    if (!instream) {
        cerr << "No save game in this slot" << endl;
        return false;
    }

    string text;
    volatile int linesRead = 0; //Remove volatile after debugging
    volatile int linesRead100s = 0;
    // Load player data
    getline(instream, text); plyr.gender = stoi(text); linesRead++;
    getline(instream, text); plyr.hp = stoi(text); linesRead++;
    getline(instream, text); plyr.maxhp = stoi(text); linesRead++;
    getline(instream, text); plyr.scenario = stoi(text); linesRead++;
    getline(instream, text); plyr.map = stoi(text); linesRead++;
    getline(instream, text); plyr.mapWidth = stoi(text); linesRead++;
    getline(instream, text); plyr.mapHeight = stoi(text); linesRead++;
    getline(instream, text); plyr.x = stoi(text); linesRead++;
    getline(instream, text); plyr.oldx = stoi(text); linesRead++;
    getline(instream, text); plyr.y = stoi(text); linesRead++;
    getline(instream, text); plyr.oldy = stoi(text); linesRead++;
    getline(instream, text); plyr.facing = stoi(text); linesRead++;
    getline(instream, text); plyr.front = stoi(text); linesRead++;
    getline(instream, text); plyr.back = stoi(text); linesRead++;
    getline(instream, text); plyr.left = stoi(text); linesRead++;
    getline(instream, text); plyr.right = stoi(text); linesRead++;
    getline(instream, text); plyr.frontheight = stoi(text); linesRead++;
    getline(instream, text); plyr.leftheight = stoi(text); linesRead++;
    getline(instream, text); plyr.rightheight = stoi(text); linesRead++;
    getline(instream, text); plyr.floorTexture = stoi(text); linesRead++;
    getline(instream, text); plyr.ceiling = stoi(text); linesRead++;
    getline(instream, text); plyr.location = stoi(text); linesRead++;
    getline(instream, text); plyr.special = stoi(text); linesRead++;
    getline(instream, text); plyr.alive = stringToBool( text); linesRead++;
    getline(instream, text); plyr.teleporting = stoi(text); linesRead++;
    getline(instream, text); plyr.buffer_index = stoi(text); linesRead++;
    getline(instream, text); plyr.infoPanel = stoi(text); linesRead++;
    getline(instream, text); plyr.priWeapon = stoi(text); linesRead++;
    getline(instream, text); plyr.secWeapon = stoi(text); linesRead++;
    getline(instream, text); plyr.headArmour = stoi(text); linesRead++;
    getline(instream, text); plyr.bodyArmour = stoi(text); linesRead++;
    getline(instream, text); plyr.legsArmour = stoi(text); linesRead++;
    getline(instream, text); plyr.armsArmour = stoi(text); linesRead++;
    getline(instream, text); plyr.timeOfDay = stoi(text); linesRead++;
    getline(instream, text); plyr.minutes = stoi(text); linesRead++;
    getline(instream, text); plyr.hours = stoi(text); linesRead++;
    getline(instream, text); plyr.days = stoi(text); linesRead++;
    getline(instream, text); plyr.months = stoi(text); linesRead++;
    getline(instream, text); plyr.years = stoi(text); linesRead++;
    getline(instream, text); plyr.sta = stoi(text); linesRead++;
    getline(instream, text); plyr.chr = stoi(text); linesRead++;
    getline(instream, text); plyr.str = stoi(text); linesRead++;
    getline(instream, text); plyr.inte = stoi(text); linesRead++;
    getline(instream, text); plyr.wis = stoi(text); linesRead++;
    getline(instream, text); plyr.skl = stoi(text); linesRead++;
    
    getline(instream, text); linesRead++;
    try {
        plyr.xp = std::stoi(text);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument exception caught: " << e.what() << std::endl;
        plyr.xp = 0; // or another default value
    }
    linesRead++;

    getline(instream, text); plyr.level = stoi(text); linesRead++;// xp leve

    getline(instream, text); linesRead++;
    try {
        plyr.chrPartials = std::stoi(text);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument exception caught: " << e.what() << std::endl;
        plyr.chrPartials = 0; // or another default value
    }
    linesRead++;

    getline(instream, text); 
    try {
        plyr.intPartials = std::stoi(text); 
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument exception caught: " << e.what() << std::endl;
        plyr.intPartials = 0; // or another default value
    }
    linesRead++;

    getline(instream, text);
    try {
        plyr.strPartials = std::stoi(text); 
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument exception caught: " << e.what() << std::endl;
        plyr.strPartials = 0; // or another default value
    }
    linesRead++;

    getline(instream, text); 
    try {
        plyr.speed = std::stoi(text);
    }
    catch (const std::invalid_argument& e) {
        // Use e to handle the exception or its details
        std::cerr << "Invalid argument exception caught: " << e.what() << std::endl;
        plyr.speed = 0; // or another default value
    }

    linesRead++;

    getline(instream, text); plyr.stealth = stoi(text); linesRead++;
    getline(instream, text); plyr.diagOn = stringToBool(text); linesRead++;  //52
    getline(instream, text); plyr.mapOn = stringToBool(text); linesRead++;
    getline(instream, text); plyr.fpsOn = stringToBool(text); linesRead++;
    getline(instream, text); plyr.miniMapOn = stringToBool(text); linesRead++;
    getline(instream, text); plyr.silver = stoi(text); linesRead++;
    getline(instream, text); plyr.gold = stoi(text); linesRead++;
    getline(instream, text); plyr.copper = stoi(text); linesRead++;
    getline(instream, text); plyr.food = stoi(text); linesRead++;
    getline(instream, text); plyr.torches = stoi(text); linesRead++;
    getline(instream, text); plyr.water = stoi(text); linesRead++;
    getline(instream, text); plyr.timepieces = stoi(text); linesRead++;
    getline(instream, text); plyr.crystals = stoi(text); linesRead++;
    getline(instream, text); plyr.jewels = stoi(text); linesRead++;
    getline(instream, text); plyr.gems = stoi(text); linesRead++;
    getline(instream, text); plyr.compasses = stoi(text); linesRead++;
    getline(instream, text); plyr.keys = stoi(text); linesRead++;
    getline(instream, text); plyr.encounter_done = stringToBool(text); linesRead++;
    getline(instream, text); plyr.game_on = stringToBool(text); linesRead++;
    getline(instream, text); plyr.zone = stoi(text); linesRead++;
    getline(instream, text); plyr.zoneSet = stoi(text); linesRead++;
    getline(instream, text); plyr.current_zone = stoi(text); linesRead++;// used by drawing function
    getline(instream, text); plyr.status = stoi(text); linesRead++;
    getline(instream, text); plyr.specialwall = stoi(text); linesRead++;
    getline(instream, text); plyr.fixedEncounter = stringToBool(text); linesRead++;
    getline(instream, text); plyr.fixedEncounterRef = stoi(text); linesRead++;
    getline(instream, text); plyr.thirst = stoi(text); linesRead++;
    getline(instream, text); plyr.hunger = stoi(text); linesRead++;
    getline(instream, text); plyr.digestion = stoi(text); linesRead++;
    getline(instream, text); plyr.alcohol = stoi(text); linesRead++;


    for (size_t i = 0; i < noOfguildAwards; ++i) {
        getline(instream, text); plyr.guildAwards[i] = stoi(text); linesRead++;
        // Save other item fields...
    }

    for (size_t i = 0; i < noOfFixedEncounters; ++i) {
        getline(instream, text); plyr.fixedEncounters[i] = stoi(text); linesRead++;
        // Save other item fields...
    }

    for (size_t i = 0; i < noOfGuilds; ++i) {
        getline(instream, text); plyr.guildMemberships[i] = stoi(text); linesRead++;
        // Save other item fields...
    }


    getline(instream, text); plyr.ringCharges = stoi(text); linesRead++;
    getline(instream, text); plyr.lwater = stoi(text); linesRead++;
    getline(instream, text); plyr.alignment = stoi(text); linesRead++;
    getline(instream, text); plyr.lfood = stoi(text); linesRead++;
    getline(instream, text); plyr.ltorches = stoi(text); linesRead++;
    getline(instream, text); plyr.ltimepieces = stoi(text); linesRead++;
    getline(instream, text); plyr.lcompasses = stoi(text); linesRead++;
    getline(instream, text); plyr.lkeys = stoi(text); linesRead++;
    getline(instream, text); plyr.lcrystals = stoi(text); linesRead++;
    getline(instream, text); plyr.lgems = stoi(text); linesRead++;
    getline(instream, text); plyr.ljewels = stoi(text); linesRead++;
    getline(instream, text); plyr.lgold = stoi(text); linesRead++;
    getline(instream, text); plyr.lsilver = stoi(text); linesRead++;
    getline(instream, text); plyr.lcopper = stoi(text); linesRead++;
    getline(instream, text); plyr.spellIndex = stoi(text); linesRead++;
    getline(instream, text); plyr.effectIndex = stoi(text); linesRead++;
    getline(instream, text); plyr.retreatFriendship = stoi(text); linesRead++;
    getline(instream, text); plyr.damonFriendship = stoi(text); linesRead++;

    for (size_t i = 0; i < noOfSmithyFriendships; ++i) {
        getline(instream, text); plyr.smithyFriendships[i] = stoi(text); linesRead++;
        // Save other item fields...
    }

    for (size_t i = 0; i < noOfBankAccountStatuses; ++i) {
        getline(instream, text); plyr.bankAccountStatuses[i] = stoi(text); linesRead++;
        // Save other item fields...
    }

    for (size_t i = 0; i < noOfBankAccountBalances; ++i) {
        getline(instream, text); plyr.bankAccountBalances[i] = stoi(text); linesRead++;
        // Save other item fields...
    }

    for (size_t i = 0; i < noOfClothing; ++i) {
        getline(instream, text); plyr.clothing[i] = stoi(text); linesRead++;
        // Save other item fields...
    }



    getline(instream, text); plyr.goblinsVisited = stringToBool(text); linesRead++;
    getline(instream, text); plyr.goblinsChallenged = stringToBool(text); linesRead++;
    getline(instream, text); plyr.goblinsDefeated = stringToBool(text); linesRead++;
    getline(instream, text); plyr.goblinsCombat = stringToBool(text); linesRead++;
    getline(instream, text); plyr.goblinsReforged = stringToBool(text); linesRead++;
    getline(instream, text); plyr.trollsVisited = stringToBool(text); linesRead++;
    getline(instream, text); plyr.trollsChallenged = stringToBool(text); linesRead++;
    getline(instream, text); plyr.trollsDefeated = stringToBool(text); linesRead++;
    getline(instream, text); plyr.trollsCombat = stringToBool(text); linesRead++;
    getline(instream, text); plyr.trollsReforged = stringToBool(text); linesRead++;
    getline(instream, text); plyr.oracleReturnTomorrow = stringToBool(text); linesRead++;
    getline(instream, text); plyr.oracleDay = stoi(text); linesRead++;
    getline(instream, text); plyr.oracleMonth = stoi(text); linesRead++;
    getline(instream, text); plyr.oracleYear = stoi(text); linesRead++;
    getline(instream, text); plyr.oracleQuestNo = stoi(text); linesRead++;

    for (size_t i = 0; i < noOfHealerDays; ++i) {
        getline(instream, text); plyr.healerDays[i] = stoi(text); linesRead++;
        // Save other item fields...
    }

    for (size_t i = 0; i < noOfHealerHours; ++i) {
        getline(instream, text); plyr.healerHours[i] = stoi(text); linesRead++;
        // Save other item fields...
    }

    for (size_t i = 0; i < noOfHealerMinutes; ++i) {
        getline(instream, text); plyr.healerMinutes[i] = stoi(text); linesRead++;
        // Save other item fields...
    }




    getline(instream, text); plyr.treasureFinding = stoi(text); linesRead++;
    getline(instream, text); plyr.invisibility = stoi(text); linesRead++;

    for (size_t i = 0; i < noOfDiseases; ++i) {
        getline(instream, text); plyr.diseases[i] = stoi(text); linesRead++;
        // Save other item fields...
    }

    for (size_t i = 0; i < noOfPoison; ++i) {
        getline(instream, text); plyr.poison[i] = stoi(text); linesRead++;
        // Save other item fields...
    }



    getline(instream, text); plyr.delusion = stoi(text); linesRead++;

    for (size_t i = 0; i < noOfElements; ++i) {
        getline(instream, text); plyr.invulnerability[i] = stoi(text); linesRead++;
        // Save other item fields...
    }

    getline(instream, text); plyr.noticeability = stoi(text); linesRead++;
    getline(instream, text); plyr.protection1 = stoi(text); linesRead++;
    getline(instream, text); plyr.protection2 = stoi(text); linesRead++;
    getline(instream, text); plyr.forgeDays = stoi(text); linesRead++;
    getline(instream, text); plyr.forgeType = stoi(text); linesRead++;
    getline(instream, text); plyr.forgeBonus = stoi(text); linesRead++;
    getline(instream, text); plyr.forgeName = text; linesRead++;
    getline(instream, text); plyr.stolenFromVault = stoi(text); linesRead++;

    for (size_t i = 0; i < noOfGuilds; ++i) {
        getline(instream, text); plyr.guildAccepting[i] = stringToBool(text); linesRead++;
        // Save other item fields...
    }
    linesRead100s = linesRead;

    getline(instream, text); plyr.light = stoi(text); linesRead++;
    getline(instream, text); plyr.supervision = stoi(text); linesRead++;
    getline(instream, text); plyr.fatigue = stof(text); linesRead++;
    getline(instream, text); plyr.fatigueRate = stof(text); linesRead++;
    getline(instream, text); plyr.spell_index = stoi(text); linesRead++;
    getline(instream, text); plyr.temperature = stoi(text); linesRead++;
    getline(instream, text); plyr.hungerRate = stof(text); linesRead++;
    getline(instream, text); plyr.thirstRate = stof(text); linesRead++;
    getline(instream, text); plyr.items_index = stoi(text); linesRead++;
    getline(instream, text); plyr.luck = stoi(text); linesRead++;
    getline(instream, text); plyr.TemporalAdjustment = stoi(text); linesRead++;  //261

    for (size_t i = 0; i < noOfFixedTreasures; ++i) {
        getline(instream, text); plyr.fixedTreasures[i] = stringToBool(text); linesRead++;
        // Save other item fields...
    }

    
    for (int i = linesRead; i < itemBufferStart+2; ++i) {
        getline(instream, text); linesRead++;
    }
    getline(instream, text); linesRead++;
    if (text != "Item Buffer follows") {
        cerr << "Error: Expected 'Item Buffer follows' at line 400" << endl;
        return false;
    }



    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].hp = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].index = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].level = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].location = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].type = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].x = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].y = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].name = text; linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].maxHP = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].flags = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].minStrength = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].minDexterity = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].useStrength = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].blunt = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].sharp = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].earth = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].air = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].fire = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].water = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].power = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].magic = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].good = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].evil = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].cold = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].acid = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].nature = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].weight = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].alignment = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].melee = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].ammo = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].parry = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].cat = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].iLevel = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfItemBuffer; ++i) {
        getline(instream, text); itemBuffer[i].buffType = stoi(text); linesRead++;
        // Save other item fields...
    }

    for (int i = linesRead; i < spellBufferStart+3; ++i) {
        getline(instream, text); linesRead++;
    }
    getline(instream, text); linesRead++;
    if (text != "Spell Buffer follows") {
        cerr << "Error: Expected 'Item Buffer follows' at line 400" << endl;
        return false;
    }

    for (size_t i = 0; i < noOfSpellBuffer; ++i) {
        getline(instream, text); spellBuffer[i].no = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfSpellBuffer; ++i) {
        getline(instream, text); spellBuffer[i].percentage = stoi(text); linesRead++;
        // Save other item fields...
    }

    for (int i = linesRead; i < effectBufferStart+3; ++i) {
        getline(instream, text); linesRead++;
    }
    getline(instream, text); linesRead++;
    if (text != "Effect Buffer follows") {
        cerr << "Error: Expected 'Item Buffer follows' at line 400" << endl;
        return false;
    }
    
    for (size_t i = 0; i < noOfSpellBuffer; ++i) {
        getline(instream, text); effectBuffer[i].effectNo = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfSpellBuffer; ++i) {
        getline(instream, text); effectBuffer[i].negativeValue = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfSpellBuffer; ++i) {
        getline(instream, text); effectBuffer[i].positiveValue = stoi(text); linesRead++;
        // Save other item fields...
    }
    for (size_t i = 0; i < noOfSpellBuffer; ++i) {
        getline(instream, text); effectBuffer[i].duration = stof(text); linesRead++;
        // Save other item fields...
    }

    for (int i = linesRead; i < smithyDailyWaresStart+4; ++i) {
        getline(instream, text); linesRead++;
    }
    getline(instream, text); linesRead++;
    if (text != "Smithy Daily Wares follows") {
        cerr << "Error: Expected 'Item Buffer follows' at line 400" << endl;
        return false;
    }
    for (int z = 0; z < noOfSmithyFriendships; ++z)
    {
        for (int x = 0; x < noOfSmithDailyWares; ++x)
        {
            getline(instream, text); itos(smithyDailyWares[z][x]) = stoi(text); linesRead++;
        }
    }

    for (int i = linesRead; i < saveGameIndex+4; ++i) {
        getline(instream, text); linesRead++;
    }
    getline(instream, text); linesRead++;
    if (text != "Automap follows") {
        cerr << "Error: Expected 'Item Buffer follows' at line 400" << endl;
        return false;
    }
    for (int z = 0; z < noOfLevels; ++z)
    {
        for (int x = 0; x < noOfAutoMapExplored; ++x)
        {
            getline(instream, text); itos(autoMapExplored[z][x]) = stringToBool(text); linesRead++;
        }
    }
    getline(instream, text); plyr.name = text; linesRead++;
    getline(instream, text); plyr.z_offset = stof(text); linesRead++;
    
    // Load other player fields...



 
    instream.close();
    plyr.status = 1; // Assuming 1 corresponds to EXPLORE
    return true;
}


/*
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

        for(int y = 0; y < noOfElements; ++y) { character[220+y] = itos(plyr.invulnerability[y]); }
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

    // Version 0.90
    character[251] = itos(plyr.light);
    character[252] = itos(plyr.supervision);
    character[253] = itos(plyr.fatigue);
    character[254] = itos(plyr.fatigueRate);
    character[255] = itos(plyr.spell_index);
    character[256] = itos(plyr.temperature);
    character[257] = itos(plyr.hungerRate);
    character[258] = itos(plyr.thirstRate);
    character[259] = itos(plyr.items_index);
    character[260] = itos(plyr.luck);
    character[261] = itos(plyr.TemporalAdjustment);

 
        for (int y = 0; y < 64; ++y)
        {
            character[300+y] = itos(plyr.fixedTreasures[y]);
        }
	character[399] = "Line 400: Item Buffer follows";

 // Copy item buffer

    int saveGameIndex = 400; // start location for object buffer items
    for(int z = 0; z < noOfItemBuffer; ++z)
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
        character[saveGameIndex+29] = itos(itemBuffer[z].cat);
        character[saveGameIndex+30] = itos(itemBuffer[z].iLevel);
        character[saveGameIndex+31] = itos(itemBuffer[z].buffType);




        saveGameIndex = saveGameIndex + 32;
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
character[28540] = "Release 0.85";

    string filename = "data/saves/save"+itos(saveSlot)+".txt";
    outdata.open(filename.c_str()); // opens the file
    if( !outdata )
    {
        cerr << "Error: character file could not be saved" << endl;
    }


    for(int y = 0; y < noOfSaveGame; ++y)
    {
        outdata << character[y] << endl;
    }


   // outdata << plyr.name << endl;
   // outdata << plyr.z_offset << endl;

    outdata.close();
    return (1); // Added by me
}
*/

bool newsaveCharacter(int saveSlot, const Player& plyr) {


    saveGameDescriptions[saveSlot] = plyr.name;
    updateSaveGameDescriptions();

    initcharacter(); // Clear out string array

    string filename = "data/saves/save" + to_string(saveSlot) + ".txt";
    ofstream outstream(filename.c_str());

    if (!outstream) {
        cerr << "Failed to open save file in this slot" << endl;
        return false;
    }

    int linesWritten = 0;

    // Save player data
    outstream << plyr.gender << endl; linesWritten++;
    outstream << plyr.hp << endl; linesWritten++;
    outstream << plyr.maxhp << endl; linesWritten++;
    outstream << plyr.scenario << endl; linesWritten++;
    outstream << plyr.map << endl; linesWritten++;
    outstream << plyr.mapWidth << endl; linesWritten++;
    outstream << plyr.mapHeight << endl; linesWritten++;
    outstream << plyr.x << endl; linesWritten++;
    outstream << plyr.oldx << endl; linesWritten++;
    outstream << plyr.y << endl; linesWritten++;
    outstream << plyr.oldy << endl; linesWritten++;
    outstream << plyr.facing << endl; linesWritten++;
    outstream << plyr.front << endl; linesWritten++;
    outstream << plyr.back << endl; linesWritten++;
    outstream << plyr.left << endl; linesWritten++;
    outstream << plyr.right << endl; linesWritten++;
    outstream << plyr.frontheight << endl; linesWritten++;
    outstream << plyr.leftheight << endl; linesWritten++;
    outstream << plyr.rightheight << endl; linesWritten++;
    outstream << plyr.floorTexture << endl; linesWritten++;
    outstream << plyr.ceiling << endl; linesWritten++;
    outstream << plyr.location << endl; linesWritten++;
    outstream << plyr.special << endl; linesWritten++;
    outstream << plyr.alive << endl; linesWritten++;
    outstream << plyr.teleporting << endl; linesWritten++;
    outstream << plyr.buffer_index << endl; linesWritten++;
    outstream << plyr.infoPanel << endl; linesWritten++;
    outstream << plyr.priWeapon << endl; linesWritten++;
    outstream << plyr.secWeapon << endl; linesWritten++;
    outstream << plyr.headArmour << endl; linesWritten++;
    outstream << plyr.bodyArmour << endl; linesWritten++;
    outstream << plyr.legsArmour << endl; linesWritten++;
    outstream << plyr.armsArmour << endl; linesWritten++;
    outstream << plyr.timeOfDay << endl; linesWritten++;
    outstream << plyr.minutes << endl; linesWritten++;
    outstream << plyr.hours << endl; linesWritten++;
    outstream << plyr.days << endl; linesWritten++;
    outstream << plyr.months << endl; linesWritten++;
    outstream << plyr.years << endl; linesWritten++;
    outstream << plyr.sta << endl; linesWritten++;
    outstream << plyr.chr << endl; linesWritten++;
    outstream << plyr.str << endl; linesWritten++;
    outstream << plyr.inte << endl; linesWritten++;
    outstream << plyr.wis << endl; linesWritten++;
    outstream << plyr.skl << endl ; linesWritten++;
    outstream << plyr.xp << endl ; linesWritten++;
    outstream << plyr.level << endl; linesWritten++;// xp leve
    outstream << plyr.chrPartials << endl; linesWritten++;
    outstream << plyr.intPartials << endl; linesWritten++;
    outstream << plyr.strPartials << endl; linesWritten++;
    outstream << plyr.speed << endl; linesWritten++;
    outstream << plyr.stealth << endl; linesWritten++;
    outstream << plyr.diagOn << endl; linesWritten++;  //53
    outstream << plyr.mapOn << endl; linesWritten++;
    outstream << plyr.fpsOn << endl; linesWritten++;
    outstream << plyr.miniMapOn << endl; linesWritten++;
    outstream << plyr.silver << endl; linesWritten++;
    outstream << plyr.gold << endl; linesWritten++;
    outstream << plyr.copper << endl; linesWritten++;
    outstream << plyr.food << endl; linesWritten++;
    outstream << plyr.torches << endl; linesWritten++;
    outstream << plyr.water << endl; linesWritten++;
    outstream << plyr.timepieces << endl; linesWritten++;
    outstream << plyr.crystals << endl; linesWritten++;
    outstream << plyr.jewels << endl; linesWritten++;
    outstream << plyr.gems << endl; linesWritten++;
    outstream << plyr.compasses << endl; linesWritten++;
    outstream << plyr.keys << endl; linesWritten++;
    outstream << plyr.encounter_done << endl; linesWritten++;
    outstream << plyr.game_on << endl; linesWritten++;
    outstream << plyr.zone << endl; linesWritten++;
    outstream << plyr.zoneSet << endl; linesWritten++;
    outstream << plyr.current_zone << endl; linesWritten++;// used by drawing function
    outstream << plyr.status << endl; linesWritten++;
    outstream << plyr.specialwall << endl; linesWritten++;
    outstream << plyr.fixedEncounter << endl; linesWritten++;
    outstream << plyr.fixedEncounterRef << endl; linesWritten++;
    outstream << plyr.thirst << endl; linesWritten++;
    outstream << plyr.hunger << endl; linesWritten++;
    outstream << plyr.digestion << endl; linesWritten++;
    outstream << plyr.alcohol << endl; linesWritten++;

        for (size_t i = 0; i < noOfguildAwards; ++i) {
            outstream << plyr.guildAwards[i] << endl; linesWritten++;
            // Save other item fields...
        }

        for (size_t i = 0; i < noOfFixedEncounters; ++i) {
            outstream << plyr.fixedEncounters[i] << endl; linesWritten++;
            // Save other item fields...
        }

        for (size_t i = 0; i < noOfGuilds; ++i) {
            outstream << plyr.guildMemberships[i] << endl; linesWritten++;
            // Save other item fields...
        }


        outstream << plyr.ringCharges << endl; linesWritten++;
        outstream << plyr.lwater << endl; linesWritten++;
            outstream << plyr.alignment << endl; linesWritten++;
            outstream << plyr.lfood << endl; linesWritten++;
            outstream << plyr.ltorches << endl; linesWritten++;
            outstream << plyr.ltimepieces << endl; linesWritten++;
            outstream << plyr.lcompasses << endl; linesWritten++;
            outstream << plyr.lkeys << endl; linesWritten++;
            outstream << plyr.lcrystals << endl; linesWritten++;
            outstream << plyr.lgems << endl; linesWritten++;
            outstream << plyr.ljewels << endl; linesWritten++;
            outstream << plyr.lgold << endl; linesWritten++;
            outstream << plyr.lsilver << endl; linesWritten++;
            outstream << plyr.lcopper << endl; linesWritten++;
            outstream << plyr.spellIndex << endl; linesWritten++;
            outstream << plyr.effectIndex << endl; linesWritten++;
            outstream << plyr.retreatFriendship << endl; linesWritten++;
            outstream << plyr.damonFriendship << endl; linesWritten++;

            for (size_t i = 0; i < noOfSmithyFriendships; ++i) {
                outstream << plyr.smithyFriendships[i] << endl; linesWritten++;
                // Save other item fields...
            }
       
            for (size_t i = 0; i < noOfBankAccountStatuses; ++i) {
                outstream << plyr.bankAccountStatuses[i] << endl; linesWritten++;
                // Save other item fields...
            }

            for (size_t i = 0; i < noOfBankAccountBalances; ++i) {
                outstream << plyr.bankAccountBalances[i] << endl; linesWritten++;
                // Save other item fields...
            }

            for (size_t i = 0; i < noOfClothing; ++i) {
                outstream << plyr.clothing[i] << endl; linesWritten++;
                // Save other item fields...
            }
 
 

            outstream << plyr.goblinsVisited << endl; linesWritten++;
                outstream << plyr.goblinsChallenged << endl; linesWritten++;
                outstream << plyr.goblinsDefeated << endl; linesWritten++;
                outstream << plyr.goblinsCombat << endl; linesWritten++;
                outstream << plyr.goblinsReforged << endl; linesWritten++;
                outstream << plyr.trollsVisited << endl; linesWritten++;
                outstream << plyr.trollsChallenged << endl; linesWritten++;
                outstream << plyr.trollsDefeated << endl; linesWritten++;
                outstream << plyr.trollsCombat << endl; linesWritten++;
                outstream << plyr.trollsReforged << endl; linesWritten++;
                outstream << plyr.oracleReturnTomorrow << endl; linesWritten++;
                outstream << plyr.oracleDay << endl; linesWritten++;
                outstream << plyr.oracleMonth << endl; linesWritten++;
                outstream << plyr.oracleYear << endl; linesWritten++;
                outstream << plyr.oracleQuestNo << endl; linesWritten++;

                for (size_t i = 0; i < noOfHealerDays; ++i) {
                    outstream << plyr.healerDays[i] << endl; linesWritten++;
                    // Save other item fields...
                }

                for (size_t i = 0; i < noOfHealerHours; ++i) {
                    outstream << plyr.healerHours[i] << endl; linesWritten++;
                    // Save other item fields...
                }

                for (size_t i = 0; i < noOfHealerMinutes; ++i) {
                    outstream << plyr.healerMinutes[i] << endl; linesWritten++;
                    // Save other item fields...
                }

       


                outstream << plyr.treasureFinding << endl; linesWritten++;
                    outstream << plyr.invisibility << endl; linesWritten++;

                    for (size_t i = 0; i < noOfDiseases; ++i) {
                        outstream << plyr.diseases[i] << endl; linesWritten++;
                        // Save other item fields...
                    }

                    for (size_t i = 0; i < noOfPoison; ++i) {
                        outstream << plyr.poison[i] << endl; linesWritten++;
                        // Save other item fields...
                    }


     
                    outstream << plyr.delusion << endl; linesWritten++;

                    for (size_t i = 0; i < noOfElements; ++i) {
                        outstream << plyr.invulnerability[i] << endl; linesWritten++;
                        // Save other item fields...
                    }

                        outstream << plyr.noticeability << endl; linesWritten++;
                        outstream << plyr.protection1 << endl; linesWritten++;
                        outstream << plyr.protection2 << endl; linesWritten++;
                        outstream << plyr.forgeDays << endl; linesWritten++;
                        outstream << plyr.forgeType << endl; linesWritten++;
                        outstream << plyr.forgeBonus << endl; linesWritten++;
                        outstream << plyr.forgeName << endl; linesWritten++;
                        outstream << plyr.stolenFromVault << endl; linesWritten++;
                        
                        for (size_t i = 0; i < noOfGuilds; ++i) {
                            outstream << plyr.guildAccepting[i] << endl; linesWritten++;
                            // Save other item fields...
                        }

    
                            outstream << plyr.light << endl; linesWritten++;
                            outstream << plyr.supervision << endl; linesWritten++;
                            outstream << plyr.fatigue << endl; linesWritten++;
                            outstream << plyr.fatigueRate << endl; linesWritten++;
                            outstream << plyr.spell_index << endl; linesWritten++;
                            outstream << plyr.temperature << endl; linesWritten++;
                            outstream << plyr.hungerRate << endl; linesWritten++;
                            outstream << plyr.thirstRate << endl; linesWritten++;
                            outstream << plyr.items_index << endl; linesWritten++;
                            outstream << plyr.luck << endl; linesWritten++;
                            outstream << plyr.TemporalAdjustment << endl; linesWritten++;

                            for (size_t i = 0; i < noOfFixedTreasures; ++i) {
                                outstream << plyr.fixedTreasures[i] << endl; linesWritten++;
                                // Save other item fields...
                            }

                            for (int i = linesWritten; i < itemBufferStart; ++i) {
                                outstream << endl; linesWritten++;
                            }
                            outstream <<  "Item Buffer follows" << endl;  


                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].hp << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].index << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].level << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].location << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].type << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].x << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].y << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].name << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].maxHP << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].flags << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].minStrength << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].minDexterity << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].useStrength << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].blunt << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].sharp << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].earth << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].air << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].fire << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].water << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].power << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].magic << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].good << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].evil << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].cold << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].acid << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].nature << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].weight << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].alignment << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].melee << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].ammo << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].parry << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].cat << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].iLevel << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfItemBuffer; ++i) {
                                outstream << itemBuffer[i].buffType << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (int i = linesWritten; i < spellBufferStart; ++i) {
                                outstream << endl; linesWritten++;
                            }
                            outstream << "Spell Buffer follows" << endl; linesWritten++;
                            for (size_t i = 0; i < noOfSpellBuffer; ++i) {
                                outstream << spellBuffer[i].no << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfSpellBuffer; ++i) {
                                outstream << spellBuffer[i].percentage << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (int i = linesWritten; i < effectBufferStart; ++i) {
                                outstream << endl; linesWritten++;
                            }
                            outstream << "Effect Buffer follows" << endl;
                            for (size_t i = 0; i < noOfSpellBuffer; ++i) {
                                outstream << effectBuffer[i].effectNo << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfSpellBuffer; ++i) {
                                outstream << effectBuffer[i].negativeValue << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfSpellBuffer; ++i) {
                                outstream << effectBuffer[i].positiveValue << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (size_t i = 0; i < noOfSpellBuffer; ++i) {
                                outstream << effectBuffer[i].duration << endl; linesWritten++;
                                // Save other item fields...
                            }
                            for (int i = linesWritten; i < smithyDailyWaresStart; ++i) {
                                outstream << endl; linesWritten++;
                            }
                            outstream << "Smithy Daily Wares follows" << endl; linesWritten++;
                            for (int z = 0; z < noOfSmithyFriendships; ++z)
                            {
                                for (int x = 0; x < noOfSmithDailyWares; ++x)
                                {
                                    outstream << itos(smithyDailyWares[z][x]) << endl; linesWritten++;
                                }
                            }
                            for (int i = linesWritten; i < saveGameIndex; ++i) {
                                outstream << endl; linesWritten++;
                            }
                            outstream << "Automap follows" << endl; linesWritten++;
                            for (int z = 0; z < noOfLevels; ++z)
                            {
                                for (int x = 0; x < noOfAutoMapExplored; ++x)
                                {
                                    outstream << itos(autoMapExplored[z][x]) << endl; linesWritten++;   
                                }
                            }
                            outstream << plyr.name << endl; linesWritten++;
                            outstream << plyr.z_offset << endl; linesWritten++;
                            outstream << plyr.prisonRelease << endl; linesWritten++;
                            outstream << progrelease << endl; linesWritten++;
                            outstream.close();
                            return true;
    
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



