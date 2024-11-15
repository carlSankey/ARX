
#ifndef _encounterh 
#define _encounterh 

#include <string>


enum damageImpacts
{
	INVULNERABLE = 0xF0,
	VULNERABLE = 0x0F
};


string getAttackDesc(int chosenWeapon, int damage);
string getPlayerAttackDesc(int damage);

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

extern struct MapEncounter ;

std::vector<int> randomEncounterPick(const MapEncounter* newMapEncounter, int arraySize, int ZoneToMatch, int TimeToMatch);
int randomEncounterPicker(int ZoneToMatch, int TimeToMatch, int arraySize);
double round(double d);


 
// Your existing header content goes here 
 
#endif // _encounterh 

