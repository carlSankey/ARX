
 #include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <map>
#include <chrono>
#include <thread>
#include <iomanip>
#include <random>

#include "constants.h"
#include "globals.h"
#include "player.h"
#include "font.h"
#include "display.h"
#include "level.h"
#include "game.h"
#include "misc.h"
#include "encounter.h"
#include "items.h"
#include "automap.h"
#include "audio.h"
#include "spells.h"
#include "actor.h"



bool    checkForTreasure;
bool    encounterRunning;
bool    playerTurn;
bool    playerSurprised;
bool    waitingForAnyKey;
bool    waitingForSpaceKey;
int     encounterTurns;
int     encounterMenu;
bool    playerStunned;
bool	opponentStunned;
int		no_found = 0;
bool	foundTreasure = false; 
int OpponentSwitchWeapons;


extern int elementMap[];
extern Map maps[];



string  consoleMessages[MAX_CONSOLE_MESSAGES];

void awardExperience(int opponentNo);
void text(string str);



int spellDamageValues[13];

 extern MapEncounter* newMapEncounter;

extern newMonster* Monster_Buffer;


extern openingMessages* opMessages;

extern openingMessages* Message_Buffer;

 bonusDamage weaponBonusMap;


std::map<int, int*> damageMap = {
	{0, &spellDamageValues[0]},  // Blunt
	{1, &spellDamageValues[1]},  // Sharp
	{2, &spellDamageValues[2]},  // Earth
	{3, &spellDamageValues[3]},  // Air
	{4, &spellDamageValues[4]},  // Fire
	{5, &spellDamageValues[5]},  // Water
	{6, &spellDamageValues[6]},  // Power
	{7, &spellDamageValues[7]},  // Magic
	{8, &spellDamageValues[8]},  // Good
	{9, &spellDamageValues[9]},  // Evit
	{10, &spellDamageValues[10]}, // Cold
	{11, &spellDamageValues[11]}, // Nature
	{12, &spellDamageValues[12]}  // Acid
};



//read this in from Scene file rather than hard code
encRecord dungeonTable[DUNGEON_TABLE_ENCOUNTERS]=
{
    {8,THIEF},
	{6,THIEF},
	{3,THIEF},
	{5,THIEF},
	{3,GOBLIN},
	{3,TROLL},
	{3,LICH},
	{6,GUARD},
	{3,DARK_KNIGHT},
	{3,CHAMPION},
	{13,HEALER},
	{8,KNIGHT},
	{8,PAUPER},
	{8,NOBLEMAN},
	{8,NOVICE},
	{3,APPRENTICE},
	{3,MAGE},
	{3,WIZARD},
	{3,ACOLYTE},
	{3,SAGE},
	{3,ORC},
	{3,GNOME},
	{3,DWARF},
	{8,SLIME},
	{8,MOLD},
	{3,HOMUNCULUS},
	{3,PHOENIX},
	{3,SORCERESS},
	{3,WHIRLWIND},
	{8,GIANT_RAT},
	{3,SMALL_DRAGON},
	{3,SKELETON},
	{3,ZOMBIE},
	{3,GHOUL},
	{3,GHOST},
	{3,SPECTRE},
	{3,WRAITH},
	{3,VAMPIRE},
	{8,GREAT_BAT},
	{3,HELLHOUND},
	{3,HARPY},
	{3,GREMLIN},
	{3,IMP},
	{3,FLAME_DEMON},
	{3,STORM_DEVIL},
	{3,GIANT_WOLF},
	{3,WEREWOLF},
	{3,WARRIOR},
	{3,WEAPONMASTER},
	{3,VALKYRIE},
	{3,GLADIATOR},
	{3,MERCENARY},
	{3,DOPPLEGANGER},
	{3,ADVENTURER},
	{3,WATERSPRITE},
	{3,NIGHTSTALKER},
	{3,SALAMANDER},
	{3,RONIN},
	{3,SERPENTMAN},
	{3,BIG_SNAKE},
	{3,GREAT_NAGA},
	{3,BERSERKER},
	{3,ICE_DEMON},
	{3,HORNED_DEVIL}
};

//read this in from Scene file rather than hard code use map params to determine encounter type rather than hard code
encRecord wellLitEncTable[11]=
{
	{30,	GUARD},
	{30,	THIEF},
	{30,	GIANT_RAT},
	{30,	NOBLEMAN},
	{15,    PAUPER},
	{30,	HEALER},
	{20,	KNIGHT},
	{20,	GREAT_BAT},
	{20,	SLIME},
	{15,	ADVENTURER},
    {15,	ACOLYTE}
};

//read this in from Scene file rather than hard code use map params to determine encounter type rather than hard code
int maxNightEncTable = 51;
encRecord nightEncTable[51] =
{
	{19,	GHOST},
	{17,	MUGGER},
	{17,	GIANT_RAT},
	{17,	SKELETON},
	{16,	ZOMBIE},
	{15,	GREMLIN},
	{12,	MOLD}, // was brown mold
	{10,	THIEF},
	{9,		THIEF},
	{9,		GIANT},
	{8,		SLIME}, // was black slime
	{7,		GIANT_WOLF},
	{7,		CHAMPION},
	{7,		IMP},
	{7,		GHOUL},
	{6,		FIGHTER},
	{5,		SWORDSMAN},
	{5,		WARRIOR},
	{4,		GNOLL},
	{4,		THIEF},
	{3,		SMALL_GREEN_DRAGON},
	{3,		GLADIATOR},
	{3,		GOBLIN},
	{3,		HOBBIT},
	{3,		ORC},
	{3,		DWARF},
	{2,		WRAITH},
	{2,		APPRENTICE},
	{2,		KNIGHT},
	{2,		ACOLYTE},
	{2,		TROLL},
	{2,		ARCHMAGE},
	{2,		ROBBER},
	{2,		GUARD},
	{2,		THIEF},
	{1,		NOVICE},
	{1,		NIGHTSTALKER},
	{1,		SPECTRE},
	{1,		KNIGHT},
	{1,		WIZARD},
	{1,		COMMONER},
	{1,		MERCHANT},
	{1,		COURIER},
	{1,		WEAPONMASTER},
	{1,		NOBLEMAN},
	{1,		PAUPER},
	{1,		MAGE},
	{1,		ASSASSIN},
	{2,		MASTER_THIEF},
	{2,		BRIGAND},
	{2,		CUTTHROAT}
};

//read this in from Scene file rather than hard code use map params to determine 
// encounter type rather than hard code
// may need to add location type param for example 0 = safe, 1 - 5 = level of encounter.... 

int maxDayEncTable = 32;
encRecord dayEncTable[32] =
{
	{41,	GUARD},
	{19,	COMMONER},
	{19,	MERCHANT},
	{18,	APPRENTICE},
	{18,	ACOLYTE},
	{17,	ARCHMAGE},
	{16,	COURIER},
	{12,	KNIGHT},
	{10,	NOVICE},
	{10,	MUGGER},
	{8,		WIZARD},
	{8,		CHAMPION},
	{7,		FIGHTER},
	{6,		SWORDSMAN},
	{6,		THIEF},
	{6,		WARRIOR},
	{5,		THIEF},
	{5,		NOBLEMAN},
	{4,		PAUPER},
	{4,		GLADIATOR},
	{3,		GUARD},
	{3,		KNIGHT},
	{3,		THIEF},
	{1,		ROBBER},
	{1,		MAGE},
	{1,		ASSASSIN},
	{1,		THIEF},
	{1,		WEAPONMASTER},
	{2,     NOBLEWOMAN},
	{1,		CUTTHROAT},
	{1,		BRIGAND},
	{1,		MASTER_THIEF}
	
};

extern int HackedMonsters;
extern int statPanelY;
extern bool musicPlaying;

newMonster Opponents[8]; // max 8 monsters against you

newMonster opponent;

bonusDamage weaponBonus;
int weaponProbabilities[6] = { 0,0,0,0,0,0 };
int weaponReferences[6] = { 0,0,0,0,0,0 };

string str, key, str2, prefix; // for message text

int opponentNoAttacking;
int opponentType;
int flashCount;
int encAnimation;
bool flashTextOn;
bool encounterNotHostile;

bool opponentSurprised;
bool playerOnGround;
int groundTurnsRemaining;
int stunnedTurnsRemaining;
bool playerRunsAway = false;
int whoStartedIt = 0;

int encounterQuantity;
int curOpponent; // 0-7

sf::Time attackCheckTime;
sf::Time attackTimer;
sf::Clock attackClock;
sf::Clock actionClock;
sf::Clock  OpponentLeftTimer;
sf::Time lastActionTimer = actionClock.restart();

bool opponentLeft;

std::string prefixes[] = { "The", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th" };

void updateAttack(std::bitset<13> binaryElems, int negativeValue) {
	for (int i = 0; i < 13; i++) {
		if (binaryElems.test(i)) {
			*(damageMap[i]) = negativeValue;
		}
		else
		{
			*(damageMap[i]) = 0;
		}
	}
}


void encounterLoop(int encounterType, int opponentQuantity)
{
    opponentType = encounterType;
    checkForTreasure = false;
	animationNotStarted = true;
	firstFrame = Monster_Buffer[opponentType].image;
	lastFrame = Monster_Buffer[opponentType].image2;
	encounterRunning = true;
	encounterTurns = 0;
	playerTurn = true;
	playerStunned = false;
	playerOnGround = false;
	playerSurprised = false;
	opponentSurprised = false;
	waitingForAnyKey = false;
	waitingForSpaceKey = false;
  	opponentLeft = false;
    encounterMenu = 1;
    groundTurnsRemaining = 0;
	stunnedTurnsRemaining = 0;
    plyr.status_text = "                                        ";
    playerRunsAway = false;
	
	if (plyr.TemporalAdjustment == 0) { plyr.TemporalAdjustment = 1; }

	
	int adjustmentFactor = static_cast<int>(plyr.TemporalAdjustment);
    // Move to display!
	if (graphicMode==ALTERNATE_LARGE) plyr.z_offset = 0.3;
	else plyr.z_offset = 1.5;

	clearConsoleMessages();
	initialiseOpponents(opponentType, opponentQuantity);

	determineOpponentOpeningMessage();
	checkHostility();   // Check if opponent is hostile
    //checkSurprise();  // Check if player or opponent surprised

    selectEncounterTheme();
  	attackCheckTime = sf::Time::Zero;
	

    while ( (encounterRunning) || (waitingForSpaceKey) )
	{
		
		attackTimer = attackClock.restart();
		lastActionTimer += attackTimer;
		drawEncounterView();
		if (!waitingForSpaceKey )
        {

		  	//Opponent will choice to do something after 4.0 no matter what the player does        
			attackCheckTime += attackTimer;
			if ((attackCheckTime >= (sf::seconds(4.0f) * static_cast<float>(plyr.TemporalAdjustment)) && Opponents[0].stunnedTurnsRemaining <1) || playerStunned == true)
			{
			  	processOpponentAction();
			  	attackCheckTime = attackClock.restart();
			}

            if (playerTurn) processPlayerAction();
            else processOpponentAction();
        }


		updateDisplay();
		//std::this_thread::sleep_for(0.4s);
        key = getSingleKey();

        // Check if player died this turn
        if (plyr.hp < 0) { encounterRunning = false; }

        // Handle dismissed encounter messages
		

		if (key == "SPACE" || lastActionTimer >= sf::seconds(20.0f))
		{
 			updateConsoleMessages(); // Checks for further messages to be printed.
			if (consoleMessages[0] == "NO MESSAGE") waitingForSpaceKey = false; // player pressed space to acknowledge last message read
			lastActionTimer = actionClock.restart();
		}

		// ...and see if any opponents still active
		updateOpponents();
        checkForActiveOpponents();



		  if (opponentLeft == true)
		  {
			  if (OpponentLeftTimer.getElapsedTime() >= sf::seconds(20.0f))
			  {
	  		  	waitingForSpaceKey = false;
	  		  	opponentLeft = false;
			  }
		  	
		  }

	}

	// If opponent killed, charmed or tricked then player might find treasure afterwards
    //checkTreasure();


	if (plyr.fixedEncounter)
    {
        plyr.fixedEncounter = false;
        if (playerRunsAway)
        {
			RunAway(1);
        }
        else
        {
            // Fixed encounter killed
            plyr.fixedEncounters[plyr.fixedEncounterRef] = true;
        }
    }

	plyr.status = EXPLORE;

	if ((checkForTreasure) && (plyr.hp >= 0)) checkTreasure();
}


void RunAway(int backup)
{
 	if (backup == 1)
	{
		if (plyr.facing == WEST) { plyr.x = plyr.oldx; }
		if (plyr.facing == NORTH) { plyr.x = plyr.oldx; }
		if (plyr.facing == EAST) { plyr.y = plyr.oldy; }
		if (plyr.facing == SOUTH) { plyr.y = plyr.oldy; }
		plyr.z_offset = 0.1;
	}
	else
	{
		plyr.facing = randn(1, 5);
	}
}

void initialiseOpponents(int opponentType, int opponentQuantity)
{
	
    // Clean out all 8 opponent slots with an empty monster object (using the unused FBI Agent for this)
    for(int i = 0; i < MAX_OPPONENTS; ++i)
	{
		Opponents[i] = Monster_Buffer[NOBODYHERE];
	}
    for(int i = 0; i < opponentQuantity; ++i)
	{
		Opponents[i] = Monster_Buffer[opponentType];
		Opponents[i].chosenWeapon = opponentChooseWeapon();
		if (opponentType==DOPPLEGANGER)
        {
            // Doppleganger
            Opponents[i].hp = plyr.hp;
            Opponents[i].maxHP = plyr.hp;
            Opponents[i].str = plyr.str;
            Opponents[i].skl = plyr.skl;
            Opponents[i].inte = plyr.inte;
            Opponents[i].spd = plyr.speed;
            Opponents[i].w1 = plyr.priWeapon;
            Opponents[i].c1 = 100;
			
        }
	}

	plyr.encounterAnimationRef = Opponents[0].image;
	plyr.encounterRef = opponentType;
	encounterQuantity = opponentQuantity;

    // Display encounter initial values in console

    cout << "Qty:  " << encounterQuantity << "\n";
    cout << "Name:  " << Opponents[0].name << "\n";
    cout << "HP:    " << Opponents[0].hp << "\n";
    cout << "Align: " << Opponents[0].alignment << "\n\n";
    cout << "Sta: " << Opponents[0].sta << "\nCha: " << Opponents[0].cha << "\nStr: " << Opponents[0].str << "\nInt: " << Opponents[0].inte <<
        "\nWis: " << Opponents[0].wis << "\nSkl: " << Opponents[0].skl << "\nSpd: " << Opponents[0].sta << "\n\n";

    cout << "Blunt: " << Opponents[0].aBlunt << "\nSharp: " << Opponents[0].aSharp << "\nEarth: " << Opponents[0].aEarth << "\nAir:   " << Opponents[0].aAir
    << "\nFire:  " << Opponents[0].aFire << "\nWater: " << Opponents[0].aWater << "\nPower: " << Opponents[0].aPower << "\nMagic: " << Opponents[0].aMagic
    << "\nGood:  " << Opponents[0].aGood << "\nEvil:  " << Opponents[0].aEvil << "\nCold:  " << Opponents[0].aCold << "\n\n";

    cout << "Com: " << Opponents[0].tCompasses << " Cop: " << Opponents[0].tCopper << " Cry: " << Opponents[0].tCrystals << " Foo:   " << Opponents[0].tFood
    << " Gem: " << Opponents[0].tGems << " Gol: " << Opponents[0].tGold << " Jew: " << Opponents[0].tJewels << " Key: " << Opponents[0].tKeys
    << " Sil:  " << Opponents[0].tSilver<< " Tim:  " << Opponents[0].tTimepieces << " Tor:  " << Opponents[0].tTorches << "\n\n";


    int w = Opponents[0].w1;
    cout << "Name:  " << newItemArray[w].name << "\n";
    cout << "HP:    " << newItemArray[w].hp << "\n";
    cout << "Align: " << newItemArray[w].alignment << "\n\n";
    cout << "Blunt: " << newItemArray[w].blunt << "\nSharp: " << newItemArray[w].sharp << "\nEarth: "
        << newItemArray[w].earth << "\nAir:   " << newItemArray[w].air
        << "\nFire:  " << newItemArray[w].fire << "\nWater: " << newItemArray[w].water << "\nPower: "
        << newItemArray[w].power << "\nMagic: " << newItemArray[w].magic
        << "\nGood:  " << newItemArray[w].good << "\nEvil:  " << newItemArray[w].evil << "\nCold:  "
        << newItemArray[w].cold << "\n\n";


			MoveItemToMonsterWeapon(0, Opponents[0].w1);
			MoveItemToMonsterWeapon(1, Opponents[0].w2);
			MoveItemToMonsterWeapon(2, Opponents[0].w3);
			MoveItemToMonsterWeapon(3, Opponents[0].w4);
			MoveItemToMonsterWeapon(4, Opponents[0].w5);
			MoveItemToMonsterWeapon(5, Opponents[0].w6);
	
			OpponentSwitchWeapons = 0;
}

void MoveItemToMonsterWeapon(int WeaponIndex, int ItemIndex)
{
		monsterWeapons[WeaponIndex].article = "a";
		monsterWeapons[WeaponIndex].name = newItemArray[ItemIndex].name;
		monsterWeapons[WeaponIndex].desc = "";
		monsterWeapons[WeaponIndex].descMon = "";
		monsterWeapons[WeaponIndex].type = newItemArray[ItemIndex].itemType;
		monsterWeapons[WeaponIndex].index = ItemIndex;
		monsterWeapons[WeaponIndex].flags = newItemArray[ItemIndex].flags;
		monsterWeapons[WeaponIndex].minStrength = newItemArray[ItemIndex].minStrength;
		monsterWeapons[WeaponIndex].minDexterity = newItemArray[ItemIndex].minDexterity;
		monsterWeapons[WeaponIndex].useStrength = newItemArray[ItemIndex].useStrength;
		monsterWeapons[WeaponIndex].blunt = newItemArray[ItemIndex].blunt;
		monsterWeapons[WeaponIndex].sharp = newItemArray[ItemIndex].sharp;
		monsterWeapons[WeaponIndex].earth = newItemArray[ItemIndex].earth;
		monsterWeapons[WeaponIndex].air = newItemArray[ItemIndex].air;
		monsterWeapons[WeaponIndex].fire = newItemArray[ItemIndex].fire;
		monsterWeapons[WeaponIndex].water = newItemArray[ItemIndex].water;
		monsterWeapons[WeaponIndex].power = newItemArray[ItemIndex].power;
		monsterWeapons[WeaponIndex].magic = newItemArray[ItemIndex].magic;
		monsterWeapons[WeaponIndex].good = newItemArray[ItemIndex].good;
		monsterWeapons[WeaponIndex].evil = newItemArray[ItemIndex].evil;
		monsterWeapons[WeaponIndex].cold = newItemArray[ItemIndex].cold;
		monsterWeapons[WeaponIndex].nature = newItemArray[ItemIndex].nature;
		monsterWeapons[WeaponIndex].acid = newItemArray[ItemIndex].acid;
		monsterWeapons[WeaponIndex].weight = newItemArray[ItemIndex].weight;
		monsterWeapons[WeaponIndex].hp = newItemArray[ItemIndex].hp;
		monsterWeapons[WeaponIndex].maxHP = newItemArray[ItemIndex].maxHP;
		monsterWeapons[WeaponIndex].alignment = newItemArray[ItemIndex].alignment;
		monsterWeapons[WeaponIndex].melee = newItemArray[ItemIndex].melee;
		monsterWeapons[WeaponIndex].ammo = newItemArray[ItemIndex].ammo;
		monsterWeapons[WeaponIndex].parry = newItemArray[ItemIndex].parry;
		monsterWeapons[WeaponIndex].special = 0;
		monsterWeapons[WeaponIndex].specialType = 0;
		monsterWeapons[WeaponIndex].specialName = "";


}

void removeOpponent()
{
    // Removes Opponents[0], shuffles the other 7 and adds an Empty
	encounterQuantity -= 1;

	Opponents[curOpponent] = Monster_Buffer[NOBODYHERE];
	
    for(int i = 0; i < (MAX_OPPONENTS)-1; ++i)
	{
		if (Opponents[i].hp >0) {
			Opponents[i] = Opponents[i + 1];
		}
		
	}
	// Add an empty slot to the end of the array (the unused FBI Agent is used for this)
	Opponents[(MAX_OPPONENTS-1)] = Monster_Buffer[NOBODYHERE];
}

void addOpponent()
{
	// Removes Opponents[0], shuffles the other 7 and adds an Empty
	//if No Of opponents is < MAX_OPPONENTS then add one
	//add a Monster_Buffer[opponent.index]
	if (encounterQuantity < MAX_OPPONENTS)
	{
		Opponents[(encounterQuantity +1)] = Monster_Buffer[Opponents[0].index];
		
	}
}

void checkForActiveOpponents()
{
	// Check first encounter slot
	if (Opponents[0].hp <= 0) encounterRunning = false;
}

void updateOpponents()
{
    // Count total remaining opponents
	encounterQuantity=0;
	for (int i = 0; i < MAX_OPPONENTS - 1; ++i) { // Ensure we do not go out of bounds
		if (Opponents[i].hp > 0) {
			encounterQuantity++;
			// Skip the entire group of consecutive matching indices
		
		}
	}
	//if (encounterQuantity > 1)
	//std:cout << "Num Opn# " << encounterQuantity << "\n";
}


/**
* @brief Only attack if the encounterNotHostile is true
* @brief leaves after 3 turns
* @brief reset the action timer so we are not waiting for player to do an action
* 
*/
void processOpponentAction()
{
	//Need to add Opponent spell casting, but need to add list of spells monster can have first

	// Might change this to use case 
	// also add break stun before this, so they can break the spell and then attack in the same action
	// work out which action the encounter is going to do and set a case value 
	// 1 attack
	// 2 cast spell
	// 3 



    if (encounterNotHostile)
    {
        if (encounterTurns == 3) { opponentLeaves(); }
        else { playerTurn = true; }
    }
    else  //The opponenent can attack
    {
		int braveness = Opponents[curOpponent].str + Opponents[curOpponent].inte + encounterQuantity + Opponents[curOpponent].hp + Opponents[curOpponent].braveness;
		//Is the opponent stunned/paralysed etc
		if (Opponents[curOpponent].stunnedTurnsRemaining > 0)
		{
			int breakSpell = randn(0, plyr.inte * (spellBuffer[Opponents[curOpponent].spellBuffer[0]].percentage / 100));

			if (breakSpell > Opponents[0].inte)  // Did the opponent break the spell?
			{
				str = "The " + Opponents[0].name + " has broken your spell@";
				str = str + " and is no longer " + Opponents[curOpponent].stunnedtext;
				Opponents[curOpponent].stunnedTurnsRemaining = 0;
			}
			else
			{
 				str = "The " + Opponents[0].name + " is " + Opponents[curOpponent].stunnedtext;
				Opponents[curOpponent].stunnedTurnsRemaining--;
				consoleMessage(str);

			}
		} //opponent is able to fight
		else if (braveness < randn(0, ((plyr.str + plyr.level) - plyr.alcohol))) // check to see if the oppoenent is scraed and flees
		{
			opponentFlees();
		}
		else if ((Opponents[curOpponent].callForHelp == 1) && (braveness < 20))
		{
			if (encounterQuantity < MAX_OPPONENTS) 
			{ 
				opponentCallsForHelp();
			} 
			else
			{
				opponentAttack();
			}
		}
		else
		{
			opponentAttack();
		}
    }
    encounterTurns++;
  	attackCheckTime = sf::Time::Zero;

    // If last opponent then switch to player turn
    if (curOpponent==(encounterQuantity-1)) 
	 { 
	    playerTurn = true; 
		 curOpponent = 0; 
	 } else { 
	    curOpponent++; 
		if(curOpponent>7){ curOpponent = 0; }
	    std::cout << "next opponent active";
		playerTurn = false;
	 }
	lastActionTimer = actionClock.restart();
}

/**
 * @brief this is player interaction with the encounter
 */
void processPlayerAction()
{
	//Within a while loop
	//Should exit of the player has done something or can't do anything
	
	if (stunnedTurnsRemaining == 0) { playerStunned = false; }

    if (opponentSurprised)          encounterMenu = 3;
    if (playerSurprised)            encounterMenu = 10;
    if (playerStunned)              encounterMenu = 11;


    if (encounterMenu == 1)
    {
        cyText (1, "Battle Options");
        bText (12,3, "(1) Attack");
        bText (12,4, "(2) Charge");
        bText (12,5, "(3) Aimed Attack");
        bText (12,6, "(4) Transact");
        bText (12,7, "(5) Switch weapon");
        bText (12,8, "(0) Turn and run!");

        if ( key == "0" ) 
		{
		  	  // "You didn't escape."  
			 // City logic
			if (whoStartedIt == 0) { whoStartedIt = 1; }
			int playerescape = randn(0, plyr.speed) / whoStartedIt;
			int opponentalignment;
			if (Opponents[0].alignment == 0) { opponentalignment = 1; }
			else { opponentalignment = Opponents[0].alignment; }
			int opponentescape = randn(0, Opponents[0].spd) / opponentalignment;

		  	 // int escapeperc = 4 - ((int)(plyr.stealth * 0.05));
		  	//  if (escapeperc < 1)
		  	//  	  escapeperc = 1;
		  	  	  
		  	 // int value = randn(1,10);

			if (playerescape < opponentescape)
		  	{
			      consoleMessage( "You didn't escape.");
				  updateDisplay();
				  bool keynotpressed = true;
				  key = "";
				  while (keynotpressed)
				  {
					  key = getSingleKey();
					  if (key == "SPACE")
					  {
						  encounterMenu = 1;
						  //playerStunned == true;
						  playerTurn == false;
						  if ((Opponents[0].alignment < 128) || (!encounterNotHostile))
							  opponentAttack();
						  keynotpressed = true;
					  }

					  if (key != "") { keynotpressed = false; }
				  }
				 
				  
			} else  
			{
				playerRunsAway = true; 
				encounterRunning = false; 
				RunAway(0);
			}
		}
        if ( key == "P" ) pauseEncounter();
        if ( key == "1" ) playerAttack(1,0.00);               // Attack (No attack bonus)
        if ( key == "2" ) playerAttack(2,0.50);               // Charge
        if ( key == "3" ) playerAttack(3,0.50);               // Aimed attack
        if ( key == "4" ) playerTransact();
        if ( key == "5" ) playerSwitchWeapons();
        if ( key == "U" ) { updateDisplay(); selectItem(USE); }
        if ( key == "D" ) { updateDisplay(); selectItem(DROP); }
		if ( key == "C") {  updateDisplay(); castSpells(); }
    }

    else if (encounterMenu == 2)
    {
        //if (key=="4") key = "";
        //cout << "menu 2\n";
        cyText (2, "Transact Options");
        bText (16,4, "(1) Offer");
        bText (16,5, "(2) Charm");
        bText (16,6, "(3) Trick");
        bText (16,7, "(4) Hail");
        bText (16,8, "(0) Leave");

        if ( key=="1" ) { playerOffer(); } // offer
        if ( key=="2" ) { playerCharm(); } // charm
        if ( key=="3" ) { playerTrick(); } // trick
        if ( key=="4" )
        {
            if ((plyr.encounterRef == HEALER) && (plyr.scenario>0)) { encounterMenu = 5; } else { playerHail(); }
        }
        if ( key=="0" ) { encounterRunning = false; } // leave
    }

    else if (encounterMenu == 3)
    {
        str = "You surprise the " + Opponents[0].name + ".";
        cyText (2, str);
        cyText (3, "Attempt to");
        str = "(1) waylay the " + Opponents[0].name + ".";

        bText (12,4, str);
        bText (12,5, "(2) snatch something.");
        bText (12,6, "(3) none of these.");
        bText (12,8, "(0) Leave.");

		if ( key=="1" ) { playerWaylay(); } // Attempt to waylay
		if ( key=="2" ) { playerSnatch(); } // Attempt to snatch
        if  (key=="3" ) { opponentSurprised = false; encounterMenu = 1; }
        if  (key=="0" ) encounterRunning = false;

    }

    else if (encounterMenu == 10)
    {
        cyText (2, "SURPRISED@@No options");
        if ( key=="SPACE" ) { encounterMenu = 1; determineOpponentOpeningMessage(); playerSurprised = false; /* playerTurn = false; */ }
    }

    else if (encounterMenu == 11)
    {
        cyText (2, "STUNNED@@No options");
		if (key == "SPACE") {
			
			if (curOpponent = encounterQuantity) { stunnedTurnsRemaining--; } /* only reduce the stuuned time on last attacker */
			if (stunnedTurnsRemaining == 0) { encounterMenu = 1; playerStunned = false; /* playerTurn = false; */ }
		
		}
    }

    else if (encounterMenu == 4) // thief grouping
    {
        str = "The "+ Opponents[0].name +" demands:";
        cyText (2, str);
        cyText (4, "\"Stand and deliver.");
        bText (9,5, "Thy money or thy life!\"");
        bText (8,7, "Dost thou yield? (Y or N)");
        if ( key=="Y" ) { thiefYield(); }
        if ( key=="N" ) { encounterNotHostile = false; encounterMenu = 1; }
    }

    else if (encounterMenu == 6) // Knight
    {
        cyText (2, "The Knight demands:");
        cyText (4, "\"Yield the passage or face cold steel!\"");
        bText (8,7, "Dost thou yield? (Y or N)");
        if ( key=="Y" ) { plyr.alignment++; if (plyr.alignment>255) plyr.alignment=255; encounterRunning = false;  }
        if ( key=="N" ) { consoleMessage("Have at you!"); plyr.alignment--; if (plyr.alignment<0) plyr.alignment=0; encounterNotHostile = false; encounterMenu = 1; }

        // Knight should yield to you in time!
    }

    else if (encounterMenu == 7) // Pauper
    {
        cyText(2,"The pauper pleads:@@\"Please help a fellow human@trapped in this foul world.\"");
        if ( key=="SPACE" ) { encounterMenu = 1; }
    }

    else if (encounterMenu == 5) // healer transact
    {
        //cout << "Healer transact menu\n";
        cyText (2, "Would you like the healer to:");
        bText (8,4, "(1) Heal wounds (1 silver each)");
        bText (8,5, "(2) Cure diseases (100 silvers)");
        bText (8,6, "(3) Cure poisons (50 silvers)");
        bText (8,8, "(0) Nothing at all");

        if  (key=="1" ) healerHealWounds();
        if  (key=="2" ) healerCureDiseases();
        if  (key=="3" ) healerCurePoisons();
        if  (key=="0" ) opponentLeaves();
    }

    else if (encounterMenu == 12)   // Guard after vault raid
    {
        cyText (2, "The guard demands:");
        cyText (4, "\"Surrender in the name of his majesty@the King! Thou art under arrest varlet!\"");
        cyText (7, "Dost thou yield? (Y or N)");
        if ( key=="Y" ) { surrenderToGuard(); }
        if ( key=="N" ) { encounterNotHostile = false; encounterMenu = 1; playerTurn = false; }
    }

	if(key != "") { lastActionTimer = actionClock.restart(); }
	
	

}   // <- End of process player


void surrenderToGuard()
{
    consoleMessage("Enough of thy antics,@to the pokey with thee!");
    if (plyr.guildMemberships[0] > 0)
    {
        consoleMessage("Fellow Thieves Guild members rescue@you from prison before your trial.");
        encounterRunning = false;
        plyr.x = 43;
        plyr.y = 30;
    }
    else
    {
        consoleMessage("You receive a swift trial,@and a slow execution.");
        plyr.hp = -1;
    }
	whoStartedIt = 0;
}


void clearConsoleMessages()
{
    // Sets all console message slots to empty
    for(int i = 0; i < MAX_CONSOLE_MESSAGES; ++i)
	{
		consoleMessages[i] = "NO MESSAGE";
	}
}


void updateConsoleMessages()
{
    // Moves messages along so index [0] contains next message to be printed (if any).
    for(int i = 0; i < MAX_CONSOLE_MESSAGES; ++i)
	{
        if (i == (MAX_CONSOLE_MESSAGES-1)) { consoleMessages[i] = "NO MESSAGE"; }
        else { consoleMessages[i] = consoleMessages[i+1]; }
	}
}


void consoleMessage(string messageText)
{
    bool messageNotAddedToQueue = true;
    int messagesIndex = 0;
    while (messageNotAddedToQueue)
    {
        if (consoleMessages[messagesIndex] == "NO MESSAGE")
        {
            consoleMessages[messagesIndex] = messageText;
            waitingForSpaceKey = true;
            messageNotAddedToQueue = false;
        }
        messagesIndex++;
        if (messagesIndex > MAX_CONSOLE_MESSAGES)
        {
            // Will currently discard the message
            cout << "ERROR: Console messages maximum exceeded!\n";
            messageNotAddedToQueue = false; // To break out of loop after reporting error
        }
  }
}


void drawEncounterView()
{
	drawAtariAnimation();
	updateEncounterStatusText();
	if (waitingForSpaceKey)
	{
	    cyText(3,consoleMessages[0]);
	}

	if (graphicMode== ALTERNATE_LARGE) drawConsoleBackground();
}


void playerWaylay()
{
	if (whoStartedIt == 0) { whoStartedIt = 2; }
}


void playerSnatch()
{
	if (whoStartedIt == 0) { whoStartedIt = 2; }
}


void determineOpponentOpeningMessage()
{
	// Determines the following:
	// 1) Does the opponent have an opening message (e.g. thief, pauper, knight)
	//Agains need to dermine from opponent rather than hard code
	// for example
	// when monster.openingmessage > 0 then encounterMenu = monster.openingmessage
	// Add a new property encounterMenu

	switch (plyr.encounterRef)
	{
		case THIEF:
			encounterMenu = 4;
			break;
		case KNIGHT:
			encounterMenu = 6;
			break;
		case PAUPER:
			encounterMenu = 7;
			break;
        case GUARD:
            if (plyr.stolenFromVault==2) { plyr.stolenFromVault = 1; encounterMenu = 12; }
            break;
	}
}


void checkHostility()
{
	// Use behavior and alignment instead of this
	// Behavior 0 Don't care about what race you are
	// Behaviour 1 = hate humans
	// Behaviour 2 = love humand

    encounterNotHostile = true;

    // Check alignment - Evil
    if (Opponents[0].alignment < 128) encounterNotHostile = false;

    // Good but hostile to humans
	if (Opponents[0].behaviour == 1 ) encounterNotHostile = false;

	// Check for neutral encounters without intelligence or wisdom - e.g. giant rat
	if ((Opponents[0].inte == 0) && (Opponents[0].wis == 0)) encounterNotHostile = false;

	// Check anti-guild status - only applies to humans

}


void checkSurprise()
{
	// Determines whether the player or opponent have element of surprise
	// If player then go to encounterMenu 3

	int playerStealth = randn(1,plyr.stealth+plyr.alcohol);
//cout <<  "player:" << playerStealth << "\n";
//cout << "opponent:" << Opponents[0].stealth;
    int opponentStealth = randn(0,Opponents[0].stealth);
	//int encounterStealth = randn(1,5);
//	cout << "opp stealth:" << opponentStealth << "\n";
	int surpriseValue = -opponentStealth + playerStealth;
//	cout << "value:" << surpriseValue << "\n";
	if (surpriseValue < -21) { playerSurprised = true; }
	if (surpriseValue > 20) { opponentSurprised = true; }

}


void playerSwitchWeapons()
{
        str = "Switching to " + itemBuffer[plyr.secWeapon].name + ".";
        int oldPrimary = plyr.priWeapon;
        plyr.priWeapon = plyr.secWeapon;
        plyr.secWeapon = oldPrimary;
		consoleMessage(str);
}


void playerTransact()
{
	if ((Opponents[0].alignment>127) && (encounterNotHostile)) { encounterMenu = 2; }

	if ((Opponents[0].alignment<128) || (!encounterNotHostile))
	{
			str = "The " + Opponents[0].name + " seems uninterested.";
			consoleMessage(str);
			playerTurn = false;
			//checkSurprise
    }
}


//Agains read this from a file instead of hard coding
// and link monster responce to responce list
// Split out Message and action
void playerHail()
{

	//Determin what messages can be returned from an encounter if 

	//i want one number to relate to multiple 
	int response = 0;
	string itemText = "";
	// using encounterbType
	// only type 1 can communicate
	// if openingMessage = 0
	// then use a base level of messages
	// if above 0 then use specific message





	if (Monster_Buffer[plyr.encounterRef].openingMessage == 0 && Monster_Buffer[plyr.encounterRef].type ==1)
	{
		response = randn(0, 14);
	}
	else
	{
		response = Monster_Buffer[plyr.encounterRef].openingMessage;
	}

	

	


	/*
	if (response==1) str = "\"Run! The Devourer comes!\"";
	if (response==2) str = "The " + Opponents[0].name + " mumbles@@something unintelligible.";
	if (response==3) str = "\"Beware of false alarms.\"";
	if (response==4) str = "\"No good deed ever goes unrewarded.\"";
	if (response==5) str = "\"Don't tell the peasants how good@@the pears are with the cheese!\"";
	if (response==6) str = "\"Prepare, the Apocalypse is soon.\"";
	if (response==7) str = "\"Greetings adventurer!\"";
	if (response==8) str = "\"Beware of Arena slavers!\"";
	if (response==9) str = "\"They say that the ghost of the dead@@king lurks in the Palace gardens.\"";
	if (response==10) str = "\"Seek out the guilds for learning.\"";
	if (response==11) str = "\"Few have survived the Trial of Kings\"";
	if (response==12) str = "\"The dead of the Arena haunt the@@ruined Five Blades tavern.\"";
	if (response==13) str = "\"The House of Shadows is the most@@mysterious of all the noble houses.\"";
	if (response==14) str = "\"Only those who have fought in@@the Great Arena can drink@@in the Block and Parry.\"";
	if (response==15) str = "\"A flaming torch can make@@an effective weapon.\"";



	*/

	if ((plyr.encounterRef == NOBLEMAN))
	{
		plyr.gold++;
	}



	if ((plyr.encounterRef == ACOLYTE))
	{
		//Use monster name instead of nobleman
	    string items[7] = { "food packets" , "water flasks" , "torches" , "timepieces", "compasses", "keys" , "crystals" };  
	 
	    bool itemRequired = true;

	    // Check for friendship

	    for (int i=0 ; i<7 ; i++)
        {
            if ((i==0) && (plyr.food==0))                         { itemRequired = false;  itemText = items[i]; plyr.food++; }
            if ((i==1) && (plyr.water==0) && (itemRequired))       { itemRequired = false;  itemText = items[i]; plyr.water++; }
            if ((i==2) && (plyr.torches==0) && (itemRequired))     { itemRequired = false;  itemText = items[i]; plyr.torches++; }
            if ((i==3) && (plyr.timepieces==0) && (itemRequired))  { itemRequired = false;  itemText = items[i]; plyr.timepieces++; }
            if ((i==4) && (plyr.compasses==0) && (itemRequired))   { itemRequired = false;  itemText = items[i]; plyr.compasses++; }
            if ((i==5) && (plyr.keys==0) && (itemRequired))        { itemRequired = false;  itemText = items[i]; plyr.keys++; }
            if ((i==6) && (plyr.crystals==0) && (itemRequired))    { itemRequired = false;  itemText = items[i]; plyr.crystals++; }
        }

            if (itemRequired) response = Monster_Buffer[plyr.encounterRef].openingMessage +1;
            else { response; }

	}

	str = processMessage(Message_Buffer[response].message, itemText);

    consoleMessage(str);

    str = "The " + Opponents[0].name + " leaves.";
	consoleMessage(str);
	encounterRunning = false;
}


void opponentLeaves()
{

	
    str = "The " + Opponents[0].name + " leaves.";
    consoleMessage(str);
    encounterRunning = false;
    opponentLeft = true;
    OpponentLeftTimer.restart();
	whoStartedIt = 0;
}


void opponentFlees()
{

	str = "The " + Opponents[0].name + " Flees.";
	consoleMessage(str);
	encounterRunning = false;
	if (curOpponent >= 0 && curOpponent < encounterQuantity) {
		for (int i = curOpponent; i < encounterQuantity; ++i) {
			Opponents[i] = Opponents[i + 1];
			encounterQuantity--;
		}
	}
	else {
		cout << "Invalid index!" << endl;
	}
	if (curOpponent == 0) { opponentLeft = true; }
	OpponentLeftTimer.restart();
	whoStartedIt = 0;
}


void playerOffer()
{
    updateDisplay(); // temporary - won't work when rain is added or for an animated character
	bool offermade = false;
	// opponentNo == 1
	int itemQuantity = 0;
	int itemRef = selectItem(OFFER); // select an item in OFFER mode
	if ((itemRef > 999) && (itemRef != 9999)) itemQuantity = inputItemQuantity(3);
	if ((itemRef > 999) && (itemRef != 9999))
	{
		if ((itemRef==1000) && (plyr.food>0)) { if (itemQuantity > plyr.food) { itemQuantity = plyr.food; } plyr.food-=itemQuantity; offermade = true; }
		if ((itemRef==1001) && (plyr.water>0)) { if (itemQuantity > plyr.water) { itemQuantity = plyr.water; } plyr.water-=itemQuantity; offermade = true; }
		if ((itemRef==1002) && (plyr.torches>0)) { if (itemQuantity > plyr.torches) { itemQuantity = plyr.torches; } plyr.torches-=itemQuantity; offermade = true; }
		if ((itemRef==1003) && (plyr.timepieces>0)) { if (itemQuantity > plyr.timepieces) { itemQuantity = plyr.timepieces; } plyr.timepieces-=itemQuantity; offermade = true; }
		if ((itemRef==1004) && (plyr.compasses>0)) { if (itemQuantity > plyr.compasses) { itemQuantity = plyr.compasses; } plyr.compasses-=itemQuantity; offermade = true; }
		if ((itemRef==1005) && (plyr.keys>0)) { if (itemQuantity > plyr.keys) { itemQuantity = plyr.keys; } plyr.keys-=itemQuantity; offermade = true; }
		if ((itemRef==1006) && (plyr.crystals>0)) { if (itemQuantity > plyr.crystals) { itemQuantity = plyr.crystals; } plyr.crystals-=itemQuantity; offermade = true; }
		if ((itemRef==1007) && (plyr.gems>0)) { if (itemQuantity > plyr.gems) { itemQuantity = plyr.gems; } plyr.gems-=itemQuantity; offermade = true; }
		if ((itemRef==1008) && (plyr.jewels>0)) { if (itemQuantity > plyr.jewels) { itemQuantity = plyr.jewels; } plyr.jewels-=itemQuantity; offermade = true; }
		if ((itemRef==1009) && (plyr.gold>0)) { if (itemQuantity > plyr.gold) { itemQuantity = plyr.gold; } plyr.gold-=itemQuantity; offermade = true; }
		if ((itemRef==1010) && (plyr.silver>0)) { if (itemQuantity > plyr.silver) { itemQuantity = plyr.silver; } plyr.silver-=itemQuantity; offermade = true; }
		if ((itemRef==1011) && (plyr.copper>0)) { if (itemQuantity > plyr.copper) { itemQuantity = plyr.copper; } plyr.copper-=itemQuantity; offermade = true; }
	}

	if (itemRef<100)
	{
		// Remember corpses for undead!
		moveItem(itemRef,0); // move the inventory item to the void
	}

	if ((offermade) && (plyr.encounterRef == PAUPER))
	{
		// Paupers see items 1000 - 1011 as postive offers
		int pauperAcceptance = randn(0,6);
		if (pauperAcceptance > 1) { if (plyr.gender==1) str = "\"Bless you, kind sir.\""; if (plyr.gender==2) str = "\"Bless you, kind madam.\""; plyr.alignment += 1; }
		if (pauperAcceptance < 2) { str = "\"I was hoping for pears and cheese.\""; } // Ungrateful pauper, no alignmnent improvement
	}
	else
	{
		str = "The " + Opponents[0].name + " leaves.";
	}
	consoleMessage(str);
	encounterRunning = false;

}


void playerCharm()
{
    updateDisplay(); // sloppy!
	opponent = Opponents[0];
	if (whoStartedIt == 0) { whoStartedIt = 2; }
	checkAlignmentEncounter(curOpponent);

	encounterNotHostile = false;
	bool charmSuccess = false;
	if ( (opponent.inte > 5) && (plyr.chr != 0) )
	{
		// Check for a successful Charm attempt
		int chrIntDifference = plyr.chr - opponent.inte;
		int charmProbability;
		if (chrIntDifference <= -128) { charmProbability = 1; }
		if ( (chrIntDifference >= -128) && (chrIntDifference < -64) )  { charmProbability = 2; }
		if ( (chrIntDifference >= -64) && (chrIntDifference < 0) )  { charmProbability = 7; }
		if ( (chrIntDifference >= 0) && (chrIntDifference < 64) )  { charmProbability = 25; }
		if ( (chrIntDifference >= 64) && (chrIntDifference < 128) )  { charmProbability = 50; }
		if ( (chrIntDifference >= 128) && (chrIntDifference < 192) )  { charmProbability = 70; }
		if (chrIntDifference >= 192) { charmProbability = 80; }

		int hitRoll = randn(1,100);
		if (hitRoll <= charmProbability) { charmSuccess = true; }
	}
	//if (!charmSuccess) { // 50% test that future charm attempts always fail }
		if ( charmSuccess) { str = "You charmed it!"; }
		else { str = "You failed to charm it!"; }
		consoleMessage(str);

	if ( charmSuccess ) 
	{ 
	  checkForTreasure = true; 
	  Opponents[curOpponent].hp=0; 
	  awardExperience(opponentType); 
	  plyr.chrPartials++; 
	  if (plyr.chrPartials==255) 
	  { 
	  	 plyr.chr++; 
			plyr.chrPartials = 0; 
	   } 
		encounterRunning = false; 
	}

    // should be managed in processOpponent() turn
	//if ( !charmSuccess ) { opponentAttack(); encounterNotHostile = false; encounterMenu = 1; }
    if (!charmSuccess) { encounterNotHostile = false; encounterMenu = 1; playerTurn = false; }
}


void playerTrick()
{
    updateDisplay();
	encounterNotHostile = false;
	opponent = Opponents[0];
	if (whoStartedIt == 0) { whoStartedIt = 2; }
	checkAlignmentEncounter(curOpponent);
	bool trickSuccess = false;
	if ( (opponent.inte > 3) && (plyr.inte != 0) )
	{
		// Check for a successful trick attempt
		int intelligenceDifference = plyr.inte - opponent.inte;
		int trickProbability;
		if (intelligenceDifference <= -128) { trickProbability = 1; }
		if ( (intelligenceDifference >= -128) && (intelligenceDifference < -64) )  { trickProbability = 1; }
		if ( (intelligenceDifference >= -64) && (intelligenceDifference < 0) )  { trickProbability = 7; }
		if ( (intelligenceDifference >= 0) && (intelligenceDifference < 64) )  { trickProbability = 19; }
		if ( (intelligenceDifference >= 64) && (intelligenceDifference < 128) )  { trickProbability = 47; }
		if ( (intelligenceDifference >= 128) && (intelligenceDifference < 192) )  { trickProbability = 75; }
		if (intelligenceDifference >= 192) { trickProbability = 90; }

		int hitRoll = randn(1,100);
		if (hitRoll <= trickProbability) { trickSuccess = true; }
	}
	// TODO 50% test that future trick attempts always fail during this encounter
	if ( trickSuccess) { plyr.intPartials++; str = "You tricked it!"; }
	else { str = "You failed to trick it!"; }
	if (plyr.intPartials==255) { plyr.inte++; plyr.intPartials = 0; }
    consoleMessage(str);
	if ( trickSuccess ) {  checkForTreasure = true; Opponents[curOpponent].hp=0; awardExperience(opponentType); encounterMenu = 0; }

	// Move to opponent turn
	//if ( !trickSuccess ) { opponentAttack(); encounterNotHostile = false; encounterMenu = 1; }
    if (!trickSuccess) { encounterNotHostile = false; encounterMenu = 1; playerTurn = false;}

}


void playerAttack(int attackType, float attackFactorBonus)
{
	
 	bool missileWeapon = false;
	bool missileAmmoAvailable = false;
	bool hitSuccess;
	float attackFactor = 1.00;
	int weapon = plyr.priWeapon;
	bool hitAttempt = true;
	if (whoStartedIt == 0) { whoStartedIt = 2; }

	string weaponDesc = "ERROR";
	weaponDesc = itemBuffer[plyr.priWeapon].name;

	opponent = Opponents[0];
	checkAlignmentEncounter(curOpponent);
	
	//Now change to hostile..
	encounterNotHostile = false; // Opponent now hostile as they have been attacked
	if (attackType==3)
	{
			if (randn(1,5) > 3)
            {
                str = "You wait for an opening.";
                hitAttempt = false;
            }
    }

	// Check for missile weapon (e.g Crossbow)
	//melee = no of hands needed to use. for example Greatsword = 2 hands, Bow = 3 hands, Crossbow = 3 hands, dagger = 1
	if ((itemBuffer[plyr.priWeapon].melee > 2) && (hitAttempt))
    {
         missileWeapon = true;
        if (itemBuffer[plyr.priWeapon].ammo > 0)
        {
            missileAmmoAvailable = true;
            itemBuffer[plyr.priWeapon].ammo--;
            int remainingAmmo = itemBuffer[plyr.priWeapon].ammo;
			updateAmmoCount( plyr.priWeapon, remainingAmmo);
        }
        weaponDesc = itemBuffer[plyr.priWeapon].name;
    }

	if ((itemBuffer[weapon].useStrength==1))
	{
		//Calculate attack factor bonus if weapon uses Strength
		int strengthDifference = plyr.str - Opponents[0].str;
		if (strengthDifference <= -128) { attackFactorBonus += 0.0625; }
		if ( (strengthDifference >= -128) && (strengthDifference < -64) )  { attackFactorBonus += 0.125; }
		if ( (strengthDifference >= -64) && (strengthDifference < 0) )  { attackFactorBonus += 0.5; }
		if ( (strengthDifference >= 0) && (strengthDifference < 64) )  { attackFactorBonus += 0.75; }
		if ( (strengthDifference >= 64) && (strengthDifference < 128) )  { attackFactorBonus += 0.875; }
		if ( (strengthDifference >= 128) && (strengthDifference < 192) )  { attackFactorBonus += 1; }
		if (strengthDifference >= 192) { attackFactorBonus += 1; }
	}
	if (itemBuffer[plyr.priWeapon].melee == 2 && plyr.secWeapon == 0) { attackFactorBonus += 0.5; } // using two handed weapon two handed



	attackFactor += attackFactorBonus;

	// Hit probability
	int skillDifference = plyr.skl - Opponents[0].skl;
	int hitProbability;
	if (skillDifference <= -128) { hitProbability = 1; }
	if ( (skillDifference >= -128) && (skillDifference < -64) )  { hitProbability = 19; }
	if ( (skillDifference >= -64) && (skillDifference < 0) )  { hitProbability = 50; }
	if ( (skillDifference >= 0) && (skillDifference < 64) )  { hitProbability = 63; }
	if ( (skillDifference >= 64) && (skillDifference < 128) )  { hitProbability = 69; }
	if ( (skillDifference >= 128) && (skillDifference < 192) )  { hitProbability = 76; }
	if (skillDifference >= 192) { hitProbability = 100; }
	if (attackType==3) { hitProbability -= 5; } // penalty for charge attack
	int hitRoll = randn(1,100 -plyr.luck); //improved chance to hit
	if (hitRoll <= hitProbability) { hitSuccess = true; }
	else {  hitSuccess = false; }

	if ((!hitSuccess) && (hitAttempt))
	{
			str = "You miss.";
	}

	if ((missileWeapon) && (!missileAmmoAvailable))
    {
        str = itemBuffer[plyr.priWeapon].name + " not loaded.";
        hitSuccess = false;
    }

	if ((hitSuccess) && (hitAttempt))
	{
		plyr.strPartials++; // Each successful hit contributes towards an extra strength point
		if (plyr.strPartials==255) { plyr.str++; plyr.strPartials = 0; }

		int damage = calcPlayerWeaponDamage(weapon, attackFactor, 0);
//damage = 85; // debugging damage

        string attackDesc = "ERROR";
        attackDesc = getPlayerAttackDesc(damage);


		if (damage != 1000) { Opponents[0].hp-=damage; increaseExperience(damage/2); }

		//damage [type] = Random [0; Round (base weapon damage [type] * attack factor)]

        //str = prefix + Opponents[0].name + " " + attackDesc + "@" + bPartText + " with "+ weaponName + "@for " + itos(damage) + ".";
        //if (damage==0) { str = prefix + Opponents[0].name + " " + attackDesc + "@" + bPartText + " with "+ weaponName + "@which has no effect!"; }

		str = "You " + attackDesc + " the " + Opponents[0].name + "@with your "+ weaponDesc + "@for " + itos(damage) + ".";
		if (damage==0) { str = "You " + attackDesc + " the " + Opponents[0].name + "@with your "+ weaponDesc + "@which has no effect!"; }
		if ((plyr.scenario==0) && (damage==1000)) { str = "You attack the " + Opponents[0].name + "@with your "+ weaponDesc + "@which is stopped by its " + Opponents[0].armorText + "."; }
		if ((plyr.scenario==1) && (damage==1000)) { str = "You attack the " + Opponents[0].name + "@with your "+ weaponDesc + "@which is stopped by its armour."; }
    }

    consoleMessage(str);
  	attackCheckTime = sf::Time::Zero;

//    cout << Opponents[0].name << " health:" << Opponents[0].hp << ".\n";
	if ( Opponents[0].hp<1) {opponentDeath(0); }

	playerTurn = false;
}


std::string itos2(int i) {
	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << i;
	return ss.str();
}


void updateAmmoCount( int plyrWeaponIndex, int remainingAmmo) {
	std::string& itemName = itemBuffer[plyrWeaponIndex].name;
	size_t openBracketPos = itemName.find('[');
	size_t closeBracketPos = itemName.find(']', openBracketPos);

	if (openBracketPos != std::string::npos && closeBracketPos != std::string::npos) {
		std::string numberString = itos2(remainingAmmo);
		itemName.replace(openBracketPos + 1, closeBracketPos - openBracketPos - 1, numberString);
	}
}

/**
* @brief combat spell 
* @brief will only do something if the spell was a combat spell
* 
* 
*/
void playerCast(int spellNo)
{
	if (whoStartedIt == 0) { whoStartedIt = 2; }
	opponent = Opponents[0];

	if (spells[spellNo].spelltype != 2 && spells[spellNo].spelltype != 11 || encounterQuantity == 0  )
	{

		return;
	}
	
	
		checkAlignmentEncounter(curOpponent);
		encounterNotHostile = false; // Opponent now hostile as they have been attacked
		
		int damage = calcPlayerSpellDamage(spellNo, curOpponent);
		string attackDesc = "ERROR";
		
		if (damage != 1000)
		{
			//single opponent effected
			if (spells[spellNo].effect == 1)  //single opponent effected
			{
				// Damaging spell
				if (spells[spellNo].stattype == 0)  //Damaging spells
				{
					Opponents[0].hp -= damage; increaseExperience(damage / 2);
					if (damage == 0) { str = prefixes[0] + " " + Opponents[0].name + " resists your " + "@" + spells[spellNo].name; }
					else if (encounterQuantity > 1) { str = "The " + Opponents[0].name + " is hit for " + "@" + itos(damage) + " points of damage"; }  //AOE damage
					else { str = prefix + " " + Opponents[0].name + " is hit for " + "@" + itos(damage) + " points of damage"; }
					consoleMessage(str);
				}
				// non damaging spell
				else // if the the spell is a non damaging spell for example bewilder
				{
					switch(spells[spellNo].stattype)
					{

						case 1:
						encounterNotHostile = true;
						break;

						case 2:
						Opponents[0].stunnedTurnsRemaining = 2;
						Opponents[0].stunnedtext = spells[spellNo].castText;
						Opponents[0].spellBuffer[0] = spells[spellNo].no;
						break;

						case 3: 
						//Oponenent scared
							Opponents[0].braveness = -spells[spellNo].negativeValue;
						break;

						case 4: 
						// increases hit probability
						break;

						case 5:
						// decreases opponent chance to hit
						break;

						case 6:
						 // damage over time
						break;
					}
				}
			}
			//multiple opponents effected but based on Level
			else if (spells[spellNo].effect > 8)  //multiple opponents effected but based on Level
			{
				// Damaging spell
				if (spells[spellNo].stattype == 0)
				{
					string opponentsName = "";
					int smallerValue = (plyr.level < encounterQuantity) ? plyr.level : encounterQuantity;  //finds the smaller either player level or no of opponents
					if (Opponents[0].pluName == "" || encounterQuantity == 1) { opponentsName = Opponents[0].name + " is"; }
					else { opponentsName = Opponents[0].pluName + " are"; }
					if (damage == 0) { str = "The " + Opponents[0].name + " resists your " + "@" + spells[spellNo].name; }
					else { str = "The " + opponentsName + " hit for " + "@" + itos(damage) + " points of damage" + "@"; }
					consoleMessage(str);
					for (int i = 0; i < smallerValue; i++) {
						if (randn(0, Opponents[i].skl) < Opponents[i].skl / 10)
						{
							consoleMessage(prefixes[i] + " " + Opponents[i].name + " resists your " + "@" + spells[spellNo].name);
							Opponents[i].hp -= damage / 2; increaseExperience((damage / 2) / plyr.level);
						}
						else
						{
							consoleMessage(prefixes[i] + " " + Opponents[i].name + " takes full effect " + "@" + "from your " + spells[spellNo].name);
							Opponents[i].hp -= damage; increaseExperience((damage / 2) / plyr.level);
						}
					}
				}
				// non damaging spell
				else
				{
					switch (spells[spellNo].stattype)
					{

						case 1:
						encounterNotHostile = true;
						break;
					}
				}
			}
			//All opponents effected
			else 
			{
				// Damaging spell
				if (spells[spellNo].stattype == 0)
				{
					string opponentsName = "";
					if (Opponents[0].pluName == "" || encounterQuantity == 1) { opponentsName = Opponents[0].name; }else  { opponentsName = Opponents[0].pluName; }
				
					if (damage == 0) 
					{ 
						str = "The " + Opponents[0].name + " resists your " + "@" + spells[spellNo].name; 
					}
					else 
					{
						str = "The " + opponentsName + " is hit for " + "@" + itos(damage) + " points of damage" + "@"; 
					}
					consoleMessage(str);
					for (int i = 0; i < encounterQuantity; i++) {
						if (randn(0, Opponents[i].skl) < (Opponents[i].skl / 10))
						{
							consoleMessage(prefixes[i] + " " + Opponents[i].name + " resists your " + "@" + spells[spellNo].name);
							Opponents[i].hp -= damage / 2; increaseExperience((damage / 2) / plyr.level);
						}
						else
						{
							if (damage > 0)
							{
								consoleMessage(prefixes[i] + " " + Opponents[i].name + " takes full effect " + "@" + "from your " + spells[spellNo].name);
								Opponents[i].hp -= damage; increaseExperience((damage / 2) / plyr.level);
							}
						}
					}
				}
				// non damaging spell
				else
				{
					for (int i = 0; i < encounterQuantity; i++) {
						switch (spells[spellNo].stattype)
						{

						case 1:
							encounterNotHostile = true;
							break;

						case 2:
							Opponents[i].stunnedTurnsRemaining = spells[spellNo].duration;
							Opponents[i].stunnedtext = spells[spellNo].castText;
							consoleMessage(prefixes[i] + " " + Opponents[i].name + " takes full effect " + "@" + "from your " + spells[spellNo].name);
							break;

						case 3:
							//Oponenent scared
							Opponents[i].braveness = -spells[spellNo].negativeValue;
							break;

						case 4:
							// increases hit probability
							break;

						case 5:
							// decreases opponent chance to hit
							break;

						case 6:
							// damage over time
							break;
						}
					}
				}

			}
		}
		//consoleMessage(str);
		attackCheckTime = sf::Time::Zero;
 		for (int i = 0; i < encounterQuantity; i++) {
			if (Opponents[0].hp < 1) { opponentDeath(0); }
		}
	
	playerTurn = false;
}


void opponentDeath(int opponent)
{
    str = "The "  + Opponents[opponent].name + " " + Opponents[opponent].armorText + ".";
    consoleMessage(str);
	awardExperience(opponentType);
	removeOpponent();
    checkForTreasure = true;
	 whoStartedIt = 0;
}


void opponentAttack()
{
	bool opponentIsAttacking;
	bool hitSuccess;
	int ChooseWeapon = 0;
	if (whoStartedIt == 0) { whoStartedIt = 1; }


	// Choose which type of attack

	if (Opponents[0].s1 > 0)
	{
		// Can cast spells

		int OpponentCastSpell = randn(0,100);
	}

	if (Opponents[0].callForHelp == 1)
	{

 		opponentCallsForHelp();
	}





	int bPart = randn(0,5);
	string bPartText = "you";
	string weaponName = "ERROR";
    string attackDesc = "ERROR";
    if (bPart == 3) bPartText = "your head";
    if (bPart == 4) bPartText = "your arm";
    if (bPart == 5) bPartText = "your legs";

	//weapon opponentWeapon;
	opponentNoAttacking = curOpponent;
std::cout << "opponent #: " << curOpponent << "\n";
    opponentIsAttacking = true;

			//opponentNoAttacking++;

    if (opponentNoAttacking==0) prefix="The ";
    if (opponentNoAttacking==1) prefix="2nd ";
    if (opponentNoAttacking==2) prefix="3rd ";
    if (opponentNoAttacking==3) prefix="4th ";
    if (opponentNoAttacking==4) prefix="5th ";
    if (opponentNoAttacking==5) prefix="6th ";
    if (opponentNoAttacking==6) prefix="7th ";
    if (opponentNoAttacking==7) prefix="8th ";

	
	if (OpponentSwitchWeapons < 1 || OpponentSwitchWeapons >6)
	{
		int ChooseWeapon = opponentChooseWeapon();
		OpponentSwitchWeapons++;
	}

	if (Opponents[0].chosenWeapon != ChooseWeapon) //Logic for opponent to swicth weapons if they are doing no damage
	{
		OpponentSwitchWeapons = 0;
	}
	Opponents[0].chosenWeapon = ChooseWeapon;
		
		
	//}
	// Opponents[0].
    //int chosenWeapon = Opponents[0].w1;

	// need to add Spells to this for magic users etc

	// need to add 

    // Hit probability
    int skillDifference = Opponents[0].skl - plyr.skl;
    int hitProbability;

    if ( skillDifference    <= -128 )                                    { hitProbability = 1; }
    if ( (skillDifference   >= -128)  && (skillDifference < -64) )    { hitProbability = 19; }
    if ( (skillDifference   >= -64)   && (skillDifference < 0) )      { hitProbability = 50; }
    if ( (skillDifference   >= 0)     && (skillDifference < 64) )     { hitProbability = 63; }
    if ( (skillDifference   >= 64)    && (skillDifference < 128) )    { hitProbability = 69; }
    if ( (skillDifference   >= 128)   && (skillDifference < 192) )    { hitProbability = 76; }
    if ( skillDifference    >= 192 )                                     { hitProbability = 100; }

    int hitRoll = randn(1,100);
    if (hitRoll <= hitProbability) { hitSuccess = true; }
    else {  hitSuccess = false; }

    if (!hitSuccess)
    {
        str = prefix + Opponents[0].name + " misses.";
        // Add additional player dodges type messages here


    }

    if (hitSuccess)
    {
				//int damage = randn(1,6); // CHANGE!!!  3

			//Check for disease type from monster carrier value (to be added)
       
        float attackFactor = 1.0; //
        int damage = NewcalcOpponentWeaponDamage(Opponents[0].chosenWeapon,attackFactor, 1);

        if (opponentType==DOPPLEGANGER)
        {
            weaponName = itemBuffer[(Opponents[0].w1)].name;
            attackDesc = "hits";
        }
	  	  else if (opponentType == GHOST && randn(1,10) < 4)
	  	  {
	  	 	//There is a chance on a GHOST hit to take a point
	  	 	// away from STR
   	 		plyr.str--;
   	 		attackDesc = "touches";
   	 		weaponName = "Bone-Chilling Touch";
        } else  {
            weaponName = monsterWeapons[Opponents[0].chosenWeapon].name;
            //int weaponIndex = Opponents[0].w1;
std::cout << " weapon desc " << Opponents[0].chosenWeapon << "\n";
            attackDesc = getAttackDesc(Opponents[0].chosenWeapon, damage);
        }

        if (damage!=1000) { plyr.hp-=damage; }

        str = prefix + Opponents[0].name + " " + attackDesc + "@" + bPartText + " with "+ weaponName + "@for " + itos(damage) + ".";
        if (damage==0) 
		{ str = prefix + Opponents[0].name + " " + attackDesc + "@" + bPartText + " with "+ weaponName + "@which has no effect!";
		OpponentSwitchWeapons++;
		}
        //if (damage==1000) { str = prefix + opponent.name + " " + attackDesc + "@you with "+ opponentWeapon.name + "@which is stopped by your skin!"; }

		if (bPart == 3 && plyr.headArmour != 255) checkDurablility(plyr.headArmour, damage);
		if (bPart == 4 && plyr.armsArmour != 255) checkDurablility(plyr.armsArmour, damage);
		if (bPart == 5 && plyr.legsArmour != 255) checkDurablility(plyr.legsArmour, damage);
			
		if (opponentType == GIANT_RAT && damage >0) { if (plyr.diseases[0] == 0) plyr.diseases[0] = 1; } // Rabies
		if (opponentType == MOLD && damage > 0) { if (plyr.diseases[1] == 0) plyr.diseases[1] = 1; } // Mold
		if (opponentType == SLIME && damage > 0) { if (plyr.diseases[2] == 0) plyr.diseases[2] = 1; } // Fungus
		
    }

    consoleMessage(str);

    // Check if player is knocked down and PLAYER STILL ALIVE
    if ((hitSuccess) && (!(playerOnGround)) && (plyr.hp >= 0 ))
    {
        int grounded = randn(0,12);
        if (grounded < 1)
        {
            playerOnGround = true;
            groundTurnsRemaining = 2;
            consoleMessage("You have been knocked down.");
        }
    }

    // Check if player can rise from ground
    if ((playerOnGround) && (groundTurnsRemaining == 0 ) && (plyr.hp >= 0 ))
    {
        consoleMessage("You rise from the ground.");
        playerOnGround = false;

	}
    else groundTurnsRemaining--;


	//Check if player Stunned
	if ((hitSuccess) && (bPart==3) && (plyr.hp >= 0))
	{
 		int stunned = randn(0, 12);
		if (stunned < 1)
		{
			playerStunned = true;
			stunnedTurnsRemaining = 1;
			consoleMessage("You have been stunned.");
		}
		
	}
}


void opponentCallsForHelp()
{

	consoleMessage("The "+ Opponents[0].name + " Calls for help...");
	addOpponent();
	consoleMessage("Another " + Opponents[0].name + " Arrives");
	updateOpponents();
	
}


void opponentCast(int spellNo)
{
	//This is where the opponent spells go

	//Healing  (could also be use a potion of healing)

	// Spell list

	// 	* Spelltype 0 = stat increase
	//  * Spelltype 1 = Buff
	//    Blink   = single onetime boosts damage done
	// 
	//	* Spelltype 2 = Combat spell
	//	* Spelltype 3 = hidden stat increase
	//	* Spelltype 4 = Weapon enchant
	//	* Spelltype 5 = Misc
	
}


string getAttackDesc(int chosenWeapon, int damage)
{
    /*
        0x00 - hack/slash
        0x01 - poke/stab
        0x02 - bash/wallop
        0x03 - spear/impale
        0x04 - whip/lash
        0x05 - blast
        0x06 - punch/whomp
        0x07 - hit
    */

    /*
        claws
        kicks
        bites
        tears
        chokes
        lashes
        burns
        stings
        rips
        stomps
        gnaws
        rends
        asphyxiates
        wallops
        fries
        stings
    */

    string result = "hits";
    int value = (monsterWeapons[chosenWeapon].flags)&7;

    if (monsterWeapons[chosenWeapon].type == 0xFF)
    {
        if (value==0) result = "claws at";
        if (value==1) result = "kicks at";
        if (value==2) result = "bites";
        if (value==3) result = "tears at";
        if (value==4) result = "chokes";
        if (value==5) result = "lashes at";
        if (value==6) result = "burns";
        if (value==7) result = "stings";
        if (value>7) result = "greater than 7";

    }

    if (monsterWeapons[chosenWeapon].type == 0x03)
    {
        if (damage > 3)
        {
            if (value==0) result = "slashes at";
            if (value==1) result = "stabs at";
            if (value==2) result = "wallops";
            if (value==3) result = "impales";
            if (value==4) result = "lashes at";
            if (value==5) result = "blasts";
            if (value==6) result = "whomps";
            if (value==7) result = "hits";
            if (value>7) result = "greater than 7";
        }
        else
        {
            if (value==0) result = "hacks at";
            if (value==1) result = "pokes at";
            if (value==2) result = "bashes";
            if (value==3) result = "spears";
            if (value==4) result = "whips at";
            if (value==5) result = "blasts";
            if (value==6) result = "punches";
            if (value==7) result = "hits";
            if (value>7) result = "greater than 7";
        }

    }

    return result;
}


string getPlayerAttackDesc(int damage)
{
    /*
        0x00 - hack/slash
        0x01 - poke/stab
        0x02 - bash/wallop
        0x03 - spear/impale
        0x04 - whip/lash
        0x05 - blast
        0x06 - punch/whomp
        0x07 - hit
    */

    string result = "hit";
    int value = (itemBuffer[plyr.priWeapon].flags)&7;

    if (damage < 4)
    {
        if (value==0) result = "hack";
        if (value==1) result = "poke";
        if (value==2) result = "bash";
        if (value==3) result = "spear";
        if (value==4) result = "whip";
        if (value==5) result = "blast";
        if (value==6) result = "punch";
        if (value==7) result = "hit";
        if (value>7) result = "greater than 7";
    }
    else
    {
        if (value==0) result = "slash";
        if (value==1) result = "stab";
        if (value==2) result = "wallop";
        if (value==3) result = "impale";
        if (value==4) result = "lash";
        if (value==5) result = "blast";
        if (value==6) result = "whomp";
        if (value==7) result = "hit";
        if (value>7) result = "greater than 7";
    }


    return result;
}


int NewcalcOpponentWeaponDamage(int weaponNo, float attackFactor, int attacker)
{
	// CALCULATE MONSTER WEAPON / ATTACK DAMAGE

	// attacker - 1 = monster
	int weaponDamageValues[13];
	weaponDamageValues[0] = monsterWeapons[weaponNo].blunt;
	weaponDamageValues[1] = monsterWeapons[weaponNo].sharp;
	weaponDamageValues[2] = monsterWeapons[weaponNo].earth;
	weaponDamageValues[3] = monsterWeapons[weaponNo].air;
	weaponDamageValues[4] = monsterWeapons[weaponNo].fire;
	weaponDamageValues[5] = monsterWeapons[weaponNo].water;
	weaponDamageValues[6] = monsterWeapons[weaponNo].power;
	weaponDamageValues[7] = monsterWeapons[weaponNo].magic;
	weaponDamageValues[8] = monsterWeapons[weaponNo].good;
	weaponDamageValues[9] = monsterWeapons[weaponNo].evil;
	weaponDamageValues[10] = monsterWeapons[weaponNo].cold;
	weaponDamageValues[10] = monsterWeapons[weaponNo].nature;
	weaponDamageValues[10] = monsterWeapons[weaponNo].acid;
	if (opponentType == DOPPLEGANGER)
	{
		weaponDamageValues[0] = itemBuffer[weaponNo].blunt;
		weaponDamageValues[1] = itemBuffer[weaponNo].sharp;
		weaponDamageValues[2] = itemBuffer[weaponNo].earth;
		weaponDamageValues[3] = itemBuffer[weaponNo].air;
		weaponDamageValues[4] = itemBuffer[weaponNo].fire;
		weaponDamageValues[5] = itemBuffer[weaponNo].water;
		weaponDamageValues[6] = itemBuffer[weaponNo].power;
		weaponDamageValues[7] = itemBuffer[weaponNo].magic;
		weaponDamageValues[8] = itemBuffer[weaponNo].good;
		weaponDamageValues[9] = itemBuffer[weaponNo].evil;
		weaponDamageValues[10] = itemBuffer[weaponNo].cold;
		weaponDamageValues[11] = itemBuffer[weaponNo].nature;
		weaponDamageValues[12] = itemBuffer[weaponNo].acid;
	}

	int armorValues[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };
	// Need to add modifier for player armor values & armor body parts

	int armors[13]; // holds results of rolling for armor protection

	//Check to see if the user is wearing the entire armor suit.
	//If yes, they they get full protection,  otherwise no
	float numArmorPieces = 0;
	if (plyr.headArmour != 255)
		numArmorPieces++;
	if (plyr.bodyArmour != 255)
		numArmorPieces++;
	if (plyr.legsArmour != 255)
		numArmorPieces++;
	if (plyr.armsArmour != 255)
		numArmorPieces++;

	numArmorPieces = numArmorPieces / 4;
	armorValues[0] = itemBuffer[plyr.bodyArmour].blunt;
	armorValues[1] = itemBuffer[plyr.bodyArmour].sharp;
	armorValues[2] = itemBuffer[plyr.bodyArmour].earth;
	armorValues[3] = itemBuffer[plyr.bodyArmour].air;
	armorValues[4] = itemBuffer[plyr.bodyArmour].fire;
	armorValues[5] = itemBuffer[plyr.bodyArmour].water;
	armorValues[6] = itemBuffer[plyr.bodyArmour].power;
	armorValues[7] = itemBuffer[plyr.bodyArmour].magic;
	armorValues[8] = itemBuffer[plyr.bodyArmour].good;
	armorValues[9] = itemBuffer[plyr.bodyArmour].evil;
	armorValues[10] = itemBuffer[plyr.bodyArmour].cold;
	armorValues[11] = itemBuffer[plyr.bodyArmour].nature;
	armorValues[12] = itemBuffer[plyr.bodyArmour].acid;

	int armorIndex = 0;
	while (armorIndex < 11)
	{

		std::pair<int, int> dice = getDigits(armorValues[armorIndex]);
		int noDice = dice.first;
		int noSides = dice.second;

		noDice = ((int)noDice * numArmorPieces);
		noSides = ((int)noDice * numArmorPieces);
		if (noDice > 0 && noSides > 0)
		{
			armors[armorIndex] = rollDice(noDice, noSides);
		}
		else
		{
			armors[armorIndex] = 0;
		}
		armorIndex++;
	}



	int damages[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 }; // holds results of rolling for damage

	int damageIndex = 0; // 0 is blunt, 1 is sharp, 11 is cold - 11 damage types in total

	//    cout << "Opponent:";

	while (damageIndex < 11)
	{
		
		std::pair<int, int> dice = getDigits(weaponDamageValues[damageIndex]);
		int noDice = dice.first;
		int noSides = dice.second;

		//int currentDamage = round((weaponDamageValues[damageIndex])* attackFactor);
		if (noDice > 0 && noSides > 0)
		{
			damages[damageIndex] = rollDice(noDice, noSides);
			if (armorValues[damageIndex] == 0xff) { damages[damageIndex] = 0; }
			else if (armorValues[damageIndex] == 0xf0) { damages[damageIndex] = damages[damageIndex] * -1; }
			else if (armorValues[damageIndex] == 0x0f) { damages[damageIndex] = damages[damageIndex] * 2; }
			else {
				damages[damageIndex] -= armors[damageIndex];
				if (damages[damageIndex] < 0)
					damages[damageIndex] = 0;
			}


			//            cout << damages[damageIndex] << " (" << noDice << " D" << noSides << "), ";
		}
		else
		{
			damages[damageIndex] = 0;
		}
		damageIndex++;
	}

	
	//std::cout << "B:" << damages[0] << " S:" << damages[1] << " E:" << damages[2] << " A:" << damages[3] << " F:" << damages[4] << " W:" << damages[5] << " P:" << damages[6] << " M:" << damages[7] << " G:" << damages[8] << " E:" << damages[9] << " C:" << damages[10] << std::endl;
	int totalDamage = damages[0] + damages[1] + damages[2] + damages[3] + damages[4] + damages[5] + damages[6] + damages[7] + damages[8] + damages[9] + damages[10];
	return totalDamage;

}


/*
int calcOpponentWeaponDamage(int weaponNo, float attackFactor, int attacker)
{
    // CALCULATE MONSTER WEAPON / ATTACK DAMAGE

	// attacker - 1 = monster
	int weaponDamageValues[13];
 	weaponDamageValues[0] = monsterWeapons[weaponNo].blunt;
	weaponDamageValues[1] = monsterWeapons[weaponNo].sharp;
	weaponDamageValues[2] = monsterWeapons[weaponNo].earth;
	weaponDamageValues[3] = monsterWeapons[weaponNo].air;
	weaponDamageValues[4] = monsterWeapons[weaponNo].fire;
	weaponDamageValues[5] = monsterWeapons[weaponNo].water;
	weaponDamageValues[6] = monsterWeapons[weaponNo].power;
	weaponDamageValues[7] = monsterWeapons[weaponNo].magic;
	weaponDamageValues[8] = monsterWeapons[weaponNo].good;
	weaponDamageValues[9] = monsterWeapons[weaponNo].evil;
	weaponDamageValues[10] = monsterWeapons[weaponNo].cold;
	weaponDamageValues[10] = monsterWeapons[weaponNo].nature;
	weaponDamageValues[10] = monsterWeapons[weaponNo].acid;
	if (opponentType==DOPPLEGANGER)
    {
        weaponDamageValues[0] = itemBuffer[weaponNo].blunt;
        weaponDamageValues[1] = itemBuffer[weaponNo].sharp;
        weaponDamageValues[2] = itemBuffer[weaponNo].earth;
        weaponDamageValues[3] = itemBuffer[weaponNo].air;
        weaponDamageValues[4] = itemBuffer[weaponNo].fire;
        weaponDamageValues[5] = itemBuffer[weaponNo].water;
        weaponDamageValues[6] = itemBuffer[weaponNo].power;
        weaponDamageValues[7] = itemBuffer[weaponNo].magic;
        weaponDamageValues[8] = itemBuffer[weaponNo].good;
        weaponDamageValues[9] = itemBuffer[weaponNo].evil;
        weaponDamageValues[10] = itemBuffer[weaponNo].cold;
		weaponDamageValues[11] = itemBuffer[weaponNo].nature;
		weaponDamageValues[12] = itemBuffer[weaponNo].acid;
    }

	int armorValues[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };
	// Need to add modifier for player armor values & armor body parts

	int armors[13]; // holds results of rolling for armor protection

		//Check to see if the user is wearing the entire armor suit.
		//If yes, they they get full protection,  otherwise no
		float numArmorPieces = 0;
		if (plyr.headArmour != 255)
			numArmorPieces++;
		if (plyr.bodyArmour != 255)
			numArmorPieces++;
		if (plyr.legsArmour != 255 )
		   numArmorPieces++;
		if (plyr.armsArmour != 255)
		   numArmorPieces++;

      numArmorPieces = numArmorPieces / 4;
		armorValues[0] = itemBuffer[plyr.bodyArmour].blunt;
		armorValues[1] = itemBuffer[plyr.bodyArmour].sharp;
		armorValues[2] = itemBuffer[plyr.bodyArmour].earth;
		armorValues[3] = itemBuffer[plyr.bodyArmour].air;
		armorValues[4] = itemBuffer[plyr.bodyArmour].fire;
		armorValues[5] = itemBuffer[plyr.bodyArmour].water;
		armorValues[6] = itemBuffer[plyr.bodyArmour].power;
		armorValues[7] = itemBuffer[plyr.bodyArmour].magic;
		armorValues[8] = itemBuffer[plyr.bodyArmour].good;
		armorValues[9] = itemBuffer[plyr.bodyArmour].evil;
		armorValues[10] = itemBuffer[plyr.bodyArmour].cold;
		armorValues[11] = itemBuffer[plyr.bodyArmour].nature;
		armorValues[12] = itemBuffer[plyr.bodyArmour].acid;

		int armorIndex = 0;
		while (armorIndex < 11)
		{
			
			std::pair<int, int> dice = getDigits(armorValues[armorIndex]);
			int noDice = dice.first;
			int noSides = dice.second;

			noDice = ((int)noDice * numArmorPieces);
			noSides = ((int)noDice * numArmorPieces);
			if (noDice > 0 && noSides > 0)
			{
				armors[armorIndex] = rollDice(noDice, noSides);
			}
			else
			{
				armors[armorIndex] = 0;
			}
			armorIndex++;
		}



	int damages[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 }; // holds results of rolling for damage

	int damageIndex = 0; // 0 is blunt, 1 is sharp, 11 is cold - 11 damage types in total

//    cout << "Opponent:";

	while (damageIndex < 11)
	{

		std::pair<int, int> dice = getDigits(weaponDamageValues[damageIndex]);
		int noDice = dice.first;
		int noSides = dice.second;

		//int currentDamage = round((weaponDamageValues[damageIndex])* attackFactor);
		if (noDice > 0 && noSides >0)
		{
            damages[damageIndex] = rollDice(noDice, noSides);
            if (armorValues[damageIndex]==0xff) { damages[damageIndex] = 0; }
            else if (armorValues[damageIndex]==0xf0) { damages[damageIndex] = damages[damageIndex]*-1; }
            else if (armorValues[damageIndex]==0x0f) { damages[damageIndex] = damages[damageIndex]*2; }
				else {
				 damages[damageIndex] -= armors[damageIndex];
				 if (damages[damageIndex] < 0 )
				 	 damages[damageIndex] = 0;
				}
				 
				 	 
//            cout << damages[damageIndex] << " (" << noDice << " D" << noSides << "), ";
		}
		else
		{
			damages[damageIndex] = 0;
		}
		damageIndex++;
	}




	//std::cout << "B:" << damages[0] << " S:" << damages[1] << " E:" << damages[2] << " A:" << damages[3] << " F:" << damages[4] << " W:" << damages[5] << " P:" << damages[6] << " M:" << damages[7] << " G:" << damages[8] << " E:" << damages[9] << " C:" << damages[10] << std::endl;
	int totalDamage = damages[0]+damages[1]+damages[2]+damages[3]+damages[4]+damages[5]+damages[6]+damages[7]+damages[8]+damages[9]+damages[10] + damages[11] + damages[12];
	return totalDamage;

}
*/

int calcPlayerSpellDamage(int spellNo, int attacker)
{
    // CALCULATE PLAYER WEAPON / ATTACK DAMAGE



	std::bitset<13> binarySpells(spells[spellNo].elementtype);

	updateAttack(binarySpells, spells[spellNo].positiveValue);

	int armorValues[noOfElements] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };
    int armors[noOfElements] = { 0,0,0,0,0,0,0,0,0,0,0,0 };

	if (attacker==0) // Player attacking
	{
		armorValues[0] = Opponents[0].aBlunt;
		armorValues[1] = Opponents[0].aSharp;
		armorValues[2] = Opponents[0].aEarth;
		armorValues[3] = Opponents[0].aAir;
		armorValues[4] = Opponents[0].aFire;
		armorValues[5] = Opponents[0].aWater;
		armorValues[6] = Opponents[0].aPower;
		armorValues[7] = Opponents[0].aMagic;
		armorValues[8] = Opponents[0].aGood;
		armorValues[9] = Opponents[0].aEvil;
		armorValues[10] = Opponents[0].aCold;
		armorValues[11] = Opponents[0].aNature;
		armorValues[12] = Opponents[0].aAcid;


		int armorIndex = 0;
		while (armorIndex < 12)
		{
			
			std::pair<int, int> dice = getDigits(armorValues[armorIndex]);
			int noDice = dice.first;
			int noSides = dice.second;

			if (noDice > 0 ) armors[armorIndex] = rollDice(noDice, noSides);

			armorIndex++;
		}
	}


	int damages[noOfElements] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 }; // holds results of rolling for damage

	int damageIndex = 0; // 0 is blunt, 1 is sharp, 11 is cold - 13 damage types in total

	cout << "Player Damage:";
std::cout << "B:" << spellDamageValues[0] << " S:" << spellDamageValues[1] << " E:" << spellDamageValues[2] << " A:" << spellDamageValues[3] << " F:" << spellDamageValues[4] << " W:" << spellDamageValues[5] << " P:" << spellDamageValues[6] << " M:" << spellDamageValues[7] << " G:" << spellDamageValues[8] << " E:" << spellDamageValues[9] << " C:" << spellDamageValues[10] << std::endl;
	while (damageIndex < noOfElements)
	{
		std::pair<int, int> dice = getDigits(spellDamageValues[damageIndex]);
		int noDice = dice.first;
		int noSides = dice.second;

		//int currentDamage = round((weaponDamageValues[damageIndex])* attackFactor);
		if (noDice > 0 || noSides > 0)
        {
            damages[damageIndex] = rollDice(noDice, noSides);
			
            if (armorValues[damageIndex]==0xff) { damages[damageIndex] = 0; }  // if armorValue for specif element is 255 then invunerable
            else if (armorValues[damageIndex]==0xf0) 
			{ 
				damages[damageIndex] = damages[damageIndex]*-1; 
				str = opponent.name + " Resist the damage";
			} //If 240 the absorbs damage
            else if (armorValues[damageIndex]==0x0f) 
			{ 
				damages[damageIndex] = damages[damageIndex]*2; 
				str = opponent.name + " Appears strengthened by the attack";
			} // If 15 then double damage
            else 
			{
				 damages[damageIndex] -= armors[damageIndex];
				 if (damages[damageIndex] < 0 )
				 	 damages[damageIndex] = 0;
			}

           cout << damages[damageIndex] << " (" << noDice << " D" << noSides << ") \n";
        }
		damageIndex++;
	}
//	cout << "\n";

	// Compare weapon damages against encounter armour values inc. vulnerabilities and invulnerabilities
	// 0xff = invulnerable.
	// 0xf0 = absorbs power from this damage type.
	// 0x0f = takes double damage from this damage type.




std::cout << "Base:" << spells[spellNo].baseDamage << " B:" << damages[0] << " S:" << damages[1] << " E:" << damages[2] << " A:" << damages[3] << " F:" << damages[4] << " W:" << damages[5] << " P:" << damages[6] << " M:" << damages[7] << " G:" << damages[8] << " E:" << damages[9] << " C:" << damages[10] << " N:" << damages[11] << " AC:" << damages[12] << std::endl;
	int totalDamage = damages[0]+damages[1]+damages[2]+damages[3]+damages[4]+damages[5]+damages[6]+damages[7]+damages[8]+damages[9]+damages[10]+damages[11] +damages[12] + spells[spellNo].baseDamage;
	return totalDamage;

}


int calcPlayerWeaponDamage(int weaponNo, float attackFactor, int attacker)
{
	// CALCULATE PLAYER WEAPON / ATTACK DAMAGE

	// attacker - 0 = player
	int weaponDamageValues[13];
	
 		weaponDamageValues[0] = itemBuffer[weaponNo].blunt + weaponBonus.blunt;
		weaponDamageValues[1] = itemBuffer[weaponNo].sharp + weaponBonus.sharp;
		weaponDamageValues[2] = itemBuffer[weaponNo].earth + weaponBonus.earth;
		weaponDamageValues[3] = itemBuffer[weaponNo].air + weaponBonus.air;
		weaponDamageValues[4] = itemBuffer[weaponNo].fire + weaponBonus.fire;
		weaponDamageValues[5] = itemBuffer[weaponNo].water + weaponBonus.water;
		weaponDamageValues[6] = itemBuffer[weaponNo].power + weaponBonus.power;
		weaponDamageValues[7] = itemBuffer[weaponNo].magic + weaponBonus.magic;
		weaponDamageValues[8] = itemBuffer[weaponNo].good + weaponBonus.good;
		weaponDamageValues[9] = itemBuffer[weaponNo].evil + weaponBonus.evil;
		weaponDamageValues[10] = itemBuffer[weaponNo].cold + weaponBonus.cold;
		weaponDamageValues[11] = itemBuffer[weaponNo].nature + weaponBonus.nature;
		weaponDamageValues[12] = itemBuffer[weaponNo].acid + weaponBonus.acid;


	int armorValues[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int armors[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };

	if (attacker == 0) // Player attacking
	{
		armorValues[0] = Opponents[0].aBlunt;
		armorValues[1] = Opponents[0].aSharp;
		armorValues[2] = Opponents[0].aEarth;
		armorValues[3] = Opponents[0].aAir;
		armorValues[4] = Opponents[0].aFire;
		armorValues[5] = Opponents[0].aWater;
		armorValues[6] = Opponents[0].aPower;
		armorValues[7] = Opponents[0].aMagic;
		armorValues[8] = Opponents[0].aGood;
		armorValues[9] = Opponents[0].aEvil;
		armorValues[10] = Opponents[0].aCold;
		armorValues[11] = Opponents[0].aNature;
		armorValues[12] = Opponents[0].aAcid;

		int armorIndex = 0;
		while (armorIndex < 13)
		{
		
			std::pair<int, int> dice = getDigits(armorValues[armorIndex]);
			int noDice = dice.first;
			int noSides = dice.second;

			if (noDice > 0) armors[armorIndex] = rollDice(noDice, noSides);

			armorIndex++;
		}
	}



	int damages[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 }; // holds results of rolling for damage

	int damageIndex = 0; // 0 is blunt, 1 is sharp, 11 is cold - 13 damage types in total

	cout << "Player Damage:";
	std::cout << "B:" << weaponDamageValues[0] << " S:" << weaponDamageValues[1] << " E:" << weaponDamageValues[2] << " A:" << weaponDamageValues[3] << " F:" << weaponDamageValues[4] << " W:" << weaponDamageValues[5] << " P:" << weaponDamageValues[6] << " M:" << weaponDamageValues[7] << " G:" << weaponDamageValues[8] << " E:" << weaponDamageValues[9] << " C:" << weaponDamageValues[10] << std::endl;
	while (damageIndex < 11)
	{
		//		int noDice = (weaponDamageValues[damageIndex] & 0xf0) >> 4;
		//		int noSides = (weaponDamageValues[damageIndex] & 0x0f);
		//int noDice = ((int)weaponDamageValues[damageIndex] / 10);
		//int noDice = ((int)left(weaponDamageValues[damageIndex],1))
		
		std::pair<int, int> dice = getDigits(weaponDamageValues[damageIndex]);
		int noDice = dice.first;
		int noSides = dice.second;

		//int currentDamage = round((weaponDamageValues[damageIndex])* attackFactor);
		if (noDice > 0 || noSides > 0)
		{
			damages[damageIndex] = rollDice(noDice, noSides);
			if (armorValues[damageIndex] == 0xff) { damages[damageIndex] = 0; }
			else if (armorValues[damageIndex] == 0xf0) { damages[damageIndex] = damages[damageIndex] * -1; }
			else if (armorValues[damageIndex] == 0x0f) { damages[damageIndex] = damages[damageIndex] * 2; }
			else {
				damages[damageIndex] -= armors[damageIndex];
				if (damages[damageIndex] < 0)
					damages[damageIndex] = 0;
			}

			cout << damages[damageIndex] << " (" << noDice << " D" << noSides << ") \n";
		}
		damageIndex++;
	}
	//	cout << "\n";

		// Compare weapon damages against encounter armour values inc. vulnerabilities and invulnerabilities
		// 0xff = invulnerable.
		// 0xf0 = absorbs power from this damage type.
		// 0x0f = takes double damage from this damage type.

	if (attacker == 0) // Player attacking
	{
		for (int i = 0; i < 11; ++i) // number of damage slots to compare against armour slots
		{
			if (armors[i] == 0xf0)	// Double damage
				damages[i] *= 2;
			else if (armors[i] == 0xff)	// Invulnerable to this weapon
				damages[i] = 0;
			else
				damages[i] -= armors[i];
			if (damages[i] < 0) { damages[i] = 0; }
		}
	}


	std::cout << "B:" << damages[0] << " S:" << damages[1] << " E:" << damages[2] << " A:" << damages[3] << " F:" << damages[4] << " W:" << damages[5] << " P:" << damages[6] << " M:" << damages[7] << " G:" << damages[8] << " E:" << damages[9] << " C:" << damages[10] << std::endl;
	int totalDamage = damages[0] + damages[1] + damages[2] + damages[3] + damages[4] + damages[5] + damages[6] + damages[7] + damages[8] + damages[9] + damages[10];
	return totalDamage;

}


int opponentChooseWeapon()
{
	//c = the probability of each weapopn

	

	weaponProbabilities[0] = Opponents[0].c1;
	weaponProbabilities[1] = Opponents[0].c2;
	weaponProbabilities[2] = Opponents[0].c3;
	weaponProbabilities[3] = Opponents[0].c4;
	weaponProbabilities[4] = Opponents[0].c5;
	weaponProbabilities[5] = Opponents[0].c6;


	weaponProbabilities[Opponents[0].chosenWeapon] - OpponentSwitchWeapons;


std::cout <<" Weapon chose " << Opponents[0].c1 << " 1 " << Opponents[0].c2 << " 2 " << Opponents[0].c3 << "\n";


	weaponReferences[0] = Opponents[0].w1;
	weaponReferences[1] = Opponents[0].w2;
	weaponReferences[2] = Opponents[0].w3;
	weaponReferences[3] = Opponents[0].w4;
	weaponReferences[4] = Opponents[0].w5;
	weaponReferences[5] = Opponents[0].w6;

std::cout <<" Weapon chose " << Opponents[0].w1 << " 1 " << Opponents[0].w2 << " 2 " << Opponents[0].w3 << "\n";

	int chosenWeaponNo = 255;
	int weaponProbability = randn(1,100);
	int weaponProbabilityTotal = 0;
	int weaponIndex = 0;
	//int weaponIndexProbability = 0;
	while (chosenWeaponNo == 255)
	{
		weaponProbabilityTotal += weaponProbabilities[weaponIndex];
		if ( weaponProbability <= weaponProbabilityTotal ) { chosenWeaponNo = weaponIndex; }
		if ( weaponIndex == 6) { chosenWeaponNo = 0; } // error weapon!
		weaponIndex++;
	}
std::cout <<" Weapon chose " << chosenWeaponNo << " name " << monsterWeapons[chosenWeaponNo].name << "\n";
	OpponentSwitchWeapons = 1;
	return chosenWeaponNo;
}


void initMonster(int monsterNo)
{
    // Initialise a SINGLE monster - legacy City logic

    /*
    Opponents[a] = Monster_Buffer[monsterNo]; // copy monster details to current Opponents[a] object
    Opponents[a].hp += randn(0,Monster_Buffer[monsterNo].randomHP);
    Opponents[a].str += randn(0,Monster_Buffer[monsterNo].randomStrength);
    Opponents[a].skl += randn(0,Monster_Buffer[monsterNo].randomSkill);
    Opponents[a].inte += randn(0,Monster_Buffer[monsterNo].randomIntelligence);
    Opponents[a].spd += randn(0,Monster_Buffer[monsterNo].randomSpeed);
    Opponents[a].maxHP = Opponents[a].hp;

    if (monsterNo==DOPPLEGANGER)
    {
        // Doppelganger
        Opponents[a].hp = plyr.hp;
        Opponents[a].maxHP = plyr.hp;
        Opponents[a].str = plyr.str;
        Opponents[a].skl = plyr.skl;
        Opponents[a].inte = plyr.inte;
        Opponents[a].spd = plyr.speed;
        Opponents[a].w1 = itemBuffer[plyr.priWeapon].index;
        if (plyr.priWeapon==255) Opponents[a].w1=0; // bare hands
        Opponents[a].c1 = 100;
    }
    */

}


void thiefYield()
{
	if ((plyr.gold==0) && (plyr.silver==0) && (plyr.copper==0) && (plyr.gems==0) && (plyr.jewels==0))
	{
		str = "\"Thou pitiful fool! Have a copper.\"";
		plyr.copper++;
	}
	else
	{
		str = "\"Farewell pidgeon.\"";
		plyr.gold = 0; plyr.silver = 0; plyr.copper = 0; plyr.gems = 0; plyr.jewels = 0;
	}
    consoleMessage(str);
	encounterRunning = false;
}


void healerCureDiseases()
{
    // Clear all the disease flags
	if (!checkCoins(0,100,0))
	{
		// Insufficient funds
		plyr.gold = 0;
		plyr.silver = 0;
		plyr.copper = 0;
        consoleMessage("The Healer grumbles:@@Thy purse is too small, however...");
	}
	else
	{
		deductCoins(0,100,0);
	}
    consoleMessage("The Healer lays his@hands upon you.");
    if (plyr.diseases[0] > 0)			// Rabies
	     plyr.diseases[0] = 0;
    if (plyr.diseases[1] > 0)			// Mold
	     plyr.diseases[1] = 0;
    if (plyr.diseases[2] > 0)			// Fungus
	     plyr.diseases[2] = 0;
	opponentLeaves();
}


void healerCurePoisons()
{
	// Clear all the poison flags
	if (!checkCoins(0,50,0))
	{
		// Insufficient funds
		plyr.gold = 0;
		plyr.silver = 0;
		plyr.copper = 0;
			consoleMessage("The Healer grumbles:@@Thy purse is too small, however...");
	}
	else
	{
		deductCoins(0,50,0);
	}
	consoleMessage("The Healer lays his@hands upon you.");
    plyr.poison[0] = 0; plyr.poison[1] = 0; plyr.poison[2] = 0; plyr.poison[3] = 0;
	opponentLeaves();
}


int inputNumber()
{
	updateDisplay(); // Messy visual fix before the display loop below - probably break animation later
	string str;
	string inputText = "";
	int maxNumberSize = 6;
	bool enterKeyNotPressed = true;
	while ( enterKeyNotPressed )
	{
		drawEncounterView();
		cyText(3, "Cure how many hits at@@ 1 silver each?");
		str = ">" + inputText + "_";
		bText(10,7, str);
		updateDisplay();
		key = getSingleKey();
		if ( (key=="0") || (key=="1") || (key=="2") || (key=="3") || (key=="4") || (key=="5") || (key=="6") || (key=="7") || (key=="8") || (key=="9") )
		{
			int numberLength = inputText.size();
			if (numberLength < maxNumberSize) { inputText = inputText + key; }
		}
		if (key=="BACKSPACE")
		{
			int numberLength = inputText.size();
			if (numberLength!=0)
			{
			  int numberLength = inputText.size();
			  inputText = inputText.substr(0,(numberLength-1));
			}
		}
		if (key=="RETURN") { enterKeyNotPressed = false; }
	}
	int value = atoi(inputText.c_str());
	return value;
}


void healerHealWounds()
{
	int hpToHeal = inputNumber();
	bool keynotpressed = true;

	if (hpToHeal>0)
	{
		keynotpressed = true;
		if (!checkCoins(0,hpToHeal,0))
		{
			// Insufficient funds
			plyr.gold = 0;
			plyr.silver = 0;
			plyr.copper = 0;
            consoleMessage("The Healer grumbles:@@Thy purse is too small, however...");
		}
		else
		{
			deductCoins(0,hpToHeal,0);
		}
		consoleMessage("The Healer lays his@hands upon you.");
		plyr.hp = plyr.hp + hpToHeal;
		if (plyr.hp > plyr.maxhp) { plyr.hp = plyr.maxhp; }
		opponentLeaves();
	}
}


void pauseEncounter()
{
   consoleMessage("(Paused)@@@@@(Press SPACE to continue)");
}


void awardExperience(int opponentNo)
{
	// x2 is default experience multiplier value for defeating an opponent in the Dungeon
	int experienceMultiplier = 2;

	if (opponentNo==GHOST)			experienceMultiplier = 8;
	if (opponentNo==DOPPLEGANGER)	experienceMultiplier = 7;
	if (opponentNo==MOLD)			experienceMultiplier = 3;

	int opponentXP = Opponents[0].maxHP*experienceMultiplier;
	increaseExperience(opponentXP);
}


void text(string str)
{
	bool keynotpressed = true;
	while ( keynotpressed )
	{
		drawEncounterView();
		cyText(4, str);
		updateDisplay();
		key = getSingleKey();
		if (key!="") { keynotpressed = false; }
	}
}


void checkFixedEncounters()
{
    // Check for guards outside bank vaults
    if (plyr.stolenFromVault==2)
    {
        // Check for Palace Elite as well
        encounterLoop(GUARD,3);
    }

	switch(plyr.special)
    {
		case 1001: // troll tyrant & gang of 8 trolls
        if ((plyr.trollsDefeated == false) && (plyr.trollsCombat))
        {
            plyr.trollsCombat = false;
            plyr.status = ENCOUNTER;
            encounterLoop(TROLL_TYRANT,1);
            if ( plyr.trollsDefeated ) { plyr.status = ENCOUNTER; encounterLoop(TROLL,8); }
        }
        if ((plyr.trollsDefeated) && (plyr.trollsCombat))
        {
            plyr.trollsCombat = false;
            plyr.status = ENCOUNTER;
            encounterLoop(TROLL,8);
        }
        break;

		case 1002: // goblin lord & gang of 8 goblins
        if ((plyr.goblinsDefeated == false) && (plyr.goblinsCombat))
        {
            plyr.goblinsCombat = false;
            plyr.status = ENCOUNTER;
            encounterLoop(GOBLIN_LORD,1);
            if ( plyr.goblinsDefeated ) { plyr.status = ENCOUNTER; encounterLoop(GOBLIN,8); }
        }
        if ((plyr.goblinsDefeated) && (plyr.goblinsCombat))
        {
            plyr.goblinsCombat = false;
            plyr.status = ENCOUNTER;
            encounterLoop(GOBLIN,8);
        }
        break;
	}



	if ((plyr.special>0x79) && (plyr.special<0xA0)) // Dungeon only just now
	{
		plyr.fixedEncounterRef = plyr.special-128;
		if (plyr.fixedEncounters[plyr.fixedEncounterRef] == false)
        {
            plyr.fixedEncounter = true;
            switch(plyr.special)
            {
                case 0x80:  encounterLoop(UNDEAD_KNIGHT,1); break;
                case 0x81:  encounterLoop(UNDEAD_KNIGHT,1); break;
                case 0x82:  encounterLoop(UNDEAD_KNIGHT,1); break;
                case 0x83:  encounterLoop(UNDEAD_KNIGHT,1); break;
                case 0x84:  encounterLoop(UNDEAD_KNIGHT,1); break;
                case 0x85:  encounterLoop(UNDEAD_KNIGHT,1); break;
                case 0x86:  encounterLoop(UNDEAD_KNIGHT,1); break;
                case 0x87:  encounterLoop(BASILISK,1); break;
                case 0x88:  encounterLoop(DOPPLEGANGER,1); break;
                case 0x89:  encounterLoop(LICH,1); break;
                case 0x8A:  encounterLoop(VALKYRIE,1); break;
                case 0x8B:  encounterLoop(GREAT_NAGA,1); break;
                case 0x8C:  encounterLoop(WRAITH,3); break;
                case 0x8D:  encounterLoop(FLAME_DEMON,4); break;
                case 0x8E:  encounterLoop(DWARF,7); break;
                case 0x8F:  encounterLoop(VAMPIRE,8); break;
                case 0x90:  encounterLoop(WHIRLWIND,1); break;
                case 0x91:  encounterLoop(SMALL_DRAGON,1); break;
                case 0x92:  encounterLoop(GIANT_WOLF,3); break;
                case 0x93:  encounterLoop(DOPPLEGANGER,1); break;
                case 0x94:  encounterLoop(37,8); break;
                case 0x95:  encounterLoop(HOMUNCULUS,8); break;
                case 0x96:  encounterLoop(SKELETON,8); break;
                case 0x97:  encounterLoop(PHOENIX,1); break;
                case 0x98:  encounterLoop(GHOST,1); break;
                case 0x99:  encounterLoop(FLAME_DEMON,4); break;
                case 0x9A:  encounterLoop(VALKYRIE,1); break;
                case 0x9B:  encounterLoop(HORNED_DEVIL,1); break;
                case 0x9C:  encounterLoop(VAMPIRE,1); break;
                case 0x9D:  encounterLoop(SMALL_DRAGON,1); break;
                case 0x9E:  encounterLoop(DOPPLEGANGER,8); break;
                case 0x9F:  encounterLoop(DEVOURER,1); break;
            }
        }
	}

}


void selectEncounterTheme()
{
    //int opponentAlignment = Monster_Buffer[opponentNo].alignment;
    if ((plyr.scenario==0) && (Opponents[0].alignment > 127))  { playEncounterTheme(0); }
    if ((plyr.scenario==0) && (Opponents[0].alignment < 128))  { playEncounterTheme(1); }
    if ((plyr.scenario==1) && (Opponents[0].alignment < 128))  { playEncounterTheme(2); }
    if ((plyr.scenario==1) && (Opponents[0].alignment == 128)) { playEncounterTheme(3); }
    if ((plyr.scenario==1) && (Opponents[0].alignment > 128))  { playEncounterTheme(4); }
}


void updateEncounterStatusText()
{

	// Draw status line text
	string text;
	string turnText; // encounter or face
	//SetFontColour(168, 241, 154, 255);
	SetFontColour(111, 159, 6, 255);
    if (encounterTurns==0) turnText = "encounter"; else turnText = "face";


	if (encounterQuantity==1)
	{
			if (
			(Opponents[0].name.substr (0,1)=="A") ||
			(Opponents[0].name.substr (0,1)=="O") ||
			(Opponents[0].name.substr (0,1)=="I") ||
			(Opponents[0].name.substr (0,1)=="o") ||
			(Opponents[0].name.substr (0,1)=="U") ||
			(Opponents[0].name.substr (0,1)=="a") ||
			(Opponents[0].name.substr (0,1)=="i"))
			{text = "You encounter an "+Opponents[0].name + ".";;}
			else { text = "You "+turnText+" a "+Opponents[0].name + "."; }
	}
	if (encounterQuantity>1) { text = "You "+turnText+ " "+ itos(encounterQuantity) + " " +Opponents[0].pluName + "."; }
    int length = text.size();
    int xpos = (40 - length)/2;
    //int y = (statPanelY/16)-1;
	drawText(xpos,5,text);
	SetFontColour(215, 215, 215, 255);
}


void checkEncounter()
{
	int Suprisecheck = (plyr.stealth - plyr.noticeability) - plyr.alcohol ;  // Alcohol higer the more drunk, Stealth = camo more the better, noticebilty the noiec your making
    if (encounterThemeNotPlaying())
    {
		if (Suprisecheck < 0) Suprisecheck = 1;
       int encounter_check = randn(0, Suprisecheck);  //a little different from the City

		if (encounter_check == 0)  //character is suprised
		{
			chooseEncounter();  //
		}
	


    }
}


void chooseEncounter()
{
	int monsterNo = 0;
	int numMonsters = 1;
    plyr.status = ENCOUNTER;

	

    // CITY - Day
	if ((plyr.timeOfDay!=1) && (plyr.scenario==CITY))
	{
		int encCount = 0;
		int monsterProb = randn(0, 255);
		//int monsterProb = 253;
		for(int i = 0; i < maxDayEncTable; ++i) // 28???
		{
			if ((monsterProb >= encCount) && (monsterProb < dayEncTable[i].encProb+encCount))
			{ monsterNo = dayEncTable[i].encType; }
			encCount = encCount+dayEncTable[i].encProb;
		}
	}

	// CITY - Night
	if ((plyr.timeOfDay==1) && (plyr.scenario==CITY))
	{
		int encCount = 0;
		int monsterProb = randn(0, 255);
		for(int i = 0; i < maxNightEncTable; ++i)
		{
			if ((monsterProb >= encCount) && (monsterProb < nightEncTable[i].encProb+encCount))
			{ monsterNo = nightEncTable[i].encType; }
			encCount = encCount+nightEncTable[i].encProb;
		}
	}

	if ((plyr.scenario==DUNGEON) && ((plyr.zone == 17) || (plyr.zone == 16)))  // Dungeon - Well Lit Area
	{
		monsterNo = randomEncounterPicker(plyr.zone, plyr.timeOfDay, maps[plyr.map].encounterIndex);
	}

	if ((plyr.scenario==DUNGEON) && !((plyr.zone == 17) || (plyr.zone == 16)))
    {
		monsterNo = randomEncounterPicker(plyr.zone, plyr.timeOfDay, maps[plyr.map].encounterIndex);
    }

	if (plyr.scenario != DUNGEON || plyr.scenario != CITY)
	{
		monsterNo = randomEncounterPicker(plyr.zone, plyr.timeOfDay, maps[plyr.map].encounterIndex);

	}

	if ((plyr.scenario==DUNGEON) && (plyr.map==4)) monsterNo = 19;
// cout << "Monster: "<< monsterNo << " " << Monster_Buffer[monsterNo].name << "\n";
	if (HackedMonsters == 1) monsterNo = 8;
    




    plyr.fixedEncounter = false;

	 //adjust the max encounter based on other player stats.
	 if (plyr.scenario==DUNGEON)
	 {
	 	numMonsters = Monster_Buffer[monsterNo].maxencounters;
	 	int adjustment =  (plyr.level+1) /2;	//increase the chances of more encounters for higher levels.
	 	if (plyr.alignment < 128 && Monster_Buffer[monsterNo].alignment> 127)	//Good encounter vs. evil player.  Make it tougher
	 	 adjustment ++;
	 	if (plyr.alignment < 20 )	//Pure evil player
	 	 adjustment ++;
	 	 adjustment += plyr.map - 1;

		 //Grab the lower of the two
	 	 if (adjustment < numMonsters )
	 	 {
	 	 	 if (adjustment < 1)
	 	 	 	 numMonsters = 1;
	 	 	 else
	 	 	 	  numMonsters = adjustment;
		  }
		  numMonsters = randn(1,numMonsters);	// Grab a random number of monsters for the dungeon		  
	 	
	 }

std::cout << "Monsterno " << monsterNo;

    encounterLoop( monsterNo, numMonsters ); // Only one currently except for fixed encounters


    //if (checkForTreasure) checkTreasure();
    // FBI Agent and Basilisk images missing
}


void checkAlignmentEncounter(int opponentNo)
{
	// Attacking, trick or charming  certain opponents is an evil act.
	//  need to adjust the alignment to reflect this evil act.
	
//std::cout << "checkAlignmentEncounter   oppn align " << opponent.alignment << " ";	
	if (opponent.alignment < 128)
		return;		//  No harm comes to attacking evil opponents.
		
	if (encounterNotHostile && opponent.alignment > 128)
	{
  			    plyr.alignment -= 5;	//Evil act.  Lower = Evil
  			    
  			    if (plyr.alignment > 130)  //Not sure why theres a maximum check here as we only decrease the alignment?
  			    	 plyr.alignment = 100;
  			    	 
			   if (plyr.alignment < 0) 	 
			   	plyr.alignment = 0;
	
	}	
//std::cout << "plyr align " << plyr.alignment << " opn# " << opponentNo << "\n";	

}

// Logic to be used
// randomly pick from 0-100 
// need a way of setting the rand adjustment per item for each opponent
// seems wrong that every monster hs the same chance but different quantties
// 
int checkForTreasureItem(int treasure,int itemno, int randAdjustment)
{
	int upperRange = 100;		// Reduced it down, since Treasure should be at 15 most of the time Not sure about this comment
   
	int no_found = randn(0,treasure); //number of items found
	int found = randn(0,upperRange);  //percentage rate
	if ((no_found>0) && (found+randAdjustment <= plyr.treasureFinding)) 
	{ 
	  createGenericItem(itemno,no_found); 
	  foundTreasure = true; 
  } 	else 
		  no_found = 0;
//cout << "\n" << found << " , " << no_found << "\n";

    return no_found;
}


void checkTreasure()
{
	bool foundTreasure = false;
	int found = 0;
	int tot_found =  0;
	int max_treasure = 20;
	int weapon = 0;
   
	// Check to see if the opponent was carrying a weapon (as opposed to claws or teeth)
	// Only type 0x03 weapons can be dropped - type 0xFF refers to natural weapons such as bites, tails, claws, spells
	
	
    Opponents[0] = Monster_Buffer[opponentType];

	
	found = randn(0,100);

	// need to work out a way of specifying what a monster can drop
	// need to add a treasure finding filter for each scenario
	//


	if (monsterWeapons[opponent.chosenWeapon].type == 178 && found <= plyr.treasureFinding)
	{
		//Most of the time you don't get a weapon from creatures.  It should be hard to gain weapons.
		//Original code was get weapon after each encounter.
		createWeapon(opponent.chosenWeapon); // Create a new instance of this weapon type on the floor
		foundTreasure = true; // need to ensure that only weapons get created!
	}

	

	tot_found += checkForTreasureItem(Opponents[0].tCopper,12,15);
	tot_found += checkForTreasureItem(Opponents[0].tSilver,11,10);
	tot_found += checkForTreasureItem(Opponents[0].tFood,1,5);
	tot_found += checkForTreasureItem(Opponents[0].tWater,2,8);
	
	 // Keys are for Dungeon only area.  City never had torches.		 
	if (tot_found < max_treasure)
    if ((plyr.scenario==DUNGEON))
	  	 tot_found += checkForTreasureItem(Opponents[0].tTorches,3,4);
	if (tot_found < max_treasure)
	   tot_found += checkForTreasureItem(Opponents[0].tTimepieces,4,15);
	if (tot_found < max_treasure)
	   tot_found += checkForTreasureItem(Opponents[0].tCompasses,5,5);

	 // Keys are for Dungeon only area.  City never had keys.		 
	if (tot_found < max_treasure)
    if ((plyr.scenario==DUNGEON))
	  	 tot_found += checkForTreasureItem(Opponents[0].tKeys,6,10);

	if (tot_found < max_treasure)
	   tot_found += checkForTreasureItem(Opponents[0].tCrystals,7,2);
	if (tot_found < max_treasure)
	   if(checkForTreasureItem(Opponents[0].tGems,8,0))
		   {
			tot_found += true;
			increaseExperience(128*no_found);
	   }
	if (tot_found < max_treasure)
   if(checkForTreasureItem(Opponents[0].tJewels,9,0))
   {
	   tot_found += true;
	   increaseExperience(255*no_found);
	}
	if (tot_found < max_treasure)
   tot_found += checkForTreasureItem(Opponents[0].tGold,10,0);

    found = randn(0,100);
	if (found < 22)
    {
        // No checking for suitability of encounter to be carrying potions yet
        foundTreasure = true;
        int potionType = randn(0,43);
        int potionKnown = randn(0,100);
        int potionRef = createPotion(potionType);
        if (potionKnown>90) itemBuffer[potionRef].hp = 1; // Potion identified or labelled
      	increaseExperience(120);	// City gave 120 xp for a potions treasure.

    }
 //   cout << "Treasure finding: " << plyr.treasureFinding <<"\n\n";

	    if (opponentType == UNDEAD_KNIGHT)
        {
            switch(plyr.special) // Stat bonus for defeating The Seven Undead Knights
            {
                case 0x80: plyr.sta++; break;
                case 0x81: plyr.chr++; break;
                case 0x82: plyr.str++; break;
                case 0x83: plyr.inte++; break;
                case 0x84: plyr.wis++; break;
                case 0x85: plyr.skl++; break;
                case 0x86: plyr.str++; break;
            }
        }

		if (opponentType == TROLL_TYRANT) // troll tyrant killed
		{
			if (plyr.trollsReforged == false){ plyr.trollsDefeated = true; createQuestItem(0); }
			else { plyr.trollsDefeated = true; createQuestItem(2);  }
			foundTreasure = false;
		}

		if (opponentType == GOBLIN_LORD) // goblin lord killed
		{
			if (plyr.goblinsReforged == false) { plyr.goblinsDefeated = true; createQuestItem(1); }
			else { plyr.goblinsDefeated = true; createQuestItem(2); }
			foundTreasure = false;
		}

		if (opponentType == MASTER_THIEF) // MASTER THEIF KILLED 
		{
			if (plyr.prisonRelease < 2)
			{
				int SilverKey = randn(0, 100);
				if (SilverKey < 10)
				{
					createQuestItem(187);
					foundTreasure = false;
				}
			}
		}

		

		//Other Items find
		
		int tempFound = randn(0, plyr.items_index) ;


		if (found <= plyr.treasureFinding)
		{

			//Most of the time you don't get a weapon from creatures.  It should be hard to gain weapons.
			//Original code was get weapon after each encounter.

			// want to use ilevel and cat to determine which item the encounter has dropped. eliminating things like weapons/potions etc dropping from a rat.
			// 
			//did encounter drop their weapon?


			if (monsterWeapons[opponent.chosenWeapon].type == 178 ) //Dropable weapons are only type 178
			{
			
				createGeneralItem(monsterWeapons[opponent.chosenWeapon].index); // Create a new instance of this weapon type on the floor
				foundTreasure = true; 
			}

			//Need to update this so potions are only found some of the time when treasure is found.

			if (Monster_Buffer[opponent.index].tPotions > 0)
			{ 
				createGeneralItem(randomItemPicker(0, plyr.level, 1, 175));

			}

		
			foundTreasure = true; 
			}

		//Corpse find
		if (opponent.tCorpse > 0) { 
			createCorpseItem(opponent.index); 
			foundTreasure = true;
		}




		//need to write another check if buffed with treasurefinding
   if ((foundTreasure) && (plyr.treasureFinding>15)) { plyr.treasureFinding = 15; }	//Once you find treasure then remove the potion affect.

	if (foundTreasure) getItems();

}



// Level 1 - Fixed Encounters
//51,4,88,
//54,5,95,
//60,5,98,
//58,7,96,
//41,19,8d,
//44,19,8c,
//47,19,8a,
//41,25,8e,
//44,25,8b,
//47,25,89,
//25,32,91,
//18,39,91,
//21,41,92,
//6,43,91,
//18,43,92,
//19,45,92,
//7,48,91,
//36,57,97,

// Level 2
//17,1,8f
//28,5,93
//28,12,93
//17,21,82
//19,21,81
//21,21,80
//17,23,83
//13,25,86
//15,25,85
//17,25,84

// Level 3
//11,7,99 4 flame demons
//10,8,9e
//12,8,9a
//10,9,9d
//11,9,9c
//12,10,9b
//14,13,87 basilisk


std::vector<int> randomEncounterPick(const MapEncounter* newMapEncounter, int arraySize, int ZoneToMatch, int TimeToMatch) {
	std::vector<int> matchingIndices;

	int DayOrNight = 0;
	if (TimeToMatch > 1) {
		DayOrNight = 0; // Assuming 0 represents day
	}
	else {
		DayOrNight = 1; // Assuming 1 represents night
	}
	int zoneFound = 0;
	for (int i = 0; i < arraySize; ++i) {
		if (newMapEncounter[i].Zone == ZoneToMatch)
		{
			// do  
			zoneFound = 1;
			i = arraySize;
		}

	}
	if (zoneFound == 0){ ZoneToMatch = 0;}

	// Find indices matching the criteria
	for (int i = 0; i < arraySize; ++i) {
		if (newMapEncounter[i].Zone == ZoneToMatch && (newMapEncounter[i].TimeOfDay == DayOrNight || newMapEncounter[i].TimeOfDay == 2) && plyr.level >= Monster_Buffer[newMapEncounter[i].Encounter].minLevel) {
			matchingIndices.push_back(i);
		}
	}

	return matchingIndices;
}

int randomEncounterPicker(int ZoneToMatch, int TimeToMatch, int arraySize)
{


	std::vector<int> matchingIndices = randomEncounterPick(newMapEncounter, arraySize, ZoneToMatch, TimeToMatch);

	// Randomly pick an index from matching indices
	// Check if there are matching indices
	if (!matchingIndices.empty()) {
		// Initialize random number generator
		std::random_device rd;
		std::mt19937 gen(rd());

		// Calculate total probability of matching encounters
		double totalProbability = 0.0;
		for (int index : matchingIndices) {
			totalProbability += newMapEncounter[index].Probability;
		}

		// Generate a random number between 0 and the total probability
		std::uniform_real_distribution<> dis(0.0, totalProbability);
		double randomProbability = dis(gen);

		// Select the encounter based on probabilities
		double cumulativeProbability = 0.0;
		int selectedIndex = -1;
		for (int index : matchingIndices) {
			cumulativeProbability += newMapEncounter[index].Probability;
			if (randomProbability < cumulativeProbability) {
				selectedIndex = index;
				break;
			}
		}

		// Display the randomly selected encounter
		if (selectedIndex != -1) {
			std::cout << "Randomly picked Encounter: " << Monster_Buffer[newMapEncounter[selectedIndex].Encounter].name << std::endl;
			return newMapEncounter[selectedIndex].Encounter;
		}
	}

	// Handle the case when no matching encounter is found
	// You may want to return some default value or throw an exception

	else {
		std::cout << "No Encounters match the criteria." << std::endl;
		return 0;
	}


}