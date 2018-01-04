#ifndef PLAYER_H
#define PLAYER_H

#include <string>

using namespace std;

void createNewCharacter(int scenario);

void initStats();

void increaseExperience(int xp);
void increaseLevel();
void checkActiveMagic();

void checkBackgroundTime();
void addMinute();
void addHour();
void addDay();
void addMonth();
void addYear();

string checkAlcohol();
string checkHunger();
string checkThirst();
string checkEncumbrance();
string checkPoison();
string checkDisease();
void updateDisease();
void updatePoison();





struct effectItem
{
	//string name;
	int effect;
	int negativeValue;
	int positiveValue;
	int duration;
};


struct doorDetail
{
	int x;
	int y;
	int level;
	int direction;
};



struct Player
{
  int teleporting;
  int buffer_index;
  int infoPanel;
  int priWeapon;
  int secWeapon;
  int headArmour;
  int bodyArmour;
  int legsArmour;
  int armsArmour;
  int timeOfDay;
  int minutes;
  int hours;
  int days;
  int months;
  int years;
  int scenario;
  int map;
  int mapWidth;
  int mapHeight;
  int x;
  int oldx;
  int y;
  int oldy;
  int facing;
  int front;
  int back;
  int left;
  int right;
  int frontheight;
  int leftheight;
  int rightheight;
  int floorTexture;
  int ceiling;
  int location;
  int special;

  int sta;
  int chr;
  int str;
  int inte;
  int wis;
  int skl;
  int maxhp;
  int hp;
  int xp;
  int level; // xp level

  int chrPartials;
  int intPartials;
  int strPartials;

  int speed;
  int stealth;
  bool diagOn;
  bool mapOn;
  bool fpsOn;
  bool miniMapOn;

  int silver;
  int gold;
  int copper;
  int food;
  int torches;
  int water;
  int timepieces;
  int crystals;
  int jewels;
  int gems;
  int compasses;
  int keys;

  string status_text;
  bool alive;
  bool encounter_done;
  bool game_on;
  int gender;
  string name;
  string message;
  string weapon1;
  string weapon2;
  int zone;
  int zoneSet;
  int current_zone; // used by drawing function
  int status;

  int specialwall;

  int windowStyle;

  int fontStyle;
  bool fixedEncounter;

  int fixedEncounterRef;

  int thirst;
  int hunger;
  int digestion;
  int alcohol;


  int guildMemberships[14]; // slot 1 for full membership, other 13 for associate membership(s)
  int ringCharges;
  int alignment;
  int lfood;
  int lwater;
  int ltorches;
  int ltimepieces;
  int lcompasses;
  int lkeys;
  int lcrystals;
  int lgems;
  int ljewels;
  int lgold;
  int lsilver;
  int lcopper;
  int spellIndex;
  int effectIndex;

  int shopFriendships[15];
  int tavernFriendships[14];
  int retreatFriendship;
  int damonFriendship;
  int smithyFriendships[4];
  bool drawingBigAutomap;
  int encounterRef;
  int encounterAnimationRef;
  int bankAccountStatuses[9];
  int bankAccountBalances[9];

  int clothing[4];
  bool musicStyle;
  int maxBufferSize;

  doorDetail doorDetails[20];
  int doorDetailIndex;
  bool movingForward;

  bool oracleReturnTomorrow;
  int oracleDay;
  int oracleMonth;
  int oracleYear;
  int oracleQuestNo;

  // new to be added to save game!
  int healerDays[2];
  int healerHours[2];
  int healerMinutes[2];

  int treasureFinding;
  int invisibility;
  int diseases[4];
  int poison[4]; // Four strengths of poison
  int delusion;
  int invulnerability[9];
  int noticeability;
  int protection1;
  int protection2;



  // end new items for save game!

  bool goblinsVisited;
  bool goblinsChallenged;
  bool goblinsDefeated;
  bool goblinsCombat;
  bool goblinsReforged;

  bool trollsVisited;
  bool trollsChallenged;
  bool trollsDefeated;
  bool trollsCombat;
  bool trollsReforged;

  float z_offset;

  bool guildAwards[12];
  bool fixedEncounters[32];
  bool fixedTreasures[64];


  // New0.70 variables

  bool undeadKingVisited;
  int rathskellerFriendship;

  // New 0.81 variables - Dwarven Smithy

  int forgeDays;
  int forgeType;    // Sword, Axe, Mace or Hammer (1,2,3,4)
  int forgeBonus;
  string forgeName;

  int stolenFromVault;
};


extern Player plyr;

extern effectItem effectBuffer[50]; // active time limited effects from spells, scrolls, eyes

extern bool autoMapExplored[5][4096]; // 5 levels of 4096 on/off values

extern int shopDailyWares[15][12]; //15 shops with 12 items each a day for sale
extern int smithyDailyWares[4][10]; // 4 smithies with 10 items each a day for sale
extern int tavernDailyFoods[14][6]; // 14 taverns with 6 food items each day for sale
extern int tavernDailyDrinks[14][6]; // 14 taverns with 6 drink items each day for sale

 #endif
