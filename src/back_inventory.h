// Inventory.h
//
// Handles the binary object block that holds all information for active, in game:
//
// Items, Corpses, Curses, Diseases, Spells "carried" by player
//
// Carried items and those on the City or Dungeon floor
//

#include <string>



const int inventorySize = 4096;
extern unsigned char itemBlock[inventorySize];

struct bufferItem
{
    int offset; // item number in inventory[] OR binary offset???
    std::string name;    // Added for convenience of building item lists?

    // 0 - limbo, 1 - floor, 10 - carried, 11 - primary, 12 - secondary,
    // 13 - worn, 14 - head, 15 - arms, 16 - legs, 17 - body
    int location;
    int x;
    int y;
    int level;
};


//extern bufferItem newItemBuffer[1024];

void clearInventory();


void addNewItem(int itemRef);        // E.g addNewItem(EBON_BLADE)
void deleteItem(int itemNo);
//std::string getItemDesc(int itemOffset);






