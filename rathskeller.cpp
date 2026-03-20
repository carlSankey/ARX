/* RATHSKELLER.CPP
 *
 * TODO:
 * carrying corpse message once corpses implemented, dragon/wyrm corpse gold offer,
 * enclosed booth???
 * all music and on screen lyrics
 *
 */


// #include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <iostream>
#include <sstream>

#include "module.h"
#include "player.h"
#include "font.h"
#include "display.h"
#include "game.h"
#include "misc.h"
#include "rathskeller.h"
#include "automap.h"

enum MENUS
{
    MENU_LEFT,
    MENU_MAIN,
    MENU_SEATED,
    MENU_ORDER,
    MENU_ROUND,
    MENU_LEAVE_A_TIP,
    MENU_TRANSACT,
    MENU_THANKS,
    MENU_NO_FUNDS,
    MENU_NO_TIPPING_FUNDS,
    MENU_GET_TIP_VALUE,
    MENU_GET_ITEM_CHOICE,
    MENU_RIGHT_AWAY,
    MENU_ORDER_ANYTHING_ELSE,
    MENU_LEAVING_ALREADY,
    MENU_NPC_ENTERS,
    MENU_NPC_OPENER,
    MENU_NPC_MEAL,
    MENU_NPC_DRINK,
    MENU_NPC_TRANSACT,
    MENU_NPC_RUMOUR
};

int rathskellerFoodDrink[18];
bool rathskellerFoodDrinkCheck[40];

struct foodDrinkItem
{
	int                 basePrice; // Multiplied by 2 to get cost in silvers
	int                 hungerValue;
	int                 thirstValue;
	int                 alcoholValue;
	string              name;
};

foodDrinkItem rathskellerItems[41] =
{
    {0x05,0x12,0x02,0x00, "Rack of Lamb"        },
    {0x06,0x18,0x04,0x00, "Roast Beef"          },
    {0x04,0x11,0x02,0x00, "Roast Fowl"          },
    {0x1E,0x20,0x04,0x00, "Roast Dragon"        },
    {0x04,0x0D,0x01,0x00, "Spare Ribs"          },
    {0x07,0x14,0x02,0x00, "Roast Mutton"        },
    {0x0A,0x16,0x03,0x00, "Leg of Dragon"       },
    {0x28,0x14,0x04,0x00, "a Burger"            },
    {0x06,0x0E,0x02,0x00, "Roast Pig"           },
    {0x06,0x10,0x02,0x00, "Sausages"            },
    {0x0A,0x14,0x02,0x00, "Fillet of Beef"      },
    {0x07,0x10,0x04,0x00, "Ragout of Beef"      },
    {0x0A,0x18,0x04,0x00, "Ragout of Dragon"    },
    {0x07,0x10,0x02,0x00, "Smoked Fish"         },
    {0x07,0x0E,0x02,0x00, "Crayfish"            },
    {0x09,0x12,0x02,0x00, "Lobster"             },
    {0x03,0x08,0x04,0x00, "a Bowl of Fruit"     },
    {0x03,0x05,0x03,0x00, "a Plate of Greens"   },
    {0x02,0x04,0x00,0x00, "a Loaf of Bread"     },
    {0x03,0x08,0x00,0x00, "a Block of Cheese"   },
    {0x04,0x0A,0x04,0x00, "a Bowl of Chili"     },
    {0x03,0x07,0x04,0x00, "Pasta"               },
    {0x04,0x09,0x04,0x00, "Lasagna"             },
    {0x03,0x08,0x00,0x00, "a Sandwich"          },
    {0x03,0x07,0x0A,0x00, "Lentil Soup"         },
    {0x04,0x0C,0x00,0x00, "Pemmican"            },
    {0x03,0x0A,0x08,0x00, "Gruel"               },
    {0x02,0x06,0x00,0x00, "Sweet Meats"         },
    {0x02,0x06,0x02,0x00, "Blood Pudding"       },
    {0x02,0x08,0x00,0x00, "Haggis"              },
    {0x02,0x02,0x0A,0x05, "Spirits"             },
    {0x03,0x03,0x0C,0x04, "Mead"                },
    {0x02,0x02,0x0A,0x03, "Beer"                },
    {0x02,0x03,0x0A,0x04, "Ale"                 },
    {0x03,0x03,0x0A,0x03, "Wine"                },
    {0x02,0x02,0x0A,0x04, "Grog"                },
    {0x02,0x04,0x10,0x00, "Sarsaparilla"        },
    {0x02,0x04,0x10,0x00, "Milk"                },
    {0x02,0x06,0x0C,0x00, "Grape Juice"         },
    {0x01,0x02,0x10,0x00, "Mineral Water"       },
    {0x02,0x06,0x0C,0x00, "Orange Juice"        }
};




/* Dost thou wish to sell that fine..£=‰¥dragon meat for ²q.. golds? (¡Y  or ¡N ) */

string npcDescriptions[5] =
{
    "A sly looking stranger sits down.",
    "A sloppy guy stumbles in.",
    "A dwarf wearing a raincoat walks over.",
    "A human wearing a fur-lined outfit@@says \"Hello Adventurer!\"",
    "A large, burly human approaches."
};

string npcOpeners[5] =
{
    "It's too bad they beefed up@@the security in the Dungeon.@@It used to be an easy life stealing@@from others.",
    "It's been..@@Why, I can't remember the@@last time I've had a drink.",
    "I'm tired. I've just finished@@a show in The City. I hate@@dancing, but it's a living.",
    "I've just lost a third of@@what I own. You sure can't@@trust the banks around here.",
    "Greetings Adventurer.@@I'm known as Salin Wauthra."
};

string npcRumours[98] =
{
"A wise oracle dwells beneath@the Floating Gate.",
"Bank vault basements can be found@on the first level.",
"Acrinimiril's Tomb is haunted.",
"The Chapel dispenses@pragmatic salvation.",
"A fountain that heals wounds@is on the first level.",
"The Troll King eats@punks like you for breakfast.",
"The Goblin King@is an underhanded fink.",
"There is no honor among Thieves.",
"All magic has a price.",
"The Guilds of the undercity@war upon each other.",
"The river leads to@lands of the Undead.",
"There is a fountain that cures disease@on the second level.",
"You can always trust the guards.",
"Step lightly in the crystal caverns.",
"Lucky's got the brew for you.",
"You can trust what you hear@from the Horse's mouth.",
"Never trust a demon.",
"The dwarf on the second level is@interested in old weapons and armor.",
"A special fountain on the third level@can enliven your day.",
"There is a dreaded dragon@on the third level.",
"Beware the gauntlet of doom@on the third level.",
"There is a very special door@hidden on the third level.",
"Evil magic takes a special toll.",
"Seek the ways of the Wizards of Law.",
"We're being watched all the time.",
"The music heard in the tavern@comes from beyond this world.",
"The temptations of evil are strong.",
"Fruit juice is.¥very invigorating.",
"Many great treasures@are carefully guarded.",
"Be sure your friends@are not your foes.",
"Always leave a tip@for services rendered.",
"The pure in heart know@how to show mercy.",
"The Oracle always tells the truth.",
"Never fight fire with fire.",
"The truly brave are not@afraid to make peace.",
"In the Rooms of Confusion,@seek out a secret door.",
"Fine clothing attracts more friends.",
"Gluttony is hazardous to your health.",
"Some treasures are better left alone.",
"Cross the river at midnight.",
"One magic ring@can cause a lot of trouble.",
"Answering riddles brings great reward.",
"Give a generous donation at@the Retreat.",
"Be careful when dealing with@\"Honest\" Omar and his brother Jeff.",
"The Dwarf loves sparkling gems.",
"Greedy adventurers are often visited by@by the Devourer!.",
"Read the Guidebook thoroughly!.",
"Sermons are good for your soul.",
"About half the rumors you hear@in the Dungeon are false.",
"Beware the screamer that@walks on the wind.",
"Locked doors require skill to pass.",
"Muggers can make powerful allies.",
"Occum gets the best weapons for his shop@by capturing them in a clever trap.",
"Keep an eye on your purse@if you sleep at the Sanctuary.",
"There are magical baths@hidden on the fourth level.",
"All goblins have a hidden zipper.",
"Seek the leg of Jeerbeef.",
"Be kind to zombies.",
"The third level of the dungeon@is patrolled by elephants.",
"The Troglodytes are valuable friends.",
"Don't drink from the fountains.",
"The Troll Tyrant is fair@and honorable.",
"The Goblin Lord is a decent fellow.",
"Black Magic is more powerful@than White Magic.",
"The Thieves' Guild is a@Trustworthy organization.",
"My dog told me to watch out for you.",
"Seek magical knowledge@at the Green Wizards Academy.",
"Never eat anything@you didn't kill yourself.",
"The Red Wizards teach the Black Arts.",
"Beware the Clown that@laughs by the bedside.",
"The Guilds conspire together@against the King.",
"Those about to die dwell beneath@the Arena on the third level.",
"They need a new jester at the Palace.",
"The Palace Prison is full of@convicts and madmen.",
"Beware the boatman on the second level.",
"There is a fountain on the fourth level@that makes you invulnerable.",
"Mutilation has its good points.",
"Beware of the poisoner named Lucky.",
"The Clothes Horse will let you ride him@if you bring him a nice hat.",
"Demons are great guys.",
"Seek the Dwarf by the Wharf.",
"The fountain on the third level@flows directly from Hell.",
"The Great Wyrm likes to bluff.",
"The third level is actually@safer than the first.",
"If you push something hard enough,@it will fall over.",
"Two and two does not always equal four@on the fourth level.",
"Seek the teleporters@to rapidly travel The Dungeon.",
"Look out for the guy@with the high arches in his feet.",
"Exploring the third level@requires high stamina.",
"The one-winged dog flies tonight.",
"You can get rich quick@on the fourth level.",
"Armor takes skill to use well.",
"An unspeakable horror dwells@at the center of time.",
"Avoid the elf in the raincoat.",
"Quest for the man@in the purple trousers.",
"Never allow an enemy to surrender,@They have nothing true to say.",
"Walk backwards through@the Hall of Mirrors.",
"There's money to be made@in the blacksmithing trade."
};

sf::Music rathskellerMusic;

int menu, value, roundCost, stillEating, npcMealCost, npcDrinkCost;
bool bar, npcNotPresent, rathMusicPlaying;
string eatingDescription = "";
string greetingText, npcDescription, npcOpener, npcRumour;

void runRathskeller()
{
    menu = MENU_MAIN;
    loadShopImage(2);
    stillEating = 0;
    bar = true;
    npcNotPresent = true;
    roundCost = randn(5,20);
    rathMusicPlaying = false;

    buildFoodDrinkMenuOptions(); // Each visit currently
    addRathskellerToMap();
    setGreetingText();
    playRathskellerMusic();

    while ( menu != MENU_LEFT )
    {
        updateFoodConsumption();
        checkForNPC();
        clearShopDisplay();
        displayModuleText();
        updateDisplay();
        processMenuInput();
    }

    rathskellerMusic.stop();
}


void displayModuleText()
{
    string str;
    if ( menu == MENU_MAIN )
    {
        cyText (0, greetingText);
        cyText (2, "Where dost thou wish to sit?");
        bText (15,4, "(1) At the bar");
        bText (15,5, "(2) At a table");
        bText (15,7, "(0) Leave");
    }

    if ( menu == MENU_SEATED )
    {
        if (bar) { str = "Thou art sitting at the bar."; } else { str = "Thou art sitting at a table."; }
        if (stillEating > 0) str = "Thou art eating " + eatingDescription + ".";
        cyText (0, str);
        cyText (2, "What dost thou wish?");
        bText (6,4, "(1) Order something");
        bText (6,5, "(2) Buy a round for the house");
        bText (6,7, "(0) Leave");
    }

    if ( menu == MENU_LEAVE_A_TIP )
    {
        bText (11,2, "(1) Say goodbye");
        bText (11,4, "(2) Leave a tip");
        bText (11,7, "(0) Leave quietly");
    }

    if (menu == MENU_TRANSACT)
    {
        cyText (0, "Dost thou wish to:");
        bText (10,2, "(1) Buy him a drink");
        bText (10,3, "(2) Buy him a meal");
        bText (10,4, "(3) Transact");
        bText (10,6, "(0) Ignore him");
    }

    if (menu == MENU_ROUND)
    {
        cyText (1, "A round for the house will cost:");
        cyText (3, itos(roundCost)+ " silvers.");
        cyText (6, "Dost thou still wish to buy? (Y or N)");
    }
    if (menu == MENU_NPC_DRINK)
    {
        cyText (1, "The drink will cost you "+ itos(npcDrinkCost)+ " silvers.");
        cyText (3, "OK (Y or N)");
    }
    if (menu == MENU_NPC_MEAL)
    {
        cyText (1, "The meal will cost you "+ itos(npcMealCost)+ " silvers.");
        cyText (3, "OK (Y or N)");
    }
    if (menu == MENU_NPC_ENTERS) { cyText (1, npcDescription); }
    if (menu == MENU_NPC_OPENER) { cyText (1, npcOpener); }
    if (menu == MENU_NPC_RUMOUR) { cyText (1, npcRumour); }

    if (menu == MENU_THANKS) cyText (1,"Thank you!  Please come again.");
    if (menu == MENU_RIGHT_AWAY) cyText (1,"Coming right up!");
    if (menu == MENU_NO_FUNDS) cyText (2,"I'm sorry, you have not the funds.");
    //if (menu == MENU_NO_FUNDS) cyText (2,"I'm sorry, you have not the funds.@@@@@Note: We do not accept coppers here.");
    if (menu == MENU_NO_TIPPING_FUNDS) cyText (1,"Your generosity is greatly appreciated.@@However, your humor is not.");
    if (menu == MENU_ORDER_ANYTHING_ELSE) cyText (1,"Coming up!@@Is there anything else@@I can get for you? (Y or N)");
    if (menu == MENU_LEAVING_ALREADY) cyText (1,"Leaving already?  You haven't@@finished your meal.  I'll wrap it@@in a packet for you.");
    if (menu == MENU_GET_TIP_VALUE) { /* This condition does not print a message in the normal way - see leaveTip() */ }
    if (menu == MENU_GET_ITEM_CHOICE) { /* This condition does not print a message in the normal way - see leaveTip() */ }
}


void  processMenuInput()
{
    string key = readKey();
    //value = atoi(key.c_str());

    switch (menu)
    {
        case MENU_MAIN:
            if (key=="1")   menu = MENU_SEATED;
            if (key=="2")   { bar = false; menu = MENU_SEATED; }
            if (key=="0")   menu = MENU_LEFT;
            if (key=="down")   menu = MENU_LEFT;
            break;
        case MENU_SEATED:
            if (key=="1")   menu = MENU_GET_ITEM_CHOICE;
            if (key=="2")   menu = MENU_ROUND;
            if (key=="0")   { if ( stillEating > 0 ) menu = MENU_LEAVING_ALREADY; else  menu = MENU_LEAVE_A_TIP; }
            break;
        case MENU_LEAVE_A_TIP:
            if (key=="1")   { menu = MENU_THANKS; plyr.rathskellerFriendship++; }
            if (key=="2")   { menu = MENU_GET_TIP_VALUE; }
            if (key=="0")   menu = MENU_LEFT;
            break;
        case MENU_TRANSACT:
            if (key=="1")   { menu = MENU_NPC_DRINK; }
            if (key=="2")   { menu = MENU_NPC_MEAL; }
            if (key=="3")   { menu = MENU_NPC_OPENER; }
            if (key=="0")   { npcNotPresent = true; menu = MENU_SEATED; }
            break;
        case MENU_ROUND:
            if (key=="Y")
            {
                if (checkCoins(0,roundCost,0))
                {
                    deductCoins(0,roundCost,0);
                    menu = MENU_SEATED;
                    plyr.rathskellerFriendship++;
                }
                else
                {
                    plyr.rathskellerFriendship--;
                    menu = MENU_NO_FUNDS;
                }
            }
            if (key=="N")   menu = MENU_SEATED;
            break;
        case MENU_NO_FUNDS:
            if (key != "") menu = MENU_SEATED;
            break;
        case MENU_THANKS:
            if (key != "") menu = MENU_LEFT;
            break;
        case MENU_RIGHT_AWAY:
            if (key != "") menu = MENU_SEATED;
            break;
        case MENU_NO_TIPPING_FUNDS:
            if (key != "") menu = MENU_GET_TIP_VALUE;
            break;
        case MENU_GET_TIP_VALUE:
            leaveTip();
            break;
        case MENU_GET_ITEM_CHOICE:
            chooseFoodDrinkMenuItem();
            break;
        case MENU_ORDER_ANYTHING_ELSE:
            if (key=="Y") menu = MENU_GET_ITEM_CHOICE;
            if (key=="N") menu = MENU_SEATED;
            break;
        case MENU_NPC_ENTERS:
            if (key != "") menu = MENU_TRANSACT;
            break;
        case MENU_NPC_DRINK:
            if (key=="Y")
            {
                if (checkCoins(0,npcDrinkCost,0))
                {
                    deductCoins(0,npcDrinkCost,0);
                    menu = MENU_NPC_RUMOUR;
                }
                else
                {
                    menu = MENU_NO_FUNDS;
                }
            }
            if (key=="N") menu = MENU_TRANSACT;
            break;
        case MENU_NPC_MEAL:
            if (key=="Y")
            {
                if (checkCoins(0,npcMealCost,0))
                {
                    deductCoins(0,npcMealCost,0);
                    menu = MENU_NPC_RUMOUR;
                }
                else
                {
                    menu = MENU_NO_FUNDS;
                }
            }
            if (key=="N") menu = MENU_TRANSACT;
            break;
        case MENU_NPC_RUMOUR:
            if (key != "") menu = MENU_SEATED;
            break;
        case MENU_NPC_OPENER:
            if (key != "") menu = MENU_TRANSACT;
            break;
        case MENU_LEAVING_ALREADY:
            if (key != "") { plyr.food++ ; menu = MENU_LEAVE_A_TIP; }
            break;
    }
}

void leaveTip()
{
    int tip = inputNumber("How many silvers@dost thou wish to leave?");
    if (checkCoins(0,tip,0))
    {
        deductCoins(0,tip,0);
        menu = MENU_THANKS;
        plyr.rathskellerFriendship++;
        plyr.rathskellerFriendship++;
    }
    else
    {
        plyr.rathskellerFriendship--;
        menu = MENU_NO_TIPPING_FUNDS;
    }
}



void chooseFoodDrinkMenuItem()
{
    int foodDrinkChoice = inputItemChoice("What would thou like? (0 to go back)",20);
    if (foodDrinkChoice < 255 )
    {
        //cout << "Item choice:" << foodDrinkChoice << endl;
        int itemCost = rathskellerItems[foodDrinkChoice].basePrice * 2;

        if (checkCoins(0,itemCost,0))
        {
            deductCoins(0,itemCost,0);
            consumeFoodDrinkItem(foodDrinkChoice);
            menu = MENU_ORDER_ANYTHING_ELSE;
        }
        else
        {
            plyr.rathskellerFriendship--;
            menu = MENU_NO_FUNDS;
        }

    }
    else { menu = MENU_SEATED; }
}



void consumeFoodDrinkItem(int foodDrinkItem)
{
    // Unlike the City taverns Rathskeller items appear to have both food and drink values

    plyr.hunger -= rathskellerItems[foodDrinkItem].hungerValue;
    if (plyr.hunger < 0) { plyr.hunger = 0; }
    plyr.digestion += (rathskellerItems[foodDrinkItem].hungerValue)*2;

    if (rathskellerItems[foodDrinkItem].hungerValue > 9)
    {
        eatingDescription = rathskellerItems[foodDrinkItem].name;
        stillEating = rathskellerItems[foodDrinkItem].hungerValue;
    }

    plyr.thirst -= rathskellerItems[foodDrinkItem].thirstValue;
    if (plyr.thirst < 0) { plyr.thirst = 0; }
    plyr.alcohol += rathskellerItems[foodDrinkItem].alcoholValue;
}

void updateFoodConsumption()
{
    // Check for time 30 seconds?
    //if ( stillEating > 0 ) stllEating--;
    //if ( stillEating == 0 ) eatingDescription = "";
}


void buildFoodDrinkMenuOptions()
{
	// Run daily to randomly pick 19 unique items
	// Check for duplicates using Check array of bools
	// Set bools for duplicate items check to false

    string str;
	int itemNo = 0;

    for (int y=0; y<40; y++) { rathskellerFoodDrinkCheck[y] = false; }

    for (int waresNo=0 ; waresNo<20 ; waresNo++)
    {
        // Current code may create duplicate items in each tavern
        bool uniqueItem = false;
        while (!uniqueItem)
        {
            itemNo = randn(0,40); // was 12

            if (!rathskellerFoodDrinkCheck[itemNo])
            {
                menuItems[waresNo].menuName = rathskellerItems[itemNo].name; // its not a duplicate
                str = itos((rathskellerItems[itemNo].basePrice)*2) + "           "; // its not a duplicate
                str.replace(3,7,"silvers");
                menuItems[waresNo].menuPrice  = str;
                menuItems[waresNo].objRef = itemNo; // its not a duplicate
                rathskellerFoodDrinkCheck[itemNo] = true;
                uniqueItem = true;
            }
        }
    }
}

	// Simple sort of items in numeric order
    //	for (int tavernNo=0 ; tavernNo<14 ; tavernNo++)
    //	{
    //		sort(tavernDailyDrinks[tavernNo], tavernDailyDrinks[tavernNo]+6); // 6 max no drinks available on menu
    //	}




void setGreetingText()
{
    // There is nobody else here..
    // Get that <CORPSE> out of here!
    if (plyr.rathskellerFriendship == -6) greetingText = "Slimy thing, must thee darken my door?";
    if (plyr.rathskellerFriendship == -5) greetingText = "Thou fewmet, why art thou here?";
    if (plyr.rathskellerFriendship == -4) greetingText = "What now, filthy Cheapskate?";
    if (plyr.rathskellerFriendship == -3) greetingText = "What dost thou here, insolent one!";
    if (plyr.rathskellerFriendship == -2) greetingText = "Hast thou brought enough cash?";
    if (plyr.rathskellerFriendship == -1) greetingText = "Thy welcome is wearing thin.";
    if (plyr.rathskellerFriendship == 0)  greetingText = "Hello, Stranger!";
    if (plyr.rathskellerFriendship == 1)  greetingText = "Hello, "+ plyr.name + "!";
    if (plyr.rathskellerFriendship >= 2)  greetingText = "Well met, "+ plyr.name + " is welcome here!";
}



void checkForNPC()
{
    if ((npcNotPresent) && (menu==MENU_SEATED))
    {
        int npcCheck = randn(1,1000);
        if (npcCheck == 1)
        {
            npcDescription = npcDescriptions[randn(1,4)];
            npcOpener = npcOpeners[randn(1,4)];
            npcRumour = npcRumours[randn(1,98)];
            npcDrinkCost = randn(1,8);
            npcMealCost = randn(1,18);
            menu = MENU_NPC_ENTERS;
            npcNotPresent = false;
        }
    }
}



void addRathskellerToMap()
{
    setAutoMapFlag(plyr.map, 58, 3);
	setAutoMapFlag(plyr.map, 62, 3);
	setAutoMapFlag(plyr.map, 58, 2);
	setAutoMapFlag(plyr.map, 59, 2);
	setAutoMapFlag(plyr.map, 60, 2);
	setAutoMapFlag(plyr.map, 61, 2);
	setAutoMapFlag(plyr.map, 62, 2);
	setAutoMapFlag(plyr.map, 58, 1);
	setAutoMapFlag(plyr.map, 59, 1);
	setAutoMapFlag(plyr.map, 60, 1);
	setAutoMapFlag(plyr.map, 61, 1);
	setAutoMapFlag(plyr.map, 62, 1);
}


void playRathskellerMusic()
{
    if (!rathMusicPlaying)
    {
        int randomSong = randn(0,2);
        if (randomSong==1) { rathskellerMusic.openFromFile("data/audio/rathskeller.ogg"); }
        else { rathskellerMusic.openFromFile("data/audio/rathskeller2.ogg"); }
        rathskellerMusic.play(); rathMusicPlaying = true;
    }
}

