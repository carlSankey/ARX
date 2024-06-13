#include <string>

#include "constants.h"
#include "globals.h"
#include "module.h"
#include "rathskeller.h"
#include "dwarvenSmithy.h"
#include "vaults.h"
#include "display.h"
#include "player.h"
#include "font.h"
#include "misc.h"

menuItem menuItems[20]; // Should be usable for building any type of general shop menus with a maximum of 20 multi page items

void updateModule(int module)
{
    if (module==RATHSKELLER)        runRathskeller();
    if (module==DWARVEN_SMITHY)     runDwarvenSmithy();
    if (module==VAULT)              runVault();
}

void runModule(int module)
{
    plyr.status = MODULE;
    updateModule (module);
    if (plyr.facing==WEST) { plyr.x = plyr.oldx; }
	if (plyr.facing==EAST) { plyr.x = plyr.oldx; }
	if (plyr.facing==NORTH) { plyr.y = plyr.oldy; }
	if (plyr.facing==SOUTH) { plyr.y = plyr.oldy; }
	plyr.z_offset=1.6; // position player just outside door
	plyr.status = EXPLORE;
}

void displayModuleImage(int module)
{
    App.clear();
	App.pushGLStates();
	//App.draw(ShopSprite);
	drawStatsPanel();
}

// Returns an item reference based on a multi page menu e.g. food item, weapon item
int inputItemChoice(std::string message, int totalItems)
{
    // totalItems = maximum number of items possible across multiple pages
    bool noMenuSelection = true;
    string key, str;
    int itemRef = 255;      // Selected nothing / option 0 to go back
    int currentItem = 0;
    int menuPage = 0;
    int maximumMenuPage = calculateMaximumMenuPage(totalItems);
    int minimumMenuPage = 0;
    int currentItemRefs[MAX_MENU_ITEMS];

    // calculate number of menu pages

    while (noMenuSelection)
    {
        clearShopDisplay();
        cyText (0, message);

        for (int i=0 ; i<MAX_MENU_ITEMS ; i++)
        {
            currentItem = (menuPage*6) + i;
            if (currentItem >= totalItems)
            {
                // Menu slot without an item
                currentItemRefs[i] = 256; // No item to select
                str = "(" + itos(i+1) + ")";
                bText(1,(2+i), str);
            }
            else
            {
                // Menu slot showing an item
                currentItemRefs[i] = menuItems[currentItem].objRef;
                str = "(" + itos(i+1) + ") " + menuItems[currentItem].menuName;
                bText(1,(2+i), str);
                if (MODULE==DWARVEN_SMITHY) bText(27,(2+i), menuItems[currentItem].menuPrice);
                else bText(27,(2+i), menuItems[currentItem].menuPrice); // was 31
            }
        }
        int bottomOfDisplay = 1 + MAX_MENU_ITEMS + 2;
        cyText (bottomOfDisplay,"Forward, Backward or ESC to exit");
        updateDisplay();

        key = getSingleKey();
        if ( key=="1" )     { itemRef = currentItemRefs[0]; noMenuSelection = false; }
        if ( key=="2" )     { itemRef = currentItemRefs[1]; noMenuSelection = false; }
        if ( key=="3" )     { itemRef = currentItemRefs[2]; noMenuSelection = false; }
        if ( key=="4" )     { itemRef = currentItemRefs[3]; noMenuSelection = false; }
        if ( key=="5" )     { itemRef = currentItemRefs[4]; noMenuSelection = false; }
        if ( key=="6" )     { itemRef = currentItemRefs[5]; noMenuSelection = false; }
        if ( key=="ESC" )   { noMenuSelection = false; }
        if ( key=="0" )     { noMenuSelection = false; }
        if ( key=="F" )     { if (menuPage < maximumMenuPage) menuPage++; }
        if ( key=="B" )     { if (menuPage > minimumMenuPage) menuPage--; }
        if ( key=="down" )  { if (menuPage < maximumMenuPage) menuPage++; }
        if ( key=="up" )    { if (menuPage > minimumMenuPage) menuPage--; }

        if (itemRef == 256) { itemRef = 255; noMenuSelection = true; } // Return to menu if empty menu item slot
    }
    return itemRef;
}



int calculateMaximumMenuPage(int numberOfItems)
{
    int maxPageNumber = (numberOfItems / MAX_MENU_ITEMS);
    if (numberOfItems % MAX_MENU_ITEMS > 0 ) maxPageNumber++;
    maxPageNumber--;
    return maxPageNumber;
}



int inputNumber(std::string message)
{
	string str;
	string key;
	string inputText = "";
	int maxNumberSize = 6;
	bool enterKeyNotPressed = true;
	while ( enterKeyNotPressed )
	{
		clearShopDisplay();
		cyText(1,message);
		str = ">" + inputText + "_";
		bText(10,7, str);
		updateDisplay();
		key = getSingleKey();
		if ( (key=="0") || (key=="1") || (key=="2") || (key=="3") || (key=="4") || (key=="5") || (key=="6") || (key=="7") || (key=="8") || (key=="9") )
		{
			int numberLength = inputText.size();
			if (numberLength < maxNumberSize) { inputText = inputText + key; }
		}
		if (key=="BACKSPACE")
		{
			int numberLength = inputText.size();
			if (numberLength!=0)
			{
			  int numberLength = inputText.size();
			  inputText = inputText.substr(0,(numberLength-1));
			}
		}
		if (key=="RETURN") { enterKeyNotPressed = false; }
	}
	int value = atoi(inputText.c_str());
	return value;
}



std::string inputText(std::string message)
{
    string str;
	string key;
	string inputText = "";
	int maxTextSize = 26;
	bool enterKeyNotPressed = true;
	while ( enterKeyNotPressed )
	{
		clearShopDisplay();
		cyText(1,message);
		str = ">" + inputText + "_";
		bText(10,7, str);
		updateDisplay();
		key = getTextChar();
		if ( !((key=="BACKSPACE") || (key=="RETURN")) )
		{
		    if (key == "SPACE") key = " ";
			int textLength = inputText.size();
			if (textLength < maxTextSize) { inputText = inputText + key; }
		}
		if (key=="BACKSPACE")
		{
			int textLength = inputText.size();
			if (textLength!=0)
			{
			  int textLength = inputText.size();
			  inputText = inputText.substr(0,(textLength-1));
			}
		}
		if (key=="RETURN") { enterKeyNotPressed = false; }
	}
	string myText = inputText;
	return myText;
}
