
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <iostream>
#include <sstream>

#include "player.h"
#include "font.h"
#include "display.h"
#include "game.h"
#include "misc.h"
#include "guild.h"
#include "items.h"
#include "automap.h"
#include "lyrics.h"

#include "spells.h"

extern sf::Clock        clock1;
extern int              iCounter;



int guildNo;

//extern spellItem spellBuffer[35];
extern questItem questItems[4];

sf::Music Music1;
string guildLyricsFilename;


int itemQuantity; // used for number of charges to be paid for

const int numberOfGuilds = 14; // 14 excluding - 2 extras in City



struct guild
{
       string name;
       int x;
       int y;
	   int minAlignment;
	   int maxAlignment;
	   int minLevel;
	   int type;
       int enemyGuild;
	   int fullDues;
	   int associateDues;
};




struct guildSpell
{
    string name;
    int index;
	int cost;
};

guildSpell guildSpells[35];


guild guilds[numberOfGuilds] =
{
	{"Thieves Guild",			43, 29,	64, 128,2, 1, 8 ,150,100}, //0
	{"Blue Wizards Guild",		18,	16,	120, 192,2,2,6 ,150,100}, //1
	{"Light Wizards Guild",		2,	59,	144, 255, 2,2, 5 ,150,100}, //2
	{"Green Wizards Academy",	11,	21,	96, 160, 2,2, 4,150,100}, //3
	{"Red Wizards University",	47,	49,	48, 127, 2,1, 3,150,100}, //4
	{"Dark Wizards Guild",		33,	42,	0, 64, 2,1, 2,150,100}, //5
	{"Star Wizards Guild",		27,	52,	120, 176,2,2,1 ,150,100}, //6

	{"Wizards of Chaos Guild",	50,	4,	64, 128, 2,1, 9,150,100}, //7
	{"Wizards of Law Guild",	61,	14,	132, 208, 2,2, 0,150,100}, //8
	{"Guild of Order",			57,	14,	132, 255,	 2,2, 7,150,100}, //9
	{"Physicians Guild",		5,	49,	128, 224, 2,2, 11,150,100}, //10
	{"Assassins Guild",			55,	61,	16, 112, 2,1,	10,150,100}, //11
	{"Mercenaries' Guild",		32,	8,	64,	128, 5,1, 13,150,100}, //12
	{"Paladins' Guild",			25,	19,	152, 255, 5,2, 12,150,100} //13
};


void shopGuild()
{
	//string lyricsFilename;


	int itemChoice;
	int menuStartItem = 0;
	int guildSpellsNo = 0;
	guildNo = getGuildNo();
	int guildSpellIndex = 0;
std::cout << guildNo <<"\n";
	if (guildNo==5)
	{
		setAutoMapFlag(plyr.map, 33, 41);
		setAutoMapFlag(plyr.map, 33, 42);
		setAutoMapFlag(plyr.map, 33, 43);
	}

	//guildSpell guildSpells[35];
	//for (int i=0 ; i<=35 ; i++) { guildSpells[i].name = " "; }

	// Stock guild spells each visit - crash with name?

	for (int i=0 ; i<35 ; i++)
	{
		if (spells[i].guilds[guildNo]==1)
		{
			guildSpellsNo++;
			guildSpells[guildSpellIndex].cost = spells[i].cost;
			guildSpells[guildSpellIndex].name = spells[i].name;
			guildSpells[guildSpellIndex].index = i;
			guildSpellIndex++;
		}
	}

	guildSpellsNo -= 6; // adjustment for menu
	if (guildNo==0) guildSpellsNo--; // DIRTY FIX FOR THIEVES GUILD!

	bool musicPlaying = false;


    if (!musicPlaying)
    {

        if (plyr.musicStyle==0)
        {
            if (plyr.scenario==0) { Music1.openFromFile("data/audio/cityGuild.ogg");  guildLyricsFilename = "goodGuild.txt"; }
            if ((plyr.scenario==1) && (guilds[guildNo].type==1)) { Music1.openFromFile("data/audio/evilGuild.ogg"); guildLyricsFilename = "evilGuild.txt"; }
            if ((plyr.scenario==1) && (guilds[guildNo].type==2)) { Music1.openFromFile("data/audio/goodGuild.ogg"); guildLyricsFilename = "goodGuild.txt"; }
        }
        if (plyr.musicStyle==1)
        {
            if (guilds[guildNo].type==1) { Music1.openFromFile("data/audio/B/evilGuild.ogg"); guildLyricsFilename = "evilGuild.txt"; }
            if (guilds[guildNo].type==2) { Music1.openFromFile("data/audio/B/goodGuild.ogg"); guildLyricsFilename = "goodGuild.txt";}
        }
        loadLyrics(guildLyricsFilename);
        Music1.play();
        musicPlaying = true;
    }


	int guildMenu = 1; // high level menu
	string str,key;
	plyr.status = 2; // shopping

	if (plyr.scenario==0) loadShopImage(14);;
	if ((plyr.scenario==1) && (guilds[guildNo].type==1)) loadShopImage(4);
	if ((plyr.scenario==1) && (guilds[guildNo].type==2)) loadShopImage(5);

	// Check for enemy guilds. Assume both full and associate membership counts
	bool enemyGuild = false;
	for (int i=0 ; i<=numberOfGuilds ; i++)
	{
		if ((plyr.guildMemberships[i]==255) && (guilds[i].enemyGuild==guildNo)) enemyGuild = true;
		if ((plyr.guildMemberships[i]==240) && (guilds[i].enemyGuild==guildNo)) enemyGuild = true;
	}

	if (enemyGuild)
	{
		while (guildMenu == 1)
		{
				clearGuildDisplay();
				cyText (2,"Leave!  Thou art not wanted here scum!");
				updateDisplay();
				key = getSingleKey();
				if ( key!="" ) { guildMenu = 0;}
		}
	}

	if (plyr.guildMemberships[guildNo]==255) guildMenu = 11; // Full member menu
	if (plyr.guildMemberships[guildNo]==240) guildMenu = 11; // Associate member menu

	while (guildMenu > 0)
	{
		while (guildMenu == 1) // main menu
		{


			if (plyr.guildAwards[guildNo] == false)
			{
				bool guildawardnotchecked = true;

				if (guildNo == 0) { plyr.skl++; }
				if (guildNo == 1) {plyr.speed++;  }
				if (guildNo == 2) {plyr.wis++;  }
				if (guildNo == 3) {plyr.sta++;  }
				if (guildNo == 4) {plyr.str++;  }
				if (guildNo == 5) {plyr.chr++;  }
				if (guildNo == 6) { plyr.str++; plyr.maxhp+=5; plyr.hp+=5; }
				if (guildNo == 7) {plyr.chr++;   }
				if (guildNo == 8) {plyr.wis++; }
				if (guildNo == 9) {plyr.inte++;  }
				if (guildNo == 10) {plyr.hp+=3; plyr.maxhp+=3;  }
				if (guildNo == 11) { plyr.stealth+=30; }
				if (guildNo == 12) { plyr.str++;  }
				if (guildNo == 13) { plyr.sta++; }

				while (guildawardnotchecked)
				{
					clearGuildDisplay();
					std::ostringstream text;
					if (guildNo == 0) { text << "Master Thieves show you techniques@that improve your skill."; }
					if (guildNo == 1) { text << "A Mage from the Blue Wizards@Guild uses special magic to@increase your physical speed to " << plyr.speed << "."; }
					if (guildNo == 2) { text << "A Mage from the Light Wizards@Guild uses special magic to@increase your wisdom to " << plyr.wis << "."; }
					if (guildNo == 3) { text << "A Mage from the Green Wizards Academy@uses special magic to@increase your stamina to " << plyr.sta << "."; }
					if (guildNo == 4) { text << "A Mage from the Red Wizards University@uses special magic to@increase your strength to " << plyr.str << "."; }
					if (guildNo == 5) { text << "A Mage from the Dark Wizards Guild@uses special magic to increase@your charm to " << plyr.chr << "."; }
					if (guildNo == 6) { text << "An Arch Mage from the Star Wizards@Guild uses special magic to increase@your hit points and strength."; }
					if (guildNo == 7) { text << "A Mage from the Wizards of Chaos Guild@uses special magic to increase@your charm to " << plyr.chr << ".";  }
					if (guildNo == 8) { text << "A Mage from the Wizards of Law Guild@uses special magic to increase@your wisdom to " << plyr.wis << "."; }
					if (guildNo == 9) { text << "A Mage from the Guild of Order uses@special magic to increase your@intelligence to " << plyr.inte << "."; }
					if (guildNo == 10) { text << "A Doctor from the Physicians Guild@teaches you first aid beyond what@is commonly known. This increases@your hit points.";  }
					if (guildNo == 11) { text << "A Master Assassin shows you some basic@forms of hiding and quiet approach@which increase your ability to@surprise attackers.";  }
                    if (guildNo == 12) { text << "A Mercenary from the Mercenaries Guild@uses special magic to increase@your strength to " << plyr.str << "."; }
					if (guildNo == 13) { text << "A Knight from the Paladin's Guild uses@special magic to increase your stamina."; }

					cText (text.str());
					cyText (9,"< Press any key to continue >");
					updateDisplay();





					key = getSingleKey();
					if (( key!="" ) && (key!="up") && (key!="down") && (key!="I") && (key!="K")) { guildawardnotchecked = false; }

				}
				plyr.guildAwards[guildNo] = true;
			}


			clearGuildDisplay();
			str = "Welcome to the " + guilds[guildNo].name + ".";
			cyText (1, str);
			bText (6,3, "(1) Apply for Guild membership.");
			bText (6,5, "(0) Leave.");
			updateDisplay();

			//if (!musicPlaying) { Music1.play(); musicPlaying = true; }


			key = getSingleKey();
			if (key=="F1") { Music1.stop(); loadLyrics(guildLyricsFilename); Music1.play(); }
			if ( key=="0" ) { guildMenu = 0; }
			if ( key=="down" ) { guildMenu = 0; }
			if ( key=="1" )
			{
				key="";
				if (plyr.level < guilds[guildNo].minLevel)
				{
					while (key=="")
					{
						clearGuildDisplay();
						cyText (2,"I am deeply sorry but you have not");
						cyText (4,"the experience to join our guild yet.");
						cyText (9,"( Press a key )");
						updateDisplay();
						key = getSingleKey();
						if ( key!="" ) { guildMenu = 0; } // Throw you out of the guild
					}
				}
				if ((plyr.alignment < guilds[guildNo].minAlignment) && (plyr.level >= guilds[guildNo].minLevel))
				{
					while (key=="")
					{
						clearGuildDisplay();
						cyText (2,"I am sorry but your soul is too dark");
						cyText (4,"to become a member of our guild.");
						updateDisplay();
						key = getSingleKey();
						if ( key!="" ) { guildMenu = 0; } // Throw you out of the guild
					}
				}
				if ((plyr.alignment > guilds[guildNo].maxAlignment) && (plyr.level >= guilds[guildNo].minLevel))
				{
					while (key=="")
					{
						clearGuildDisplay();
						cyText (2,"I am sorry, but you are too righteous");
						cyText (4,"to become a member of our guild.");
						updateDisplay();
						key = getSingleKey();
						if ( key!="" ) { guildMenu = 0; } // Throw you out of the guild
					}
				}


				if ((plyr.level >= guilds[guildNo].minLevel) && (plyr.alignment >= guilds[guildNo].minAlignment) && (plyr.alignment <= guilds[guildNo].maxAlignment))
				{
					int guildFullMembership = 255;
					for (int i=0 ; i<=numberOfGuilds ; i++) { if (plyr.guildMemberships[i]==255) guildFullMembership = i; }
					if (guildFullMembership == 255) guildMenu = 3;
					if (guildFullMembership < 255) guildMenu = 2;
				}

		}
}



		while (guildMenu == 2) // Associate or full guild membership?
		{
			clearGuildDisplay();
			cyText (2,"You can only have full membership@privileges in but one guild.@@Do you want:");
			bText (7,7,"(1) Full membership or");
			bText (7,8,"(2) Associate membership");

			updateDisplay();
			key = getSingleKey();
			if ( key=="1" ) { guildMenu = 3; }
			if ( key=="2" ) { guildMenu = 12; }
		}

		while (guildMenu == 3) // Join guild full membership menu
		{
			clearGuildDisplay();
			str = "Dues are " + itos(guilds[guildNo].fullDues) + " silvers.";
			cyText (2,str);
			cyText (4,"Do you still wish to join? (Y or N)");
			displaySilverCoins();
			updateDisplay();
			key = getSingleKey();
			if ( key=="N" ) { guildMenu = 0; } // Throw you out of the guild
			if ( key=="Y" ) { guildMenu = 4; } // Attempt to join with full membership
		}

		while (guildMenu == 4)
		{
			if (checkCoins(0,(guilds[guildNo].fullDues),0))
			{
				int enemyGuild = guilds[guildNo].enemyGuild;
				clearGuildDisplay();
				cyText (1,"You are now part of our ancient order!");
				cyText (2,"We give you your own Guild Ring to");
				cyText (3,"store spell energy and identify yourself");
				cyText (4,"to other members.  Wear it with pride!");
				cyText (5,"You may also keep your valuables safe");
				cyText (6,"in your guild locker. Beware of anyone");
				str = "from the " + guilds[enemyGuild].name + "!";
				cyText (7,str);
				cyText (9,"<<< Press any key to continue >>>");
				updateDisplay();
				key = getSingleKey();
				if ( key!="" )
				{
					// Now joined guild
					deductCoins(0,(guilds[guildNo].fullDues),0);

					// remove or replace any previous "full membership" guild records
					for (int i=0 ; i<=numberOfGuilds ; i++) { if (plyr.guildMemberships[i]==255) plyr.guildMemberships[i]=240; }

					int guildRingRef = getQuestItemRef(3);
					//itemBuffer[ringRef].location = 0; // move this guild ring to the void
					if (guildRingRef==255){ guildRingRef = createQuestItem(3); } // create a guild ring object if 255 no match found
					itemBuffer[guildRingRef].type = 201; // type 201 just for guild ring
					itemBuffer[guildRingRef].location = 10; // move to players inventory
					string newGuildName;
					if (guildNo==0) newGuildName = "Thieves Ring";
					if (guildNo==1) newGuildName = "Blue Ring";
					if (guildNo==2) newGuildName = "Light Ring";
					if (guildNo==3) newGuildName = "Green Ring";
					if (guildNo==4) newGuildName = "Red Ring";
					if (guildNo==5) newGuildName = "Dark Ring";
					if (guildNo==6) newGuildName = "Star Ring";
					if (guildNo==7) newGuildName = "Chaos Ring";
					if (guildNo==8) newGuildName = "Law Ring";
					if (guildNo==9) newGuildName = "Order Ring";
					if (guildNo==10) newGuildName = "Physicians Ring";
					if (guildNo==11) newGuildName = "Assassins Ring";
					if (guildNo==12) newGuildName = "Mercenaries Ring";
					if (guildNo==13) newGuildName = "Paladins Ring";
					questItems[(itemBuffer[guildRingRef].index)].name = newGuildName;
					plyr.ringCharges = 99;

					plyr.guildMemberships[guildNo] = 255; // Now full member of this guild
					guildMenu = 5;
				} // Throw you out of the guild
			}
			else
			{
				// Insufficient guild fees
				clearGuildDisplay();
				cyText (2,"You have not the funds!");
				updateDisplay();
				key = getSingleKey();
				if ( key!="" ) { guildMenu = 0; }
			}
		}


		while (guildMenu == 5)
		{
			clearGuildDisplay();
			if (checkForQuestItem(3)) { bText (8,1,"(1) Charge your Guild Ring"); } else  { bText (8,1,"(1) Replace your Guild Ring"); }
			bText (8,2,"(2) Have curses removed");
			bText (8,3,"(3) Learn Guild spells");
			bText (8,4,"(4) Practice Guild spells");
			bText (8,5,"(5) Resign from the Guild");
			bText (8,6,"(6) Check your Guild Locker");
			bText (8,7,"(0) Leave");

			updateDisplay();
			key = getSingleKey();
			if ( key=="1" ) { if (checkForQuestItem(3)) { guildMenu = 10; } else { guildMenu = 13; } }
			if ( key=="2" ) { guildMenu = 20; }
			if ( key=="3" ) { guildMenu = 16; }
			if ( key=="4" ) { guildMenu = 21; }
			if ( key=="5" ) { guildMenu = 8; }
			if ( key=="6" ) { guildMenu = 15; }
			if ( key=="0" ) { guildMenu = 7; }

		}

		while (guildMenu == 6)
		{
			clearGuildDisplay();
			bText (8,1,"(1) Apply for full membership");
			bText (8,2,"(2) Have curses removed");
			bText (8,3,"(3) Learn Guild spells");
			bText (8,4,"(4) Practice Guild spells");
			bText (8,5,"(5) Resign from the Guild");
			bText (8,7,"(0) Leave");

			updateDisplay();
			key = getSingleKey();
			if ( key=="1" ) { guildMenu = 3; }
			if ( key=="2" ) { guildMenu = 20; }
			if ( key=="3" ) { guildMenu = 16; }
			if ( key=="4" ) { guildMenu = 21; }
			if ( key=="5" ) { guildMenu = 8; }

			if ( key=="0" ) { guildMenu = 7; }

		}

		while (guildMenu == 7)
		{
			clearGuildDisplay();
			cyText (2,"Come again soon,");
			string str;
			if (plyr.gender==1) str = "Brother " + plyr.name + ".";
			if (plyr.gender==2) str = "Sister " + plyr.name + ".";
			cyText (4,str);
			updateDisplay();
			key = getSingleKey();
			if ( key!="" ) { guildMenu = 0; }
		}

		while (guildMenu == 8) // resign
		{
			clearGuildDisplay();
			cyText (2,"Are you sure you want to");
			cyText (4,"terminate your membership? (Y or N)");
			updateDisplay();
			key = getSingleKey();
			if ( key=="Y" )
			{

				if ( plyr.guildMemberships[guildNo] == 255)
				{
					// only remove ring if resigning from primary guild!
					int ringRef = getQuestItemRef(3);
					itemBuffer[ringRef].location = 0; // move this guild ring to the void
					// ADD 12 lines to set plyr.lockerGold etc = 0
					// Spells are retained by character even after guild resignation and still usable without ring
				}
				plyr.guildMemberships[guildNo] = 0;
				guildMenu = 9;
			}
			if ( key=="N" ) { if (plyr.guildMemberships[guildNo]==255) guildMenu = 5;  if (plyr.guildMemberships[guildNo]==240) guildMenu = 6;}
		}

		while (guildMenu == 9)
		{
			clearGuildDisplay();
			string str;
			if (plyr.gender==1) str = "Farewell Brother " + plyr.name + ".";
			if (plyr.gender==2) str = "Farewell Sister " + plyr.name + ".";
			cyText (2,str);
			updateDisplay();
			key = getSingleKey();
			if ( key!="" )
			{
				// All locker contents will be lost if you don't clear out your guild locker first
				plyr.lcompasses = 0;
				plyr.lcopper = 0;
				plyr.lcrystals = 0;
				plyr.lfood = 0;
				plyr.lwater = 0;
				plyr.lkeys = 0;
				plyr.ltimepieces = 0;
				plyr.lsilver = 0;
				plyr.lgold = 0;
				plyr.lgems = 0;
				plyr.ljewels = 0;
				plyr.ltorches = 0;
				guildMenu = 0;
			}
		}

		while (guildMenu == 10)
		{
			clearGuildDisplay();
			if (plyr.ringCharges==99) cyText (2,"Your ring is fully charged!");
			updateDisplay();
			key = getSingleKey();
			if (plyr.ringCharges<99) { guildMenu = 100; }
			if ( key!="" ) { guildMenu = 5; }
		}

		while (guildMenu == 100)
		{
			itemQuantity = inputDepositQuantity(1012);
			if ( itemQuantity==0 ) { guildMenu = 5; }
			if ( itemQuantity>0 ) { guildMenu = 101; }

		}

		while (guildMenu == 101)
		{
			clearGuildDisplay();
			str = "That will cost " + itos(itemQuantity) + " silvers.";
			cyText (2,str);
			cyText (4,"Are you sure? (Y or N)");
			displaySilverCoins();
			updateDisplay();
			key = getSingleKey();
			if ( key=="N" ) { guildMenu = 5; } // Throw you out of the guild
			if ( key=="Y" )
			{
				if (checkCoins(0,itemQuantity,0))
				{
					deductCoins(0,itemQuantity,0);
					plyr.ringCharges+=itemQuantity;
					guildMenu = 5;
				}
				else
				{
					// Insufficient guild fees
					clearGuildDisplay();
					cyText (2,"You have not the funds!");
					updateDisplay();
					key = getSingleKey();
					if ( key!="" ) { guildMenu = 5; }
				}
			}
		}


		while (guildMenu == 11)
		{
			clearGuildDisplay();
			string str;
			if (plyr.gender==1) str = "Welcome Brother " + plyr.name + "!";
			if (plyr.gender==2) str = "Welcome Sister " + plyr.name + "!";
			cyText (2,str);
			updateDisplay();
			key = getSingleKey();
			if ( key!="" ) { if (plyr.guildMemberships[guildNo]==255) guildMenu = 5;  if (plyr.guildMemberships[guildNo]==240) guildMenu = 6;}
		}

		while (guildMenu == 12) // Join guild associate membership menu
		{
			clearGuildDisplay();
			str = "Dues are " + itos(guilds[guildNo].associateDues) + " silvers.";
			cyText (2,str);
			cyText (4,"Do you still wish to join? (Y or N)");
			displaySilverCoins();
			updateDisplay();
			key = getSingleKey();
			if ( key=="N" ) { guildMenu = 0; } // Throw you out of the guild
			if ( key=="Y" )
			{
				if (checkCoins(0,(guilds[guildNo].associateDues),0))
				{
					deductCoins(0,(guilds[guildNo].associateDues),0);
					plyr.guildMemberships[guildNo] = 240; guildMenu = 11;
				}
				else
				{
					// Insufficient guild fees
					clearGuildDisplay();
					cyText (2,"You have not the funds!");
					updateDisplay();
					key = getSingleKey();
					if ( key!="" ) { guildMenu = 0; }
				}
			}
		}

		while (guildMenu == 13) // Replace guild ring if missing from inventory
		{
			clearGuildDisplay();
			cyText (2,"It will cost 120 silvers to");
			cyText (4,"replace your ring.");
			cyText (6,"Are you sure? (Y or N)");
			displaySilverCoins();
			updateDisplay();
			key = getSingleKey();
			if ( key=="N" ) { guildMenu = 5; } // Throw you out of the guild
			if ( key=="Y" )
			{
				if (checkCoins(0,120,0))
				{
					int guildRingRef = getQuestItemRef(3);
					itemBuffer[guildRingRef].location = 10; // move back to players inventory
					plyr.ringCharges = 99; // Put charges back to 99
					guildMenu = 5;
				}
				else
				{
					guildMenu = 14; // insufficient funds
				}

			} // Attempt to replace ring
		}

		while (guildMenu == 14) // Replace guild ring - insufficient funds
		{
			clearGuildDisplay();
			cyText (2,"You have not the funds!");
			updateDisplay();
			key = getSingleKey();
			if ( key!="" ) {  if (plyr.guildMemberships[guildNo]==255) guildMenu = 5;  if (plyr.guildMemberships[guildNo]==240) guildMenu = 6; }
		}

		while (guildMenu == 15) // Guild locker
		{
			clearGuildDisplay();
			cyText (1,"You are at your locker.");
			cyText (2,"What do you want to do?");
			bText (8,4,"(1) Make a deposit");
			bText (8,5,"(2) Make a withdrawal");
			bText (8,7,"(0) Go back to main room");
			updateDisplay();
			key = getSingleKey();
			if ( key=="1" ) { selectItem(4); }
			if ( key=="2" ) { selectItem(5); }
			if ( key=="0" ) {  if (plyr.guildMemberships[guildNo]==255) guildMenu = 5;  if (plyr.guildMemberships[guildNo]==240) guildMenu = 6; }

		}

		while (guildMenu == 16) // Buy spells
		{
			// Works on the assumption that all items will be displayed and have been validated to appear in this list

			int maxMenuItems = 6;


			clearGuildDisplay();
			//str = "menuStartItem: " +itos(menuStartItem)+ " " + itos(guildSpellsNo);
			str = "Which spell would you like to learn? ";
			cyText (0,str );
			//SetFontColour(40, 96, 244, 255);
			//cyText (0, "                          0            ");
			//SetFontColour(215, 215, 215, 255);

			for (int i=0 ; i<maxMenuItems ; i++)
			{
				int itemNo = menuStartItem+i;
				str = "( ) " + guildSpells[itemNo].name;
				bText(1,(2+i), str); //was 4
				bText(1,(2+i), "                                 silvers");

			}
			displaySilverCoins();

			int itemCost, x;
			for (int i=0 ; i<maxMenuItems ; i++) // Max number of item prices in this menu display
			{
				string itemCostDesc;

				int itemNo = menuStartItem+i;
				itemCost = guildSpells[itemNo].cost;

				if (itemCost<1000) { x = 30;}
				if (itemCost<100) { x = 31;}

				itemCostDesc = toCurrency(itemCost);
				bText (x,(i+2),itemCostDesc);

			}

			SetFontColour(40, 96, 244, 255);
			bText (2,2, "1");
			bText (2,3, "2");
			bText (2,4, "3");
			bText (2,5, "4");
			bText (2,6, "5");
			bText (2,7, "6");
			if (menuStartItem!=0) { bText (2,1, "}"); }
			if (menuStartItem!=guildSpellsNo) { bText (2,8, "{"); } // tweak when number of purchase items changes
			SetFontColour(215, 215, 215, 255);

			updateDisplay();

			key = getSingleKey();
			if ( key=="1" ) { itemChoice = 0; guildMenu = 17; }
			if ( key=="2" ) { itemChoice = 1; guildMenu = 17; }
			if ( key=="3" ) { itemChoice = 2; guildMenu = 17; }
			if ( key=="4" ) { itemChoice = 3; guildMenu = 17; }
			if ( key=="5" ) { itemChoice = 4; guildMenu = 17; }
			if ( key=="6" ) { itemChoice = 5; guildMenu = 17; }
			if ( (key=="up") && (menuStartItem>0) ) { menuStartItem--; }
			if ( (key=="down") && (menuStartItem<guildSpellsNo) ) { menuStartItem++; } // tweak when number of purchase items changes
			if ( key=="ESC" ) {  if (plyr.guildMemberships[guildNo]==255) guildMenu = 5;  if (plyr.guildMemberships[guildNo]==240) guildMenu = 6; }
			if ( key=="0" ) {  if (plyr.guildMemberships[guildNo]==255) guildMenu = 5;  if (plyr.guildMemberships[guildNo]==240) guildMenu = 6; }

		}

		while (guildMenu == 17) // Check funds to learn a spell
		{
			int spellNo = menuStartItem+itemChoice;
			int spellCost = guildSpells[spellNo].cost;
			// ADD Check for maximum number of spells per level

			for (int i=0 ; i<=(plyr.spellIndex) ; i++) { if (spellBuffer[i].no==(guildSpells[spellNo].index)) guildMenu=19; } // Check if spell already learnt
			if (!checkCoins(0,spellCost,0)) { guildMenu = 14; } // Check if funds ok
			if ((checkCoins(0,spellCost,0)) && ( guildMenu == 17 ))
			{
				spellBuffer[plyr.spellIndex].no = guildSpells[spellNo].index; // Add new spell to spellBuffer
				//spellBuffer[plyr.spellIndex].name = spells[guildSpells[spellNo].no].name; // Add new spell name to spellBuffer
				spellBuffer[plyr.spellIndex].percentage = 40; // Starting percentage success for this spell
				deductCoins(0,spellCost,0);
				plyr.spellIndex++;

				guildMenu = 18;
			}

		}

		while (guildMenu == 18) // Confirmation message for learning a spell
		{
				clearGuildDisplay();
				cyText (2,"It is done!");
				updateDisplay();
				key = getSingleKey();
				if ( key!="" ) {  guildMenu = 16; }
		}

		while (guildMenu == 19) // Already have the spell
		{
				clearGuildDisplay();
				cyText (2,"You have already learnt that spell!");
				updateDisplay();
				key = getSingleKey();
				if ( key!="" ) { guildMenu = 16; }
		}

		while (guildMenu == 20) // All curses are removed
		{
				clearGuildDisplay();
				cyText (1,"The Guild Master performs a few@chants and gestures and then@proclaims");

				cyText (5,"\"All curses have been removed,");
				if (plyr.gender==1) str = "Brother " + plyr.name + ".\"";
				if (plyr.gender==2) str = "Sister " + plyr.name + ".\"";
				cyText (6,str);
				updateDisplay();
				key = getSingleKey();
				if ( key!="" )  {  if (plyr.guildMemberships[guildNo]==255) guildMenu = 5;  if (plyr.guildMemberships[guildNo]==240) guildMenu = 6; }
		}

		while (guildMenu == 21) // Practice spells
		{
			clearGuildDisplay();
			cyText (2,"Spell casting practice takes four@hours and costs 100 silvers.");
			cyText (5,"Is this alright? (Y or N)");
			displaySilverCoins();
			updateDisplay();
			key = getSingleKey();
			if ( key=="N" ) { if (plyr.guildMemberships[guildNo]==255) guildMenu = 5;  if (plyr.guildMemberships[guildNo]==240) guildMenu = 6;}
			if ( key=="Y" )
			{
				if (checkCoins(0,100,0))
				{
					practiceSpells();
					if (plyr.guildMemberships[guildNo]==255) guildMenu = 5;
					if (plyr.guildMemberships[guildNo]==240) guildMenu = 6;
				}
				else
				{
					guildMenu = 14; // insufficient funds for spell practice
				}

			}
		}



	}
	Music1.stop();
	leaveShop();
}

int getGuildNo()
{
	int guild_no;

	if (plyr.scenario==0) // City
	{

		for (int i=0 ; i<numberOfGuilds ; i++) // Max number of guild objects
		{
			if ((guilds[i].x == plyr.x) && (guilds[i].y == plyr.y))
			{
				guild_no = i; // The number of the guild you have entered
			}
	   }
	}


	if (plyr.scenario==1) // Dungeon
	{
		if (plyr.location==49) guild_no = 7;// chaos
		if (plyr.location==50) guild_no = 9;// order
		if (plyr.location==51) guild_no = 8;// law
		if (plyr.location==55) guild_no = 0;// thieves
		if (plyr.location==138) guild_no = 2;// light
		if (plyr.location==181) guild_no = 5;// dark
		if (plyr.location==42) guild_no = 12;// mercenaries
		if (plyr.location==41) guild_no = 13;// paladins

	}
	return guild_no;
}

void clearGuildDisplay()
{
    clock1.restart();
	if (plyr.scenario==0) clearShopDisplay();
	if ((plyr.scenario==1) && (guilds[guildNo].type==1)) clearShopDisplay();
	if ((plyr.scenario==1) && (guilds[guildNo].type==2)) clearShopDisplay();
	updateLyrics();
	iCounter++;
}

void practiceSpells()
{
	// Based on SelectItem code using "pages" of spells hence reference to "pages > 2" etc

	int itemRef = 9999; // Nothing selected
	std::string str, selectDesc;
	selectDesc = "Would you like to practice your spell of";

    int menuitem1 = 255; // 255 is used here as nil
    int menuitem2 = 255;
    int menuitem3 = 255;
    int menuitem4 = 255;
    bool selectDone = false;

    int no_items = plyr.spellIndex; // Number of spells in players inventory
    int cur_idx = 0;
    int pages = 0;
    int page = 3;
    //int item_idx = 0;
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
				clearGuildDisplay();
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

	if (itemRef != 9999)
	{
		int practiceHours = 4;
		while (practiceHours>0)
		{
			clearGuildDisplay();
			cyText (2,"Practicing the spell of");
			str = spells[(spellBuffer[itemRef].no)].name;
			cyText (4, str);
			updateDisplay();
			sf::sleep(sf::seconds(1));
			addHour();
			practiceHours--;
		}
		deductCoins(0,100,0);
		spellBuffer[itemRef].percentage++; // Add 1% to spell percentage
	}
}
