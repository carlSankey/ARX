#include <string>

const int itemBufferSize = 250;

const int dungeonItemsSize = 10496;
extern unsigned char dungeonItems[dungeonItemsSize];
extern int itemOffsets[141];

void loadDungeonItems();
void createBareHands();


int createItem(
        int type,
        int index,
        std::string name,
        int hp,
        int maxHP,
        int flags,
        int minStrength,
        int minDexterity,
        int useStrength,
        int blunt,
        int sharp,
        int earth,
        int air,
        int fire,
        int water,
        int power,
        int magic,
        int good,
        int evil,
        int cold,
        int weight,
        int alignment,
        int melee,
        int ammo,
        int parry)
;

struct clothingItem
{
	// Description is created from values below if name blank
	// Clothing items have no body location
	// 4 items can be worn at same time
	std::string name;
	int quality;
	int colour;
	int fabric;
	int type;
	int weight;
};


struct potionItem
{
       std::string name;
	   std::string color;
       std::string taste;
	   std::string sip;
};

struct questItem
{
       std::string name;
	   int weight;
};

std::string getItemDesc(int itemRef);
void moveItem(int itemRef,int newLocation);

void cannotCarryMessage();
void displayLocation();

int createQuestItem(int questItemNo);
int createWeapon(int weapon_no);
int createClothing(int clothing_no);
int createArmor(int armor_no);
int createPotion(int potion_no);

void swapClothing(int object_ref);
int createGenericItem(int type, int value);
bool checkForQuestItem(int itemNo);
int getQuestItemRef(int itemNo);
void getItems();

int selectItem(int selectMode);

void determineItemAction(int selectItemMode, int itemRef);
int inputItemQuantity(int selectItemMode);
void dropVolumeObject(int selectItemMode, int object_ref);
void checkForItemsHere();
int checkForGenericItemsHere(int type);
void use_food();
void use_water();
void use_torch();
void use_timepiece();
void use_potion(int object_ref);
void quaffPotion(int object_ref);
void use_weapon(int object_ref);
void use_clothing(int object_ref);

void useObject(int object_ref);
void dropObject(int object_ref);

void itemMessage(std::string message);
//void depositObject(int object_ref);

void depositObject(int itemRef);
void withdrawalObject(int itemRef);
int inputDepositQuantity(int itemRef);
int inputWithdrawalQuantity(int itemRef);

void use_armor(int object_ref);
void use_questItem(int object_ref);
void use_ammoItem(int object_ref);

void displayObjectBuffer();

void tidyObjectBuffer();

int returnCarriedWeight();

struct buffer_item
{
    int type;  // 83, 03, etc
    int index; // in appropriate array (e.g. armour array) dependent on type above
    // 0 - limbo, 1 - floor, 10 - carried, 11 - primary, 12 - secondary,
    // 13 - worn, 14 - head, 15 - arms, 16 - legs, 17 - body
    int location;
    int x;
    int y;
    int level;
    int hp; // hp or no. of charges or no. of items for generic items like food packets

    std::string name;
    int maxHP;
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
	int weight;
	int alignment;
	int melee;
	int ammo;
	int parry;
};

extern buffer_item itemBuffer[itemBufferSize];
