
#ifndef _encounterh 
#define _encounterh 

#include <string>


enum damageImpacts
{
	INVULNERABLE = 0xF0,
	VULNERABLE = 0x0F
};

enum SurpriseLevel {
	Surprised,
	Noticed,
	Neutral,
	Observant,
	Dominant
};

string getAttackDesc(int chosenWeapon, int damage);
string getPlayerAttackDesc(int damage);

struct buffer_item;
void encounterLoop(int opponentNo, int opponentQuantity);
void initialiseOpponents(int opponentType, int opponentQuantity);
void processPlayerAction();
void processOpponentAction();
//void displayPlayerOptions();

void clearConsoleMessages();
void consoleMessage(string messageText);
void updateConsoleMessages();


void checkEncounter();
void checkForActiveOpponents();

void determineOpponentOpeningMessage();

void playerWaylay();
void playerSnatch();

void checkSurprise();
void checkHostility();
void selectEncounterTheme();
void checkFixedEncounters();
void opponentLeaves();
void updateOpponents();
void chooseEncounter();
void opponentCallsForHelp();
void opponentFlees();

void RunAway(int backup);
//void SetTileImage(int tile_no);
//void loadCityMonsters();
//void loadWeapons();

//void convertDungeonMonsters();
void checkTreasure();
void thiefYield();

extern int inputNumber();

void healerHealWounds();
void healerCureDiseases();
void healerCurePoisons();
void drawEncounterView();
void updateEncounterStatusText();
void initMonster(int monsterNo);
void playerAttack(int attackType,float attackFactorBonus);
//void displayMonsterDiag();
void opponentAttack();
void opponentDeath(int opponent);
void playerCharm();
void playerTrick();
void playerTransact();
void playerHail();
void playerOffer();
void playerSwitchWeapons();
void playerCast(int spellNo);
int opponentChooseWeapon();
int calcPlayerWeaponDamage(int weaponNo, float attackFactor, int attacker);
int calcPlayerSpellDamage(int spellNo, int attacker);
int NewcalcOpponentWeaponDamage(int weaponNo,float attackFactor, int attacker);

void checkAlignmentEncounter(int opponentNo);
void updateAmmoCount(int plyrWeaponIndex, int remainingAmmo);
void surrenderToGuard();

void pauseEncounter();


void MoveItemToMonsterWeapon(int WeaponIndex, int ItemIndex);
struct encRecord
{
	int encProb;
	int encType;
};

struct MapEncounter ;

std::vector<int> randomEncounterPick(const MapEncounter* newMapEncounter, int arraySize, int ZoneToMatch, int TimeToMatch);
int randomEncounterPicker(int ZoneToMatch, int TimeToMatch, int arraySize);
double round(double d);

SurpriseLevel getSurpriseLevelEnum(int Surprisecheck);
 
bool calcSnatchSuccess();

int snatchedItem(int minlevel);

std::string selectRandomResource(Player& plyr);
// Your existing header content goes here 
 
int whichOpponentSpell(int curOpponent);
#endif // _encounterh 

