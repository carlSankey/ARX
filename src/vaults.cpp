/* VAULTS.CPP
 *
 * TODO:
 *
 *  Choose random item
 *  Check for guards
 *
 */


#include <string>

#include "module.h"
#include "player.h"
#include "font.h"
#include "display.h"
#include "game.h"
#include "misc.h"
#include "vaults.h"
#include "automap.h"


enum MENUS
{
    MENU_LEFT,
    MENU_DRAGGED_OUTSIDE,
    MENU_MAIN,
    MENU_GRAB_A_BAG,
    MENU_SEARCHING
};

int vmenu;
sf::Clock gmyclock;
sf::Time gdt;
sf::Time guardCheckTime;

int bagType = 0;
string bagDesc = "copper coins";
string vaultName = "NO NAME.";


void runVault()
{
    vmenu = MENU_MAIN;
    addVaultToMap();
    loadShopImage(27);
    setBagType();

    while ( vmenu != MENU_LEFT )
    {
        gdt = gmyclock.restart();

        clearShopDisplay();
        displayVaultModuleText();
        updateDisplay();
        processVaultMenuInput();

        guardCheckTime += gdt;
        if (guardCheckTime >= sf::seconds(0.8f)) // was 0.8f
        {
              checkForGuard();
              guardCheckTime = sf::Time::Zero;
              //addMinute();
        }


    }
}


void checkForGuard()
{
    int x = randn(0,100);
    if ( x < 5 )
    {
        if (plyr.stolenFromVault == 1) { plyr.stolenFromVault = 2; } // Marker for a hostile encounter
        vmenu = MENU_DRAGGED_OUTSIDE;
    }
}

void displayVaultModuleText()
{
    if ( vmenu == MENU_MAIN )
    {
        string str = "You are in " + vaultName;
        string str2 = "You see a bag of " + bagDesc + ".";
        cyText (1, str);
        cyText (3, str2);
        bText (1,5, "Do you (1) Grab the bag and run,");
        bText (8,6, "(2) Search for something else or");
        bText (8,7, "(0) Leave?");
    }
    else if ( vmenu == MENU_GRAB_A_BAG)
    {
        string str = "@@You grab the bag of " + bagDesc + "@@and run!";
        cyText (1, str);
    }
    else if ( vmenu == MENU_SEARCHING)
    {
        string str = "@@@Searching...@@@(Hit SPACE key to stop searching)";
        cyText (1, str);
    }
    else if ( vmenu == MENU_DRAGGED_OUTSIDE)
    {
        if (plyr.stolenFromVault == 0) { cyText (1, "@@A guard escorts you@@out of the bank's vault."); }
        else { cyText (1, "@@A guard drags you outside."); }
    }
}



void  processVaultMenuInput()
{
    string key = readKey();

    switch (vmenu)
    {
        case MENU_MAIN:
            if (key=="0")       vmenu = MENU_LEFT;
            if (key=="1")       vmenu = MENU_GRAB_A_BAG;
            if (key=="2")       vmenu = MENU_SEARCHING;
            break;
        case MENU_GRAB_A_BAG:
            if (key=="SPACE")   { grabBag(); vmenu = MENU_LEFT; }
            break;
        case MENU_SEARCHING:
            if (key=="SPACE")   { setBagType(); vmenu = MENU_MAIN; }
            break;
        case MENU_DRAGGED_OUTSIDE:
            if (key=="SPACE")   { vmenu = MENU_LEFT; }
            break;
    }
}


void setBagType()
{
    // Copper, Silver, Gold, Gems or Jewels
    int x = randn(0,4);

    switch (x)
    {
        case 0:
            bagType = 0; bagDesc = "copper coins";
            break;
        case 1:
            bagType = 1; bagDesc = "silver coins";
            break;
        case 2:
            bagType = 2; bagDesc = "gold coins";
            break;
        case 3:
            bagType = 3; bagDesc = "gems";
            break;
        case 4:
            bagType = 4; bagDesc = "jewels";
            break;
        default:
            bagType = 0; bagDesc = "copper coins";
            break;
    }
}

void grabBag()
{
    int amount = randn(0,10) + 40;
    if (bagType == 0) plyr.copper += amount;
    if (bagType == 1) plyr.silver += amount;
    if (bagType == 2) plyr.gold += amount;
    if (bagType == 3) plyr.gems += amount;
    if (bagType == 4) plyr.jewels += amount;

    // Alignment penalty
    plyr.stolenFromVault = 1;   // Determines what happens if discovered by guard
    plyr.alignment = plyr.alignment - 5;
    if (plyr.alignment < 0) plyr.alignment = 0;
}

void addVaultToMap()
{
        if (plyr.x == 2) { setAutoMapFlag(plyr.map, 2, 2); vaultName = "Gram's Gold Exchange vault."; }
        if (plyr.x == 30) { setAutoMapFlag(plyr.map, 30, 57); vaultName = "the First City vault."; }
}
