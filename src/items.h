#ifndef _itemsh 
#define _itemsh 

#include <string>
#include <vector>

const int itemBufferSize = 250;

const int dungeonItemsSize = 10496;
extern unsigned char dungeonItems[dungeonItemsSize];
extern int itemOffsets[141];

//const newItem newItemArray[250];

void loadDungeonItems();

void createBareHands();

void readItemData();

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
    int nature,
    int acid,
    int weight,
    int alignment,
    int melee,
    int ammo,
    int parry,
    int effect)
;

int newcreateItem(
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
    int nature,
    int acid,
    int weight,
    int alignment,
    int melee,
    int ammo,
    int parry,
    int effect)
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
    int effect = 0;
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


struct newItem
{
    int index;
    std::string name;
    int itemType;
    int cost;
    int effect;
    int elementType;
    int statType;
    int negativeValue;
    int positiveValue;
    int duration;
    int damage;
    int hp;
    int maxHP;
    int flags;
    int minStrength;
    int minDexterity;
    int useStrength;
    int weight;
    int alignment;
    int melee;
    int ammo;
    int parry;
    int cat;
    int level;
};

extern newItem* newItemArray; // Declaration of the array

std::string getItemDesc(int itemRef);
void moveItem(int itemRef,int newLocation);

void cannotCarryMessage();
void displayLocation();

int createQuestItem(int questItemNo);
int createWeapon(int weapon_no);
int createClothing(int clothing_no, int itemlocation);
int createArmor(int armor_no);
int createPotion(int potion_no);
int createGeneralItem(int ItemNo);

void swapClothing(int object_ref);
int createGenericItem(int type, int value);
bool checkForQuestItem(int itemNo);
int getQuestItemRef(int itemNo);
void getItems();

void checkDurablility(int itemIndex, int itemDamage);

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
void use_item(int object_ref);
void use_singleUse(int object_ref);
void use_wand(int object_ref);
void use_eye(int object_ref);


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

void checkItemBuff();

int returnCarriedWeight();

std::map<std::pair<int, int>, int> countItemTypes(newItem* items, int size);



int randomItemPicker(int itemCatToMatch, int levelToMatch, int arraySize, int typeToMatch);

std::vector<int> randomPick(const newItem* newItemArray, int arraySize, int itemCatToMatch, int levelToMatch, int typeToMatch);
void readItemData();

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
    int nature;
    int acid;
	int weight;
	int alignment;
	int melee;
	int ammo;
	int parry;
    int buffType = 0; //0 is not buffed
    int effect = 0  ;  //same as spell effect
    int statType = 0; //same as spell stattype
};

extern buffer_item itemBuffer[itemBufferSize];

// Your existing header content goes here 
 void removeItemBuff( int itemId);

 void updateItemBuff(std::bitset<13> binaryItemBuff, int positiveValue);
#endif // _itemsh 

