#include <optional>
#include <string>
#include <iostream>

#include "platform/ArxClock.h"

#include "game.h"
#include "globals.h"
#include "platform/Window.h"

#include "dev.h"
#include "player.h"
#include "font.h"
#include "display.h"
#include "level.h"
#include "misc.h"
#include "encounter.h"
#include "items.h"
#include "spells.h"
#include "module.h"

#include "vaults.h"
#include "damon.h"
#include "guild.h"
#include "bank.h"
#include "tavern.h"
#include "smithy.h"
#include "inn.h"
#include "shop.h"
#include "healer.h"
#include "goblins.h"
#include "trolls.h"
#include "chapel.h"
#include "oracle.h"
#include "fountain.h"
#include "ferry.h"
#include "undeadKing.h"
#include "staircase.h"
#include "prison.h"
#include "lift.h"
#include "automap.h"
#include "lyrics.h"
#include "arena.h"
#include "actor.h"
#include "saveGame.h"
#include "input.h"

arx::Time encounterCheckTime = arx::Time::Zero; // Accumulated time

extern arx::Clock        clock1;
extern int              iCounter;

arx::Clock myclock;
float Framerate;

extern bool autoMapExplored[5][4096];
extern string mess[25];
extern string room_descriptions[203];
extern Mapcell levelmap[4096]; // 12288

extern Teleport teleports[33];

bool gameQuit = false;

int teleColour = 1;

bool Running;

namespace {
bool g_smithyPlaying = false;

// Non-blocking item action sub-state (D=drop, C=cast)
int g_itemActionMode = 0;     // 0=none, 2=DROP, 3=CAST
int g_itemActionStep = 0;     // 0=selecting, 1=entering quantity
int g_itemActionRef = 9999;   // selected item ref
int g_itemActionPage = 0;     // current page in item list
int g_dropQuantity = 0;       // quantity to drop
int g_getItemIndex = 0;       // current floor item index for GET mode
}

void gameInit()
{
Running = true;
gameQuit = false;
g_smithyPlaying = false;
	encounterCheckTime = arx::Time::Zero;
}

bool gameIsRunning()
{
return Running;
}

void gameEndSession()
{
extern bool encounterRunning;
encounterRunning = false;
g_smithyPlaying = false;
    // Audio cleanup handled by SDL2
Running = false;
}

#ifdef ARX_USE_SDL2
bool gameTick()
{
    // SDL2 web path: minimal game tick
    clearDisplay();
    dispMain();
    drawConsoleBackground();
    drawInfoPanels();
    updateDisplay();
    
    // Handle non-blocking item action sub-state (D=drop, C=cast, G=get)
    if (plyr.subState == SubState::ItemAction && g_itemActionMode > 0) {
        string actionKey = readKey();
        int sel = -1;
        if (actionKey == "1" || actionKey == "2" || actionKey == "3" || actionKey == "4") {
            sel = actionKey[0] - '0';
        }
        
        if (actionKey == "ESC") {
            std::cout << "GAME_INPUT: ESC in item action, canceling" << std::endl;
            plyr.subState = SubState::None;
            g_itemActionMode = 0;
            g_itemActionStep = 0;
            g_itemActionRef = 9999;
            g_itemActionPage = 0;
        }
        else if (actionKey == "F" || actionKey == "down") {
            if (g_itemActionPage < 3) g_itemActionPage++;
        }
        else if (actionKey == "B" || actionKey == "up") {
            if (g_itemActionPage > 0) g_itemActionPage--;
        }
        else if (g_itemActionMode == 1 && g_itemActionStep == 0 && (actionKey == "Y" || actionKey == "N")) {
            // GET mode: handle Y/N for one-item-at-a-time pickup
            if (actionKey == "Y") {
                // Pick up the item at g_getItemIndex
                string encText = checkEncumbrance();
                if (encText != "Immobilized!") {
                    int idx = g_getItemIndex;
                    if (itemBuffer[idx].type < 13) {
                        int type = itemBuffer[idx].type;
                        if (type == 1) plyr.food += itemBuffer[idx].hp;
                        else if (type == 2) plyr.water += itemBuffer[idx].hp;
                        else if (type == 3) plyr.torches += itemBuffer[idx].hp;
                        else if (type == 4) plyr.timepieces += itemBuffer[idx].hp;
                        else if (type == 5) plyr.compasses += itemBuffer[idx].hp;
                        else if (type == 6) plyr.keys += itemBuffer[idx].hp;
                        else if (type == 7) plyr.crystals += itemBuffer[idx].hp;
                        else if (type == 8) plyr.gems += itemBuffer[idx].hp;
                        else if (type == 9) plyr.jewels += itemBuffer[idx].hp;
                        else if (type == 10) plyr.gold += itemBuffer[idx].hp;
                        else if (type == 11) plyr.silver += itemBuffer[idx].hp;
                        else if (type == 12) plyr.copper += itemBuffer[idx].hp;
                        itemBuffer[idx].location = 0; // remove from floor
                    }
                    else if (itemBuffer[idx].type > 100) {
                        itemBuffer[idx].location = 10; // move to inventory
                    }
                }
            }
            // Advance to next floor item (for both Y and N)
            bool foundNext = false;
            for (int i = g_getItemIndex + 1; i < plyr.buffer_index; i++) {
                if (itemBuffer[i].x == plyr.x && itemBuffer[i].y == plyr.y && 
                    itemBuffer[i].level == plyr.map && itemBuffer[i].location == 1) {
                    g_getItemIndex = i;
                    foundNext = true;
                    break;
                }
            }
            if (!foundNext) {
                // No more items - exit GET mode
                plyr.subState = SubState::None;
                g_itemActionMode = 0;
                g_itemActionStep = 0;
                g_itemActionRef = 9999;
            }
        }
        else if (sel >= 1 && sel <= 4 && g_itemActionStep == 0) {
            // Process item selection for DROP and CAST modes
            if (g_itemActionMode == 2) {
                // DROP mode: select carried item
                if (g_itemActionPage == 0) {
                    // Consumables page
                    int refs[] = {1000, 1001, 1002, 1003};
                    // First press - ask for quantity
                    g_itemActionRef = refs[sel-1];
                    g_itemActionStep = 1; // Now entering quantity
                    g_dropQuantity = 0;
                }
            }
            else if (g_itemActionPage == 1) {
                // Valuables page
                if (g_itemActionMode == 2) { // DROP
                    int refs[] = {1004, 1005, 1006, 1007};
                    int ref = refs[sel-1];
                    if (ref == 1004 && plyr.compasses > 0) { dropVolumeObject(2, ref); }
                    else if (ref == 1005 && plyr.keys > 0) { dropVolumeObject(2, ref); }
                    else if (ref == 1006 && plyr.crystals > 0) { dropVolumeObject(2, ref); }
                    else if (ref == 1007 && plyr.gems > 0) { dropVolumeObject(2, ref); }
                    plyr.subState = SubState::None;
                    g_itemActionMode = 0;
                }
            }
            else if (g_itemActionPage == 2) {
                // Gold/silver/copper page
                if (g_itemActionMode == 2) { // DROP
                    int refs[] = {1008, 1009, 1010, 1011};
                    int ref = refs[sel-1];
                    if (ref == 1008 && plyr.jewels > 0) { dropVolumeObject(2, ref); }
                    else if (ref == 1009 && plyr.gold > 0) { dropVolumeObject(2, ref); }
                    else if (ref == 1010 && plyr.silver > 0) { dropVolumeObject(2, ref); }
                    else if (ref == 1011 && plyr.copper > 0) { dropVolumeObject(2, ref); }
                    plyr.subState = SubState::None;
                    g_itemActionMode = 0;
                }
            }
            else if (g_itemActionPage >= 3) {
                // Carried items page - find items at location 10
                int pageIdx = g_itemActionPage - 3;
                int startIdx = pageIdx * 4;
                int found = 0;
                int itemRefs[4] = {9999, 9999, 9999, 9999};
                for (int i = 0; i < plyr.buffer_index && found < 4; i++) {
                    if (itemBuffer[i].location == 10) {
                        if (found >= startIdx) {
                            itemRefs[found - startIdx] = i;
                        }
                        found++;
                    }
                }
                if (sel <= 4 && itemRefs[sel-1] != 9999) {
                    int ref = itemRefs[sel-1];
                    if (g_itemActionMode == 2) { // DROP
                        dropObject(ref);
                    }
                    plyr.subState = SubState::None;
                    g_itemActionMode = 0;
                }
            }
        }
        else if (g_itemActionStep == 1) {
            // Step 1: For DROP mode, enter quantity. For GET mode, confirm pickup.
            if (g_itemActionMode == 2) { // DROP mode
                if (actionKey == "ESC") {
                    // Cancel drop
                    g_itemActionStep = 0;
                    g_itemActionRef = 9999;
                    g_dropQuantity = 0;
                }
                else if (actionKey == "RETURN") {
                    // Confirm drop
                    if (g_dropQuantity > 0) {
                        int existingItem = 9999;
                        if (g_itemActionRef == 1000 && plyr.food >= g_dropQuantity) {
                            existingItem = checkForGenericItemsHere(1);
                            if (existingItem == 9999) createGenericItem(1, g_dropQuantity); else itemBuffer[existingItem].hp += g_dropQuantity;
                            plyr.food -= g_dropQuantity;
                            std::cout << "Dropped " << g_dropQuantity << " food packets, new count: " << plyr.food << std::endl;
                        }
                        else if (g_itemActionRef == 1001 && plyr.water >= g_dropQuantity) {
                            existingItem = checkForGenericItemsHere(2);
                            if (existingItem == 9999) createGenericItem(2, g_dropQuantity); else itemBuffer[existingItem].hp += g_dropQuantity;
                            plyr.water -= g_dropQuantity;
                            std::cout << "Dropped " << g_dropQuantity << " water flasks, new count: " << plyr.water << std::endl;
                        }
                        else if (g_itemActionRef == 1002 && plyr.torches >= g_dropQuantity) {
                            existingItem = checkForGenericItemsHere(3);
                            if (existingItem == 9999) createGenericItem(3, g_dropQuantity); else itemBuffer[existingItem].hp += g_dropQuantity;
                            plyr.torches -= g_dropQuantity;
                            std::cout << "Dropped " << g_dropQuantity << " torches, new count: " << plyr.torches << std::endl;
                        }
                        else if (g_itemActionRef == 1003 && plyr.timepieces >= g_dropQuantity) {
                            existingItem = checkForGenericItemsHere(4);
                            if (existingItem == 9999) createGenericItem(4, g_dropQuantity); else itemBuffer[existingItem].hp += g_dropQuantity;
                            plyr.timepieces -= g_dropQuantity;
                            std::cout << "Dropped " << g_dropQuantity << " timepieces, new count: " << plyr.timepieces << std::endl;
                        }
                    }
                    // Exit drop mode and return to normal exploration
                    plyr.subState = SubState::None;
                    g_itemActionMode = 0;
                    g_itemActionStep = 0;
                    g_itemActionRef = 9999;
                    g_dropQuantity = 0;
                }
                else if (actionKey >= "0" && actionKey <= "9") {
                    // Build quantity number
                    g_dropQuantity = g_dropQuantity * 10 + (actionKey[0] - '0');
                }
                else if (actionKey == "BACKSPACE") {
                    // Remove last digit
                    g_dropQuantity = g_dropQuantity / 10;
                }
            }
        }
        // Redraw the item action menu
        clearDisplay();
        draw3DView();
        drawStatsPanel();
        drawConsoleBackground();
        
        // GET mode: show one floor item at a time with yes/no prompt
        if (g_itemActionMode == 1) {
            int idx = g_getItemIndex;
            if (idx < plyr.buffer_index && 
                itemBuffer[idx].x == plyr.x && itemBuffer[idx].y == plyr.y && 
                itemBuffer[idx].level == plyr.map && itemBuffer[idx].location == 1) {
                
                std::string str;
                if (itemBuffer[idx].type == 1) {str = itos(itemBuffer[idx].hp) + " Food Packet(s)"; }
                else if (itemBuffer[idx].type == 2) {str = itos(itemBuffer[idx].hp) + " Water Flask(s)"; }
                else if (itemBuffer[idx].type == 3) {str = itos(itemBuffer[idx].hp) + " Torch(es)"; }
                else if (itemBuffer[idx].type == 4) {str = itos(itemBuffer[idx].hp) + " Timepiece(s)"; }
                else if (itemBuffer[idx].type == 5) {str = itos(itemBuffer[idx].hp) + " Compass(es)"; }
                else if (itemBuffer[idx].type == 6) {str = itos(itemBuffer[idx].hp) + " Key(s)"; }
                else if (itemBuffer[idx].type == 7) {str = itos(itemBuffer[idx].hp) + " Crystal(s)"; }
                else if (itemBuffer[idx].type == 8) {str = itos(itemBuffer[idx].hp) + " Gem(s)"; }
                else if (itemBuffer[idx].type == 9) {str = itos(itemBuffer[idx].hp) + " Jewel(s)"; }
                else if (itemBuffer[idx].type == 10) {str = itos(itemBuffer[idx].hp) + " Gold"; }
                else if (itemBuffer[idx].type == 11) {str = itos(itemBuffer[idx].hp) + " Silver"; }
                else if (itemBuffer[idx].type == 12) {str = itos(itemBuffer[idx].hp) + " Copper"; }
                else {
                    str = getItemDesc(idx);
                    if (str == "ERROR") str = itemBuffer[idx].name;
                }
                
                cyText(1, "GET?");
                cyText(4, str);
                cyText(7, "Yes, No or ESC.");
            } else {
                // No item at current index - find next or exit
                bool foundNext = false;
                for (int i = idx; i < plyr.buffer_index; i++) {
                    if (itemBuffer[i].x == plyr.x && itemBuffer[i].y == plyr.y && 
                        itemBuffer[i].level == plyr.map && itemBuffer[i].location == 1) {
                        g_getItemIndex = i;
                        foundNext = true;
                        break;
                    }
                }
                if (!foundNext) {
                    // No more items - exit GET mode
                    plyr.subState = SubState::None;
                    g_itemActionMode = 0;
                    g_itemActionStep = 0;
                    g_itemActionRef = 9999;
                }
            }
        }
        // DROP mode: show item list or quantity input
        else if (g_itemActionMode == 2) {
            if (g_itemActionStep == 1) {
                // Show quantity input
                cyText(1, "DROP");
                cyText(3, "Drop how many?");
                if (g_dropQuantity > 0) {
                    bText(10, 4, itos(g_dropQuantity) + "_");
                } else {
                    bText(10, 4, "_");
                }
                cyText(6, "Enter amount or press ESC.");
            }
            else if (g_itemActionStep == 0) {
                // Show item list
                cyText(1, "DROP");
            if (g_itemActionPage == 0) {
                bText(5, 3, "(1) Food Packets: " + itos(plyr.food));
                bText(5, 4, "(2) Water Flasks: " + itos(plyr.water));
                bText(5, 5, "(3) Unlit Torches: " + itos(plyr.torches));
                bText(5, 6, "(4) Timepieces: " + itos(plyr.timepieces));
            } else if (g_itemActionPage == 1) {
                bText(5, 3, "(1) Compasses: " + itos(plyr.compasses));
                bText(5, 4, "(2) Keys: " + itos(plyr.keys));
                bText(5, 5, "(3) Crystals: " + itos(plyr.crystals));
                bText(5, 6, "(4) Gems: " + itos(plyr.gems));
            } else if (g_itemActionPage == 2) {
                bText(5, 3, "(1) Jewels: " + itos(plyr.jewels));
                bText(5, 4, "(2) Gold: " + itos(plyr.gold));
                bText(5, 5, "(3) Silver: " + itos(plyr.silver));
                bText(5, 6, "(4) Copper: " + itos(plyr.copper));
            } else if (g_itemActionPage >= 3) {
                int pageIdx = g_itemActionPage - 3;
                int startIdx = pageIdx * 4;
                int found = 0;
                int itemRefs[4] = {9999, 9999, 9999, 9999};
                for (int i = 0; i < plyr.buffer_index && found < 4; i++) {
                    if (itemBuffer[i].location == 10) {
                        if (found >= startIdx) {
                            itemRefs[found - startIdx] = i;
                        }
                        found++;
                    }
                }
                for (int s = 0; s < 4; s++) {
                    std::string label = "(" + std::to_string(s+1) + ")";
                    if (itemRefs[s] != 9999) {
                        label += " " + getItemDesc(itemRefs[s]);
                    }
                    bText(5, 3+s, label);
                }
            }
            bText(2, 8, "Item #, Forward, Back, or ESC to exit");
            SetFontColour(40, 96, 244, 255);
            bText(2, 8, "     #  F        B        ESC");
            SetFontColour(215, 215, 215, 255);
        }
        }
        updateDisplay();
        return Running;
    }

    // Poll input and dispatch only commands that are non-blocking on SDL2.
    // Legacy item/spell/get menus still use modal getSingleKey() loops and
    // will stall this tick-driven path if entered directly from gameplay.
    string key = readKey();
    const string rawKey = key;
    if (key == "left")  { key = "LEFT"; }
    if (key == "right") { key = "RIGHT"; }
    if (key == "up")    { key = "UP"; }
    if (key == "down")  { key = "DOWN"; }

    if (!rawKey.empty()) {
        std::cout << "GAME_INPUT: raw='" << rawKey << "' normalized='" << key << "'" << std::endl;
    }

    if (key == "LEFT") {
        std::cout << "GAME_INPUT: handled turnLeft" << std::endl;
        turnLeft();
    }
    else if (key == "RIGHT") {
        std::cout << "GAME_INPUT: handled turnRight" << std::endl;
        turnRight();
    }
    else if (key == "UP") {
        std::cout << "GAME_INPUT: handled moveForward" << std::endl;
        moveForward();
    }
    else if (key == "DOWN") {
        std::cout << "GAME_INPUT: handled moveBack" << std::endl;
        moveBack();
    }
    else if (key == "ESC") {
        if (plyr.infoPanel == 9) {
            std::cout << "GAME_INPUT: ESC in Use panel, returning to default panel" << std::endl;
            plyr.infoPanel = 1;
        } else {
            std::cout << "GAME_INPUT: handled optionsMenu" << std::endl;
            plyr.subState = SubState::OptionsMenu;
        }
    }
    else if (key == "QUIT") {
        std::cout << "GAME_INPUT: handled quit" << std::endl;
        Running = false;
        gameQuit = true;
    }
    else if (key == "," || key == "<") {
        std::cout << "GAME_INPUT: handled togglePanelsBackward (key='" << key << "')" << std::endl;
        togglePanelsBackward();
    }
    else if (key == "." || key == ">") {
        std::cout << "GAME_INPUT: handled togglePanelsForward (key='" << key << "')" << std::endl;
        togglePanelsForward();
    }
    else if (key == "A") {
        std::cout << "GAME_INPUT: handled toggleMiniMap" << std::endl;
        plyr.miniMapOn = !plyr.miniMapOn;
    }
    else if (key == "U") {
        std::cout << "GAME_INPUT: handled showUsePanel" << std::endl;
        plyr.infoPanel = 9;
    }
    else if (key == "G") {
        std::cout << "GAME_INPUT: handled getItem" << std::endl;
        
        // Check if there are any floor items at player's location
        bool foundItem = false;
        for (int i = 0; i < plyr.buffer_index; i++) {
            if (itemBuffer[i].x == plyr.x && itemBuffer[i].y == plyr.y && 
                itemBuffer[i].level == plyr.map && itemBuffer[i].location == 1) {
                foundItem = true;
                break;
            }
        }
        
        if (foundItem) {
            // Found items - enter GET mode to show floor items one at a time
            plyr.subState = SubState::ItemAction;
            g_itemActionMode = 1; // GET
            g_itemActionStep = 0; // Show one item at a time with yes/no
            g_itemActionRef = 9999;
            g_itemActionPage = 0;
            // Find first floor item index
            g_getItemIndex = 0;
            for (int i = 0; i < plyr.buffer_index; i++) {
                if (itemBuffer[i].x == plyr.x && itemBuffer[i].y == plyr.y && 
                    itemBuffer[i].level == plyr.map && itemBuffer[i].location == 1) {
                    g_getItemIndex = i;
                    break;
                }
            }
        } else {
            // No items found - show message and don't enter sub-state
            std::cout << "GAME_INPUT: no items to get" << std::endl;
            // Could set a status message here if needed
        }
    }
    else if (key == "D") {
        std::cout << "GAME_INPUT: handled dropItem" << std::endl;
        plyr.subState = SubState::ItemAction;
        g_itemActionMode = 2; // DROP
        g_itemActionStep = 0;
        g_itemActionRef = 9999;
        g_itemActionPage = 0;
    }
    else if (key == "C") {
        std::cout << "GAME_INPUT: handled castSpell" << std::endl;
        plyr.subState = SubState::ItemAction;
        g_itemActionMode = 3; // CAST
        g_itemActionStep = 0;
        g_itemActionRef = 9999;
        g_itemActionPage = 0;
    }
    else if (!key.empty()) {
        std::cout << "GAME_INPUT: ignored key='" << key << "'" << std::endl;
    }

    return Running;
}
#else
bool gameTick()
{
    // Web stub version - minimal implementation
    // Just render the game state and handle basic input without calling blocking functions
    
    if (!Running) {
        return false;
    }

    // Handle ESC key to quit
    string key = readKey();
    if (key == "ESC") {
        Running = false;
        return false;
    }

    // Render the current game view
    draw3DView();
    
    // For now, just return true to keep the game running
    return Running;
}
#endif // ARX_USE_SDL2

void gameLoop()
{
gameInit();
while (gameTick()) {
}
}

void optionsMenu()
{
clearDisplay();
displayOptionsMenu();
updateDisplay();

string key = getSingleKey();
if (key.empty()) return; // no key yet, come back next frame

if (key == "ESC")  { plyr.subState = SubState::None; plyr.status = EXPLORE; }
if (key == "S")    { plyr.subState = SubState::None; displaySaveGame(); }
if (key == "Q")    { plyr.subState = SubState::QuitMenu; }
if (key == "QUIT") { plyr.subState = SubState::None; Running = false; gameQuit = true; }
}

void quitMenu()
{
clearDisplay();
displayQuitMenu();
updateDisplay();

string key = getSingleKey();
if (key.empty()) return; // no key yet, come back next frame

if (key == "N")    { plyr.subState = SubState::None; }
if (key == "Y")    { plyr.subState = SubState::None; Running = false; gameQuit = true; }
if (key == "QUIT") { plyr.subState = SubState::None; Running = false; gameQuit = true; }
}

#ifndef ARX_USE_SDL2
void playerDies()
{
// First call: set up music and lyrics
static bool musicPlaying = false;
if (!musicPlaying) {
plyr.fixedEncounter = false;
plyr.miniMapOn = false;
plyr.status = 5; // Dead
if (plyr.musicStyle == 0) deathMusic.openFromFile("data/audio/death.ogg");
else deathMusic.openFromFile("data/audio/B/death.ogg");
deathMusic.play();
musicPlaying = true;
loadLyrics("death.txt");
}

// Draw one frame
clock1.restart();
clearDisplay();
drawStatsPanel();
updateLyrics();
iCounter++;
cyText(3, "You're DEAD.");
cyText(8, "(Press SPACE to continue)");
updateDisplay();

string key = getSingleKey();
if (key == "F1") { deathMusic.stop(); loadLyrics("death.txt"); deathMusic.play(); }
if (key == "SPACE") {
deathMusic.stop();
musicPlaying = false;       // reset static for next death
plyr.subState = SubState::None;
Running = false;            // end session, return to main menu
}
}
#else
void playerDies()
{
clearDisplay();
dispMain();
drawInfoPanels();
cyText(3, "You're DEAD.");
cyText(8, "(Press SPACE to continue)");
updateDisplay();

string key = readKey();
if (key == "SPACE") {
    plyr.subState = SubState::None;
    Running = false;
}
}
#endif

void togglePanelsForward()
{
     plyr.infoPanel++;
     if (plyr.infoPanel == 9) { plyr.infoPanel = 10; } // Skip Use panel
     if (plyr.infoPanel == 10) { plyr.infoPanel = 1; }
}

void togglePanelsBackward()
{
     plyr.infoPanel--;
     if (plyr.infoPanel == 9) { plyr.infoPanel = 8; } // Skip Use panel
     if (plyr.infoPanel == 0) { plyr.infoPanel = 8; }
}

void barredDoor()
{
//bool keynotpressed = true;
string str, key;
str = "";
int doorType;
if (plyr.movingForward) doorType = plyr.front;
else doorType = plyr.back;
int doorMenu = 0;
bool doorNotExamined = true;
while (doorMenu < 255) // closed
{
while (doorMenu == 0)
{
int useRef;
doorNotExamined = true;
dispMain();
drawConsoleBackground();
cyText(1, "The door won't open. You may:");
bText(9, 3, "(1) Examine the door");
bText(9, 4, "(2) Force the door");
bText(9, 5, "(3) Use a key");
bText(9, 6, "(4) Break an enchantment");
bText(9, 8, "(0) Leave it");
updateDisplay();
key = getSingleKey();
if (key == "0") { doorMenu = 255; }
if (key == "1") { doorMenu = 1; }
if (key == "2") { doorMenu = 2; }
if (key == "3") { doorMenu = 3; }
if (key == "4") { doorMenu = 4; }
if (key == "U")
{
useRef = selectItem(1);
if (useRef == 666) { doorMessage("@@@The door opens!@@@@<<< Press any key to continue >>>"); updateDoorDetails(); doorMenu = 255; moveThroughBarredDoor(); }
}
}
while (doorMenu == 1)
{
if (doorNotExamined)
{
doorNotExamined = false;
int doorIdentificationSuccess = rollDice(4, 6); // roll 3D6
doorTimedMessage("@@@Examining...");
str = "@You can't discern what bars the door.";
if (doorIdentificationSuccess < plyr.wis)
{
if (doorType == 8) str = "@The door appears to need a key.";
if (doorType == 9) str = "@The door appears to be bolted.";
if (doorType == 10) str = "@The door appears to be enchanted.";
}
}
dispMain();
drawConsoleBackground();
cText(str);
updateDisplay();
key = getSingleKey();
if (key == "SPACE") { doorMenu = 0; }
}

while (doorMenu == 2)
{
doorMessage("@@@Wham!");
int playerDamage = rollDice(1, 4);
plyr.hp -= playerDamage;
int doorRoll = rollDice(7, 6);
bool doorOpenSuccess = false;
if ((doorRoll <= plyr.str) && (doorType == 9)) doorOpenSuccess = true;
if ((doorOpenSuccess) && (plyr.hp > -1)) { doorMessage("@@@The door opens!@@@@<<< Press any key to continue >>>"); updateDoorDetails(); doorMenu = 255; moveThroughBarredDoor(); }
if ((!doorOpenSuccess) && (plyr.hp > -1)) { doorMessage("@@@The door remains shut.");  doorMenu = 0; }
if (plyr.hp < 0) { doorMenu = 255; } // player dead
}

while (doorMenu == 3)
{
if (plyr.keys == 0) { doorMessage("@@@You have none."); doorMenu = 0; }
else
{
if (doorType == 9) { doorMessage("@@@The door remains shut."); plyr.keys--; doorMenu = 0; }
if (doorType == 10) { doorMessage("@@@The door remains shut."); plyr.keys--; doorMenu = 0; }
if (doorType == 8) { doorMessage("@@@The door opens!@@@@<<< Press any key to continue >>>"); plyr.keys--; updateDoorDetails(); doorMenu = 255; moveThroughBarredDoor(); }
}

}

while (doorMenu == 4)
{
doorTimedMessage("@@@Concentrating...");

int doorRoll = rollDice(7, 6);
// Add to fatigue?
bool doorOpenSuccess = false;
if ((doorRoll <= plyr.inte) && (doorType == 10)) doorOpenSuccess = true;
if (doorOpenSuccess) { doorMessage("@@@The door opens!@@@@<<< Press any key to continue >>>"); updateDoorDetails(); doorMenu = 255; moveThroughBarredDoor(); }
if (!doorOpenSuccess) { doorMessage("@@@The door remains shut.");  doorMenu = 0; }
}

}
}

void moveWest()
{
if (plyr.facing==WEST) // west (forward)
{

if (plyr.z_offset>1.9f)
{
plyr.z_offset=0.0f;
plyr.oldx = plyr.x;
plyr.x--;
}
else
{
plyr.z_offset=plyr.z_offset+0.1f;
}

}

if (plyr.facing==EAST) // east (back)
{
if (plyr.z_offset<0.1f)
{
plyr.z_offset=1.9f;
plyr.oldx = plyr.x;
plyr.x--;
}
else
{
plyr.z_offset=plyr.z_offset-0.1f;
}
}

}

void moveEast()
{
if (plyr.facing==EAST) // east (forward)
{
if (plyr.z_offset>1.9f)
{
plyr.z_offset=0.0f;
plyr.oldx = plyr.x;
plyr.x++;
}
else
{
plyr.z_offset=plyr.z_offset+0.1f;
}

}

if (plyr.facing==WEST) // west (back)
{
if (plyr.z_offset<0.1f)
{
plyr.z_offset=1.9f;
plyr.oldx = plyr.x;
plyr.x++;
}
else
{
plyr.z_offset=plyr.z_offset-0.1f;
}
}

}

void moveNorth()
{
if (plyr.facing==NORTH) // north (forward)
{
if (plyr.z_offset>1.9f)
{
plyr.z_offset=0.0f;
plyr.oldy = plyr.y;
plyr.y--;
}
else
{
plyr.z_offset=plyr.z_offset+0.1f;
}
}

if (plyr.facing==SOUTH) // south (back)
{
if (plyr.z_offset<0.1f)
{
plyr.z_offset=1.9f;
plyr.oldy = plyr.y;
plyr.y--;
}
else
{
plyr.z_offset=plyr.z_offset-0.1f;
}
}

}

void moveSouth()
{
if (plyr.facing==SOUTH)
{
if (plyr.z_offset>1.9f)
{
plyr.z_offset=0.0f;
plyr.oldy = plyr.y;
plyr.y++;
}
else
{
plyr.z_offset=plyr.z_offset+0.1f;
}
}

if (plyr.facing==NORTH)
{
if (plyr.z_offset<0.1f)
{
plyr.z_offset=1.9f;
plyr.oldy = plyr.y;
plyr.y++;
}
else
{
plyr.z_offset=plyr.z_offset-0.1f;
}
}

}

void moveForward()
{
plyr.movingForward=true;
string encText = checkEncumbrance();
#ifndef ARX_USE_SDL2
if (encText=="Encumbered") { arx::sleep(arx::milliseconds(50)); }
if (encText=="Immobilized!") { arx::sleep(arx::milliseconds(200)); }
#endif

if (!solidWall(plyr.front) || plyr.z_offset < 1.7) {
        switch(plyr.facing)
        {
        case WEST: // w
moveWest();
break;

        case NORTH: // n
moveNorth();
            break;

        case EAST: // e
moveEast();
break;

        case SOUTH: // s
moveSouth();
break;
        }

        // Barred door
if ( (( plyr.front == 8 ) || ( plyr.front == 9 ) || ( plyr.front == 10 )) && (plyr.z_offset > 1.7)) // barred door
{
plyr.z_offset-=0.1f;
bool doorAlreadyOpened = false;
doorAlreadyOpened = checkBarredDoor();
if (doorAlreadyOpened) {   doorMessage("@@@The door opens!@@@@<<< Press any key to continue >>>"); moveThroughBarredDoor(); }
else { barredDoor(); }
}

        if ( (( plyr.front == 5 ) || ( plyr.front == 6 )) && (plyr.z_offset > 1.8)) // secret door
        {
plyr.z_offset=0.3f;

switch(plyr.facing)
{
case WEST: // w
plyr.oldx = plyr.x;
plyr.x--;
break;

case NORTH: // n
plyr.oldy = plyr.y;
plyr.y--;
break;

case EAST: // e
plyr.oldx = plyr.x;
plyr.x++;
break;
case SOUTH:
plyr.oldy = plyr.y;
plyr.y++; // s
break;
}

#ifndef ARX_USE_SDL2
if (plyr.scenario==0) { citySecretSound.play(); } else { dungeonSecretSound.play(); }
#endif
        }

        if ( (( plyr.front == 3 ) || ( plyr.front == 4 )) && (plyr.z_offset > 1.8)) // door
        {
plyr.z_offset=0.3f;

switch(plyr.facing)
{
case WEST: // w
plyr.oldx = plyr.x;
plyr.x--;
break;

case NORTH: // n
plyr.oldy = plyr.y;
plyr.y--;
break;

case EAST: // e
plyr.oldx = plyr.x;
plyr.x++;
break;

case SOUTH:
plyr.oldy = plyr.y;
plyr.y++; // s
break;
}
plyr.noticeability = plyr.noticeability + 1;
#ifndef ARX_USE_SDL2
if (plyr.scenario==0) { cityDoorSound.play(); } else { dungeonDoorSound.play(); }
#endif
}

 if ( ( plyr.front > 25 ) && ( plyr.front < 50 ) && (plyr.z_offset > 1.8)) // City doors with signs
     {
plyr.z_offset=0.3f;

switch(plyr.facing)
{
case WEST: // w
plyr.oldx = plyr.x;
plyr.x--;
break;

case NORTH: // n
plyr.oldy = plyr.y;
plyr.y--;
break;

case EAST: // e
plyr.oldx = plyr.x;
plyr.x++;
break;

case SOUTH:
plyr.oldy = plyr.y;
plyr.y++; // s
break;

}
#ifndef ARX_USE_SDL2
if (plyr.scenario==0) { cityDoorSound.play(); } else { dungeonDoorSound.play(); }
#endif
  }

    }
/*
if (plyr.x == -1) { plyr.x = 63; } // city wrap around
if (plyr.x == 64) { plyr.x = 0; } // city wrap around
if (plyr.y == -1) { plyr.y = 63; } // city wrap around
if (plyr.y == 64) { plyr.y = 0; } // city wrap around
*/
if (plyr.x == -1)
{
scenarioEntrance(401);
}
if (plyr.x == 64)
{
scenarioEntrance(406);
}
if (plyr.y == -1)
{
scenarioEntrance(403);
}
if (plyr.y == 64)
{
scenarioEntrance(404);
}
 // city wall edges leading to wilderness

}

void moveBack()
{
plyr.movingForward=false;
string encText = checkEncumbrance();
#ifndef ARX_USE_SDL2
if (encText=="Encumbered") { arx::sleep(arx::milliseconds(50)); }
if (encText=="Immobilized!") { arx::sleep(arx::milliseconds(200)); }
#endif

if ( ((plyr.back != 13)&&(plyr.back != 14) && (plyr.back != 37)) || (plyr.z_offset> 0.2))
{

    switch(plyr.facing)
        {
        case WEST: // facing west
            moveEast(); // opposite of west
            break;

        case NORTH: // facing north
            moveSouth(); // opposite of north
            break;

        case EAST: // facing east
        moveWest();
            break;

        case SOUTH: // facing south
        moveNorth();
        break;
        }

        // Barred door
if ( (( plyr.back == 8 ) || ( plyr.back == 9 ) || ( plyr.back == 10 )) && (plyr.z_offset < 0.3)) // barred door
{
plyr.z_offset+=0.1f;
bool doorAlreadyOpened = false;
doorAlreadyOpened = checkBarredDoor();
if (doorAlreadyOpened) {   doorMessage("@@@The door opens!@@@@<<< Press any key to continue >>>"); moveThroughBarredDoor(); }
else { barredDoor(); }
}

 if ( (( plyr.back == 5 ) || ( plyr.back == 6 )) && (plyr.z_offset < 0.3)) // secret door
        {
plyr.z_offset=1.7f;

switch(plyr.facing)
{
case WEST: // w
plyr.oldx = plyr.x;
plyr.x++;
break;

case NORTH: // n
plyr.oldy = plyr.y;
plyr.y++;
break;

case EAST: // e
plyr.oldx = plyr.x;
plyr.x--;
break;
case SOUTH:
plyr.oldy = plyr.y;
plyr.y--; // s
break;
}

#ifndef ARX_USE_SDL2
if (plyr.scenario==0) { citySecretSound.play(); } else { dungeonSecretSound.play(); }
#endif

        }

        if ( (( plyr.back == 3 ) || ( plyr.back == 4 )) && (plyr.z_offset < 0.3)) // door
        {
plyr.z_offset=1.7f;

switch(plyr.facing)
{
case WEST: // w
plyr.oldx = plyr.x;
plyr.x++;
break;

case NORTH: // n
plyr.oldy = plyr.y;
plyr.y++;
break;

case EAST: // e
plyr.oldx = plyr.x;
plyr.x--;
break;

case SOUTH:
plyr.oldy = plyr.y;
plyr.y--; // s
break;
}
#ifndef ARX_USE_SDL2
if (plyr.scenario==0) { cityDoorSound.play(); } else { dungeonDoorSound.play(); }
#endif
}

 if ( ( plyr.back > 25 ) && ( plyr.back < 50 ) && (plyr.z_offset < 0.3)) // City doors with signs
     {
plyr.z_offset=1.7f;

switch(plyr.facing)
{
case WEST: // w
plyr.oldx = plyr.x;
plyr.x++;
break;

case NORTH: // n
plyr.oldy = plyr.y;
plyr.y++;
break;

case EAST: // e
plyr.oldx = plyr.x;
plyr.x--;
break;

case SOUTH:
plyr.oldy = plyr.y;
plyr.y--; // s
break;

}
#ifndef ARX_USE_SDL2
if (plyr.scenario==0) { cityDoorSound.play(); } else { dungeonDoorSound.play(); }
#endif
  }

}

}

void turnLeft()
{
switch(plyr.facing)
    {
    case NORTH: // n

        plyr.facing = WEST;
plyr.z_offset=1.0f;
        break;

    case WEST: //  facing w before turning

        plyr.facing = SOUTH;
plyr.z_offset=1.0f;
        break;

    case EAST: // e

    plyr.facing = NORTH;
plyr.z_offset=1.0f;
        break;

    case SOUTH: // s

    plyr.facing = EAST;
    plyr.z_offset=1.0f;
break;
    }
}

void turnRight()
{
    switch(plyr.facing)
    {
    case NORTH: // n

        plyr.facing = EAST;
        plyr.z_offset=1.0f;
break;

    case WEST: // w

        plyr.facing = NORTH;
        plyr.z_offset=1.0f;
break;

    case EAST: // e

    plyr.facing = SOUTH;
        plyr.z_offset=1.0f;
break;

    case SOUTH: // s
    plyr.facing = WEST;
    plyr.z_offset=1.0f;
break;
    }
}

void checkTeleport()
{

if ( (plyr.special>=0xE0) && (plyr.special<=0x100) ) // 224 - 256
 {
      for (int i=0 ; i<=32 ; i++) // Max number of teleport objects
{
 //int ind = ((plyr.special)-0xEF);
if (plyr.special == teleports[i].ref)
{
if (teleports[i].new_map == 0)
{
plyr.x = (teleports[i].new_x);
plyr.y = (teleports[i].new_y);
}
if (teleports[i].new_map == 1)
{
plyr.x = (teleports[i].new_x)+32;
plyr.y = (teleports[i].new_y);
}
if (teleports[i].new_map == 2)
{
plyr.x = (teleports[i].new_x);
plyr.y = (teleports[i].new_y)+32;
}
if (teleports[i].new_map == 3)
{
plyr.x = (teleports[i].new_x)+32;
plyr.y = (teleports[i].new_y)+32;
}
int new_map;
// Change map level to new_map value accounting for level 1 now being single 64x64 map
if (teleports[i].new_map == 0) { new_map=1; } // was Dungeon level 1.1 32x32 segment
if (teleports[i].new_map == 1) { new_map=1; } // was Dungeon level 1.2 32x32 segment
if (teleports[i].new_map == 2) { new_map=1; } // was Dungeon level 1.3 32x32 segment
if (teleports[i].new_map == 3) { new_map=1; } // was Dungeon level 1.4 32x32 segment
if (teleports[i].new_map == 4) { new_map=2; } // was Dungeon level 2 32x32
if (teleports[i].new_map == 5) { new_map=3; } // was Dungeon level 3 16x16

if (plyr.map != new_map)
{
plyr.map = new_map;
moveMapLevelTeleport();
}

// Display flashing sequence for teleport.
plyr.teleporting = 20;

//plyr.x++;
//plyr.y++;

int location_index = getMapIndex(plyr.x, plyr.y);
transMapIndex (location_index);
}
  }
}

}

void checkFixedTreasuresold()
{
int treasureRef;
switch(plyr.special)
    {
case 0xAE: // Sword of the Adept
treasureRef = plyr.special-128;
            if (plyr.fixedTreasures[treasureRef] == false)
{
treasureMessage("@A sword protrudes from a slab@of black marble.@@@");
createWeapon(82);
plyr.fixedTreasures[treasureRef] = true;
getItems();
}
break;

case 0xAF: // Razor Ice
treasureRef = plyr.special-128;
            if (plyr.fixedTreasures[treasureRef] == false)
{
treasureMessage("A katana in a ribbed black lacquered@scabbard rests against a wall.@@A pattern of snowflakes has been@honed along its single edge.");
createWeapon(81);
plyr.fixedTreasures[treasureRef] = true;
getItems();
}
break;

case 0xA4: // Razor Ice
treasureRef = plyr.special - 128;
if (plyr.fixedTreasures[treasureRef] == false)
{
treasureMessage("A helm glowing with runes floats@in midair before you.");
createQuestItem(7);
plyr.fixedTreasures[treasureRef] = true;
getItems();
}
break;

case 0xB7: // Map Stone
treasureRef = plyr.special-128;
            if (plyr.fixedTreasures[treasureRef] == false)
{
string gender = "man";
if (plyr.gender==2) gender = "woman";
string key = "";
while (key!="SPACE")
{
dispMain();
cyText(1,"A note reads:");
string str = plyr.name+",";
bText(1,2,str);
str = "Congratulations my good "+gender+" on";
bText(4,4,str);
bText(1,5,"getting this far.");
bText(33,7,"A friend");
cyText(9,"<<< Press SPACE to continue >>>");
updateDisplay();
key = getSingleKey();
}
createQuestItem(4);
plyr.fixedTreasures[treasureRef] = true;
getItems();
}
break;

//case 0xB6: // Amethyst Rod
//treasureRef = plyr.special-128;
 //           if (plyr.fixedTreasures[treasureRef] == false)
//{
//treasureMessage("@A blue-violet quartz rod lies@on a purple dias.@@@");
//createQuestItem(5);
//plyr.fixedTreasures[treasureRef] = true;
//getItems();
//}
//break;

case 0xB6: // dont know
treasureRef = plyr.special - 128;
if (plyr.fixedTreasures[treasureRef] == false)
{
treasureMessage("@a strange object.@@@");
createQuestItem(4);
plyr.fixedTreasures[treasureRef] = true;
getItems();
}
break;

}

}

void checkShop()
{
     switch(plyr.special)
     {
        case 0x0D: // damon
            shopDamon();
            break;
        case 0x0C: // guild
            shopGuild();
            break;
        case 0x0F: // retreat
            shopRetreat();
            break;
        case 11: // Palace Prison
shopPrison();// Check for completed code already
            break;
        case 19: // Ferryman
            shopFerry();
            break;
        case 28: // lift
            shopLift();
            break;
        case 87: // king
            shopUndeadKing();
            break;
case 0x1D: // rathskeller
            runModule(0x1D);
            break;
        case 21: // dwarvenSmithy
            runModule(21);
            break;
case 0x50: // City Bank
if (plyr.scenario==0) { shopBank(); }
break;
case 0x90: // City Smithy
if (plyr.scenario==0) {shopSmithy(); }
break;
case 0x70: // City Shop
if (plyr.scenario==0) { shopShop(); }
break;
case 0x10: // City Inn
if (plyr.scenario==0) { shopInn(); }
if (plyr.scenario==1) { shopOracle(); }
break;
case 0x30: // City Tavern
if (plyr.scenario==0) { shopTavern(); }
break;
case 0xF0: // City Guild
    if (plyr.scenario==0) { shopGuild(); }
break;
case 208: // City Healer
if (plyr.scenario==0) { shopHealer(); }
break;
//case 176: // Closed by order of the palace
//if (plyr.scenario==0) { shopClosed(); }
//break;
case 3:
shopFountain();
break;
case 35:
shopFountain();
break;
case 5:
staircase();
break;
case 6:
runModule(0x06);    // Vault
break;
case 7:
if ((plyr.x==2) && (plyr.y==14) && (plyr.map==1)) shopGoblins();
if ((plyr.x==56) && (plyr.y==57) && (plyr.map==1)) shopTrolls();
break;
case 10:
shopChapel();
break;
        case 4: // entrance to wilderness
scenarioEntrance(400);
break;
case 200: // entrance to wilderness
scenarioEntrance(200);
break;
case 300: // entrance to wilderness
scenarioEntrance(401);
break;
case 301: // entrance to palace
scenarioEntrance(301);
break;
        case 302: // Entrance to arena
            arenaSouthernEntrance();
            break;
        case 303: // Entrance to arena
            arenaNorthernEntrance();
            break;
        case 304: // Entrance to arena
            arenaWesternEntrance();

            break;
     }
}

void leaveShop()
{
//lyricstexture.clear(sf::Color::Black); // wipe the lyric strip
if (plyr.facing==WEST) { plyr.x = plyr.oldx; }
if (plyr.facing==EAST) { plyr.x = plyr.oldx; }
if (plyr.facing==NORTH) { plyr.y = plyr.oldy; }
if (plyr.facing==SOUTH) { plyr.y = plyr.oldy; }
plyr.z_offset=1.6f; // position player just outside door
plyr.status = EXPLORE; // explore
}

void shopClosed()
{
bool keynotpressed = true;
while (keynotpressed) // closed
{
clearDisplay();
//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
string str, key;
plyr.status = 2; // shopping
//App.pushGLStates();
drawStatsPanel();
cyText (0, "Closed by@@Order of the Palace@@@@@@( Press space to continue )");
updateDisplay();
key = getSingleKey();
if ( key != "" ) { keynotpressed = false; }
if ( key == "up" ) { keynotpressed = true; }
}
leaveShop();
}

void treasureMessage(string str)
{
dispMain();
string txt = str + "@@<<< Press SPACE to continue >>>";
cyText(1, txt);
updateDisplay();
string key = getSingleKey();
if (key == "SPACE") {
plyr.subState = SubState::None;
}
}

void pauseGame()
{
// State-machine version: draw one frame, consume one key, return.
// Caller (gameTick) keeps calling this every frame until done.
dispMain();
cText("(Paused)@@@@@(Press SPACE to continue)");
updateDisplay();
string key = getSingleKey();
if (key == "SPACE" || key == "ESC") {
plyr.subState = SubState::None;
}
}

bool checkBarredDoor()
{
// Assume moving forward for now
bool doorAlreadyOpened = false;
int doorFacing;
if (plyr.movingForward) doorFacing = plyr.facing;
else
{
if (plyr.facing==WEST) doorFacing = 3; // actually moving backwards east
if (plyr.facing==NORTH) doorFacing = 4; // actually moving backwards south
if (plyr.facing==EAST) doorFacing = 1; // actually moving backwards west
if (plyr.facing==SOUTH) doorFacing = 2; // actually moving backwards north
}
for (int i=0 ; i<20 ; i++)
{
if ((plyr.doorDetails[i].x==plyr.x) && (plyr.doorDetails[i].y==plyr.y) && (plyr.doorDetails[i].level==plyr.map) && (plyr.doorDetails[i].direction == doorFacing)) { doorAlreadyOpened = true; }
}
return doorAlreadyOpened;
}

void updateDoorDetails()
{
// Adds an entry about a door that has been successfully opened. The 1st entry is overwritten after 20 door openings.

if (plyr.movingForward) plyr.z_offset=2.0f;
else plyr.z_offset=0.0f;

if (plyr.doorDetailIndex==19) plyr.doorDetailIndex = 0;
plyr.doorDetails[plyr.doorDetailIndex].x=plyr.x;
plyr.doorDetails[plyr.doorDetailIndex].y=plyr.y;
plyr.doorDetails[plyr.doorDetailIndex].level=plyr.map;
//int direction;
if (plyr.movingForward) plyr.doorDetails[plyr.doorDetailIndex].direction = plyr.facing;
if (!plyr.movingForward)
{
if (plyr.facing==WEST) plyr.doorDetails[plyr.doorDetailIndex].direction = 3; // actually moving east
if (plyr.facing==NORTH) plyr.doorDetails[plyr.doorDetailIndex].direction = 4; // actually moving south
if (plyr.facing==EAST) plyr.doorDetails[plyr.doorDetailIndex].direction = 1; // actually moving west
if (plyr.facing==SOUTH) plyr.doorDetails[plyr.doorDetailIndex].direction = 2; // actually moving north
}
plyr.doorDetailIndex++;
}

void moveThroughBarredDoor()
{
if (plyr.movingForward) plyr.z_offset = 2.0;
else plyr.z_offset = 0.0;
if ((plyr.facing==WEST) && (plyr.movingForward)) moveWest();
if ((plyr.facing==NORTH) && (plyr.movingForward)) moveNorth();
if ((plyr.facing==EAST) && (plyr.movingForward)) moveEast();
if ((plyr.facing==SOUTH) && (plyr.movingForward)) moveSouth();
if ((plyr.facing==WEST) && (!plyr.movingForward)) moveEast();
if ((plyr.facing==NORTH) && (!plyr.movingForward)) moveSouth();
if ((plyr.facing==EAST) && (!plyr.movingForward)) moveWest();
if ((plyr.facing==SOUTH) && (!plyr.movingForward)) moveNorth();
}

void doorMessage(string str)
{
dispMain();
drawConsoleBackground();
cyText(1, str);
updateDisplay();
string key = getSingleKey();
if (key == "SPACE" || !key.empty()) {
plyr.subState = SubState::None;
}
}

void doorTimedMessage(string str)
{
// Non-blocking: just draw the message. The timer is handled by the caller
// setting a subState timer (for now keeps the sf::sleep on native only).
dispMain();
drawConsoleBackground();
cyText(1, str);
updateDisplay();
#ifndef ARX_USE_SDL2
arx::sleep(arx::milliseconds(4000));
#endif
}

void teleport()
{
string typed_coords;
bool teleportComplete = false;
int name_length;
while (!teleportComplete)
{
string str;
string single_key;
clearDisplay();
drawText(2, 2, "To XX YY (followed by Enter)");
str = "To: "+typed_coords+"_";
drawText(2,5,str);
updateDisplay();

 single_key = getSingleKey();
 if (single_key=="SPACE") { single_key=" "; }
 if ((single_key=="RETURN") && (typed_coords!="")) { single_key = ""; teleportComplete = true; }
 if (single_key=="BACKSPACE")
 {
  name_length = typed_coords.size();
  if (name_length!=0)
  {
  int name_length = typed_coords.size();
  typed_coords = typed_coords.substr(0,(name_length-1));
  }
  single_key = "";
 }
 else
 {
  if (single_key!="RETURN")
  {
  name_length = typed_coords.size();
  if (name_length!=5) // check for limit of name length
  {
 typed_coords = typed_coords + single_key;
  }
   }
         }

}
string xtxt,ytxt,map;
xtxt=typed_coords.substr(0,2);
ytxt=typed_coords.substr(3,2);
//map=typed_coords.substr(6,1);
// if moving to new map then data files need to be loaded first
int x = 0;
int y = 0;
x=atoi(xtxt.c_str());
y=atoi(ytxt.c_str());

if (!((x>=0) && (x<=63))) x = 0;
if (!((y>=0) && (y<=63))) y = 0;
plyr.x = x;
plyr.y = y;
//plyr.map=atoi(map.c_str());
// load new content?
// teleport sound
// validate!
}

void initialiseNewGame()
{
    // COPY FROM ARX.CPP - Prepare shop stock etc...
#ifndef ARX_USE_SDL2
    doorCityBuffer.loadFromFile("data/audio/cityDoor.wav");
    secretCityBuffer.loadFromFile("data/audio/citySecretDoor.wav");
    citySecretSound.setBuffer(secretCityBuffer);
    cityDoorSound.setBuffer(doorCityBuffer);

    doorDungeonBuffer.loadFromFile("data/audio/dungeonDoor.wav");
    secretDungeonBuffer.loadFromFile("data/audio/dungeonSecretDoor.wav");
    dungeonSecretSound.setBuffer(secretDungeonBuffer);
    dungeonDoorSound.setBuffer(doorDungeonBuffer);

smithyBuffer.loadFromFile("data/audio/smithyHammer3.wav");
smithySound.setBuffer(smithyBuffer);
smithySound.setLooping(true);

teleBlack.loadFromFile("data/images/core/teleport_black.png");
teleGold.loadFromFile("data/images/core/teleport_gold.png");
#endif

InitMap();

readItemData();
readSpellData();
//loadMonstersBinary(); duplicate
}

// Function to check if a value is in the solidWall array
bool solidWall(int front) {
// Define the solid wall values
std::vector<int> solidWalls = { 13, 14, 37, 68 };

// Check if 'front' is in the solidWalls vector
return std::find(solidWalls.begin(), solidWalls.end(), front) != solidWalls.end();
}
