/* DWARVEN SMITHY.CPP
 *
 * TODO:
 *
 *  Offer to reforge Goblin / Troll ring
 *
 *
 */


#include <string>
#include <iostream>
#include <sstream>

#include <stdio.h>

#include "module.h"
#include "player.h"
#include "font.h"
#include "display.h"
#include "game.h"
#include "misc.h"
#include "dwarvenSmithy.h"
#include "automap.h"
#include "items.h"

unsigned char dwarvenBinary[dwarvenFileSize];
int dwarvenItemOffset;

int customWeaponType;
string customWeaponDesc = "";

enum MENUS
{
    MENU_LEFT,
    MENU_MAIN,
    MENU_CHOOSE_SMITHY_ITEM,
    MENU_PRE_OFFER,
    MENU_SELECT_OFFER,
    MENU_SMITHY_MAKES_OFFER,
    MENU_OFFER_REFUSED,
    MENU_NO_FUNDS,
    MENU_ANYTHING_ELSE,
    MENU_ANYTHING_ELSE2,
    MENU_CUSTOM,
    MENU_CUSTOM_ORDERED,
    MENU_BUSY_FORGING,
    MENU_CUSTOM_READY,
    MENU_NO_HAGGLE,
    MENU_NO_NAME_PROVIDED
};

string itemNames [11] =
{
    "Truesilver Morion",
    "Truesilver Coat",
    "Truesilver Gauntlets",
    "Truesilver Leggings",
    "Truesilver Sword",
    "Truesilver Hammer",
    "Thunder Hammer",
    "Truesilver Mace",
    "Truesilver Axe",
    "Crossbow [10]",
    "Thunder Quarrels [10]",
};

int itemPrices [11] = {40,70,50,60,60,60,30,60,60,20,5};
int dwarvenItemOffsets [11] = {0x79,0x00,0x29,0x50,0xA0,0xCB,0x120,0x149,0xF7,0x19D,0x173};

string itemDesc = "item";
int itemValue = 0;
int itemRef = 0;
int smithyChoice = 0;

int dmenu;



void runDwarvenSmithy()
{
    if (plyr.forgeDays > 0) dmenu = MENU_BUSY_FORGING;
    else dmenu = MENU_MAIN;
    if ((plyr.forgeDays == 0) && (plyr.forgeType > 0)) { dmenu = MENU_CUSTOM_READY; }

    addDwarvenSmithyToMap();
    loadShopImage(26);

    buildSmithyMenuOptions();

    //setGreetingText();

    while ( dmenu != MENU_LEFT )
    {
        clearShopDisplay();
        displayDwarvenModuleText();
        updateDisplay();
        processDwarvenMenuInput();
    }
}



void displayDwarvenModuleText()
{
    if ( dmenu == MENU_MAIN )
    {
        string dgreetingText = "Welcome to my forge, " + plyr.name + "!";
        cyText (1, dgreetingText);
        bText (6,3, "(1) Examine my wares");
        bText (6,4, "(2) Sell weapons or armor");
        bText (6,5, "(3) Have a custom weapon made");
        bText (6,6, "(0) Leave");
    }
    else if (dmenu == MENU_PRE_OFFER)
    {
        cyText (3,"What do you offer to sell?");
    }
    else if (dmenu == MENU_SELECT_OFFER)
    {
        itemRef = selectItem(3);
        if (itemRef == 9999) dmenu = MENU_MAIN;     // No selection made
        if ((itemRef > 999) && (itemRef < 1012)) dmenu = MENU_OFFER_REFUSED;
        if (itemRef < 101)
        {
            //bool offerAccepted = false;
            int itemType = itemBuffer[itemRef].type;
            itemDesc = itemBuffer[itemRef].name;
            if ((itemType == 177) || (itemType == 178)) { calculateSaleItemValue(itemRef); dmenu = MENU_SMITHY_MAKES_OFFER; }
            else dmenu = MENU_OFFER_REFUSED;
        }
    }
    else if (dmenu == MENU_OFFER_REFUSED)
    {
        string str = "@@Sorry, but I'm not interested in your@@" + itemDesc + ".";
        cyText (1,str);
    }
    else if (dmenu == MENU_SMITHY_MAKES_OFFER)
    {
        string str = "@@I will give you " + itos(itemValue) + " silvers for@@your " +itemDesc + ".@@Okay? (Y or N)";
        cyText (1,str);
    }
    else if (dmenu == MENU_NO_FUNDS)
    {
        string str = "@@That's more than you have.";
        cyText (1,str);
    }
    else if (dmenu == MENU_NO_HAGGLE)
    {
        string str = "Who do you think I am?  Omar?!@@You'll do no haggling with me!";
        cyText (1,str);
    }
    else if (dmenu == MENU_ANYTHING_ELSE)
    {
        string itemText = itemNames[smithyChoice];
        string str = "@I'm sure that the " + itemText + "@will be to your liking@@@Will there be anything else?@@(Y or N)";
        cyText (1,str);
    }
    else if (dmenu == MENU_ANYTHING_ELSE2)
    {
        string itemText = itemNames[smithyChoice];
        string str = "@Here's the " + itemText + "@@@@@Will there be anything else?@@(Y or N)";
        cyText (1,str);
    }
    else if ( dmenu == MENU_CUSTOM )
    {
        cyText (1, "What type of weapon are you@interested in?");
        bText (13,4, "(1) Sword");
        bText (13,5, "(2) Axe");
        bText (13,6, "(3) Mace");
        bText (13,7, "(4) Hammer");
        bText (13,8, "(0) Not interested");
    }
    else if ( dmenu == MENU_CUSTOM_ORDERED )
    {
        string str = "Return in four days for your " + customWeaponDesc + ".@@It will be forged by then.";
        cyText (1, str);
    }
    else if ( dmenu == MENU_BUSY_FORGING )
    {
        string dayText = itos(plyr.forgeDays) + " days";
        if (plyr.forgeDays == 1) dayText = "1 day";

        string str = "Sorry, but I'll be busy for " + dayText + " yet,@@forging and inscribing your weapon.@@I shall see you then.";
        cyText (1, str);
    }
    else if ( dmenu == MENU_CUSTOM_READY )
    {
        string str = "Welcome "+ plyr.name +"!@@ I have your custom weapon right here!@@It is indeed a mighty weapon!";
        cyText (1, str);
    }
    else if ( dmenu == MENU_NO_NAME_PROVIDED )
    {
        string str = "Very well then, I will simply call@@it the "+ plyr.forgeName +".";
        cyText (1, str);
    }
}



void  processDwarvenMenuInput()
{
    string key = readKey();

    switch (dmenu)
    {
        case MENU_MAIN:
            if (key=="0")       dmenu = MENU_LEFT;
            if (key=="1")       dmenu = MENU_CHOOSE_SMITHY_ITEM;
            if (key=="2")       dmenu = MENU_PRE_OFFER;
            if (key=="3")       dmenu = MENU_CUSTOM;
            if (key=="down")    dmenu = MENU_LEFT;
            break;
        case MENU_CHOOSE_SMITHY_ITEM:
            chooseDwarvenSmithyItem();
            break;
        case MENU_PRE_OFFER:
            if (key != "")      dmenu = MENU_SELECT_OFFER;
            break;
        case MENU_OFFER_REFUSED:
            if (key != "")      dmenu = MENU_MAIN;
            break;
        case MENU_NO_FUNDS:
            if (key != "")      dmenu = MENU_MAIN;
            break;
        case MENU_SMITHY_MAKES_OFFER:
            if (key=="N")       dmenu = MENU_MAIN;
            if (key=="Y")       { processPayment(); dmenu = MENU_MAIN; }
            if (key=="0")       dmenu = MENU_MAIN;
            break;
        case MENU_ANYTHING_ELSE:
            if (key=="N")       dmenu = MENU_MAIN;
            if (key=="Y")       dmenu = MENU_CHOOSE_SMITHY_ITEM;
            break;
        case MENU_ANYTHING_ELSE2:
            if (key=="N")       dmenu = MENU_MAIN;
            if (key=="Y")       dmenu = MENU_CHOOSE_SMITHY_ITEM;
            break;
        case MENU_CUSTOM:
            if (key=="0")       dmenu = MENU_MAIN;
            if (key=="1")       { customWeaponType = 1; makeCustomWeaponOffer(); }
            if (key=="2")       { customWeaponType = 2; makeCustomWeaponOffer(); }
            if (key=="3")       { customWeaponType = 3; makeCustomWeaponOffer(); }
            if (key=="4")       { customWeaponType = 4; makeCustomWeaponOffer(); }
            break;
        case MENU_CUSTOM_ORDERED:
            if (key != "")       dmenu = MENU_LEFT;
            break;
        case MENU_BUSY_FORGING:
            if (key == "SPACE")       dmenu = MENU_LEFT;
            break;
        case MENU_CUSTOM_READY:
            if (key == "SPACE")      { createCustomWeapon(); dmenu = MENU_MAIN; }
            break;
        case MENU_NO_HAGGLE:
            if (key == "SPACE")      dmenu = MENU_MAIN;
            break;
        case MENU_NO_NAME_PROVIDED:
            if (key == "SPACE")      dmenu = MENU_CUSTOM_ORDERED;
            break;
    }
}



void makeCustomWeaponOffer()
{
    int custonWeaponMinimum = 0;
    if (customWeaponType==1) { customWeaponDesc = "sword";  custonWeaponMinimum = 180; }
    if (customWeaponType==2) { customWeaponDesc = "axe";    custonWeaponMinimum = 150; }
    if (customWeaponType==3) { customWeaponDesc = "mace";   custonWeaponMinimum = 110; }
    if (customWeaponType==4) { customWeaponDesc = "hammer"; custonWeaponMinimum = 90; }

    string str = "I ask at least " + itos(custonWeaponMinimum) + " gems or jewels for a@high-quality custom made " + customWeaponDesc + ".@How much are you prepared to offer?";
    int gemsJewelsOffer = inputNumber(str);

    int playerGems = calculateGemsAndJewelsTotal();
    if (gemsJewelsOffer < custonWeaponMinimum)
    {
        if (gemsJewelsOffer==0) { dmenu = MENU_MAIN; }
        else { dmenu = MENU_NO_HAGGLE; }
    }
    else
    {
        if (playerGems >= gemsJewelsOffer)
        {
            deductGems(gemsJewelsOffer);

            plyr.forgeBonus = 0;
            int additionalGemsOffered = gemsJewelsOffer - custonWeaponMinimum;
            calculateForgeBonus(additionalGemsOffered);
            string str = "@By what name do you wish your mighty@@" + customWeaponDesc + " to be called?";
            plyr.forgeName = inputText(str);
            if (plyr.forgeName == "")
            {
                if (customWeaponType == 1) plyr.forgeName = "Dwarven Sword";
                if (customWeaponType == 2) plyr.forgeName = "Dwarven Axe";
                if (customWeaponType == 3) plyr.forgeName = "Dwarven Mace";
                if (customWeaponType == 4) plyr.forgeName = "Dwarven Hammer";
                dmenu = MENU_NO_NAME_PROVIDED;
            }
            else
            {
                dmenu = MENU_CUSTOM_ORDERED;
            }
            plyr.forgeDays = 4;
            plyr.forgeType = customWeaponType;


        }
        else
        {
            dmenu = MENU_NO_FUNDS;
        }
    }

}

void createCustomWeapon()
{
    // Routine to create weapon after 4 days have elapsed

    int index,alignment,weight,wAttributes,melee,ammo,blunt,sharp,earth,air,fire,water,power,magic,good,evil,cold,nature,acid,
        minStrength,minDexterity,hp,maxHP,flags,parry,useStrength, effect;

    int offset = 0x00;
    switch (plyr.forgeType)
    {
        case 1:
            offset = 0xA0;  // Truesilver sword
            break;
        case 2:
            offset = 0xF7;  // Truesilver axe
            break;
        case 3:
            offset = 0x149;  // Truesilver mace
            break;
        case 4:
            offset = 0xCB;  // Truesilver hammer
            break;
        default:
            offset = 0xA0;
            break;
    }

    int itemType = dwarvenBinary[offset];
    string itemName = plyr.forgeName;

    itemType           = 178;       // ARX value for weapon
    index              = 0;         // No longer required
    useStrength        = 0;
    alignment          = dwarvenBinary[offset+3];
    weight             = dwarvenBinary[offset+4];

    wAttributes    = (offset + dwarvenBinary[offset+1])-20; // Working out from the end of the weapon object

    melee              = dwarvenBinary[wAttributes+1];  // or ammo type for Crossbow
    ammo               = dwarvenBinary[wAttributes+2];  // 0 for melee weapons / shields
    blunt              = (dwarvenBinary[wAttributes+3]) + 0x17 + plyr.forgeBonus;
    sharp              = dwarvenBinary[wAttributes+4] + 0x17 + plyr.forgeBonus;
    earth              = dwarvenBinary[wAttributes+5];
    air                = dwarvenBinary[wAttributes+6];
    fire               = dwarvenBinary[wAttributes+7];
    water              = dwarvenBinary[wAttributes+8];
    power              = dwarvenBinary[wAttributes+9];
    magic              = dwarvenBinary[wAttributes+10];
    good               = dwarvenBinary[wAttributes+11];
    evil               = dwarvenBinary[wAttributes+12];
    cold               = dwarvenBinary[wAttributes+13];
    nature             = dwarvenBinary[wAttributes+14];
    acid               = dwarvenBinary[wAttributes+15];
    minStrength        = dwarvenBinary[wAttributes+16];
    minDexterity       = dwarvenBinary[wAttributes+17];
    hp                 = 0xFF;
    maxHP              = 0xFF;
    flags              = 90;
    parry              = (dwarvenBinary[wAttributes+21]) * 2;
    effect             = 0;
    int newItemRef = createItem(itemType,index,itemName,hp,maxHP,flags,minStrength,minDexterity,useStrength,blunt,
                                sharp,earth,air,fire,water,power,magic,good,evil,cold,nature,acid,weight,alignment,melee,ammo,parry,effect);
    itemBuffer[newItemRef].location = 10; // Add to player inventory - 10

    // Reset related variables once custom weapon ready
    plyr.forgeBonus = 0;
    plyr.forgeDays = 0;
    plyr.forgeType = 0;
    plyr.forgeName = "";

}


int calculateGemsAndJewelsTotal()
{
    int total = 0;
    total = plyr.gems + plyr.jewels;
    return total;
}

void deductGems(int totalGems)
{
    // Deducts from gems before jewels
    if ( plyr.gems >= totalGems ) plyr.gems -= totalGems;
    else
    {
        totalGems -= plyr.gems;
        plyr.gems = 0;
        plyr.jewels -= totalGems;
    }
}



void calculateSaleItemValue(int itemRef)
{
    int damageValues[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
    int results[11] = { 0,0,0,0,0,0,0,0,0,0,0 };

    damageValues[0] = itemBuffer[itemRef].blunt;
    damageValues[1] = itemBuffer[itemRef].sharp;
    damageValues[2] = itemBuffer[itemRef].earth;
    damageValues[3] = itemBuffer[itemRef].air;
    damageValues[4] = itemBuffer[itemRef].fire;
    damageValues[5] = itemBuffer[itemRef].water;
    damageValues[6] = itemBuffer[itemRef].power;
    damageValues[7] = itemBuffer[itemRef].magic;
    damageValues[8] = itemBuffer[itemRef].good;
    damageValues[9] = itemBuffer[itemRef].evil;
    damageValues[10] = itemBuffer[itemRef].cold;

    int damageIndex = 0;
    while (damageIndex < 11)
    {
        int noDice = (damageValues[damageIndex] & 0xf0) >> 4;
        int noSides = (damageValues[damageIndex] & 0x0f);
        if (noDice > 0 ) results[damageIndex] = noDice * noSides;
        damageIndex++;
    }

    int itemValueA = (((itemBuffer[itemRef].hp)-1)*2);
    int itemValueB = results[0]+results[1]+results[2]+results[3]+results[4]+results[5]+results[6]+results[7]+results[8]+results[9]+results[10];
    itemValue = itemValueA + itemValueB;
}


void processPayment()
{
    // Check equipped items
    if (plyr.priWeapon == itemRef) plyr.priWeapon = 0;
    if (plyr.secWeapon == itemRef) plyr.secWeapon = 0;
    if (plyr.armsArmour == itemRef) plyr.armsArmour = 255;
    if (plyr.legsArmour == itemRef) plyr.legsArmour = 255;
    if (plyr.bodyArmour == itemRef) plyr.bodyArmour = 255;
    if (plyr.headArmour == itemRef) plyr.headArmour = 255;

    // Remove item from inventory
    moveItem(itemRef,0);

    // Add silvers
    plyr.silver+=itemValue;
}


// Attempt to buy a chosen Dwarven Smithy (non custom) item

void chooseDwarvenSmithyItem()
{
    smithyChoice = inputItemChoice("What would thou like? (0 to go back)",11);


    if (smithyChoice < 255 )
    {
        int gemsCost = itemPrices[smithyChoice];
        int playerGems = calculateGemsAndJewelsTotal();
        //cout << "cost:" << gemsCost << "," << playerGems << "\n";

        if (playerGems >= gemsCost)
        {
            deductGems(gemsCost);
            dwarvenItemOffset = dwarvenItemOffsets[smithyChoice];
            createDwarvenInventoryItem(dwarvenItemOffset);

            int test = randn(0,4);
            if (test < 3) dmenu = MENU_ANYTHING_ELSE;
            else dmenu = MENU_ANYTHING_ELSE2;
        }
        else
        {
            dmenu = MENU_NO_FUNDS;
        }

    }
    else { dmenu = MENU_MAIN; }     // Option 0 was selected
}


void buildSmithyMenuOptions()
{
	// Copies Dwarven Smithy items into structure for use in module.cpp
    // The Dwarven Smithy has a fixed, unchanging menu of items for purchase.

	//itemNames, itemPrices

    string str;
	int itemNo = 0;

    for (int waresNo=0 ; waresNo<11 ; waresNo++)
    {
        menuItems[waresNo].menuName = itemNames[waresNo];
        str = itos(itemPrices[waresNo]) + "  ";
        str.replace(3,7,"gems/jewels");
        menuItems[waresNo].menuPrice  = str;
        menuItems[waresNo].objRef = waresNo;
    }
}



#include <cstdio> // Include the necessary header for fopen_s

#include <cstdio> // Include the necessary header for fopen_s

#include <cstdio> // Include the necessary header for fopen

void loadDwarvenBinary() {
    FILE* fp;               // file pointer - used when reading files
    char tempString[100];   // temporary string
    sprintf_s(tempString, sizeof(tempString), "%s%s", "data/map/", "DwarvenItems.bin");

    // Use fopen for opening the file
    if (fopen_s(&fp, tempString, "rb") == 0 && fp != NULL) {
        // File opened successfully
        // Check if the file size matches the expected size
        fseek(fp, 0, SEEK_END); // Move to the end of the file
        long fileSize = ftell(fp); // Get the file size
        fseek(fp, 0, SEEK_SET); // Move back to the beginning

        if (fileSize == dwarvenFileSize) {
            // File size matches the expected size
            for (int i = 0; i < dwarvenFileSize; i++) {
                dwarvenBinary[i] = fgetc(fp);
            }
            fclose(fp); // Close the file when done
        }
        else {
            // Handle incorrect file size (e.g., print an error message)
            printf("Error: File size does not match the expected size.\n");
        }
    }
    else {
        // Handle file open error (e.g., print an error message)
        printf("Error: Failed to open the file.\n");
    }
}





string readDwarvenNameString(int stringOffset)
{
    stringstream ss;
    int z = stringOffset;   // current location in the binary
    int c = 0;              // current byte
    string result = "";

   while (!(dwarvenBinary[z]==0))
    {
        c = dwarvenBinary[z];
        ss << (char) c;
        z++;
    }
    result = ss.str();
    return result;
}


// Take a binary offset within dwarvenBinary and create a new inventory item from the binary data (weapon, armour or clothing)
// Item types:  03 - weapon, 04 - armour, 02 - ammo

int createDwarvenInventoryItem(int startByte)
{
    int index,alignment,weight,wAttributes,melee,ammo,blunt,sharp,earth,air,fire,water,power,magic,good,evil,cold,nature,acid,
        minStrength,minDexterity,hp,maxHP,flags,parry,useStrength, effect;

    int offset = startByte;
    int itemType = dwarvenBinary[offset];
    string itemName = readDwarvenNameString((offset+6));

    if (itemType == 3)
    {
        itemType           = 178;       // ARX value for weapon
        index              = 0;         // No longer required
        useStrength        = 0;
        alignment          = dwarvenBinary[offset+3];
        weight             = dwarvenBinary[offset+4];

        wAttributes    = (offset + dwarvenBinary[offset+1])-20; // Working out from the end of the weapon object

        melee              = dwarvenBinary[wAttributes+1];  // or ammo type for Crossbow
        ammo               = dwarvenBinary[wAttributes+2];  // 0 for melee weapons / shields
        blunt              = dwarvenBinary[wAttributes+3];
        sharp              = dwarvenBinary[wAttributes+4];
        earth              = dwarvenBinary[wAttributes+5];
        air                = dwarvenBinary[wAttributes+6];
        fire               = dwarvenBinary[wAttributes+7];
        water              = dwarvenBinary[wAttributes+8];
        power              = dwarvenBinary[wAttributes+9];
        magic              = dwarvenBinary[wAttributes+10];
        good               = dwarvenBinary[wAttributes+11];
        evil               = dwarvenBinary[wAttributes+12];
        cold               = dwarvenBinary[wAttributes+13];
        nature             = dwarvenBinary[wAttributes+14];
        acid               = dwarvenBinary[wAttributes+15];
        minStrength        = dwarvenBinary[wAttributes+16];
        minDexterity       = dwarvenBinary[wAttributes+17];
        hp                 = dwarvenBinary[wAttributes+18];
        maxHP              = dwarvenBinary[wAttributes+19];
        flags              = dwarvenBinary[wAttributes+20];
        parry              = dwarvenBinary[wAttributes+21];
        effect             = 0;
    }

    if (itemType == 4)
    {
        itemType           = 177;       // ARX value for armour
        index              = 0;         // No longer required
        useStrength        = 0;
        alignment          = dwarvenBinary[offset+3];
        weight             = dwarvenBinary[offset+4];

        wAttributes    = (offset + dwarvenBinary[offset+1])-15; // Working out from the end of the weapon object

        melee              = dwarvenBinary[wAttributes+1];    // Body part
        ammo               = 0;                               // Not used
        blunt              = dwarvenBinary[wAttributes+2];
        sharp              = dwarvenBinary[wAttributes+3];
        earth              = dwarvenBinary[wAttributes+4];
        air                = dwarvenBinary[wAttributes+5];
        fire               = dwarvenBinary[wAttributes+6];
        water              = dwarvenBinary[wAttributes+7];
        power              = dwarvenBinary[wAttributes+8];
        magic              = dwarvenBinary[wAttributes+9];
        good               = dwarvenBinary[wAttributes+10];
        evil               = dwarvenBinary[wAttributes+11];
        cold               = dwarvenBinary[wAttributes+12];
        nature = dwarvenBinary[wAttributes + 13];
        acid = dwarvenBinary[wAttributes + 14];
        minStrength        = 0;
        minDexterity       = 0;
        hp                 = dwarvenBinary[wAttributes+15];
        maxHP              = dwarvenBinary[wAttributes+16];
        flags              = 0;
        parry              = 0;
        effect             = 0;
    }

    if (itemType == 2)
    {
        itemType           = 199;       // ARX value for ammo????
        index              = 0;         // No longer required
        useStrength        = 0;
        alignment          = dwarvenBinary[offset+3];
        weight             = dwarvenBinary[offset+4];

        wAttributes    = (offset + dwarvenBinary[offset+1])-3; // Working out from the end of the weapon object

        melee              = dwarvenBinary[wAttributes+1];
        ammo               = dwarvenBinary[wAttributes+2];
        blunt              = dwarvenBinary[wAttributes+3];
        sharp              = 0;                             // Set to 0 for non use
        earth              = 0;
        air                = 0;
        fire               = 0;
        water              = 0;
        power              = 0;
        magic              = 0;
        good               = 0;
        evil               = 0;
        cold               = 0;
        nature             = 0;
        acid               = 0;

        minStrength        = 0;
        minDexterity       = 0;
        hp                 = 0;
        maxHP              = 0;
        flags              = 0;
        parry              = 0;
        effect             = 0;
    }

    int newItemRef = createItem(itemType,index,itemName,hp,maxHP,flags,minStrength,minDexterity,useStrength,blunt,
                                sharp,earth,air,fire,water,power,magic,good,evil,cold,nature,acid,weight,alignment,melee,ammo,parry, effect);
    itemBuffer[newItemRef].location = 10; // Add to player inventory - 10
    return 1;
}


void addDwarvenSmithyToMap()
{
    setAutoMapFlag(plyr.map, 16, 6);
	setAutoMapFlag(plyr.map, 17, 6);
	setAutoMapFlag(plyr.map, 18, 6);
	setAutoMapFlag(plyr.map, 16, 7);
	setAutoMapFlag(plyr.map, 17, 7);
	setAutoMapFlag(plyr.map, 18, 7);
	setAutoMapFlag(plyr.map, 16, 8);
	setAutoMapFlag(plyr.map, 17, 8);
	setAutoMapFlag(plyr.map, 18, 8);
}

void calculateForgeBonus(int additionalGemsOffered)
{
    int bonus = (int) additionalGemsOffered / 60;
    if ( bonus < 1) bonus = 0;
    plyr.forgeBonus = bonus;
}
