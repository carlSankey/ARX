#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>

#include "actor.h"
#include "misc.h"
#include "globals.h"

using namespace std;


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


monster Monsters[noOfEncounters];           // City and Dungeon monsters combined
weapon monsterWeapons[noOfMonsterWeapons];  // Weapons & attacks which are part of the Dungeon monsters data
//weapon Weapons[noOfWeapons];                // Weapons

unsigned char monstersBinary[monstersFileSize];

int monsterOffsets[noOfEncounters];
int pluralNameOffset;
int maxNumberEncountered;
int currentWeapon;

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
}

void convertMonstersBinary()
{
    // Reads through the binary block and creates entry in Monsters[]
    // Bytes 2, 4, 6, 8 and 10 appear to always have a value of 0xAA

    int idx = 0; // Start byte for each monster
    currentWeapon = 0; // Index for next weapon or attack to be added to monsterWeapons[]

    for (int i=0; i<noOfEncounters; i++)
    {
        idx = monsterOffsets[i];  // Sets current monster start address in binary block

        int attackOffset[3]; // Array of offsets to 3 Dungeon weapons / attacks
        maxNumberEncountered = monstersBinary[idx];

        int animationNumber = monstersBinary[idx+0x1D];
        Monsters[i].image = animations[animationNumber].startFrame;
        Monsters[i].image2 = animations[animationNumber].endFrame;

        int nameTextOffset = idx + monstersBinary[(idx+1)];
        readMonsterNameText(i,nameTextOffset);

        // Note: idx+3 might be offset for plural name - if one exists
        if (maxNumberEncountered>1) readMonsterPluralNameText(i,pluralNameOffset);

        int deathTextOffset = idx + 66;
        readMonsterDeathText(i,deathTextOffset);

        Monsters[i].hp = monstersBinary[idx+0x23];
        Monsters[i].maxHP = monstersBinary[idx+0x23];

        // Special cases - thief
        if (i==THIEF) { Monsters[i].hp = 5; Monsters[i].maxHP = 5; }
        if (i==FBI_AGENT) { Monsters[i].hp = 0; Monsters[i].maxHP = 0; }

        Monsters[i].alignment = monstersBinary[idx+12];

        Monsters[i].sta =   monstersBinary[idx+0x24];
        Monsters[i].cha =   monstersBinary[idx+0x25];
        Monsters[i].str =   monstersBinary[idx+0x26];
        Monsters[i].inte =  monstersBinary[idx+0x27];
        Monsters[i].wis =   monstersBinary[idx+0x28];
        Monsters[i].skl =   monstersBinary[idx+0x29];
        Monsters[i].spd =   monstersBinary[idx+0x2A];

        Monsters[i].tFood =         monstersBinary[idx+0x2B];
        Monsters[i].tWater =        monstersBinary[idx+0x2C];
        Monsters[i].tTorches =      monstersBinary[idx+0x2D];
        Monsters[i].tTimepieces =   monstersBinary[idx+0x2E];
        Monsters[i].tCompasses =    monstersBinary[idx+0x2F];
        Monsters[i].tKeys =         monstersBinary[idx+0x30];
        Monsters[i].tCrystals =     monstersBinary[idx+0x31];
        Monsters[i].tGems =         monstersBinary[idx+0x32];
        Monsters[i].tJewels =       monstersBinary[idx+0x33];
        Monsters[i].tGold =         monstersBinary[idx+0x34];
        Monsters[i].tSilver =       monstersBinary[idx+0x35];
        Monsters[i].tCopper =       monstersBinary[idx+0x36];

        Monsters[i].aBlunt =    monstersBinary[idx+0x37];
        Monsters[i].aSharp =    monstersBinary[idx+0x38];
        Monsters[i].aEarth =    monstersBinary[idx+0x39];
        Monsters[i].aAir =      monstersBinary[idx+0x3A];
        Monsters[i].aFire =     monstersBinary[idx+0x3B];
        Monsters[i].aWater =    monstersBinary[idx+0x3C];
        Monsters[i].aPower =    monstersBinary[idx+0x3D];
        Monsters[i].aMagic =    monstersBinary[idx+0x3E];
        Monsters[i].aGood =     monstersBinary[idx+0x3F];
        Monsters[i].aEvil =     monstersBinary[idx+0x40];
        Monsters[i].aCold =     monstersBinary[idx+0x41];


        // Temporary weapon values based on existing ARX weapon handling
        Monsters[i].w1 = 1;
        Monsters[i].w2 = 0;
        Monsters[i].w3 = 0;
        Monsters[i].w4 = 0;
        Monsters[i].w5 = 0;
        Monsters[i].w6 = 0;
        Monsters[i].c1 = 100;
        Monsters[i].c2 = 0;
        Monsters[i].c3 = 0;
        Monsters[i].c4 = 0;
        Monsters[i].c5 = 0;
        Monsters[i].c6 = 0;

        // Weapon / attack reading

        int weapon1 = idx + monstersBinary[idx+5];
        int weapon2 = idx + monstersBinary[idx+7];
        int weapon3 = idx + monstersBinary[idx+9];

        if (i==THIEF)
        {
            weapon1 = 0x32F; // Skean
            weapon1 = 0x32F;
            weapon1 = 0x32F;
        }

        createMonsterWeapon(currentWeapon, weapon1);
        Monsters[i].w1 = currentWeapon;
        currentWeapon++; // Increment each time a new weapon or attack is created

        if (!(weapon1 == weapon2))
        {
            createMonsterWeapon(currentWeapon, weapon2);
            Monsters[i].w2 = currentWeapon;
            currentWeapon++; // Increment each time a new weapon or attack is created
        }

        if ((!(weapon1 == weapon3)) && (!(weapon2 == weapon3)))
        {
            createMonsterWeapon(currentWeapon, weapon3);
            Monsters[i].w3 = currentWeapon;
            currentWeapon++; // Increment each time a new weapon or attack is created
        }
    }

}


void createMonsterWeapon(int currentWeapon, int weaponOffset)
{
    int weaponNameOffset = weaponOffset + 6;
    monsterWeapons[currentWeapon].name = readBinaryString(weaponNameOffset);

    // byte 2 is object length, byte 3 is unknown
    monsterWeapons[currentWeapon].type               = monstersBinary[weaponOffset+0];
    monsterWeapons[currentWeapon].alignment          = monstersBinary[weaponOffset+3];
    monsterWeapons[currentWeapon].weight             = monstersBinary[weaponOffset+4];

    //int wAttributes= weaponOffset+monstersBinary[weaponOffset+5];
    int wAttributes = (weaponOffset + monstersBinary[weaponOffset+1])-20; // Working out from the end of the weapon object

    monsterWeapons[currentWeapon].melee              = monstersBinary[wAttributes+1];
    monsterWeapons[currentWeapon].ammo               = monstersBinary[wAttributes+2];
    monsterWeapons[currentWeapon].blunt              = monstersBinary[wAttributes+3];
    monsterWeapons[currentWeapon].sharp              = monstersBinary[wAttributes+4];
    monsterWeapons[currentWeapon].earth              = monstersBinary[wAttributes+5];
    monsterWeapons[currentWeapon].air                = monstersBinary[wAttributes+6];
    monsterWeapons[currentWeapon].fire               = monstersBinary[wAttributes+7];
    monsterWeapons[currentWeapon].water              = monstersBinary[wAttributes+8];
    monsterWeapons[currentWeapon].power              = monstersBinary[wAttributes+9];
    monsterWeapons[currentWeapon].magic              = monstersBinary[wAttributes+10];
    monsterWeapons[currentWeapon].good               = monstersBinary[wAttributes+11];
    monsterWeapons[currentWeapon].evil               = monstersBinary[wAttributes+12];
    monsterWeapons[currentWeapon].cold               = monstersBinary[wAttributes+13];
    monsterWeapons[currentWeapon].minStrength        = monstersBinary[wAttributes+14];
    monsterWeapons[currentWeapon].minDexterity       = monstersBinary[wAttributes+15];
    monsterWeapons[currentWeapon].hp                 = monstersBinary[wAttributes+16];
    monsterWeapons[currentWeapon].maxHP              = monstersBinary[wAttributes+17];
    monsterWeapons[currentWeapon].flags              = monstersBinary[wAttributes+18];
    monsterWeapons[currentWeapon].parry              = monstersBinary[wAttributes+19];

    int weaponDescription = (monsterWeapons[currentWeapon].flags);

    //cout << "Name:  " << currentWeapon << "," << monsterWeapons[currentWeapon].name << "\n";
    /*
    cout << "Name:  " << monsterWeapons[currentWeapon].name << ", " << weaponDescription << "\n";
    cout << "HP:    " << monsterWeapons[currentWeapon].hp << "\n";
    cout << "Align: " << monsterWeapons[currentWeapon].alignment << "\n\n";
    cout << "Blunt: " << monsterWeapons[currentWeapon].blunt << "\nSharp: " << monsterWeapons[currentWeapon].sharp << "\nEarth: "
        << monsterWeapons[currentWeapon].earth << "\nAir:   " << monsterWeapons[currentWeapon].air
        << "\nFire:  " << monsterWeapons[currentWeapon].fire << "\nWater: " << monsterWeapons[currentWeapon].water << "\nPower: "
        << monsterWeapons[currentWeapon].power << "\nMagic: " << monsterWeapons[currentWeapon].magic
        << "\nGood:  " << monsterWeapons[currentWeapon].good << "\nEvil:  " << monsterWeapons[currentWeapon].evil << "\nCold:  "
        << monsterWeapons[currentWeapon].cold << "\n\n";
    */


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
    Monsters[monsterNo].name = ss.str();
    //cout << "Name " << monsterNo << ", Offset " << nameOffset << " : " << Monsters[monsterNo].name << "\n";
}


void readMonsterPluralNameText(int monsterNo, int pluralNameOffset)
{
    // Special Case - "thief" class has value 0
    if (monsterNo==1) pluralNameOffset = 0x2CF;
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
    Monsters[monsterNo].pluName = ss.str();
    //cout << "Name " << monsterNo << ", Offset " << nameOffset << " : " << Monsters[monsterNo].name << "\n";
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
    Monsters[monsterNo].armorText = ss.str();
    //cout << "Death message: " << Monsters[monsterNo].armorText << "\n\n";

//Monsters[monsterNo].armorText = "dies";

}






void loadMonstersBinary()
{
    // Loads 42kb of monster binary data into the "monstersBinary" array
    FILE *fp;               // file pointer - used when reading files
    char tempString[100];   // temporary string
    sprintf(tempString,"%s%s","data/map/","monsters.bin");
    fp = fopen(tempString, "rb");
    if( fp != NULL )
    {
        for(int i=0;i<monstersFileSize;i++)
        {
            monstersBinary[i] = fgetc(fp);
        }
    }
    fclose(fp);

    //getSingleBinaryMonster(0, int monsterEndByte);
}

void loadEncounters()
{
	std::ifstream instream;
	std::string junk, data, junk2;
	instream.open("data/map/encounters.txt");
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
			//if (a==0) cout << text << " = " << i << ",\n";

			if (a==0) { Monsters[i].name = text; }
			if (a==1) { Monsters[i].pluName = text; }
			if (a==2) { Monsters[i].armorText = text; }
			if (a==3) { Monsters[i].hp = atoi(text.c_str()); }
			if (a==4) { Monsters[i].alignment = atoi(text.c_str()); }
			if (a==5) { Monsters[i].image = atoi(text.c_str()); }

			if (a==6) { Monsters[i].sta = atoi(text.c_str()); }
			if (a==7) { Monsters[i].cha = atoi(text.c_str());}
			if (a==8) { Monsters[i].str = atoi(text.c_str());}
			if (a==9) { Monsters[i].inte = atoi(text.c_str());}
			if (a==10) { Monsters[i].wis = atoi(text.c_str()); }
			if (a==11) { Monsters[i].skl = atoi(text.c_str());}
			if (a==12) { Monsters[i].spd = atoi(text.c_str());}

			if (a==13) { Monsters[i].tFood = atoi(text.c_str()); }
			if (a==14) { Monsters[i].tWater = atoi(text.c_str());}
			if (a==15) { Monsters[i].tTorches = atoi(text.c_str());}
			if (a==16) { Monsters[i].tTimepieces = atoi(text.c_str()); }
			if (a==17) { Monsters[i].tCompasses = atoi(text.c_str()); }
			if (a==18) { Monsters[i].tKeys = atoi(text.c_str()); }
			if (a==19) { Monsters[i].tCrystals = atoi(text.c_str());}
			if (a==20) { Monsters[i].tGems = atoi(text.c_str()); }
			if (a==21) { Monsters[i].tJewels = atoi(text.c_str());}
			if (a==22) { Monsters[i].tGold = atoi(text.c_str());}
			if (a==23) { Monsters[i].tSilver = atoi(text.c_str()); }
			if (a==24) { Monsters[i].tCopper = atoi(text.c_str()); 	}

			if (a==25) { Monsters[i].aBlunt = Hex2Dec(text.c_str()); }
			if (a==26) { Monsters[i].aSharp = Hex2Dec(text.c_str()); }
			if (a==27) { Monsters[i].aEarth = Hex2Dec(text.c_str()); }
			if (a==28) { Monsters[i].aAir = Hex2Dec(text.c_str()); }
			if (a==29) { Monsters[i].aFire = Hex2Dec(text.c_str()); }
			if (a==30) { Monsters[i].aWater = Hex2Dec(text.c_str()); }
			if (a==31) { Monsters[i].aPower = Hex2Dec(text.c_str()); }
			if (a==32) { Monsters[i].aMagic = Hex2Dec(text.c_str()); }
			if (a==33) { Monsters[i].aGood = Hex2Dec(text.c_str()); }
			if (a==34) { Monsters[i].aEvil = Hex2Dec(text.c_str()); }
			if (a==35) { Monsters[i].aCold = Hex2Dec(text.c_str()); }

			if (a==36) { Monsters[i].w1 = atoi(text.c_str());}
			if (a==37) { Monsters[i].w2 = atoi(text.c_str());}
			if (a==38) { Monsters[i].w3 = atoi(text.c_str());}
			if (a==39) { Monsters[i].w4 = atoi(text.c_str());}
			if (a==40) { Monsters[i].w5 = atoi(text.c_str());}
			if (a==41) { Monsters[i].w6 = atoi(text.c_str());}

			if (a==42) { Monsters[i].c1 = atoi(text.c_str()); }
			if (a==43) { Monsters[i].c2 = atoi(text.c_str());}
			if (a==44) { Monsters[i].c3 = atoi(text.c_str());}
			if (a==45) { Monsters[i].c4 = atoi(text.c_str());}
			if (a==46) { Monsters[i].c5 = atoi(text.c_str());}
			if (a==47) { Monsters[i].c6 = atoi(text.c_str());}

		}
	}
	instream.close();
}

