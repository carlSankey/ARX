
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "globals.h"
#include "actor.h"
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

bool    checkForTreasure;
bool    encounterRunning;
bool    playerTurn;
bool    playerSurprised;
bool    waitingForAnyKey;
bool    waitingForSpaceKey;
int     encounterTurns;
int     encounterMenu;
bool    playerStunned;
//int currentOpponent;
//int currentOpponentQuantity;




const int MAX_OPPONENTS = 8;
const int MAX_CONSOLE_MESSAGES = 10;
string  consoleMessages[MAX_CONSOLE_MESSAGES];

void awardExperience(int opponentNo);
void text(string str);

const int DUNGEON_TABLE_ENCOUNTERS = 64;

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

encRecord nightEncTable[48]=
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
	{1,		ASSASSIN}
};

encRecord dayEncTable[29] =
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
	{2,     NOBLEWOMAN}
};


extern int statPanelY;
extern bool musicPlaying;

monster Opponents[8]; // max 8 monsters against you

monster opponent;

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
bool playerRunsAway = false;

int encounterQuantity;
int curOpponent; // 0-7

sf::Time attackCheckTime;
sf::Time attackTimer;
sf::Clock attackClock;


void encounterLoop(int encounterType, int opponentQuantity)
{
    opponentType = encounterType;
    checkForTreasure = false;
	animationNotStarted = true;
	firstFrame = Monsters[opponentType].image;
	lastFrame = Monsters[opponentType].image2;
	encounterRunning = true;
	encounterTurns = 0;
	playerTurn = true;
	playerStunned = false;
	playerOnGround = false;
	playerSurprised = false;
	opponentSurprised = false;
	waitingForAnyKey = false;
	waitingForSpaceKey = false;
    encounterMenu = 1;
    groundTurnsRemaining = 0;
    plyr.status_text = "                                        ";
    playerRunsAway = false;
    //currentOpponent = 0;
//    currentOpponentQuantity = opponentQuantity;

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
 
		drawEncounterView();
		if (!waitingForSpaceKey)
        {
		  		  //Opponent will choice to do something after 2.0 no matter what the player does        
			     attackCheckTime += attackTimer;
				  if (attackCheckTime >= sf::seconds(2.0f))
				  {
			  		  	processOpponentAction();
			  		  	attackCheckTime = sf::Time::Zero;
				  }

            if (playerTurn) processPlayerAction();
            else processOpponentAction();
        }


		updateDisplay();
        key = getSingleKey();

        // Check if player died this turn
        if (plyr.hp < 0) { encounterRunning = false; }




        // Handle dismissed encounter messages
        if (key == "SPACE")
        {
            updateConsoleMessages(); // Checks for further messages to be printed.
            if (consoleMessages[0] == "NO MESSAGE") waitingForSpaceKey = false; // player pressed space to acknowledge last message read
        }

		// ...and see if any opponents still active
		updateOpponents();
        checkForActiveOpponents();


	}

	// If opponent killed, charmed or tricked then player might find treasure afterwards
    //checkTreasure();


	if (plyr.fixedEncounter)
    {
        plyr.fixedEncounter = false;
        if (playerRunsAway)
        {
            if (plyr.facing==1) { plyr.x = plyr.oldx; }
            if (plyr.facing==3) { plyr.x = plyr.oldx; }
            if (plyr.facing==2) { plyr.y = plyr.oldy; }
            if (plyr.facing==4) { plyr.y = plyr.oldy; }
            plyr.z_offset=1.0;
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




void initialiseOpponents(int opponentType, int opponentQuantity)
{
    // Clean out all 8 opponent slots with an empty monster object (using the unused FBI Agent for this)
    for(int i = 0; i < MAX_OPPONENTS; ++i)
	{
		Opponents[i] = Monsters[FBI_AGENT];
	}
    for(int i = 0; i < opponentQuantity; ++i)
	{
		Opponents[i] = Monsters[opponentType];
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
    cout << "Name:  " << monsterWeapons[w].name << "\n";
    cout << "HP:    " << monsterWeapons[w].hp << "\n";
    cout << "Align: " << monsterWeapons[w].alignment << "\n\n";
    cout << "Blunt: " << monsterWeapons[w].blunt << "\nSharp: " << monsterWeapons[w].sharp << "\nEarth: "
        << monsterWeapons[w].earth << "\nAir:   " << monsterWeapons[w].air
        << "\nFire:  " << monsterWeapons[w].fire << "\nWater: " << monsterWeapons[w].water << "\nPower: "
        << monsterWeapons[w].power << "\nMagic: " << monsterWeapons[w].magic
        << "\nGood:  " << monsterWeapons[w].good << "\nEvil:  " << monsterWeapons[w].evil << "\nCold:  "
        << monsterWeapons[w].cold << "\n\n";


}


void removeOpponent()
{
    // Removes Opponents[0], shuffles the other 7 and adds an Empty

    for(int i = 0; i < (MAX_OPPONENTS); ++i)
	{
		Opponents[i] = Opponents[i+1];
	}
	// Add an empty slot to the end of the array (the unused FBI Agent is used for this)
	Opponents[(MAX_OPPONENTS-1)] = Monsters[FBI_AGENT];
}



void checkForActiveOpponents()
{
	// Check first encounter slot
	if (Opponents[0].hp == 0) encounterRunning = false;
}



void updateOpponents()
{
    // Count total remaining opponents
	encounterQuantity=0;
	for(int i = 0; i < 8; ++i) //
	{
		if (Opponents[i].hp>0) encounterQuantity++;
	}
	if (encounterQuantity > 1)
	std:cout << "Num Opn# " << encounterQuantity << "\n";
}




void processOpponentAction()
{

    if (encounterNotHostile)
    {
        if (encounterTurns == 3) { opponentLeaves(); }
        else { playerTurn = true; }
    }
    else
    {
        opponentAttack();
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
	    std::cout << "next opponent active";
	 }
}




void processPlayerAction()
{
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
		  	  int escapeperc = 4 - ((int)(plyr.stealth * 0.05));
		  	  if (escapeperc < 1)
		  	  	  escapeperc = 1;
		  	  	  
		  	  int value = randn(1,10);
		  	  if (value < 4)
		  	  {
			     consoleMessage( "You didn't escape.");
		 		  if ( key=="SPACE" )
		 		  {
						 encounterMenu = 1; 
						 playerStunned = true;
							if ((Opponents[0].alignment<128) || (!encounterNotHostile))
			         	opponentAttack();
					}

		  	  	
				 } else  {
				 	playerRunsAway = true; 
					encounterRunning = false; 
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
            if ((plyr.encounterRef == HEALER) && (plyr.scenario==1)) { encounterMenu = 5; } else { playerHail(); }
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
        if ( key=="SPACE" ) { encounterMenu = 1; playerStunned = false; /* playerTurn = false; */  }
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

}
void playerSnatch()
{

}


void determineOpponentOpeningMessage()
{
	// Determines the following:
	// 1) Does the opponent have an opening message (e.g. thief, pauper, knight)

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
    encounterNotHostile = true;

    // Check alignment - Evil
    if (Opponents[0].alignment < 128) encounterNotHostile = false;

    // Good but hostile to humans
	if ((opponentType == PHOENIX) || (opponentType == VALKYRIE)  || (opponentType == DWARF)) encounterNotHostile = false;

	// Check for neutral encounters without intelligence or wisdom - e.g. giant rat
	if ((Opponents[0].inte == 0) && (Opponents[0].wis == 0)) encounterNotHostile = false;

	// Check anti-guild status - only applies to humans

}



void checkSurprise()
{
	// Determines whether the player or opponent have element of surprise
	// If player then go to encounterMenu 3

	int playerStealth = randn(1,plyr.stealth);
//cout <<  "player:" << playerStealth << "\n";
//cout << "opponent:" << Opponents[0].stealth;
    int opponentStealth = randn(0,Opponents[0].stealth);
	//int encounterStealth = randn(1,5);
//	cout << "opp stealth:" << opponentStealth << "\n";
	int surpriseValue = -opponentStealth + playerStealth;
//	cout << "value:" << surpriseValue << "\n";
	if (surpriseValue < -5) { playerSurprised = true; }
	if (surpriseValue > 5) { opponentSurprised = true; }

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



void playerHail()
{
	int response = randn(1,15);
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

	if ((plyr.encounterRef == NOBLEMAN))
	{
		str = "The nobleman tosses you a coin@and says:@@\"Away knave! Get thyself a bath!\"";
		if (plyr.gender==2) str = "The nobleman tosses you a coin@and says@\"Away scullion! Get thyself a bath!\"";
		plyr.gold++;

	}

	if ((plyr.encounterRef == ACOLYTE))
	{
	    string items[7] = { "food packets" , "water flasks" , "torches" , "timepieces", "compasses", "keys" , "crystals" };
	    string opponentText = Opponents[0].name;
	    string itemText = "";
	    string genderText = "brother";
	    bool itemRequired = true;

	    if ( plyr.gender == 2 ) genderText = "sister";

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

            if (itemRequired) str ="Greetings!";
            else {str = "The " + opponentText + " sees you have no@" + itemText + " and tosses you one saying:@@\"For the cause " + genderText + ".\""; }

	}
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
	if ((itemBuffer[plyr.priWeapon].melee != 0xFF) && (hitAttempt))
    {
        missileWeapon = true;
        if (itemBuffer[plyr.priWeapon].ammo > 0)
        {
            missileAmmoAvailable = true;
            itemBuffer[plyr.priWeapon].ammo--;
            int remainingAmmo = itemBuffer[plyr.priWeapon].ammo;
            if (remainingAmmo < 10) itemBuffer[plyr.priWeapon].name = "Crossbow [0" + itos(remainingAmmo) + "]";
            else itemBuffer[plyr.priWeapon].name = "Crossbow [" + itos(remainingAmmo) + "]";
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
	int hitRoll = randn(1,100);
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
	if ( Opponents[0].hp<1) {opponentDeath(); }

	playerTurn = false;
}





void opponentDeath()
{
    str = "The " + Opponents[0].name + " " + Opponents[0].armorText + ".";
    consoleMessage(str);
	awardExperience(opponentType);
	removeOpponent();
    checkForTreasure = true;
}








void opponentAttack()
{
	bool opponentIsAttacking;
	bool hitSuccess;

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

    //int chosenWeapon = opponentChooseWeapon();
    int chosenWeapon = Opponents[0].w1;



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
				//int damage = randn(1,6); // CHANGE!!!
        if (opponentType==GIANT_RAT) { if (plyr.diseases[0] == 0) plyr.diseases[0] = 1;} // Rabies
        if (opponentType== MOLD) { if (plyr.diseases[1] == 0) plyr.diseases[1] = 1;} // Mold
        if (opponentType== SLIME) { if (plyr.diseases[2] == 0) plyr.diseases[2] = 1;} // Fungus
        float attackFactor = 1.0; //
        int damage = calcOpponentWeaponDamage(chosenWeapon,attackFactor, 1);

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
            weaponName = monsterWeapons[55].name;
            int weaponIndex = Opponents[0].w1;
            attackDesc = getAttackDesc(weaponIndex, damage);
        }

        if (damage!=1000) { plyr.hp-=damage; }

        str = prefix + Opponents[0].name + " " + attackDesc + "@" + bPartText + " with "+ weaponName + "@for " + itos(damage) + ".";
        if (damage==0) { str = prefix + Opponents[0].name + " " + attackDesc + "@" + bPartText + " with "+ weaponName + "@which has no effect!"; }
        //if (damage==1000) { str = prefix + opponent.name + " " + attackDesc + "@you with "+ opponentWeapon.name + "@which is stopped by your skin!"; }
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




int calcOpponentWeaponDamage(int weaponNo, float attackFactor, int attacker)
{
    // CALCULATE MONSTER WEAPON / ATTACK DAMAGE

	// attacker - 1 = monster
	int weaponDamageValues[11];
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
    }

	int armorValues[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	// Need to add modifier for player armor values & armor body parts

	int armors[11]; // holds results of rolling for armor protection
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

		int armorIndex = 0;
		while (armorIndex < 11)
		{
			int noDice = (armorValues[armorIndex] & 0xf0) >> 4;
			int noSides = (armorValues[armorIndex] & 0x0f);
			armors[armorIndex] = rollDice(noDice, noSides);
			armorIndex++;
		}
	}



	int damages[11] = { 0,0,0,0,0,0,0,0,0,0,0 }; // holds results of rolling for damage

	int damageIndex = 0; // 0 is blunt, 1 is sharp, 11 is cold - 11 damage types in total

//    cout << "Opponent:";

	while (damageIndex < 11)
	{
		int noDice = (weaponDamageValues[damageIndex] & 0xf0) >> 4;
		int noSides = (weaponDamageValues[damageIndex] & 0x0f);

		//int currentDamage = round((weaponDamageValues[damageIndex])* attackFactor);
		if (noDice > 0 )
		{
            damages[damageIndex] = rollDice(noDice, noSides);
//            cout << damages[damageIndex] << " (" << noDice << " D" << noSides << "), ";
		}
		damageIndex++;
	}

//    cout << "\n";
	// Compare weapon damages against ancounter armour values inc. vulnerabilities and invulnerabilities
	// 0xff = invulnerable.
	// 0xf0 = absorbs power from this damage type.
	// 0x0f = takes double damage from this damage type.

	/*
	if (attacker==0) // Player attacking
	{
		for(int i = 0; i < 11; ++i) // number of damages to compare against armors
		{
			damages[i] -= armors[i];

			if ( damages[i] < 0 ) { damages[i] = 0; }
		}
		//totalDamage = damages[0] + damages[1] + damages[2] + damages[3] + damages[4] + damages[5] + damages[6] + damages[7] + damages[8];
		//if (totalDamage==0) { return 1000; }
		//if (totalDamage >0) { return totalDamage; }
	}
*/

	//std::cout << "B:" << damages[0] << " S:" << damages[1] << " E:" << damages[2] << " A:" << damages[3] << " F:" << damages[4] << " W:" << damages[5] << " P:" << damages[6] << " M:" << damages[7] << " G:" << damages[8] << " E:" << damages[9] << " C:" << damages[10] << std::endl;
	int totalDamage = damages[0]+damages[1]+damages[2]+damages[3]+damages[4]+damages[5]+damages[6]+damages[7]+damages[8]+damages[9]+damages[10];
	return totalDamage;

}




int calcPlayerWeaponDamage(int weaponNo, float attackFactor, int attacker)
{
    // CALCULATE PLAYER WEAPON / ATTACK DAMAGE

	// attacker - 0 = player
	int weaponDamageValues[11];
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

	int armorValues[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
    int armors[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
/*
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

		int armorIndex = 0;
		while (armorIndex < 11)
		{
			int noDice = (armorValues[armorIndex] & 0xf0) >> 4;
			int noSides = (armorValues[armorIndex] & 0x0f);
			if (noDice > 0 ) armors[armorIndex] = rollDice(noDice, noSides);

			armorIndex++;
		}
	}
*/


	int damages[11] = { 0,0,0,0,0,0,0,0,0,0,0 }; // holds results of rolling for damage

	int damageIndex = 0; // 0 is blunt, 1 is sharp, 11 is cold - 11 damage types in total

//	cout << "Player Damage:";
	while (damageIndex < 11)
	{
		int noDice = (weaponDamageValues[damageIndex] & 0xf0) >> 4;
		int noSides = (weaponDamageValues[damageIndex] & 0x0f);

		//int currentDamage = round((weaponDamageValues[damageIndex])* attackFactor);
		if (noDice > 0 )
        {
            damages[damageIndex] = rollDice(noDice, noSides);
            if (armorValues[damageIndex]==0xff) { damages[damageIndex] = 0; }
            if (armorValues[damageIndex]==0xf0) { damages[damageIndex] = damages[damageIndex]*-1; }
            if (armorValues[damageIndex]==0xff) { damages[damageIndex] = damages[damageIndex]*2; }
 //           cout << damages[damageIndex] << " (" << noDice << " D" << noSides << ") ";
        }
		damageIndex++;
	}
//	cout << "\n";

	// Compare weapon damages against encounter armour values inc. vulnerabilities and invulnerabilities
	// 0xff = invulnerable.
	// 0xf0 = absorbs power from this damage type.
	// 0x0f = takes double damage from this damage type.
	if (attacker==0) // Player attacking
	{
		for(int i = 0; i < 11; ++i) // number of damage slots to compare against armour slots
		{
			damages[i] -= armors[i];
			if ( damages[i] < 0 ) { damages[i] = 0; }
		}
	}


	//std::cout << "B:" << damages[0] << " S:" << damages[1] << " E:" << damages[2] << " A:" << damages[3] << " F:" << damages[4] << " W:" << damages[5] << " P:" << damages[6] << " M:" << damages[7] << " G:" << damages[8] << " E:" << damages[9] << " C:" << damages[10] << std::endl;
	int totalDamage = damages[0]+damages[1]+damages[2]+damages[3]+damages[4]+damages[5]+damages[6]+damages[7]+damages[8]+damages[9]+damages[10];
	return totalDamage;

}


int opponentChooseWeapon()
{
	weaponProbabilities[0] = Opponents[0].c1;
	weaponProbabilities[1] = Opponents[0].c2;
	weaponProbabilities[2] = Opponents[0].c3;
	weaponProbabilities[3] = Opponents[0].c4;
	weaponProbabilities[4] = Opponents[0].c5;
	weaponProbabilities[5] = Opponents[0].c6;

	weaponReferences[0] = Opponents[0].w1;
	weaponReferences[1] = Opponents[0].w2;
	weaponReferences[2] = Opponents[0].w3;
	weaponReferences[3] = Opponents[0].w4;
	weaponReferences[4] = Opponents[0].w5;
	weaponReferences[5] = Opponents[0].w6;

	int chosenWeaponNo = 255;
	int weaponProbability = randn(1,100);
	int weaponProbabilityTotal = 0;
	int weaponIndex = 0;
	//int weaponIndexProbability = 0;
	while (chosenWeaponNo == 255)
	{
		weaponProbabilityTotal += weaponProbabilities[weaponIndex];
		if ( weaponProbability <= weaponProbabilityTotal ) { chosenWeaponNo = weaponReferences[weaponIndex]; }
		if ( weaponIndex == 6) { chosenWeaponNo = 0; } // error weapon!
		weaponIndex++;
	}
	return chosenWeaponNo;
}



void initMonster(int monsterNo)
{
    // Initialise a SINGLE monster - legacy City logic

    /*
    Opponents[a] = Monsters[monsterNo]; // copy monster details to current Opponents[a] object
    Opponents[a].hp += randn(0,Monsters[monsterNo].randomHP);
    Opponents[a].str += randn(0,Monsters[monsterNo].randomStrength);
    Opponents[a].skl += randn(0,Monsters[monsterNo].randomSkill);
    Opponents[a].inte += randn(0,Monsters[monsterNo].randomIntelligence);
    Opponents[a].spd += randn(0,Monsters[monsterNo].randomSpeed);
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
    //int opponentAlignment = Monsters[opponentNo].alignment;
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
    if (encounterThemeNotPlaying())
    {
        int encounter_check = randn(0, plyr.stealth);
        if (encounter_check == 0)
        {
            chooseEncounter();
        }
    }
}




void chooseEncounter()
{
	int monsterNo = 0;
    plyr.status = ENCOUNTER;

    // CITY - Day
	if ((plyr.timeOfDay!=1) && (plyr.scenario==CITY))
	{
		int encCount = 0;
		int monsterProb = randn(0, 255);
		//int monsterProb = 253;
		for(int i = 0; i < 29; ++i) // 28???
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
		for(int i = 0; i < 48; ++i)
		{
			if ((monsterProb >= encCount) && (monsterProb < nightEncTable[i].encProb+encCount))
			{ monsterNo = nightEncTable[i].encType; }
			encCount = encCount+nightEncTable[i].encProb;
		}
	}

	if ((plyr.scenario==DUNGEON) && ((plyr.zone == 17) || (plyr.zone == 16)))  // Dungeon - Well Lit Area
	{
		//monsterNo = randn(0,73); // pick random dungeon encounter
		int encCount = 0;
		int monsterProb = randn(0, 255);
		for(int i = 0; i < 11; ++i)
		{
			if ((monsterProb >= encCount) && (monsterProb < wellLitEncTable[i].encProb+encCount))
			{ monsterNo = wellLitEncTable[i].encType; }
			encCount = encCount+wellLitEncTable[i].encProb;
		}
		if (monsterNo==0) { monsterNo=1; cout << "Error: Monster 0 rolled!\n"; }
	}

	if ((plyr.scenario==DUNGEON) && !((plyr.zone == 17) || (plyr.zone == 16)))
    {
        int encCount = 0;
		int monsterProb = randn(0, 255);
		for(int i = 0; i < DUNGEON_TABLE_ENCOUNTERS; ++i)
		{
			if ((monsterProb >= encCount) && (monsterProb < dungeonTable[i].encProb+encCount))
			{ monsterNo = dungeonTable[i].encType; }
			encCount = encCount+dungeonTable[i].encProb;
		}
		if (monsterNo==0) { monsterNo=1; cout << "Error: Monster 0 rolled!\n"; }
    }

	if ((plyr.scenario==DUNGEON) && (plyr.map==4)) monsterNo = 19;
// cout << "Monster: "<< monsterNo << " " << Monsters[monsterNo].name << "\n";

    if (monsterNo > 70)
    {
        //cout << "City monster " << monsterNo << " encountered.\n";
    }
    plyr.fixedEncounter = false;
    
    
    
    monsterNo = SLIME;
    encounterLoop( monsterNo, 1 ); // Only one currently except for fixed encounters


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
		
	if (encounterNotHostile)
	{
		switch(opponentType)
		{
		   case COMMONER:
			case GUARD:
			case NOBLEMAN:
			case NOVICE:
			case PAUPER:
			case HEALER:
			case APPRENTICE:
			case ACOLYTE:
			case WIZARD:
  			    plyr.alignment -= 5;	//Evil act.
  			    
  			    if (plyr.alignment > 130)
  			    	 plyr.alignment = 100;
  			    	 
			   if (plyr.alignment < 0) 	 
			   	plyr.alignment = 0;
  			    break;
		}
	
	}	
//std::cout << "plyr align " << plyr.alignment << " opn# " << opponentNo << "\n";	

}


void checkTreasure()
{
	bool foundTreasure = false;
	int no_found = 0;
	int found = 0;

	// Check to see if the opponent was carrying a weapon (as opposed to claws or teeth)
	// Only type 0x03 weapons can be dropped - type 0xFF refers to natural weapons such as bites, tails, claws, spells

    Opponents[0] = Monsters[opponentType];

	int weapon = Opponents[0].w1; // Modify

	if (monsterWeapons[weapon].type==0x03)
	{
		createWeapon(weapon); // Create a new instance of this weapon type on the floor
		foundTreasure = true; // need to ensure that only weapons get created!
	}

	int upperRange = 75;

	no_found = randn(0,Opponents[0].tFood);
	found = randn(0,upperRange); // Adjusted from upperRange to reduce volume item drops
	if ((no_found>0) && (found<=plyr.treasureFinding)) { createGenericItem(1,no_found); foundTreasure = true; }

	no_found = randn(0,Opponents[0].tWater);
	found = randn(0,upperRange);
	if ((no_found>0) && (found<=plyr.treasureFinding)) { createGenericItem(2,no_found); foundTreasure = true; }
//cout << "\n" << found << " , " << no_found << "\n";

	no_found = randn(0,Opponents[0].tTorches);
	found = randn(0,upperRange);
	if ((no_found>0) && (found<=plyr.treasureFinding)){ createGenericItem(3,no_found); foundTreasure = true; }
//cout << "\n" << found << " , " << no_found << "\n";

	no_found = randn(0,Opponents[0].tTimepieces);
	found = randn(0,upperRange);
	if ((no_found>0) && (found<=plyr.treasureFinding)) { createGenericItem(4,no_found); foundTreasure = true; }
//cout << "\n" << found << " , " << no_found << "\n";

	no_found = randn(0,Opponents[0].tCompasses);
	found = randn(0,upperRange);
	if ((no_found>0) && (found<=plyr.treasureFinding)) { createGenericItem(5,no_found); foundTreasure = true; }
//cout << "\n" << found << " , " << no_found << "\n";

	no_found = randn(0,Opponents[0].tKeys);
	found = randn(0,upperRange);
	if ((no_found>0) &&(found<=plyr.treasureFinding)) { createGenericItem(6,no_found); foundTreasure = true; }
//cout << "\n" << found << " , " << no_found << "\n";

	no_found = randn(0,Opponents[0].tCrystals);
	found = randn(0,upperRange);
	if ((no_found>0) && (found<=plyr.treasureFinding)) { createGenericItem(7,no_found); foundTreasure = true; }
//cout << "\n" << found << " , " << no_found << "\n";

	no_found = randn(0,Opponents[0].tGems);
	found = randn(0,upperRange);
	if ((no_found>0) && (found<=plyr.treasureFinding)) { createGenericItem(8,no_found); foundTreasure = true; }
//cout << "\n" << found << " , " << no_found << "\n";
	no_found = randn(0,Opponents[0].tJewels);
	found = randn(0,upperRange);
	if ((no_found>0) && (found<=plyr.treasureFinding)) { createGenericItem(9,no_found); foundTreasure = true; }
//cout << "\n" << found << " , " << no_found << "\n";
	no_found = randn(0,Opponents[0].tGold);
	found = randn(0,upperRange);
	if ((no_found>0) && (found<=plyr.treasureFinding)) { createGenericItem(10,no_found); foundTreasure = true; }
//cout << "\n" << found << " , " << no_found << "\n";
	no_found = randn(0,Opponents[0].tSilver);
	found = randn(0,upperRange);
	if ((no_found>0) && (found<=plyr.treasureFinding)) { createGenericItem(11,no_found); foundTreasure = true; }
//cout << "\n" << found << " , " << no_found << "\n";
	no_found = randn(0,Opponents[0].tCopper);
	found = randn(0,upperRange);
	if ((no_found>0) && (found<=plyr.treasureFinding)) { createGenericItem(12,no_found); foundTreasure = true; }
//cout << "\n" << found << " , " << no_found << "\n";

    found = randn(0,20);
	if (found>17)
    {
        // No checking for suitability of encounter to be carrying potions yet
        foundTreasure = true;
        int potionType = randn(0,43);
        int potionKnown = randn(0,100);
        int potionRef = createPotion(potionType);
        if (potionKnown>90) itemBuffer[potionRef].hp = 1; // Potion identified or labelled
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

    //if ((foundTreasure) && (plyr.treasureFinding>5)) { plyr.treasureFinding--; }
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

