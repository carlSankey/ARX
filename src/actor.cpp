#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <vector>

#include "actor.h"
#include "misc.h"
#include "globals.h"


using namespace std;

int HackedMonsters = 0;

monsterFramePair animations[44] =
{
	{0,2},      // devourer 0
	{27,28},    // thief
	{72,73},    // assassin
	{98,99},    // goblin
	{109,110},  // troll
	{111,112},  // lich
	{113,114},  // undead knight
	{115,116},  // guard
	{115,116},  // fbi agent 8
	{117,122},  // knight / champion
    {3,4},      // healer / mage / sage
	{5,6},      // pauper
	{7,7},      // nobleman
	{8,9},      // alien sentry
	{10,14},    // robot 14
	{15,15},    // novice
	{16,18},    // wizard
	{19,19},    // orc 17
	{20,22},    // gnome / dwarf
	{23,26},    // slime
    {29,36},    // homunculus / bat
	{37,44},    // phoenix 21
	{45,46},    // sorceress
	{47,51},    // whirlwind
	{52,53},    // giant rat
	{54,57},    // small dragon 25
	{58,60},    // skeleton
    {61,65},    // ghost
	{66,69},    // vampire
	{70,71},    // hellhound
	{74,75},    // harpy
	{76,77},    // gremlin
    {78,83},	// flame demon
	{84,85},    // weaponmaster
    {86,87},    // valkyrie 34
	{108,108},  // doppleganger
	{89,91},    // adventurer 36
	{92,93},    // water sprite
	{94,95},    // nightstalker 38
    {96,97},    // serpentman 39
	{100,103},  // big snake 40
    {104,105},  // ice demon
    {106,107},  // warrior
	{100,103}   // basilisk - using dragon image?
};


//newMonster Monster_Buffer[noOfEncounters];           // City and Dungeon monsters combined
weapon monsterWeapons[noOfMonsterWeapons];  // Weapons & attacks which are part of the Dungeon monsters data
//weapon Weapons[noOfWeapons];                // Weapons

newMonster* Monster_Buffer = nullptr;

newMonster* monster = nullptr;

openingMessages* Message_Buffer = nullptr;

openingMessages* opMessages = nullptr;

unsigned char monstersBinary[noOfMonstersFile];

int monsterOffsets[noOfEncounters];
int pluralNameOffset;
int maxNumberEncountered;
int currentWeapon;

ofstream outdata; // outdata is like cin

void initialiseMonsterOffsets()
{
    // Temp routine to fill all values with FBI Agent until they are added below
    for(int i=0;i<noOfEncounters;i++)
        {
            monsterOffsets[i] = 0x1D1D; // Novice
        }

    monsterOffsets[0] = 0x00;       // Devourer
    monsterOffsets[1] = 0x246;      // "Thief" class
    monsterOffsets[2] = 0x763;      // Assassin 0x763
    monsterOffsets[3] = 0x875;      // Goblin
    monsterOffsets[4] = 0xA22;      // Troll
    monsterOffsets[5] = 0xB58;      // Lich
    monsterOffsets[6] = 0xBF7;      // Undead Knight
    monsterOffsets[7] = 0xCEE;      // "Guard" class
    monsterOffsets[8] = 0x10E7;     // FBI Agent
    monsterOffsets[9] = 0x117F;     // Dark Knight
    monsterOffsets[10] = 0x124F;    // Champion
    monsterOffsets[11] = 0x1364;    // Healer
    monsterOffsets[12] = 0x169B;    // Knight
    monsterOffsets[13] = 0x184F;    // Pauper
    monsterOffsets[14] = 0x1A21;    // Nobleman
    monsterOffsets[15] = 0x1B6C;    // Alien sentry
    monsterOffsets[16] = 0x1C25;    // robot
    monsterOffsets[17] = 0x1D1D;    // novice
    monsterOffsets[18] = 0x1DE1;    // apprentice
    monsterOffsets[19] = 0x1EA1;    // mage
    monsterOffsets[20] = 0x1F58;    // wizard / archmage
    monsterOffsets[21] = 0x20D7;    // acolyte
    monsterOffsets[22] = 0x21C6;    // sage
    monsterOffsets[23] = 0x230C;    // orc
    monsterOffsets[24] = 0x23C5;    // gnome
    monsterOffsets[25] = 0x2461;    // dwarf
    monsterOffsets[26] = 0x2526;    // slime
    monsterOffsets[27] = 0x2664;    // mold
    monsterOffsets[28] = 0x2748;    // homunculus
    monsterOffsets[29] = 0x27DE;    // phoenix
    monsterOffsets[30] = 0x2898;    // sorceress
    monsterOffsets[31] = 0x2981;    // whirlwind
    monsterOffsets[32] = 0x2A82;    // giant rat
    monsterOffsets[33] = 0x2B53;    // small dragon
    monsterOffsets[34] = 0x2C14;    // skeleton
    monsterOffsets[35] = 0x2CD2;    // zombie
    monsterOffsets[36] = 0x2D8E;    // ghoul
    monsterOffsets[37] = 0x2E92;    // ghost
    monsterOffsets[38] = 0x2F3D;    // spectre
    monsterOffsets[39] = 0x2FE2;    // wraith
    monsterOffsets[40] = 0x30ED;    // vampire
    monsterOffsets[41] = 0x3205;    // great bat
    monsterOffsets[42] = 0x32C8;    // hellhound
    monsterOffsets[43] = 0x339E;    // harpy
    monsterOffsets[44] = 0x3479;    // gremlin
    monsterOffsets[45] = 0x3537;    // imp
    monsterOffsets[46] = 0x35FD;    // flame demon
    monsterOffsets[47] = 0x3781;    // storm devil
    monsterOffsets[48] = 0x3833;    // giant wolf
    monsterOffsets[49] = 0x38BD;    // werewolf
    monsterOffsets[50] = 0x39C8;    // warrior
    monsterOffsets[51] = 0x3A88;    // weapon master
    monsterOffsets[52] = 0x3B55;    // valkyrie
    monsterOffsets[53] = 0x3BD9;    // gladiator
    monsterOffsets[54] = 0x3C60;    // mercenary
    monsterOffsets[55] = 0x3CEC;    // doppleganger
    monsterOffsets[56] = 0x3DCE;    // adventurer
    monsterOffsets[57] = 0x3E77;    // water sprite
    monsterOffsets[58] = 0x3EED;    // night stalker
    monsterOffsets[59] = 0x3FAC;    // salamander
    monsterOffsets[60] = 0x4068;    // ronin
    monsterOffsets[61] = 0x416C;    // serpentman
    monsterOffsets[62] = 0x4227;    // big snake
    monsterOffsets[63] = 0x42CD;    // Great Naga
    monsterOffsets[64] = 0x4350;    // berserker
    monsterOffsets[65] = 0x4423;    // basilisk
    monsterOffsets[66] = 0x4631;    // Great Wyrm
    monsterOffsets[67] = 0x4708;    // Goblin Lord
    monsterOffsets[68] = 0x47E3;    // Troll Tyrant
    monsterOffsets[69] = 0x48C8;    // ice demon
    monsterOffsets[70] = 0x494A;    // horned devil
    monsterOffsets[71] = 0x1D1D;    // MUGGER
    monsterOffsets[72] = 0x1D1D;    // ROBBER
    monsterOffsets[73] = 0x124F;    // FIGHTER
    monsterOffsets[74] = 0x124F;    // SWORDSMAN
    monsterOffsets[75] = 0x1D1D;    // COURIER
    monsterOffsets[76] = 0x1D1D;    // COMMONER
    monsterOffsets[77] = 0x1D1D;    // MERCHANT
    monsterOffsets[78] = 0x1F58;    // ARCHMAGE
    monsterOffsets[79] = 0xA22;    // GNOLL
    monsterOffsets[80] = 0x875;    // HOBBIT
    monsterOffsets[81] = 0xA22;    // GIANT
    monsterOffsets[82] = 0x2B53;    // SMALL_GREEN_DRAGON
    monsterOffsets[83] = 0x1D1D;    // NOBLEWOMAN
    monsterOffsets[84] = 0x246;    // CUTTHROAT
    monsterOffsets[85] = 0x246;    // BRIGAND
    monsterOffsets[86] = 0x246;    // Master Thief
}

/*
void convertMonstersBinaryOld()
{
    string filename = "data/map/encountersNew.txt";
    outdata.open(filename.c_str()); // opens the file
    if( !outdata )
    {
        cerr << "Error: character file could not be saved" << endl;
    }
	


	
    // Reads through the binary block and creates entry in Monster_Buffer[]
    // Bytes 2, 4, 6, 8 and 10 appear to always have a value of 0xAA

    int idx = 0; // Start byte for each monster
    currentWeapon = 0; // Index for next weapon or attack to be added to monsterWeapons[]

    for (int i=0; i<noOfEncounters; i++)
    {
        idx = monsterOffsets[i];  // Sets current monster start address in binary block
        if (i==THIEF || i == CUTTHROAT || i == BRIGAND || i == MASTER_THIEF)
		  	  idx = monsterOffsets[1];

        int attackOffset[3]; // Array of offsets to 3 Dungeon weapons / attacks
        maxNumberEncountered = monstersBinary[idx];

		  Monster_Buffer[i].maxencounters = maxNumberEncountered;
        outdata <<  endl;
        outdata << "offset: ox" << std::hex << idx << endl;
        outdata << std::dec <<  "numenc:" << Monster_Buffer[i].maxencounters << endl;


        int animationNumber = monstersBinary[idx+0x1D];
        Monster_Buffer[i].image = animations[animationNumber].startFrame;
        Monster_Buffer[i].image2 = animations[animationNumber].endFrame;
        outdata << "anim#:" << animationNumber << endl;

if (i == DEVOURER)
{
	std::cout << "GHOST" << " idx " << idx << "  monstersBinary[(idx+1)] " <<  monstersBinary[(idx+1)] << "\n";
}
        int nameTextOffset = idx + monstersBinary[(idx+1)];
		  switch(i)
		  {
		  			  case CUTTHROAT:
					  		nameTextOffset = 0x2b0;
					  		break;
		  			  case BRIGAND:
					  		nameTextOffset = 0x2ba;
					  		break;
		  			  case MASTER_THIEF:
					  		nameTextOffset = 0x2c2;
							maxNumberEncountered = 0;
        					Monster_Buffer[i].maxencounters = 0;
					  		break;
		  }

        readMonsterNameText(i,nameTextOffset);
		  switch(i)
		  {
		  			  case MUGGER:
		  			  	Monster_Buffer[i].name = "Mugger";
					  		break;
		  			  case ROBBER:
		  			  	Monster_Buffer[i].name = "Robber";
					  		break;
		  			  case FIGHTER:
		  			  	Monster_Buffer[i].name = "Fighter";
					  		break;
		  			  case SWORDSMAN:
		  			  	Monster_Buffer[i].name = "Swordsman";
					  		break;
		  			  case COURIER:
		  			  	Monster_Buffer[i].name = "Courier";
					  		break;
		  			  case COMMONER:
		  			  	Monster_Buffer[i].name = "Commoner";
					  		break;
		  			  case MERCHANT:
		  			  	Monster_Buffer[i].name = "Merchant";
					  		break;
		  			  case ARCHMAGE:
		  			  	Monster_Buffer[i].name = "Arch Mage";
					  		break;
		  			  case GNOLL:
		  			  	Monster_Buffer[i].name = "Gnoll";
					  		break;
		  			  case HOBBIT:
		  			  	Monster_Buffer[i].name = "Hobbit";
					  		break;
		  			  case GIANT:
		  			  	Monster_Buffer[i].name = "Giant";
					  		break;
		  			  case SMALL_GREEN_DRAGON:
		  			  	Monster_Buffer[i].name = "Small Green Dragon";
					  		break;
		  			  case NOBLEWOMAN:
		  			  	Monster_Buffer[i].name = "Noble Woman";
					  		break;
		  }


        outdata << "name:" << Monster_Buffer[i].name << endl;
		  

        // Note: idx+3 might be offset for plural name - if one exists
		  if( i == CUTTHROAT)
        		nameTextOffset = 0x2b0;
		  if( i == BRIGAND)
        		nameTextOffset = 0x2ba;
		  if( i == MASTER_THIEF)
        		nameTextOffset = 0x2c2;

        if (maxNumberEncountered>1) readMonsterPluralNameText(i,pluralNameOffset);
        outdata << "name plural:" <<     Monster_Buffer[i].pluName  << endl;



        int deathTextOffset = idx + 66;
        readMonsterDeathText(i,deathTextOffset);
        outdata << "death Text:" <<     Monster_Buffer[i].armorText  << endl;

        Monster_Buffer[i].hp = monstersBinary[idx+0x23];
        Monster_Buffer[i].maxHP = monstersBinary[idx+0x23];

        // Special cases - thief
        if (i==THIEF) { Monster_Buffer[i].hp = 5; Monster_Buffer[i].maxHP = 5; }
        if (i==MASTER_THIEF) { Monster_Buffer[i].hp = 35; Monster_Buffer[i].maxHP = 35; }
        if (i==FBI_AGENT) { Monster_Buffer[i].hp = 0; Monster_Buffer[i].maxHP = 0; }
        outdata << "hp:" <<     Monster_Buffer[i].hp  << endl;
        outdata << "maxhp:" <<     Monster_Buffer[i].maxHP  << endl;

if (i == DEVOURER)
{
	std::cout << "GHOST" << " idx " << idx << " i " << i << "\n";
}

        Monster_Buffer[i].alignment = monstersBinary[idx+12];
        outdata << "alignment:" <<     Monster_Buffer[i].alignment  << endl;



        Monster_Buffer[i].sta =   monstersBinary[idx+0x24];
        outdata << "sta:" <<     Monster_Buffer[i].sta  << endl;
        Monster_Buffer[i].cha =   monstersBinary[idx+0x25];
        outdata << "cha:" <<     Monster_Buffer[i].cha  << endl;
        Monster_Buffer[i].str =   monstersBinary[idx+0x26];
        outdata << "str:" <<     Monster_Buffer[i].str  << endl;
        Monster_Buffer[i].inte =  monstersBinary[idx+0x27];
        outdata << "inte:" <<     Monster_Buffer[i].inte  << endl;
        Monster_Buffer[i].wis =   monstersBinary[idx+0x28];
        outdata << "wis:" <<     Monster_Buffer[i].wis  << endl;
        Monster_Buffer[i].skl =   monstersBinary[idx+0x29];
        outdata << "skl:" <<     Monster_Buffer[i].skl  << endl;
        Monster_Buffer[i].spd =   monstersBinary[idx+0x2A];
        if (i==MASTER_THIEF) { Monster_Buffer[i].hp = 35; Monster_Buffer[i].maxHP = 35; }
        outdata << "spd:" <<     Monster_Buffer[i].spd  << endl;

        Monster_Buffer[i].tFood =         monstersBinary[idx+0x2B];
        outdata << "tFood:" <<     Monster_Buffer[i].tFood  << endl;
        Monster_Buffer[i].tWater =        monstersBinary[idx+0x2C];
        outdata << "tWater:" <<     Monster_Buffer[i].tWater  << endl;
        Monster_Buffer[i].tTorches =      monstersBinary[idx+0x2D];
        outdata << "tTorches:" <<     Monster_Buffer[i].tTorches  << endl;
        Monster_Buffer[i].tTimepieces =   monstersBinary[idx+0x2E];
        outdata << "tTimepieces:" <<     Monster_Buffer[i].tTimepieces  << endl;
        Monster_Buffer[i].tCompasses =    monstersBinary[idx+0x2F];
        outdata << "tCompasses:" <<     Monster_Buffer[i].tCompasses  << endl;
        Monster_Buffer[i].tKeys =         monstersBinary[idx+0x30];
        outdata << "tKeys:" <<     Monster_Buffer[i].tKeys  << endl;
        Monster_Buffer[i].tCrystals =     monstersBinary[idx+0x31];
        outdata << "tCrystals:" <<     Monster_Buffer[i].tCrystals  << endl;
        Monster_Buffer[i].tGems =         monstersBinary[idx+0x32];
        outdata << "tGems:" <<     Monster_Buffer[i].tGems  << endl;
        Monster_Buffer[i].tJewels =       monstersBinary[idx+0x33];
        outdata << "tJewels:" <<     Monster_Buffer[i].tJewels  << endl;
        Monster_Buffer[i].tGold =         monstersBinary[idx+0x34];
        outdata << "tGold:" <<     Monster_Buffer[i].tGold  << endl;
        Monster_Buffer[i].tSilver =       monstersBinary[idx+0x35];
        outdata << "tSilver:" <<     Monster_Buffer[i].tSilver  << endl;
        Monster_Buffer[i].tCopper =       monstersBinary[idx+0x36];
        outdata << "tCopper:" <<     Monster_Buffer[i].tCopper  << endl;

        Monster_Buffer[i].aBlunt =    monstersBinary[idx+0x37];
        outdata << "aBlunt:" <<     Monster_Buffer[i].aBlunt  << endl;
        Monster_Buffer[i].aSharp =    monstersBinary[idx+0x38];
        outdata << "aSharp:" <<     Monster_Buffer[i].aSharp  << endl;
        Monster_Buffer[i].aEarth =    monstersBinary[idx+0x39];
        outdata << "aEarth:" <<     Monster_Buffer[i].aEarth  << endl;
        Monster_Buffer[i].aAir =      monstersBinary[idx+0x3A];
        outdata << "aAir:" <<     Monster_Buffer[i].aAir  << endl;
        Monster_Buffer[i].aFire =     monstersBinary[idx+0x3B];
        outdata << "aFire:" <<     Monster_Buffer[i].aFire  << endl;
        Monster_Buffer[i].aWater =    monstersBinary[idx+0x3C];
        outdata << "aWater:" <<     Monster_Buffer[i].aWater  << endl;
        Monster_Buffer[i].aPower =    monstersBinary[idx+0x3D];
        outdata << "aPower:" <<     Monster_Buffer[i].aPower  << endl;
        Monster_Buffer[i].aMagic =    monstersBinary[idx+0x3E];
        outdata << "aMagic:" <<     Monster_Buffer[i].aMagic  << endl;
        Monster_Buffer[i].aGood =     monstersBinary[idx+0x3F];
        outdata << "aGood:" <<     Monster_Buffer[i].aGood  << endl;
        Monster_Buffer[i].aEvil =     monstersBinary[idx+0x40];
        outdata << "aEvil:" <<     Monster_Buffer[i].aEvil  << endl;
        Monster_Buffer[i].aCold =     monstersBinary[idx+0x41];
        outdata << "aCold:" <<     Monster_Buffer[i].aCold  << endl;


        // Temporary weapon values based on existing ARX weapon handling
        Monster_Buffer[i].w1 = 1;
        Monster_Buffer[i].w2 = 0;
        Monster_Buffer[i].w3 = 0;
        Monster_Buffer[i].w4 = 0;
        Monster_Buffer[i].w5 = 0;
        Monster_Buffer[i].w6 = 0;

        // Weapon / attack reading
        int weapon1 = idx + monstersBinary[idx+5];
        int weapon2 = idx + monstersBinary[idx+7];
        int weapon3 = idx + monstersBinary[idx+9];
        int weapon4 = 0;
        int weapon5 = 0;
        int weapon6 = 0;
if (i == DEVOURER)
std::cout << "weapon1 "  << weapon1 << " 2"  << weapon2 << " 3"  << weapon3 << "\n\n";

//        int weapon1 = idx + monstersBinary[idx+5];
//        int weapon2 = idx + monstersBinary[idx+7];
//        int weapon3 = idx + monstersBinary[idx+9];

        if (i==THIEF || i == CUTTHROAT || i == BRIGAND || i == MASTER_THIEF)
        {
            weapon1 = 0x2eb; // Skean
            weapon2 = 0x32F;
            weapon3 = 0x34F;
            weapon4 = 0x395;
            weapon5 = 0x3ba;
            weapon6 = 0x3e1;
        }

        outdata <<  "wpn1 offset: 0x" <<  std::hex <<   weapon1 << std::dec  << endl;
        createMonsterWeapon(currentWeapon, weapon1);
        Monster_Buffer[i].w1 = currentWeapon;
        currentWeapon++; // Increment each time a new weapon or attack is created

        if (!(weapon1 == weapon2))
        {
        outdata <<  "wpn2 offset: 0x" <<  std::hex <<   weapon2 << std::dec  << endl;
            createMonsterWeapon(currentWeapon, weapon2);
            Monster_Buffer[i].w2 = currentWeapon;
            currentWeapon++; // Increment each time a new weapon or attack is created
        }

        if ((!(weapon1 == weapon3)) && (!(weapon2 == weapon3)))
        {
        outdata <<  "wpn3 offset: 0x" <<  std::hex <<   weapon3 << std::dec  << endl;
            createMonsterWeapon(currentWeapon, weapon3);
            Monster_Buffer[i].w3 = currentWeapon;
            currentWeapon++; // Increment each time a new weapon or attack is created
        }
        
        if ((!(weapon1 == weapon4)) && (!(weapon2 == weapon4))  && weapon4 != 0)
        {
        outdata <<  "wpn4 offset: 0x" <<  std::hex <<   weapon3 << std::dec  << endl;
            createMonsterWeapon(currentWeapon, weapon3);
            Monster_Buffer[i].w4 = currentWeapon;
            currentWeapon++; // Increment each time a new weapon or attack is created
        }

        if ((!(weapon1 == weapon5)) && (!(weapon2 == weapon5))  && weapon5 != 0)
        {
        outdata <<  "wpn5 offset: 0x" <<  std::hex <<   weapon3 << std::dec  << endl;
            createMonsterWeapon(currentWeapon, weapon3);
            Monster_Buffer[i].w5 = currentWeapon;
            currentWeapon++; // Increment each time a new weapon or attack is created
        }

        if ((!(weapon1 == weapon6)) && (!(weapon2 == weapon6))  && weapon6 != 0)
        {
        outdata <<  "wpn6 offset: 0x" <<  std::hex <<   weapon3 << std::dec  << endl;
            createMonsterWeapon(currentWeapon, weapon3);
            Monster_Buffer[i].w6 = currentWeapon;
            currentWeapon++; // Increment each time a new weapon or attack is created
        }
        

        Monster_Buffer[i].c1 = 100;
        Monster_Buffer[i].c2 = 0;
        Monster_Buffer[i].c3 = 0;
        Monster_Buffer[i].c4 = 0;
        Monster_Buffer[i].c5 = 0;
        Monster_Buffer[i].c6 = 0;
        
        int numWeapons = 0;
        if (Monster_Buffer[i].w1 != 0)
		  	  numWeapons++;
        if (Monster_Buffer[i].w2 != 0)
		  	  numWeapons++;
        if (Monster_Buffer[i].w3 != 0)
		  	  numWeapons++;
        if (Monster_Buffer[i].w4 != 0)
		  	  numWeapons++;
        if (Monster_Buffer[i].w5 != 0)
		  	  numWeapons++;
        if (Monster_Buffer[i].w6 != 0)
		  	  numWeapons++;

        if (Monster_Buffer[i].w1 != 0)
		  	  Monster_Buffer[i].c1 = ((int)100 / numWeapons);
        if (Monster_Buffer[i].w2 != 0)
		  	  Monster_Buffer[i].c2 = ((int)100 / numWeapons);
        if (Monster_Buffer[i].w3 != 0)
		  	  Monster_Buffer[i].c3 = ((int)100 / numWeapons);
        if (Monster_Buffer[i].w4 != 0)
		  	  Monster_Buffer[i].c4 = ((int)100 / numWeapons);
        if (Monster_Buffer[i].w5 != 0)
		  	  Monster_Buffer[i].c5 = ((int)100 / numWeapons);
        if (Monster_Buffer[i].w6 != 0)
		  	  Monster_Buffer[i].c6 = ((int)100 / numWeapons);
         std::cout << "c1 "  << Monster_Buffer[i].c1  << "\n\n";
         std::cout << "c2 "  << Monster_Buffer[i].c2  << "\n\n";
         std::cout << "c3 "  << Monster_Buffer[i].c3  << "\n\n";
         std::cout << "c4 "  << Monster_Buffer[i].c4  << "\n\n";
         std::cout << "c5 "  << Monster_Buffer[i].c5  << "\n\n";
         std::cout << "c6 "  << Monster_Buffer[i].c6  << "\n\n";



        
if (i == DEVOURER)
{

std::cout << "Monster " << Monster_Buffer[i].name << " anim " << animationNumber << " s: " << Monster_Buffer[i].image  << ":" << Monster_Buffer[i].image2 << "algn: " << Monster_Buffer[i].alignment << std::endl ;
    std::cout << "Name:  " << Monster_Buffer[i].name << "\n";
    std::cout << "  HP:    " << Monster_Buffer[i].hp ;
    std::cout << "  Align: " << Monster_Buffer[i].alignment;
    std::cout << "  Sta: " << Monster_Buffer[i].sta << " Cha: " << Monster_Buffer[i].cha << " Str: " << Monster_Buffer[i].str << " Int: " << Monster_Buffer[i].inte <<
       " Wis: " << Monster_Buffer[i].wis << " Skl: " << Monster_Buffer[i].skl << " Spd: " << Monster_Buffer[i].sta << "\n\n";
std::cout << "blunt"  << Monster_Buffer[i].aBlunt << " sharp"  << Monster_Buffer[i].aSharp << " earth"  << Monster_Buffer[i].aEarth << " air"  << Monster_Buffer[i].aAir<< "\n\n";
std::cout << "cold "  << Monster_Buffer[i].aCold << " evil"  << Monster_Buffer[i].aEvil << " magic"  << Monster_Buffer[i].aMagic << " air"  << Monster_Buffer[i].aAir<< "\n\n";

std::cout << "w1="  << Monster_Buffer[i].w1 << " w2="  << Monster_Buffer[i].w2 << " w3="  << Monster_Buffer[i].w3 << "\n\n";
std::cout << "c1="  << Monster_Buffer[i].c1 << " c2="  << Monster_Buffer[i].c2 << " c3="  << Monster_Buffer[i].c3 << "\n\n";

std::cout << "weapon1 "  << weapon1 << " 2"  << weapon2 << " 3"  << weapon3 << "\n\n";
}

    }


    
      

  



     outdata.close();


    
}

*/





newMonster* newMonArray = nullptr;

newMonster* SelectedMonmArray = nullptr;

std::vector<newMonster> readMonsterCSV(const std::string& filename) {
    std::vector<newMonster> data;
    HackedMonsters = 0;
    // Open the CSV file
    std::ifstream file("data/map/core/" + filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data; // Return empty vector if file couldn't be opened
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        newMonster newMonster;
        std::string cell;

        // Parsing CSV fields into the struct members
        std::getline(lineStream, cell, ','); // Assuming the index is the first column
        newMonster.index = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.type = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.maxencounters = std::stoi(cell);

        std::getline(lineStream, newMonster.name, ',');

        std::getline(lineStream, newMonster.pluName, ',');

        std::getline(lineStream, newMonster.armorText, ',');


        std::getline(lineStream, cell, ',');
        newMonster.behaviour = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.alignment = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.stealth = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.randomStrength = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.randomSkill = std::stoi(cell);
        
        std::getline(lineStream, cell, ',');
        newMonster.randomIntelligence = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.randomSpeed = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.hp = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.randomHP = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.maxHP = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.image = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.image2 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.sta = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.cha = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.str = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.inte = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.wis = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.skl = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.spd = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.aBlunt = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.aSharp = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.aEarth = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.aAir = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.aFire = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.aWater = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.aPower = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.aMagic = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.aGood = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.aEvil = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.aCold = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.aNature = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.aAcid = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tPotions = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tEquipment = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tFood = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tWater = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tTorches = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tTimepieces = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tCompasses = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tKeys = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tCrystals = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tCopper = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tSilver = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tGold = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tGems = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tJewels = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.tCorpse = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.w1 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.w2 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.w3 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.w4 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.w5 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.w6 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.c1 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.c2 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.c3 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.c4 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.c5 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.c6 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.callForHelp = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.s0 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.s1 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.s2 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.s3 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.s4 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.s5 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.s6 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.s7 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.s8 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.s9 = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.braveness = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.minLevel = std::stoi(cell);

        std::getline(lineStream, cell, ',');
        newMonster.openingMessage = std::stoi(cell);

        std::getline(lineStream, newMonster.hash, ',');
        
        std::string input = std::to_string(newMonster.index) +
            std::to_string(newMonster.type) +
            std::to_string(newMonster.maxencounters) +
            newMonster.name +
            newMonster.pluName +
            newMonster.armorText +
            std::to_string(newMonster.behaviour) +
            std::to_string(newMonster.alignment) +
            std::to_string(newMonster.stealth) +
            std::to_string(newMonster.randomStrength) +
            std::to_string(newMonster.randomSkill) +
            std::to_string(newMonster.randomIntelligence) +
            std::to_string(newMonster.randomSpeed) +
            std::to_string(newMonster.hp) +
            std::to_string(newMonster.randomHP) +
            std::to_string(newMonster.maxHP) +
            std::to_string(newMonster.image) +
            std::to_string(newMonster.image2) +
            std::to_string(newMonster.sta) +
            std::to_string(newMonster.cha) +
            std::to_string(newMonster.str) +
            std::to_string(newMonster.inte) +
            std::to_string(newMonster.wis) +
            std::to_string(newMonster.skl) +
            std::to_string(newMonster.spd) +
            std::to_string(newMonster.aBlunt) +
            std::to_string(newMonster.aSharp) +
            std::to_string(newMonster.aEarth) +
            std::to_string(newMonster.aAir) +
            std::to_string(newMonster.aFire) +
            std::to_string(newMonster.aWater) +
            std::to_string(newMonster.aPower) +
            std::to_string(newMonster.aMagic) +
            std::to_string(newMonster.aGood) +
            std::to_string(newMonster.aEvil) +
            std::to_string(newMonster.aCold) +
            std::to_string(newMonster.aNature) +
            std::to_string(newMonster.aAcid) +
            std::to_string(newMonster.tPotions) +
            std::to_string(newMonster.tEquipment) +
            std::to_string(newMonster.tFood) +
            std::to_string(newMonster.tWater) +
            std::to_string(newMonster.tTorches) +
            std::to_string(newMonster.tTimepieces) +
            std::to_string(newMonster.tCompasses) +
            std::to_string(newMonster.tKeys) +
            std::to_string(newMonster.tCrystals) +
            std::to_string(newMonster.tCopper) +
            std::to_string(newMonster.tSilver) +
            std::to_string(newMonster.tGold) +
            std::to_string(newMonster.tGems) +
            std::to_string(newMonster.tJewels) +
            std::to_string(newMonster.tCorpse) +
            std::to_string(newMonster.w1) +
            std::to_string(newMonster.w2) +
            std::to_string(newMonster.w3) +
            std::to_string(newMonster.w4) +
            std::to_string(newMonster.w5) +
            std::to_string(newMonster.w6) +
            std::to_string(newMonster.c1) +
            std::to_string(newMonster.c2) +
            std::to_string(newMonster.c3) +
            std::to_string(newMonster.c4) +
            std::to_string(newMonster.c5) +
            std::to_string(newMonster.c6) +
            std::to_string(newMonster.callForHelp) +
            std::to_string(newMonster.s0) +
            std::to_string(newMonster.s1) +
            std::to_string(newMonster.s2) +
            std::to_string(newMonster.s3) +
            std::to_string(newMonster.s4) +
            std::to_string(newMonster.s5) +
            std::to_string(newMonster.s6) +
            std::to_string(newMonster.s7) +
            std::to_string(newMonster.s8) +
            std::to_string(newMonster.s9) +
            std::to_string(newMonster.braveness) +
            std::to_string(newMonster.minLevel) +
          
            "ARX2023";
      
        std::string hash = sha256(input);

        if (hash != newMonster.hash)
        {
            HackedMonsters = 1;

        }

        data.push_back(newMonster);
    }

    file.close();
    return data;

}


void convertMonstersBinary()
{
    std::string filename = "Monsters.csv";
    std::vector<newMonster>  csvData = readMonsterCSV(filename);

    // Convert vector to a dynamically allocated array of NewItem structs
    size_t itemCount = csvData.size();
    Monster_Buffer = new newMonster[itemCount];

    // Copy data from vector to the dynamically allocated array
    for (size_t i = 0; i < itemCount; ++i) {
        Monster_Buffer[i] = csvData[i];
        
    }

}


void createMonsterWeapon(int currentWeapon, int weaponOffset)
{
    int weaponNameOffset = weaponOffset + 6;
    char nameLength =    (char) monstersBinary[weaponOffset+5];
    monsterWeapons[currentWeapon].name = readBinaryString(weaponNameOffset);

    // byte 2 is object length, byte 3 is unknown

    monsterWeapons[currentWeapon].type               = monstersBinary[weaponOffset+0];
    monsterWeapons[currentWeapon].alignment          = monstersBinary[weaponOffset+3];
    monsterWeapons[currentWeapon].weight             = monstersBinary[weaponOffset+4];
    outdata <<  "name: " <<  monsterWeapons[currentWeapon].name  << endl;
    outdata <<  "type: " <<  monsterWeapons[currentWeapon].type  << endl;
    outdata <<  "alignment: " <<  monsterWeapons[currentWeapon].alignment  << endl;
    outdata <<  "weight: " <<  monsterWeapons[currentWeapon].weight  << endl;

    //int wAttributes= weaponOffset+monstersBinary[weaponOffset+5];
    int wAttributes = (weaponOffset + monstersBinary[weaponOffset+1])-20; // Working out from the end of the weapon object

	 int weaponLength = monsterWeapons[currentWeapon].name.length();
	 int special = monstersBinary[weaponNameOffset+weaponLength+1];
	 //84 spells.
	 
	 if (special == 0x83)		// Curses
	 {
	 	monsterWeapons[currentWeapon].special = special;
	 	// Curses from items.
	    outdata <<  "Special Abilit: x083" << " 0x" << std::hex << (weaponNameOffset+weaponLength+1) << std::dec;
		 //16 bytes of data.  Need to figure it out.
		 if (monstersBinary[weaponNameOffset+weaponLength+3+2] == 0x1e)		//Clumsiness
		 {
		 		 monsterWeapons[currentWeapon].specialType = CLUMSINESS;	 
		 	 outdata << "type (Clumsiness):" << CLUMSINESS <<    endl;
		 } else if (monstersBinary[weaponNameOffset+weaponLength+3+2] == 0x10)		//Thirst
		 {
		 		 monsterWeapons[currentWeapon].specialType = THIRST;	 
		 	 outdata << "type (Thirst):" << THIRST <<    endl;
		 } else if (monstersBinary[weaponNameOffset+weaponLength+3+2] == 0x1)		//Loki's Wrath
		 {
		 		 monsterWeapons[currentWeapon].specialType = LOKIS_WRATH;	 
		  outdata << "type (Loki's Wrath):" << LOKIS_WRATH <<    endl;
		 } else if (monstersBinary[weaponNameOffset+weaponLength+3+2] == 0x5)		//Slow Death
		 {
		 		 monsterWeapons[currentWeapon].specialType = SLOW_DEATH;	 
		 	 outdata << "type (Slow Death):" << SLOW_DEATH <<    endl;
		 } 
		 for(int i=0;i<16;i++)
		 {
		 	int pos = weaponNameOffset+weaponLength+3+i;
		  			outdata << " 0x" << std::hex << ((int)monstersBinary[pos]) <<std::dec ;
		 }
		 //String for the descriptiong of what happens with this weapon
		 outdata		  << std::dec <<    endl;
	    string description =  readBinaryString(weaponNameOffset+weaponLength+20);
	     monsterWeapons[currentWeapon].specialName = description;
	    outdata <<  "Spcl Descrip: " << monsterWeapons[currentWeapon].specialName  <<   endl;
	 }
	 else if (special == 0x88)
	 {	
	 	// Special Items
	    outdata <<  "Special Abilit: 0x88" << " 0x" << std::hex << (weaponNameOffset+weaponLength+1) << std::dec;
		 for(int i=0;i<33;i++)
		 {
		 	int pos = weaponNameOffset+weaponLength+3+i;
		  			outdata << " 0x" << std::hex << ((int)monstersBinary[pos]) <<std::dec;
		 }
		 
		 outdata		  << std::dec <<    endl;
	    string description =  readBinaryString(weaponNameOffset+weaponLength+36);
	    outdata <<  "Spcl Descrip: " <<description  <<   endl;
	 }
	 else
	 	  if (special != 0)
		  	    outdata <<  "Special Abilit: WARNING!  " << special << " 0x" <<std::hex <<(weaponNameOffset+weaponLength+1) << std::dec <<    endl;
	      else
	    	outdata <<  "Special Abilit: 0" << " 0x" <<std::hex <<(weaponNameOffset+weaponLength+1) << std::dec <<    endl;

    monsterWeapons[currentWeapon].melee              = monstersBinary[wAttributes+1];
    outdata <<  "melee: " <<  monsterWeapons[currentWeapon].melee  << endl;
    monsterWeapons[currentWeapon].ammo               = monstersBinary[wAttributes+2];
    outdata <<  "ammo: " <<  monsterWeapons[currentWeapon].ammo  << endl;
    monsterWeapons[currentWeapon].blunt              = monstersBinary[wAttributes+3];
    outdata <<  "blunt: " <<  monsterWeapons[currentWeapon].blunt  << endl;
    monsterWeapons[currentWeapon].sharp              = monstersBinary[wAttributes+4];
    outdata <<  "sharp: " <<  monsterWeapons[currentWeapon].sharp  << endl;
    monsterWeapons[currentWeapon].earth              = monstersBinary[wAttributes+5];
    outdata <<  "earth: " <<  monsterWeapons[currentWeapon].earth  << endl;
    monsterWeapons[currentWeapon].air                = monstersBinary[wAttributes+6];
    outdata <<  "air: " <<  monsterWeapons[currentWeapon].air  << endl;
    monsterWeapons[currentWeapon].fire               = monstersBinary[wAttributes+7];
    outdata <<  "fire: " <<  monsterWeapons[currentWeapon].fire  << endl;
    monsterWeapons[currentWeapon].water              = monstersBinary[wAttributes+8];
    outdata <<  "water: " <<  monsterWeapons[currentWeapon].water  << endl;
    monsterWeapons[currentWeapon].power              = monstersBinary[wAttributes+9];
    outdata <<  "power: " <<  monsterWeapons[currentWeapon].power  << endl;
    monsterWeapons[currentWeapon].magic              = monstersBinary[wAttributes+10];
    outdata <<  "magic: " <<  monsterWeapons[currentWeapon].magic  << endl;
    monsterWeapons[currentWeapon].good               = monstersBinary[wAttributes+11];
    outdata <<  "good: " <<  monsterWeapons[currentWeapon].good  << endl;
    monsterWeapons[currentWeapon].evil               = monstersBinary[wAttributes+12];
    outdata <<  "evil: " <<  monsterWeapons[currentWeapon].evil  << endl;
    monsterWeapons[currentWeapon].cold               = monstersBinary[wAttributes+13];
    outdata <<  "cold: " <<  monsterWeapons[currentWeapon].cold  << endl;
    monsterWeapons[currentWeapon].minStrength        = monstersBinary[wAttributes+14];
    outdata <<  "minStrength: " <<  monsterWeapons[currentWeapon].minStrength  << endl;
    monsterWeapons[currentWeapon].minDexterity       = monstersBinary[wAttributes+15];
    outdata <<  "minDexterity: " <<  monsterWeapons[currentWeapon].minDexterity  << endl;
    monsterWeapons[currentWeapon].hp                 = monstersBinary[wAttributes+16];
    outdata <<  "hp: " <<  monsterWeapons[currentWeapon].hp  << endl;
    monsterWeapons[currentWeapon].maxHP              = monstersBinary[wAttributes+17];
    outdata <<  "maxHP: " <<  monsterWeapons[currentWeapon].maxHP  << endl;
    monsterWeapons[currentWeapon].flags              = monstersBinary[wAttributes+18];
    outdata <<  "flags: " <<  monsterWeapons[currentWeapon].flags  << endl;
    monsterWeapons[currentWeapon].parry              = monstersBinary[wAttributes+19];
    outdata <<  "parry: " <<  monsterWeapons[currentWeapon].parry  << endl;

    int weaponDescription = (monsterWeapons[currentWeapon].flags);

cout << "Name:  " << currentWeapon << "," << monsterWeapons[currentWeapon].name << "\n";

    cout << "Name:  " << monsterWeapons[currentWeapon].name << ", " << weaponDescription << "\n";
    cout << "HP:    " << monsterWeapons[currentWeapon].hp << "\n";
    cout << "Align: " << monsterWeapons[currentWeapon].alignment << "\n\n";
    cout << "Blunt: " << monsterWeapons[currentWeapon].blunt << "\nSharp: " << monsterWeapons[currentWeapon].sharp << "\nEarth: "
        << monsterWeapons[currentWeapon].earth << "\nAir:   " << monsterWeapons[currentWeapon].air
        << "\nFire:  " << monsterWeapons[currentWeapon].fire << "\nWater: " << monsterWeapons[currentWeapon].water << "\nPower: "
        << monsterWeapons[currentWeapon].power << "\nMagic: " << monsterWeapons[currentWeapon].magic
        << "\nGood:  " << monsterWeapons[currentWeapon].good << "\nEvil:  " << monsterWeapons[currentWeapon].evil << "\nCold:  "
        << monsterWeapons[currentWeapon].cold << "\n\n";

    


}


string readBinaryString(int stringOffset)
{
    stringstream ss;
    int z = stringOffset; // current location in the binary
    int c = 0;          // current byte
    //string n = "";      // string to store built up string
    string result = "";

   while (!(monstersBinary[z]==0))
    {
        c = monstersBinary[z];
        ss << (char) c;
        z++;
    }
    result = ss.str();
    //cout << "Attack: " << result << " ( offset: " << z << ") - " << currentWeapon << "\n";
    return result;
}


void readMonsterNameText(int monsterNo, int nameOffset)
{
    // Special Case - "thief" class has value 0
    if (monsterNo==1) nameOffset = 0x2AA;
    //if (monsterNo==2) nameOffset = 0x7E4;

    stringstream ss;
    int z = nameOffset; // current location in the binary
    int c = 0;          // current byte
    string n = "";      // string to store built up string

    // Loop through until 0 found
    // Some special monster classes have multiple entries here

    while (!(monstersBinary[z]==0))
    {
        c = monstersBinary[z];
        ss << (char) c;
        z++;
    }
    if (maxNumberEncountered > 1) pluralNameOffset = z+1;
    Monster_Buffer[monsterNo].name = ss.str();
    //cout << "Name " << monsterNo << ", Offset " << nameOffset << " : " << Monster_Buffer[monsterNo].name << "\n";
}


void readMonsterPluralNameText(int monsterNo, int pluralNameOffset)
{
    // Special Case - "thief" class has value 0
    if (monsterNo==1) pluralNameOffset = 0x2CF;
    if (monsterNo== CUTTHROAT) pluralNameOffset = 0x2CF;
    if (monsterNo== BRIGAND) pluralNameOffset = 0x2d7;
    //if (monsterNo==2) nameOffset = 0x7E4;

    stringstream ss;
    int z = pluralNameOffset; // current location in the binary
    int c = 0;          // current byte
    string n = "";      // string to store built up string

    // Loop through until 0 found
    // Some special monster classes have multiple entries here

    while (!(monstersBinary[z]==0))
    {
        c = monstersBinary[z];
        ss << (char) c;
        z++;
    }
    Monster_Buffer[monsterNo].pluName = ss.str();
    //cout << "Name " << monsterNo << ", Offset " << nameOffset << " : " << Monster_Buffer[monsterNo].name << "\n";
}


void readMonsterDeathText(int monsterNo, int deathOffset)
{
    // Change variable name

    if (monsterNo==DEVOURER) deathOffset = 0x42;
//cout << deathOffset << "\n";
    stringstream ss;
    int z = deathOffset; // current location in the binary
    int c = 0;          // current byte
    string n = "";      // string to store built up string

    // Loop through until 0 found
    // Some special monster classes have multiple entries here

    while (!(monstersBinary[z]==0xAE))
    {
        c = monstersBinary[z];
        //if ((c==0x0D) || (c==0xA5)) c = 0x40;
        //ss << (char) c;
        if (c==0xA5) c = 0x40;
        if (!(c==0x0D)) ss << (char) c;
        z++;
    }
    Monster_Buffer[monsterNo].armorText = ss.str();
    //cout << "Death message: " << Monster_Buffer[monsterNo].armorText << "\n\n";

//Monster_Buffer[monsterNo].armorText = "dies";

}


void loadMonstersBinary() {
    FILE* fp;               // file pointer - used when reading files
    char tempString[100];   // temporary string
    int err = sprintf_s(tempString, sizeof(tempString), "%s%s", "data/map/core/", "monsters.bin");

    if (err < 0) {
        // Handle sprintf_s error
        perror("Error formatting file path");
        return; // Exit the function
    }

    err = fopen_s(&fp, tempString, "rb");
    if (err != 0 || fp == NULL) {
        // Handle file open error
        perror("Error opening file");
        return; // Exit the function
    }

    // File opened successfully
    for (int i = 0; i < noOfMonstersFile; i++) {
        monstersBinary[i] = fgetc(fp);
    }
    fclose(fp);
}


void loadEncounters()
{
	std::ifstream instream;
	std::string junk, data, junk2;
	instream.open("data/map/core/encounters.txt");
	if( !instream )
	{
      cout << "Error: Encounters file could not be loaded" << endl;
	}

	for(int i = 0; i < noOfEncounters; ++i) // number of monsters
	{
		string junk, line, text;
		int attributes = 48; // number of lines making up each record
		string::size_type idx;
		getline(instream, junk); // read first line as blank
		for (int a = 0; a < attributes; ++a) // number of attributes per record
		{
			getline(instream, line);
			idx = line.find(':');
			text = line.substr(idx+1);
if (i == DEVOURER)
			cout << "i:" << i <<" a:"<<a<<" " <<line << "  " ;
if (a==47) cout << "\n"; 
			if (a==0) { Monster_Buffer[i].name = text; }
			if (a==1) { Monster_Buffer[i].pluName = text; }
			if (a==2) { Monster_Buffer[i].armorText = text; }
			if (a==3) { Monster_Buffer[i].hp = atoi(text.c_str()); }
			if (a==4) { Monster_Buffer[i].alignment = atoi(text.c_str()); }
			if (a==5) { Monster_Buffer[i].image = atoi(text.c_str()); }

			if (a==6) { Monster_Buffer[i].sta = atoi(text.c_str()); }
			if (a==7) { Monster_Buffer[i].cha = atoi(text.c_str());}
			if (a==8) { Monster_Buffer[i].str = atoi(text.c_str());}
			if (a==9) { Monster_Buffer[i].inte = atoi(text.c_str());}
			if (a==10) { Monster_Buffer[i].wis = atoi(text.c_str()); }
			if (a==11) { Monster_Buffer[i].skl = atoi(text.c_str());}
			if (a==12) { Monster_Buffer[i].spd = atoi(text.c_str());}

			if (a==13) { Monster_Buffer[i].tFood = atoi(text.c_str()); }
			if (a==14) { Monster_Buffer[i].tWater = atoi(text.c_str());}
			if (a==15) { Monster_Buffer[i].tTorches = atoi(text.c_str());}
			if (a==16) { Monster_Buffer[i].tTimepieces = atoi(text.c_str()); }
			if (a==17) { Monster_Buffer[i].tCompasses = atoi(text.c_str()); }
			if (a==18) { Monster_Buffer[i].tKeys = atoi(text.c_str()); }
			if (a==19) { Monster_Buffer[i].tCrystals = atoi(text.c_str());}
			if (a==20) { Monster_Buffer[i].tGems = atoi(text.c_str()); }
			if (a==21) { Monster_Buffer[i].tJewels = atoi(text.c_str());}
			if (a==22) { Monster_Buffer[i].tGold = atoi(text.c_str());}
			if (a==23) { Monster_Buffer[i].tSilver = atoi(text.c_str()); }
			if (a==24) { Monster_Buffer[i].tCopper = atoi(text.c_str()); 	}

			if (a==25) { Monster_Buffer[i].aBlunt = Hex2Dec(text.c_str()); }
			if (a==26) { Monster_Buffer[i].aSharp = Hex2Dec(text.c_str()); }
			if (a==27) { Monster_Buffer[i].aEarth = Hex2Dec(text.c_str()); }
			if (a==28) { Monster_Buffer[i].aAir = Hex2Dec(text.c_str()); }
			if (a==29) { Monster_Buffer[i].aFire = Hex2Dec(text.c_str()); }
			if (a==30) { Monster_Buffer[i].aWater = Hex2Dec(text.c_str()); }
			if (a==31) { Monster_Buffer[i].aPower = Hex2Dec(text.c_str()); }
			if (a==32) { Monster_Buffer[i].aMagic = Hex2Dec(text.c_str()); }
			if (a==33) { Monster_Buffer[i].aGood = Hex2Dec(text.c_str()); }
			if (a==34) { Monster_Buffer[i].aEvil = Hex2Dec(text.c_str()); }
			if (a==35) { Monster_Buffer[i].aCold = Hex2Dec(text.c_str()); }

			if (a==36) { Monster_Buffer[i].w1 = atoi(text.c_str());}
			if (a==37) { Monster_Buffer[i].w2 = atoi(text.c_str());}
			if (a==38) { Monster_Buffer[i].w3 = atoi(text.c_str());}
			if (a==39) { Monster_Buffer[i].w4 = atoi(text.c_str());}
			if (a==40) { Monster_Buffer[i].w5 = atoi(text.c_str());}
			if (a==41) { Monster_Buffer[i].w6 = atoi(text.c_str());}

			if (a==42) { Monster_Buffer[i].c1 = atoi(text.c_str()); }
			if (a==43) { Monster_Buffer[i].c2 = atoi(text.c_str());}
			if (a==44) { Monster_Buffer[i].c3 = atoi(text.c_str());}
			if (a==45) { Monster_Buffer[i].c4 = atoi(text.c_str());}
			if (a==46) { Monster_Buffer[i].c5 = atoi(text.c_str());}
			if (a==47) { Monster_Buffer[i].c6 = atoi(text.c_str());}
if (i == DEVOURER)
std::cout << "Name :" << Monster_Buffer[i].name << "c1 "<< Monster_Buffer[i].c1 << " c2 "<< Monster_Buffer[i].c2 << "\n";
		}
	}
	instream.close();
}

std::vector<openingMessages> readMessagesCSV(const std::string& filename) {
    std::vector<openingMessages> data;
    // Open the CSV file
    std::ifstream file("data/map/core/" + filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data; // Return empty vector if file couldn't be opened
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        openingMessages opMessage;
        std::string cell;

        // Parsing CSV fields into the struct members
        std::getline(lineStream, cell, ','); // Assuming the index is the first column
        opMessage.index = std::stoi(cell);

        std::getline(lineStream, opMessage.message, ',');

        data.push_back(opMessage);
    }

    file.close();
    return data;

    
}


void convertMessagesBinary()
{
    std::string filename = "Messages.csv";
    std::vector<openingMessages>  csvData = readMessagesCSV(filename);

    // Convert vector to a dynamically allocated array of NewItem structs
    size_t itemCount = csvData.size();
    Message_Buffer = new openingMessages[itemCount];

    // Copy data from vector to the dynamically allocated array
    for (size_t i = 0; i < itemCount; ++i) {
        Message_Buffer[i] = csvData[i];

    }

}