
#ifndef _damonh 
#define _damonh 


extern unsigned char damonBinary[damonFileSize];

void message(string txt);
void shopDamon();
void stockDamon();
void loadDamonBinary();

int createInventoryItem(int startByte);
int newcreateInventoryItem(int itemNo);
string readNameString(int stringOffset);

extern struct newItem;
extern struct newdamonClothingItem;
extern struct newdamonBattleGearItem;

bool doesIndexExistClothing(int indexToCheck, const newdamonClothingItem* array, int size);
int populateUniqueIndicesCL(newdamonClothingItem* newdamonClothingWares, int desiredNumberOfItems, int itemCatToMatch, int levelToMatch, int arraySize, int typeToMatch);


bool doesIndexExistBattleGear(int indexToCheck, const newdamonBattleGearItem* array, int size);
int populateUniqueIndicesBG(newdamonBattleGearItem* newdamonBattleGearWares, int desiredNumberOfItems, int itemCatToMatch, int levelToMatch, int arraySize, int typeToMatch,  bool clear);

int find_last_zero_index(int indexToCheck, const newdamonBattleGearItem* array, int size);

void UpdateShopStock();

//extern newItem newItemArray[250];
 
// Your existing header content goes here 
 
#endif // _damonh 

