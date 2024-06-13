#ifndef _actorh 
#define _actorh 

#include <string>
#include "constants.h"

#include <vector>







std::string readBinaryString(int stringOffset);

class newMonster;

extern newMonster* Monster_Buffer;



void loadEncounters();
void loadMonstersBinary();
void initialiseMonsterOffsets();
void convertMonstersBinary();
void readMonsterNameText(int monsterNo, int nameTextOffset);
void readMonsterPluralNameText(int monsterNo, int pluralNameOffset);
void readMonsterDeathText(int monsterNo, int deathTextOffset);
void createMonsterWeapon(int currentWeapon, int weaponOffset);
void convertMonstersBinary();




struct monsterFramePair
{
    int startFrame;
    int endFrame;
};

struct monster
{

    //uint8_t from stdint.h
	
    int type;
    int maxencounters;
	std::string name;
	std::string pluName;
	std::string armorText;
	int behaviour; // evil, bad, neutral, good
	int alignment;
	int stealth;
	int randomStrength;
	int randomSkill;
	int randomIntelligence;
	int randomSpeed;
	int hp;
	int randomHP;
	int maxHP;
	int image;  // becomes first frame in animations array
	int image2; // becomes last frame in animations array

	int sta;
	int	cha;
	int	str;
	int	inte;
	int	wis;
	int	skl;
	int	spd;

	int aBlunt;
	int aSharp;
	int aEarth;
	int aAir;
	int aFire;
	int aWater;
	int aPower;
	int aMagic;
	int aGood;
	int aEvil;
	int aCold;
	int aNature;
	int aAcid;

	int tPotions;
	int tEquipment;
	int tFood;
	int tWater;
	int tTorches;
	int tTimepieces;
	int tCompasses;
	int tKeys;
	int tCrystals;
	int tCopper;
	int tSilver;
	int tGold;
	int tGems;
	int tJewels;
	int tCorpse;

	int chosenWeapon;
	int w1;
	int w2;
	int w3;
	int w4;
	int w5;
	int w6;
	int c1;
	int c2;
	int c3;
	int c4;
	int c5;
	int c6;
	int stunnedTurnsRemaining;
	std::string stunnedtext;
	int callForHelp;
	int spellBuffer[4];
	int braveness;
};

struct newMonster
{

	//uint8_t from stdint.h
	int index;
	int type;
	int maxencounters;
	std::string name;
	std::string pluName;
	std::string armorText;
	int behaviour; // evil, bad, neutral, good
	int alignment;
	int stealth;
	int randomStrength;
	int randomSkill;
	int randomIntelligence;
	int randomSpeed;
	int hp;
	int randomHP;
	int maxHP;
	int image;  // becomes first frame in animations array
	int image2; // becomes last frame in animations array

	int sta;
	int	cha;
	int	str;
	int	inte;
	int	wis;
	int	skl;
	int	spd;

	int aBlunt;
	int aSharp;
	int aEarth;
	int aAir;
	int aFire;
	int aWater;
	int aPower;
	int aMagic;
	int aGood;
	int aEvil;
	int aCold;
	int aNature;
	int aAcid;

	int tPotions;
	int tEquipment;
	int tFood;
	int tWater;
	int tTorches;
	int tTimepieces;
	int tCompasses;
	int tKeys;
	int tCrystals;
	int tCopper;
	int tSilver;
	int tGold;
	int tGems;
	int tJewels;
	int tCorpse;

	int chosenWeapon;
	int w1;
	int w2;
	int w3;
	int w4;
	int w5;
	int w6;
	int c1;
	int c2;
	int c3;
	int c4;
	int c5;
	int c6;
	int stunnedTurnsRemaining =0;
	std::string stunnedtext;
	int callForHelp;
	int s0;
	int s1;
	int s2;
	int s3;
	int s4;
	int s5;
	int s6;
	int s7;
	int s8;
	int s9;
	int spellBuffer[4];
	int braveness;
	int minLevel;
	std::string hash;
};

std::vector<newMonster> readMonsterCSV(const std::string& filename);

struct weapon
{
	std::string article;
	std::string name;
	std::string desc;
	std::string descMon;
	int type;
	int index;
	int flags;
	int minStrength;
	int minDexterity;
	int useStrength;
	int blunt;
	int sharp;
	int earth;
	int air;
	int fire;
	int water;
	int power;
	int magic; // mental
	int good; // cleric
	int evil;
	int cold;
	int nature;
	int acid;
	int weight;
	int hp;
	int maxHP;
	int alignment;
	int melee;
	int ammo;
	int parry;
	
	int special;
	int specialType;
	std::string specialName;
};



extern weapon monsterWeapons[noOfMonsterWeapons];
extern unsigned char monstersBinary[noOfMonstersFile];

 
#endif // _actorh 
