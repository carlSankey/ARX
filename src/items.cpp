
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>

#include <cstdio> // Include the necessary header
#include <vector>
#include <direct.h>
#include <random>

#include "audio.h"
#include "player.h"
#include "font.h"
#include "display.h"
#include "level.h"
#include "game.h"
#include "encounter.h"
#include "misc.h"
#include "items.h"
#include "guild.h"
#include "actor.h" // for weapons
#include "globals.h"
#include "itemSelect.h"
#include "effects.h"
#include "spells.h"


extern struct SearchResult;

buffer_item itemBuffer[itemBufferSize]; // Items on floor,carried and in void - now 250

unsigned char dungeonItems[dungeonItemsSize];

bonusDamage itemBonus;


std::map<int, int*> itemBonusMap = {
	{0, &itemBonus.blunt},
	 {1 , &itemBonus.sharp },
	 {2 , &itemBonus.earth },
	 {3 , &itemBonus.air },
	 {4 , &itemBonus.fire },
	 {5 , &itemBonus.water },
	 {6 , &itemBonus.power },
	 {7 , &itemBonus.magic },
	 {8 , &itemBonus.good },
	 {9 , &itemBonus.evil },
	 {10 , &itemBonus.cold },
	 {11 , &itemBonus.nature },
	 {12 , &itemBonus.acid }
};


int itemOffsets[141] =
{
    742, // THE_STAR_CARD
    782, // THE_FOOL_CARD
    854, // THE_HEIROPHANT_CARD
    900, // DEATH_CARD
    937, // ACE_OF_CUPS
    973, // THE_CHARIOT_CARD
    1016, // STRENGTH_CARD
    1056, // ACE_OF_WANDS
    1109, // TEMPERANCE_CARD
    1151, // KING_OF_WANDS
    1221, // PAGE_OF_CUPS_CARD
    1281, // ACE_OF_PENTACLES
    1322, // HIGH_PRIESTESS_CARD
    1400, // COLD_WAND
    1433, // FIRE_WAND
    1466, // PARALYSIS_WAND
    1504, // EYE_OF_VULNERABILTY
    1548, // LIGHT_WAND
    1614, // HEALING_WAND
    1650, // FROSTBLADE_SCROLL
    1724, // FIREBLADE_SCROLL
    1797, // CLOUT_SCROLL
    1866, // RENEW_SCROLL
    1890, // REMOVE_CURSE_SCROLL
    1921, // WIZARD_EYE_SCROLL
    2027, // RUBY_EYE
    2060, // EMERALD_EYE
    2096, // SAPPHIRE_EYE
    2133, // AMBER_EYE
    2167, // WIZARD'S_EYE
    2236, // HYPNOTIC_EYE
    2273, // TOME_OF_KNOWLEDGE
    2315, // TOME_OF_UNDERSTANDING
    2361, // TOME_OF_LEADERSHIP
    2404, // BRONZE_HORN
    2440, // SILVER_HORN
    2476, // GOLD_HORN
    2510, // GOLD_HORN
    2544, // POTION_OF_FLEETNESS
    2619, // POTION_OF_STRENGTH
    2693, // POTION_OF_INTELLIGENCE
    2771, // POTION_OF_CHARISMA
    2845, // POTION_OF_ENDURANCE
    2888, // POTION_OF_INV._BLUNT
    2964, // POTION_OF_INV._SHARP
    3040, // POTION_OF_INV._EARTH
    3116, // POTION_OF_INV._AIR
    3190, // POTION_OF_INV._FIRE
    3265, // POTION_OF_INV._WATER
    3341, // POTION_OF_REGENERATION
    3403, // POTION_OF_INV._MENTAL
    3480, // POTION_OF_INV._COLD
    3555, // POTION_OF_FRUIT_JUICE
    3600, // POTION_OF_WIZARD_EYE
    3708, // POTION_OF_DEXTERITY
    3783, // POTION_OF_INFRA-VISION
    3861, // POTION_OF_CLEANSING
    3889, // POTION_OF_ANTIDOTE
    3916, // POTION_OF_RESTORATION
    3961, // POTION_OF_HEALING
    4002, // POTION_OF_HEMLOCK
    4059, // POTION_OF_INEBRIATION
    4104, // CRYSTAL_SHIELD
    4177, // SHIELD_OF_GALAHAD
    4269, // SPIKED_SHIELD
    4309, // SHIELD_OF_MORDRED
    4401, // SPIRIT_SHIELD
    4473, // IRONWOOD_BOKEN
    4546, // IRON_FAN
    4581, // TOWER_SHIELD
    4620, // CROSSBOW_[03]
    4660, // QUARRELS_[10]
    4694, // CHAOS_CLUB
    4747, // SHORT_SWORD
    4785, // HOLY_HAND_GRENADE
    4859, // PIKE
    4890, // DIRK
    4921, // PANTHER_GLOVES
    4989, // HELM_OF_LIGHT
    5056, // DRAGONSKIN_HAUBERK
    5096, // GOLDEN_GREAVES
    5164, // PLATE_HELM
    5196, // PLATE_GAUNTLETS
    5233, // PLATE_LEGGINGS
    5269, // PLATE_ARMOR
    5302, // SCALE_ARMOR
    5335, // TRUESILVER_HELM
    5372, // TRUESILVER_COAT
    5409, // TRUESILVER_GUANTLETS
    5451, // TRUESILVER_LEGGINGS
    5492, // CUIRBOUILLI_HELM
    5530, // BRONZE_BREASTPLATE
    5570, // BRONZE_BRACERS
    5606, // WHITE_LINEN_SHIRT
    5633, // BLACK_SILK_KIMONO
    5660, // CHEAP_ROBE
    5680, // ELVEN_CLOAK
    5765, // ELVEN_BOOTS
    5818, // CRYSTAL_BELT
    5872, // BLUE_SUEDE_SHOES
    5898, // BLACK_WOOLEN_BREECHES
    5929, // SILVER_BROCADED_BODICE
    5961, // RED_PLAID_KILT
    5985, // GOLD_SILK_PANTALOONS
    6015, // LEATHER_JERKIN
    6039, // FLOPPY_LEATHER_HAT
    6067, // BLACK_COTTON_PARTLET
    6097, // SILVER_SASH
    6150, // STEALTH_SUIT
    6204, // SILVER_KEY
    6224, // GOBLIN_RING_HALF
    6250, // TROLL_RING_HALF
    6275, // STAFF_PIECE
    6296, // PAC_CARD
    6330, // MIRRORED_SHIELD
    6404, // REFORGED_RING
    6475, // BLOODSTONE
    6511, // WINGED_SANDALS
    6567, // MORGANA'S_TIARA
    6608, // CLOAK_OF_LEVITATION
    6669, // CRYSTAL_BREASTPLATE
    6710, // JUNAI'S_SWORD
    6766, // LOADSTONE
    6799, // IRON_PALM_SALVE
    6839, // SWORD_OF_THE_ADEPT
    6884, // RAZOR_ICE
    6920, // WHETSTONE
    6941, // SAURIAN_BRANDY
    6996, // BLUE_PEARL_DAGGER
    7040, // SIX_PACK_[6]
    7093, // MELVIN'S_HELM
    7128, // AMETHYST_ROD
    7182, // MAP_STONE
    7203, // FLAME_QUARRELS_[10]
    7243, // THUNDER_QUARRELS_[10]
    7285, // STAFF_OF_AMBER
    7326, // ROBIN'S_HOOD
    7380, // GOLDEN_APPLE
    7434, // GAUSS_RIFLE_[95]
    7477, // SOLAR_SUIT
    7541  // BEAM_WEAPON_[25]
}; // Dungeon items only


newItem* newItemArray = nullptr;

newItem* SelectedItemArray = nullptr;

std::vector<newItem> readItemCSV(const std::string& filename) {
	std::vector<newItem> data;

	// Open the CSV file
	std::ifstream file("data/map/"+filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return data; // Return empty vector if file couldn't be opened
	}

	std::string line;
	while (std::getline(file, line)) {
		std::stringstream lineStream(line);
		newItem newItem;
		std::string cell;

		// Parsing CSV fields into the struct members
		std::getline(lineStream, cell, ','); // Assuming the index is the first column
		newItem.index = std::stoi(cell);

		std::getline(lineStream, newItem.name, ',');

		std::getline(lineStream, cell, ',');
		newItem.itemType = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newItem.cost = std::stoi(cell);
		
		std::getline(lineStream, cell, ',');
		newItem.effect = std::stoi(cell);
		
		std::getline(lineStream, cell, ',');
		newItem.elementType = std::stoi(cell);
		
		std::getline(lineStream, cell, ',');
		newItem.statType = std::stoi(cell);
			
		std::getline(lineStream, cell, ',');
		newItem.negativeValue = std::stoi(cell);
		
		std::getline(lineStream, cell, ',');
		newItem.positiveValue = std::stoi(cell);
		
		std::getline(lineStream, cell, ',');
		newItem.duration = std::stoi(cell);
		
		std::getline(lineStream, cell, ',');
		newItem.damage = std::stoi(cell);
		
		std::getline(lineStream, cell, ',');
		newItem.hp = std::stoi(cell);
			
		std::getline(lineStream, cell, ',');
		newItem.maxHP = std::stoi(cell);
		
		std::getline(lineStream, cell, ',');
		newItem.flags = std::stoi(cell);
		
		std::getline(lineStream, cell, ',');
		newItem.minStrength = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newItem.minDexterity = std::stoi(cell);
		
		std::getline(lineStream, cell, ',');
		newItem.useStrength = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newItem.weight = std::stoi(cell);
		
		std::getline(lineStream, cell, ',');
		newItem.alignment = std::stoi(cell);
			
		std::getline(lineStream, cell, ',');
		newItem.melee = std::stoi(cell);
			
		std::getline(lineStream, cell, ',');
		newItem.ammo = std::stoi(cell);
		
		std::getline(lineStream, cell, ',');
		newItem.parry = std::stoi(cell);
		
		std::getline(lineStream, cell, ',');
		newItem.cat = std::stoi(cell);

		std::getline(lineStream, cell, ',');
		newItem.level = std::stoi(cell);

		data.push_back(newItem);
	}

	file.close();
	return data;

}




buffer_item tempBuffer[itemBufferSize]; // Temp buffer for rebuilding new object buffer when bufferIndex reaches 99


clothingItem clothingItems[30] =
{
	//							Qual	Col		Fab		Type	Weight  Effect
	{"Cheap Robe",				0,		0,		0,		0,		4,		0},
	{"Green Cap with Feather",	0,		0,		0,		1,		4,		0},
	{"Floppy Leather Hat",		0,		0,		0,		0,		4,		0},
	{"Leather Sandals",			0,		0,		0,		0,		4,		0},
	{"High Leather Boots",		0,		0,		0,		0,		4,		0},
	{"Showshoes",				0,		0,		0,		0,		4,		0},
	{"White Cotton Robe",		0,		0,		0,		0,		4,		0},
	{"White Cotton Tunic",		0,		0,		0,		0,		4,		0},
	{"Brown Cotton Breeches",	0,		0,		0,		0,		4,		0},
	{"Green Cotton Skirt",		0,		0,		0,		0,		4,		0},
	{"Purple Flowing Cape",		0,		0,		0,		0,		4,		0},
	{"Blue Woolen Sweater",		0,		0,		0,		0,		4,		0},
	{"Heavy Leather Jacket",	0,		0,		0,		0,		6,		0},

	{"Fine Yellow Wool Pants",	        283,		0,		0,		0,		4,		0},
	{"Simple Violet Jerkin",	        196,		0,		0,		0,		4,		0},
	{"Simple White Wool Robe",	        872,		0,		0,		0,		4,		0},
	{"Simple Red Jerkin",		        161,		0,		0,		0,		4,		0},
	{"Simple Purple Fur-Lined Toga",    457,		0,		0,		0,		4,		0},
	{"Simple Gold Jerkin",		        327,		0,		0,		0,		4,		0},
	{"Cheap Gray Silk Hat",		        248,		0,		0,		0,		4,		0},
	{"Cheap Gold Wool Toga",	        139,		0,		0,		0,		4,		0},
	{"Fine Silver Jerkin",	            357,		0,		0,		0,		4,		0},
	{"Simple Gray Cloak",		        170,		0,		0,		0,		4,		0},
	{"Simple White Fur-Lined Robe",	    4260,		0,		0,		0,		4,		0},
	{"Cheap Black Jerkin",		        221,		0,		0,		0,		4,		0},
	{"Fine Purple Fur-Lined Shirt",	    979,		0,		0,		0,		6,		0},
    {"Simple Purple Dragonskin Blouse",	2364,		0,		0,		0,		4,		0},
	{"Simple Orange Silk Vest",		    1107,		0,		0,		0,		4,		0},
	{"Cheap Gold Silk Skirt",	        924,		0,		0,		0,		6,		0},
	{"Morganans Tiara",					0,			0,		0,		0,		1,		0}
};


questItem questItems[8] =
{
	{"Troll Ring Half",			2 },
	{"Goblin Ring Half",		2 },
	{"Reforged Ring",			2 },
	{"Guild Ring",				2 },
	{"Map Stone",				2 },
	{"Amethyst Rod",			2 },
	{"Staff Piece",             2 },
	{"Helm of Light",			2 }
};


potionItem Potions[44] =
{
	{"Potion of Water",                     "clear",    "plain",    "safe"},
	{"Potion of Wine",                      "red",      "dry",      "caution"},
	{"Potion of Spirits",	                "amber",    "sour",     "caution"},
    {"Potion of Milk",                      "white",    "alkaline", "safe"},
	{"Potion of Fruit Juice",               "red",      "sweet",    "safe"},
	{"Potion of Mineral Water",	            "clear",    "dry",      "safe"},
    {"Potion of Saltwater",                 "clear",    "salty",    "caution"},
	{"Potion of Invisibility",              "clear",    "dry",      "safe"},
	{"Potion of Vinegar",	                "red",      "acidic",   "caution"},
    {"Potion of ACID!",                     "clear",    "acidic",   "dangerous"},
	{"Potion of Weak Poison",               "silver",   "bitter",   "dangerous"},
	{"Potion of Poison!",	                "white",    "alkaline", "dangerous"},
    {"Potion of Strong Poison!",            "black",    "sour",     "dangerous"},
	{"Potion of DEADLY POISON!",            "red",      "sweet",    "dangerous"},
	{"Potion of Heal Minor Wounds",	        "green",    "sour",     "safe"},
    {"Potion of Heal Wounds",               "yellow",   "plain",    "safe"},
	{"Potion of Heal Major Wounds",         "silver",   "plain",    "safe"},
	{"Potion of Heal All Wounds",	        "white",    "salty",    "safe"},
    {"Potion of Curing Poison",             "black",    "bitter",   "caution"},
	{"Potion of Cleansing",                 "black",    "bitter",   "caution"},
	{"Potion of Delusion",	                "black",    "bitter",   "caution"},
    {"Potion of Invulnerability Blunt",     "black",    "sweet",    "safe"},
	{"Potion of Invulnerability Sharp",     "black",    "plain",    "safe"},
	{"Potion of Invulnerability Earth",	    "black",    "sour",     "safe"},
    {"Potion of Invulnerability Air",       "black",    "salty",    "safe"},
	{"Potion of Invulnerability Fire",      "black",    "acidic",   "safe"},
	{"Potion of Invulnerability Water",	    "black",    "alkaline", "safe"},
    {"Potion of Invulnerability Power",     "black",    "dry",      "safe"},
	{"Potion of Invulnerability Mental",    "black",    "plain",    "safe"},
	{"Potion of Invulnerability Cleric",	"black",    "sweet",    "safe"},
    {"Potion of Noticeability",             "yellow",   "bitter",   "dangerous"},
	{"Potion of Inebriation",               "orange",   "plain",    "caution"},
	{"Potion of Strength",	                "red",      "bitter",   "safe"},
    {"Potion of Intelligence",              "silver",   "bitter",   "safe"},
	{"Potion of Charisma",                  "silver",   "sweet",    "safe"},
	{"Potion of Ugliness",	                "green",    "sweet",    "dangerous"},
    {"Potion of Weakness",                  "yellow",   "dry",      "dangerous"},
	{"Potion of Dumbness",                  "orange",   "sweet",    "dangerous"},
	{"Potion of Fleetness",	                "black",    "plain",    "safe"},
    {"Potion of Slowness",                  "white",    "bitter",   "dangerous"},
	{"Potion of Protection +1",             "orange",   "sweet",    "safe"},
	{"Potion of Protection +2",	            "orange",   "sour",     "safe"},
    {"Potion of TREASURE FINDING",          "red",      "sweet",    "safe"},
	{"Potion of Unnoticeability",	        "clear",    "bitter",   "safe"},
};




void readItemData()
{
	std::string filename = "items.csv";
	std::vector<newItem>  csvData = readItemCSV(filename);

	// Convert vector to a dynamically allocated array of NewItem structs
	size_t itemCount = csvData.size();
	newItemArray = new newItem[itemCount];

	// Copy data from vector to the dynamically allocated array
	for (size_t i = 0; i < itemCount; ++i) {
		newItemArray[i] = csvData[i];
		plyr.items_index = i;
	}

	std::map<std::pair<int, int>, int> itemCounts = countItemTypes(newItemArray, plyr.items_index);
}

int createItem(
        int type,
        int index,
        string name,
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
{
    // Create a new item in itemBuffer[]

    // 0 - limbo, 1 - floor, 10 - carried, 11 - primary, 12 - secondary,
    // 13 - worn, 14 - head, 15 - arms, 16 - legs, 17 - body

    // Clean up itemBuffer[] before adding a new item
    tidyObjectBuffer();

    // Create a new item
    buffer_item new_item;

    // Set item attributes
    new_item.type           = type;
    new_item.index          = index;
    new_item.name           = name;
    new_item.hp             = hp;
    new_item.maxHP          = maxHP;
    new_item.flags          = flags;
	new_item.minStrength    = minStrength;
	new_item.minDexterity   = minDexterity;
	new_item.useStrength    = useStrength;
	new_item.blunt          = blunt;
	new_item.sharp          = sharp;
	new_item.earth          = earth;
	new_item.air            = air;
	new_item.fire           = fire;
	new_item.water          = water;
	new_item.power          = power;
	new_item.magic          = magic;
	new_item.good           = good;
	new_item.evil           = evil;
	new_item.cold           = cold;
	new_item.nature			= nature;
	new_item.acid			= acid;
	new_item.weight         = weight;
	new_item.alignment      = alignment;
	new_item.melee          = melee;
	new_item.ammo           = ammo;
	new_item.parry          = parry;
	new_item.effect         = effect;
    // Set location attributes
    new_item.location       = 1;                          // the floor
    new_item.x              = plyr.x;
    new_item.y              = plyr.y;
    new_item.level          = plyr.map;

    // Update buffer and buffer references
    itemBuffer[plyr.buffer_index] = new_item;
    int new_item_ref = plyr.buffer_index;
    plyr.buffer_index++;
    return new_item_ref; // what was the new items index in the object buffer
}


int newcreateItem(
	int type,
	int index,
	string name,
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
{
	// Create a new item in itemBuffer[]

	// 0 - limbo, 1 - floor, 10 - carried, 11 - primary, 12 - secondary,
	// 13 - worn, 14 - head, 15 - arms, 16 - legs, 17 - body

	// Clean up itemBuffer[] before adding a new item
	tidyObjectBuffer();

	// Create a new item
	buffer_item new_item;

	// Set item attributes
	new_item.type = type;
	new_item.index = index;
	new_item.name = name;
	new_item.hp = hp;
	new_item.maxHP = maxHP;
	new_item.flags = flags;
	new_item.minStrength = minStrength;
	new_item.minDexterity = minDexterity;
	new_item.useStrength = useStrength;
	new_item.blunt = blunt;
	new_item.sharp = sharp;
	new_item.earth = earth;
	new_item.air = air;
	new_item.fire = fire;
	new_item.water = water;
	new_item.power = power;
	new_item.magic = magic;
	new_item.good = good;
	new_item.evil = evil;
	new_item.cold = cold;
	new_item.nature = nature;
	new_item.acid = acid;
	new_item.weight = weight;
	new_item.alignment = alignment;
	new_item.melee = melee;
	new_item.ammo = ammo;
	new_item.parry = parry;
	new_item.effect = effect;
	// Set location attributes
	new_item.location = 1;                          // the floor
	new_item.x = plyr.x;
	new_item.y = plyr.y;
	new_item.level = plyr.map;

	// Update buffer and buffer references
	itemBuffer[plyr.buffer_index] = new_item;
	int new_item_ref = plyr.buffer_index;
	plyr.buffer_index++;
	return new_item_ref; // what was the new items index in the object buffer
}

void createBareHands()
{
    int itemRef = createItem(178,0,"bare hand",255,255,6,0,0,1,0x15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0);
    itemBuffer[itemRef].location = 99; // body part of player - 99 so it doesn't show up in the inventory
    //plyr.priWeapon = itemRef;
    //plyr.secWeapon = itemRef;
}


void moveItem(int itemRef,int newLocation)
{
	itemBuffer[itemRef].location = newLocation;
	if (plyr.priWeapon == itemRef)
	{
		plyr.priWeapon = 255;
		removeItemBuff(itemRef);
	}
	if (plyr.secWeapon==itemRef) plyr.secWeapon = 255;
	if (plyr.armsArmour==itemRef) plyr.armsArmour = 255;
	if (plyr.legsArmour==itemRef) plyr.legsArmour = 255;
	if (plyr.headArmour==itemRef) plyr.headArmour = 255;
	if (plyr.bodyArmour==itemRef) plyr.bodyArmour = 255;
	if (plyr.clothing[0]==itemRef) plyr.clothing[0] = 255;
	if (plyr.clothing[1]==itemRef) plyr.clothing[1] = 255;
	if (plyr.clothing[2]==itemRef) plyr.clothing[2] = 255;
	if (plyr.clothing[3]==itemRef) plyr.clothing[3] = 255;
}


string getItemDesc(int itemRef)
{
	string itemDesc = "ERROR";
	if (itemBuffer[itemRef].type==176) itemDesc = "Potion";
	if (itemBuffer[itemRef].type==177) itemDesc = itemBuffer[itemRef].name;
	if (itemBuffer[itemRef].type==178) itemDesc = itemBuffer[itemRef].name;
	if (itemBuffer[itemRef].type==180) itemDesc = itemBuffer[itemRef].name;
	if (itemBuffer[itemRef].type==199) itemDesc = itemBuffer[itemRef].name;
	if (itemBuffer[itemRef].type==200) itemDesc = questItems[(itemBuffer[itemRef].index)].name;
	if (itemBuffer[itemRef].type==201) itemDesc = questItems[(itemBuffer[itemRef].index)].name;
	return itemDesc;
}


int createPotion(int potion_no)
{
     // location options:
     // 0 - limbo, 1 - floor, 10 - carried, 11 - primary, 12 - secondary,
    // 13 - worn, 14 - head, 15 - arms, 16 - legs, 17 - body
     tidyObjectBuffer();
	 buffer_item new_item;
     new_item.type = 176; // object type to indicate potion
     new_item.index = potion_no; // Index will define which of 42 potion types this is
     new_item.location = 1; // On floor after encounter
     new_item.x = plyr.x;
     new_item.y = plyr.y;
     new_item.level = plyr.map;
     new_item.hp = 0; // For potions 0 indicates unidentified
	 itemBuffer[plyr.buffer_index] = new_item;
     int new_item_ref = plyr.buffer_index;
	 plyr.buffer_index++;
     return new_item_ref; // what was the new items index in the object buffer
}


int createArmor(int armor_no)
{
     // location options:
     // 0 - limbo, 1 - floor, 10 - carried, 11 - primary, 12 - secondary,
     // 13 - worn, 14 - head, 15 - arms, 16 - legs, 17 - body
     tidyObjectBuffer();
	 buffer_item new_item;
     new_item.type = 177; // temporary object type to indicate armor
     new_item.index = armor_no;
     new_item.location = 10; // carried in inventory but not in use
     new_item.x = plyr.x;
     new_item.y = plyr.y;
     new_item.level = plyr.map;
     new_item.hp = 12; // temp value to act as breakable value
	 itemBuffer[plyr.buffer_index] = new_item;
     int new_item_ref = plyr.buffer_index;
	 plyr.buffer_index++;
     return new_item_ref; // what was the new items index in the object buffer
}


int createClothing(int clothing_no, int itemlocation)
{
	 tidyObjectBuffer();
	 buffer_item new_item;
	 new_item.name = clothingItems[clothing_no].name;
     new_item.type = 180; // clothing type
     new_item.index = clothing_no;
     new_item.location = itemlocation; // carried in inventory but not in use
     new_item.x = plyr.x;
     new_item.y = plyr.y;
     new_item.level = plyr.map;
     new_item.hp = 12; // temp value to act as breakable value
	 new_item.weight = clothingItems[clothing_no].weight;
	 new_item.effect = clothingItems[clothing_no].effect;

	 itemBuffer[plyr.buffer_index] = new_item;
     int new_item_ref = plyr.buffer_index;
	 plyr.buffer_index++;
     return new_item_ref; // what was the new items index in the object buffer
}


int createQuestItem(int questItemNo)
{
     // location options:
     // 0 - limbo, 1 - floor, 10 - carried, 11 - primary, 12 - secondary,
    // 13 - worn, 14 - head, 15 - arms, 16 - legs, 17 - body
     //int weapon_no = encounters[monster_no].weapon_no;
    tidyObjectBuffer();
	buffer_item new_item;
	 new_item.type = 200; // type for quest items e.g ring halves, silver key etc
     new_item.index = questItemNo; // for quest items index is used to identify the object
     new_item.location = 1; // the floor
     new_item.x = plyr.x;
     new_item.y = plyr.y;
     new_item.level = plyr.map;
	 
	 itemBuffer[plyr.buffer_index] = new_item;
     int new_item_ref = plyr.buffer_index;
	 plyr.buffer_index++;
     return new_item_ref; // what was the new items index in the object buffer
}


int createGeneralItem(int ItemNo)
{
	
	tidyObjectBuffer();
	buffer_item new_item;

	new_item.type = newItemArray[ItemNo].itemType; // type for 
	new_item.index = newItemArray[ItemNo].index; // for 
	new_item.location = 1; // the floor
	new_item.name = newItemArray[ItemNo].name;
	new_item.x = plyr.x;
	new_item.y = plyr.y;
	new_item.level = plyr.map;
	new_item.melee = newItemArray[ItemNo].melee;
	new_item.effect = newItemArray[ItemNo].effect;

	itemBuffer[plyr.buffer_index] = new_item;
	int new_item_ref = plyr.buffer_index;
	plyr.buffer_index++;
	return new_item_ref; // what was the new items index in the object buffer
} 

void getItems()
{
  // types = 1 - food, 2 - water, 3 - torches, 4 - timepieces, 5 - compasses
  int cur_idx = 0;
  bool noGetQuit = true;
  while ((cur_idx <= plyr.buffer_index) && (noGetQuit))
  {
        if ((itemBuffer[cur_idx].x == plyr.x) && (itemBuffer[cur_idx].y == plyr.y)&& (itemBuffer[cur_idx].level == plyr.map) && (itemBuffer[cur_idx].location == 1))
        {

		string str, key_value;
		bool keypressed = false;

        while (!keypressed)
		{
			dispMain();
			drawConsoleBackground();
			cyText(1, "GET?");
			if (itemBuffer[cur_idx].type == 1) {str = itos(itemBuffer[cur_idx].hp) + " Food Packet(s)"; }
			if (itemBuffer[cur_idx].type == 2) {str = itos(itemBuffer[cur_idx].hp) + " Water Flask(s)"; }
			if (itemBuffer[cur_idx].type == 3) {str = itos(itemBuffer[cur_idx].hp) + " Torch(es)"; }
			if (itemBuffer[cur_idx].type == 4) {str = itos(itemBuffer[cur_idx].hp) + " Timepiece(s)"; }
			if (itemBuffer[cur_idx].type == 5) {str = itos(itemBuffer[cur_idx].hp) + " Compass(es)"; }
			if (itemBuffer[cur_idx].type == 6) {str = itos(itemBuffer[cur_idx].hp) + " Key(s)"; }
			if (itemBuffer[cur_idx].type == 7) {str = itos(itemBuffer[cur_idx].hp) + " Crystal(s)"; }
			if (itemBuffer[cur_idx].type == 8) {str = itos(itemBuffer[cur_idx].hp) + " Gem(s)"; }
			if (itemBuffer[cur_idx].type == 9) {str = itos(itemBuffer[cur_idx].hp) + " Jewel(s)"; }
			if (itemBuffer[cur_idx].type == 10) {str = itos(itemBuffer[cur_idx].hp) + " Gold"; }
			if (itemBuffer[cur_idx].type == 11) {str = itos(itemBuffer[cur_idx].hp) + " Silver"; }
			if (itemBuffer[cur_idx].type == 12) {str = itos(itemBuffer[cur_idx].hp) + " Copper"; }
			if ((itemBuffer[cur_idx].type == 176) && (itemBuffer[cur_idx].hp == 0)) { str = "Potion"; }
			if ((itemBuffer[cur_idx].type == 176) && (itemBuffer[cur_idx].hp == 1)) { str = Potions[(itemBuffer[cur_idx].index)].name; }
			if (itemBuffer[cur_idx].type == 177) { str = itemBuffer[cur_idx].name; }
			if (itemBuffer[cur_idx].type == 178) { str = itemBuffer[cur_idx].name; }
			if (itemBuffer[cur_idx].type == 199) { str = itemBuffer[cur_idx].name; }
			if (itemBuffer[cur_idx].type == 180) { str = itemBuffer[cur_idx].name; }
			if (itemBuffer[cur_idx].type == 200) { str = questItems[(itemBuffer[cur_idx].index)].name; }
			cyText (4,str);
			cyText (7,"Yes, No or ESC.");
			updateDisplay();

			key_value = getSingleKey();

			if (key_value == "Y")
			{
				string encText = checkEncumbrance();
				if (encText=="Immobilized!") { cannotCarryMessage(); }
				else
				{
					if (itemBuffer[cur_idx].type < 13)  // get food packets
					{
						int type = itemBuffer[cur_idx].type;
						if (type==1) plyr.food+=(itemBuffer[cur_idx].hp);
						if (type==2) plyr.water+=(itemBuffer[cur_idx].hp);
						if (type==3) plyr.torches+=(itemBuffer[cur_idx].hp);
						if (type==4) plyr.timepieces+=(itemBuffer[cur_idx].hp);
						if (type==5) plyr.compasses+=(itemBuffer[cur_idx].hp);
						if (type==6) plyr.keys+=(itemBuffer[cur_idx].hp);
						if (type==7) plyr.crystals+=(itemBuffer[cur_idx].hp);
						if (type==8) plyr.gems+=(itemBuffer[cur_idx].hp);
						if (type==9) plyr.jewels+=(itemBuffer[cur_idx].hp);
						if (type==10) plyr.gold+=(itemBuffer[cur_idx].hp);
						if (type==11) plyr.silver+=(itemBuffer[cur_idx].hp);
						if (type==12) plyr.copper+=(itemBuffer[cur_idx].hp);
						itemBuffer[cur_idx].location = 0; // remove this item to the void
					}

					if (itemBuffer[cur_idx].type > 150)
					{
						itemBuffer[cur_idx].location = 10; // moved to player inventory
					}
				}
				keypressed=true;
			}


			if (key_value == "N")
			{
				keypressed=true;
			}

			if (key_value == "ESC") { keypressed=true; noGetQuit = false; }
        }

            //int weapon_idx, weapon_type;
            //weapon_idx = itemBuffer[cur_idx].index;
            //weapon_type = itemBuffer[cur_idx].type;
            //str = "GET?@@ " + weapons[weapon_idx].name + "@@Yes, No or ESC";
            //newtext(str);

        }
        cur_idx++;
  }
}


void displayObjectBuffer()
{

		string str, key_value;
		bool keypressed = false;
		int oldStatus = plyr.status;
		plyr.status = 255; // Diag screen being displayed

        while (!keypressed)
		{
			clearDisplay();

			text(0,2, "No");
			text(4,2, "Typ ");
			text(8,2, "Loc");
			text(12,2, "X");
			text(15,2, "Y");
			text(18,2, "L");
			text(20,2, "Item");

			str = "Buffer Index "+itos(plyr.buffer_index)+" of " +itos(itemBufferSize);
			text(0,0, str);

			int cur_idx = 0;
			while (cur_idx < plyr.buffer_index)
			{
				text(0,(cur_idx+3), cur_idx);
				text(4,(cur_idx+3), itemBuffer[cur_idx].type);
				text(8,(cur_idx+3), itemBuffer[cur_idx].location);
				text(12,(cur_idx+3), itemBuffer[cur_idx].x);
				text(15,(cur_idx+3), itemBuffer[cur_idx].y);
				text(18,(cur_idx+3), itemBuffer[cur_idx].level);
				if (itemBuffer[cur_idx].type<20) { str = "Volume item"; }
				//if (itemBuffer[cur_idx].type==178) { str = monsterWeapons[(itemBuffer[cur_idx].index)].name; }
				if (itemBuffer[cur_idx].type==178) { str = itemBuffer[cur_idx].name; }
				if (itemBuffer[cur_idx].type==177) { str = itemBuffer[cur_idx].name; }
                if ((itemBuffer[cur_idx].type == 176) && (itemBuffer[cur_idx].hp == 0)) { str = "Potion"; }
                if ((itemBuffer[cur_idx].type == 176) && (itemBuffer[cur_idx].hp == 1)) { str = Potions[(itemBuffer[cur_idx].index)].name; }
				if (itemBuffer[cur_idx].type==180) { str = itemBuffer[cur_idx].name; }
				if (itemBuffer[cur_idx].type==199) { str = itemBuffer[cur_idx].name; }
				if (itemBuffer[cur_idx].type==200) { str = questItems[(itemBuffer[cur_idx].index)].name; }
				if (itemBuffer[cur_idx].type==201)
				{
					str = questItems[(itemBuffer[cur_idx].index)].name;
					//str = (questItems[(itemBuffer[cur_idx].index)].name) + " [" + itos(plyr.ringCharges) + "]";
				}
				text(20,(cur_idx+3),str);

				cur_idx++;
			}
			text(0,(cur_idx+4),"(Press SPACE to continue)");
			updateDisplay();

			key_value = getSingleKey();

			if ((key_value == "SPACE") || (key_value == "B")) { keypressed=true; }
        }
		plyr.status = oldStatus;

}


int selectItem(int selectItemMode)
{
	// 1 - USE, 2 - DROP, 3 - OFFER, 4 - Deposit
	// item types : 1-weapon 177-armour
	int itemRef = 9999; // Nothing selected
	std::string str, selectDesc;
	if (selectItemMode == 1) selectDesc = "USE";
	if (selectItemMode == 2) selectDesc = "DROP";
	if (selectItemMode == 3) selectDesc = "OFFER";
	if (selectItemMode == 4) selectDesc = "Deposit";
	if (selectItemMode == 5) selectDesc = "Withdrawal";

    int menuitem1 = 255; // 255 is used here as nil
    int menuitem2 = 255;
    int menuitem3 = 255;
    int menuitem4 = 255;
    bool selectDone = false;

    int no_items = 0;
    int cur_idx = 0;
    int pages = 0;
    int page = 0;
    int weapon = 0;
	int armor = 0;
	int clothing = 0;
	//int itemCount;
	//int pageStartItem;
    int page_item = 0;

    while (cur_idx < plyr.buffer_index)
    {
        if (itemBuffer[cur_idx].location == 10) { no_items++; }
        cur_idx++;
    }
	pages = 0;
	int noPages = no_items / 4; // based on 4 oncreen items per page
	pages += noPages;
	int tempRemainder = no_items % 4;
	if (tempRemainder != 0) { pages++; }

    while ( !selectDone )
    {
        if (page == 0)
        {
            // this is effectively page 0 in terms of using items
			if (plyr.status == 2) clearGuildDisplay();
			if (plyr.status == 3) drawEncounterView();
			if (plyr.status == 1) { dispMain(); drawConsoleBackground(); }
			if (plyr.status == 0) { dispMain(); drawConsoleBackground(); }

			//if (plyr.status == 3 ) { drawEncounterView(); }
			//else { dispMain(); }

			cyText(1, selectDesc);
			str = "(1) Food Packets: "+itos(plyr.food);
			if (selectItemMode == 5) str = "(1) Food Packets: "+itos(plyr.lfood);
            bText (5, 3,str);

			str = "(2) Water Flasks: "+itos(plyr.water);
			if (selectItemMode == 5) str = "(2) Water Flasks: "+itos(plyr.lwater);
            bText (5, 4, str);

            str = "(3) Unlit Torches: " + itos(plyr.torches);
			if (selectItemMode == 5) str = "(3) Unlit Torches: " + itos(plyr.ltorches);
			bText (5, 5, str);

			str = "(4) Timepieces: " + itos(plyr.timepieces);
			if (selectItemMode == 5) str = "(4) Timepieces: " + itos(plyr.ltimepieces);
            bText (5, 6, str);

            bText (2, 8,"Item #, Forward, Back, or ESC to exit");
			SetFontColour(40, 96, 244, 255);
			bText (2, 8,"     #  F        B        ESC");
			SetFontColour(215, 215, 215, 255);
            //str = "Page: " + itos(page) + " of " + itos(pages);
	        //bText(28, 1, str);
			updateDisplay();

			std::string key_value;
            key_value = getSingleKey();
            if (key_value == "1") { itemRef=1000; selectDone = true; }
            if (key_value == "2") { itemRef=1001; selectDone = true; }
            if (key_value == "3") { itemRef=1002; selectDone = true; }
            if (key_value == "4") { itemRef=1003; selectDone = true; }
            if (key_value == "E") { selectDone = true; }
			if (key_value == "ESC") { selectDone = true;  }
            if ( (key_value == "F") || (key_value == "down"))
			{
				if ((selectItemMode == 1) && ( pages > 0 )) page = 3;
				if ((selectItemMode == 2) || (selectItemMode == 3) || (selectItemMode == 4) || (selectItemMode == 5)) page = 1;
			}

        }

		if (page == 1)
        {
            // this is effectively page 1 in terms of using items
			if (plyr.status == 2) clearGuildDisplay();
			if (plyr.status == 3) drawEncounterView();
			if (plyr.status == 1) { dispMain(); drawConsoleBackground(); }
			if (plyr.status == 0) { dispMain(); drawConsoleBackground(); }

			cyText(1, selectDesc);
			str = "(1) Compasses: "+itos(plyr.compasses);
			if (selectItemMode == 5) str = "(1) Compasses: "+itos(plyr.lcompasses);
            bText (5, 3,str);
			str = "(2) Keys: "+itos(plyr.keys);
			if (selectItemMode == 5) str = "(2) Keys: "+itos(plyr.lkeys);
            bText (5, 4, str);
			str = "(3) Crystals: " + itos(plyr.crystals);
			if (selectItemMode == 5) str = "(3) Crystals: " + itos(plyr.lcrystals);
			bText (5, 5, str);
			str = "(4) Gems: " + itos(plyr.gems);
			if (selectItemMode == 5) str = "(4) Gems: " + itos(plyr.lgems);
            bText (5, 6, str);
            bText (2, 8,"Item #, Forward, Back, or ESC to exit");
			SetFontColour(40, 96, 244, 255);
			bText (2, 8,"     #  F        B        ESC");
			SetFontColour(215, 215, 215, 255);
            //str = "Page: " + itos(page) + " of " + itos(pages);
	        //bText(28, 1, str);
			updateDisplay();

			std::string key_value;
            key_value = getSingleKey();
            if (key_value == "1") { itemRef=1004; selectDone = true; }
            if (key_value == "2") { itemRef=1005; selectDone = true; }
            if (key_value == "3") { itemRef=1006; selectDone = true; }
            if (key_value == "4") { itemRef=1007; selectDone = true; }
            if (key_value == "E") { selectDone = true; }
			if (key_value == "ESC") { selectDone = true;  }
            if (key_value == "F") page = 2;
			if (key_value == "down") page = 2;
	        if (key_value == "B") page = 0;
			if (key_value == "up") page = 0;
        }

		if (page == 2)
        {
            // this is effectively page 2 in terms of using items
			if (plyr.status == 2) clearGuildDisplay();
			if (plyr.status == 3) drawEncounterView();
			if (plyr.status == 1) { dispMain(); drawConsoleBackground(); }
			if (plyr.status == 0) { dispMain(); drawConsoleBackground(); }

			cyText(1, selectDesc);
			str = "(1) Jewels: "+itos(plyr.jewels);
			if (selectItemMode == 5) str = "(1) Jewels: "+itos(plyr.ljewels);
            bText (5, 3,str);
			str = "(2) Gold: "+itos(plyr.gold);
			if (selectItemMode == 5) str = "(2) Gold: "+itos(plyr.lgold);
            bText (5, 4, str);
			str = "(3) Silver: " + itos(plyr.silver);
			if (selectItemMode == 5) str = "(3) Silver: " + itos(plyr.lsilver);
			bText (5, 5, str);
			str = "(4) Copper: " + itos(plyr.copper);
			if (selectItemMode == 5) str = "(4) Copper: " + itos(plyr.lcopper);
            bText (5, 6, str);
            bText (2, 8,"Item #, Forward, Back, or ESC to exit");
			SetFontColour(40, 96, 244, 255);
			bText (2, 8,"     #  F        B        ESC");
			SetFontColour(215, 215, 215, 255);
            //str = "Page: " + itos(page) + " of " + itos(pages);
	        //bText(28, 1, str);
			updateDisplay();

			std::string key_value;
            key_value = getSingleKey();
            if (key_value == "1") { itemRef=1008; selectDone = true; }
            if (key_value == "2") { itemRef=1009; selectDone = true; }
            if (key_value == "3") { itemRef=1010; selectDone = true; }
            if (key_value == "4") { itemRef=1011; selectDone = true; }
            if (key_value == "E") { selectDone = true; }
			if (key_value == "ESC") { selectDone = true;  }
            if ((key_value == "F") && ( pages > 0 ) && (selectItemMode<4)) page = 3;
			if ((key_value == "down") && ( pages > 0 ) && (selectItemMode<4)) page = 3;
			if (key_value == "B") page = 1;
			if (key_value == "up") page = 1;
        }

        if (page > 2) // Variable items
		{

			bool keypressed = false;
			while (!keypressed)
			{

				//itemCount = 0;
				if (plyr.status == 2) clearGuildDisplay();
				if (plyr.status == 3) drawEncounterView();
				if (plyr.status == 1) { dispMain(); drawConsoleBackground(); }
				if (plyr.status == 0) { dispMain(); drawConsoleBackground(); }
				cyText(1, selectDesc);
				bText (5, 3, "(1)");
				bText (5, 4, "(2)");
				bText (5, 5, "(3)");
				bText (5, 6, "(4)");
				bText (2, 8,"Item #, Forward, Back, or ESC to exit");
				SetFontColour(40, 96, 244, 255);
				bText (2, 8,"     #  F        B        ESC");
				SetFontColour(215, 215, 215, 255);
				//str = "Page: " + itos(page-2) + " of " + itos(pages);
	            //bText(28, 1, str);



				// Identify starting value for cur_idx for page x of carried objects excluding items not carried
				int mypage = page-3; // should be 0 for page 0 (first page with items displayed on it)
				cur_idx = 0;
				if (mypage > 0)
				{
					int idx = 0;

					int tempPage = 0;
					int pageItems = 0;
					while ((mypage > tempPage) && (idx<=plyr.buffer_index))
					{
						if (itemBuffer[idx].location == 10)
						{
							cur_idx++;
							pageItems++;
							if (pageItems==4) { tempPage++; pageItems = 0; }
						}
						if (itemBuffer[idx].location != 10)
						{
							cur_idx++;
						}
						idx++;
					}
				}


				//cur_idx = ((page-3)*4); // Problem?

				page_item = 1;
				//pageStartItem = ((pages-1)*4)+1;
				menuitem1 = 9999; // 9999 is used as nil
				menuitem2 = 9999;
				menuitem3 = 9999;
				menuitem4 = 9999;

				while ((cur_idx < plyr.buffer_index) && (page_item<5))
				{
					if ((itemBuffer[cur_idx].location == 10) )
					{

                        // Display a Potion item
						if ( itemBuffer[cur_idx].type==176) // armour
						{
                            int potion = itemBuffer[cur_idx].index;
                            if (itemBuffer[cur_idx].hp == 0) { str = "Potion"; }
                            if (itemBuffer[cur_idx].hp == 1) { str = Potions[potion].name; }

						  //cout << cur_idx << " " << str << " " <<  Armor[armor].bodyLocation << endl;
						}

						// Display an armour, weapon or clothing item name
						if (itemBuffer[cur_idx].type==175) { str = itemBuffer[cur_idx].name; }
						if ( itemBuffer[cur_idx].type==177) { str = itemBuffer[cur_idx].name; }
						if ( itemBuffer[cur_idx].type==178) { str = itemBuffer[cur_idx].name; }
						if ( itemBuffer[cur_idx].type==180) { str = itemBuffer[cur_idx].name; }
						if ( itemBuffer[cur_idx].type==199) { str = itemBuffer[cur_idx].name; }

						// Display quest items in inventory
						if (itemBuffer[cur_idx].type==200)
						{
							str = questItems[(itemBuffer[cur_idx].index)].name;
						}

						// Display guild ring name and charges
						if (itemBuffer[cur_idx].type==201)
						{
							//str = questItems[(itemBuffer[cur_idx].index)].name;
							str = (questItems[(itemBuffer[cur_idx].index)].name) + " [" + itos(plyr.ringCharges) + "]";
						}

						// Indicate items worn or in use as primary or secondary weapons
						if ( cur_idx == plyr.priWeapon) { bText (4,(page_item+2), "*"); } // pri
						if ( cur_idx == plyr.secWeapon) { bText (4,(page_item+2), "*"); } // sec
						if ( cur_idx == plyr.headArmour) { bText (4,(page_item+2), "*"); } // head
						if ( cur_idx == plyr.bodyArmour) { bText (4,(page_item+2), "*"); } // body
						if ( cur_idx == plyr.armsArmour) { bText (4,(page_item+2), "*"); } // arms
						if ( cur_idx == plyr.legsArmour) { bText (4,(page_item+2), "*"); } // legs
						if ( cur_idx == plyr.clothing[0]) { bText (4,(page_item+2), "*"); } // clothing
						if ( cur_idx == plyr.clothing[1]) { bText (4,(page_item+2), "*"); } // clothing
						if ( cur_idx == plyr.clothing[2]) { bText (4,(page_item+2), "*"); } // clothing
						if ( cur_idx == plyr.clothing[3]) { bText (4,(page_item+2), "*"); } // clothing



						bText (9, (page_item+2), str);
						  switch (page_item)
						  {
								 case 1:
									  menuitem1 = cur_idx;
									  break;
								 case 2:
									  menuitem2 = cur_idx;
									  break;
								 case 3:
									  menuitem3 = cur_idx;
									  break;
								 case 4:
									  menuitem4 = cur_idx;
									  break;
						  }
					page_item++;
					}
					cur_idx++;
				}
				updateDisplay();

				string key_value;
				key_value = getSingleKey();
				if ((key_value == "1") && (menuitem1!=9999))
				{
					itemRef = menuitem1;
					keypressed=true;
					selectDone = true;
				}
				if ((key_value == "2") && (menuitem2!=9999))
				{
					itemRef = menuitem2;
					keypressed=true;
					selectDone = true;
				}
				if ((key_value == "3") && (menuitem3!=9999))
				{
					itemRef = menuitem3;
					keypressed=true;
					selectDone = true;
				}
				if ((key_value == "4") && (menuitem4!=9999))
				{
					itemRef = menuitem4;
					keypressed=true;
					selectDone = true;
				}

				if (key_value == "ESC") { keypressed=true; selectDone = true; }
				if ((key_value == "B") && (selectItemMode == 1) && (page==3)) { keypressed=true; page = 0; }
				if ((key_value == "B") && (selectItemMode == 2) && (page==3)) { keypressed=true; page = 2; }
				if ((key_value == "B") && (selectItemMode == 3) && (page==3)) { keypressed=true; page = 2; }
				if ((key_value == "B") && (page>3)) { keypressed=true; page--; }
				if ((key_value == "up") && (page>3)) { keypressed=true; page--; }
				if ((key_value == "F") && (pages>(page-2))) { keypressed=true; page++;} // check required
				if ((key_value == "down") && (pages>(page-2))) { keypressed=true; page++;} // check required
			}
        } // page > 0 loop


    } // while use not done

// END - SELECT DONE LOOP

    //BUG
	//if ((itemBuffer[itemRef].type==200) && (itemBuffer[itemRef].index==5)) { return 666; } // Amethyst Rod only
	if ((itemRef != 9999) && (selectItemMode!=3)) determineItemAction(selectItemMode, itemRef); // Pass on mode and index for buffer items only if something selected
	if ((itemRef != 9999) && (selectItemMode==3)) { return itemRef; }
	if ((itemRef == 9999) && (selectItemMode==3)) { return 9999; } // no selection of an object

}


void determineItemAction(int selectItemMode, int itemRef)
{
	if (selectItemMode == 1)  // Use
	{
		if (itemRef < 1000) useObject(itemRef);
		if (itemRef == 1000) use_food();
		if (itemRef == 1001) use_water();
		if (itemRef == 1002) use_torch();
		if (itemRef == 1003) use_timepiece();
	}

	if (selectItemMode == 2) // Drop
	{
		if (itemRef < 1000) dropObject(itemRef);
		if (itemRef > 999) dropVolumeObject(selectItemMode,itemRef);
	}

	if (selectItemMode == 4) depositObject(itemRef);
	if (selectItemMode == 5) withdrawalObject(itemRef);
}


void withdrawalObject(int itemRef)
{
	int itemQuantity = inputWithdrawalQuantity(itemRef);
	if (itemQuantity > 0)
	{
		if ((itemRef==1000) && (plyr.lfood>0))
		{
			if (itemQuantity > plyr.lfood) { itemQuantity = plyr.lfood; }
			plyr.food+=itemQuantity;
			plyr.lfood-=itemQuantity;
		}
		if ((itemRef==1001) && (plyr.lwater>0))
		{
			if (itemQuantity > plyr.lwater) { itemQuantity = plyr.lwater; }
			plyr.water+=itemQuantity;
			plyr.lwater-=itemQuantity;
		}
		if ((itemRef==1002) && (plyr.ltorches>0))
		{
			if (itemQuantity > plyr.ltorches) { itemQuantity = plyr.ltorches; }
			plyr.torches+=itemQuantity;
			plyr.ltorches-=itemQuantity;
		}
		if ((itemRef==1003) && (plyr.ltimepieces>0))
		{
			if (itemQuantity > plyr.ltimepieces) { itemQuantity = plyr.ltimepieces; }
			plyr.timepieces+=itemQuantity;
			plyr.ltimepieces-=itemQuantity;
		}
		if ((itemRef==1004) && (plyr.lcompasses>0))
		{
			if (itemQuantity > plyr.lcompasses) { itemQuantity = plyr.lcompasses; }
			plyr.compasses+=itemQuantity;
			plyr.lcompasses-=itemQuantity;
		}
		if ((itemRef==1005) && (plyr.lkeys>0))
		{
			if (itemQuantity > plyr.lkeys) { itemQuantity = plyr.lkeys; }
			plyr.keys+=itemQuantity;
			plyr.lkeys-=itemQuantity;
		}
		if ((itemRef==1006) && (plyr.lcrystals>0))
		{
			if (itemQuantity > plyr.lcrystals) { itemQuantity = plyr.lcrystals; }
			plyr.crystals+=itemQuantity;
			plyr.lcrystals-=itemQuantity;
		}
		if ((itemRef==1007) && (plyr.lgems>0))
		{
			if (itemQuantity > plyr.lgems) { itemQuantity = plyr.lgems; }
			plyr.gems+=itemQuantity;
			plyr.lgems-=itemQuantity;
		}
		if ((itemRef==1008) && (plyr.ljewels>0))
		{
			if (itemQuantity > plyr.ljewels) { itemQuantity = plyr.ljewels; }
			plyr.jewels+=itemQuantity;
			plyr.ljewels-=itemQuantity;
		}
		if ((itemRef==1009) && (plyr.lgold>0))
		{
			if (itemQuantity > plyr.lgold) { itemQuantity = plyr.lgold; }
			plyr.gold+=itemQuantity;
			plyr.lgold-=itemQuantity;
		}
		if ((itemRef==1010) && (plyr.lsilver>0))
		{
			if (itemQuantity > plyr.lsilver) { itemQuantity = plyr.lsilver; }
			plyr.silver+=itemQuantity;
			plyr.lsilver-=itemQuantity;
		}
		if ((itemRef==1011) && (plyr.lcopper>0))
		{
			if (itemQuantity > plyr.lcopper) { itemQuantity = plyr.lcopper; }
			plyr.copper+=itemQuantity;
			plyr.lcopper-=itemQuantity;
		}
	}
}


int inputWithdrawalQuantity(int itemRef)
{
	int itemQuantity = 0;

	string str, key;
	string inputText = "";
	int maxNumberSize = 6;
	bool enterKeyNotPressed = true;
	while ( enterKeyNotPressed )
	{
		clearGuildDisplay();
		if (itemRef==1000) { cyText(2, "Withdraw how many Food Packets?"); }
		if (itemRef==1001) { cyText(2, "Withdraw how many Water Flasks?"); }
		if (itemRef==1002) { cyText(2, "Withdraw how many Unlit Torches?"); }
		if (itemRef==1003) { cyText(2, "Withdraw how many Timepieces?"); }
		if (itemRef==1004) { cyText(2, "Withdraw how many Compasses?"); }
		if (itemRef==1005) { cyText(2, "Withdraw how many Keys?"); }
		if (itemRef==1006) { cyText(2, "Withdraw how many Crystals?"); }
		if (itemRef==1007) { cyText(2, "Withdraw how many Gems?"); }
		if (itemRef==1008) { cyText(2, "Withdraw how many Jewels?"); }
		if (itemRef==1009) { cyText(2, "Withdraw how much Gold?"); }
		if (itemRef==1010) { cyText(2, "Withdraw how much Silver?"); }
		if (itemRef==1011) { cyText(2, "Withdraw how much Copper?"); }
		if (itemRef==1000) str = "(up to " + itos(plyr.lfood) + ")";
		if (itemRef==1001) str = "(up to " + itos(plyr.lwater) + ")";
		if (itemRef==1002) str = "(up to " + itos(plyr.ltorches) + ")";
		if (itemRef==1003) str = "(up to " + itos(plyr.ltimepieces) + ")";
		if (itemRef==1004) str = "(up to " + itos(plyr.lcompasses) + ")";
		if (itemRef==1005) str = "(up to " + itos(plyr.lkeys) + ")";
		if (itemRef==1006) str = "(up to " + itos(plyr.lcrystals) + ")";
		if (itemRef==1007) str = "(up to " + itos(plyr.lgems) + ")";
		if (itemRef==1008) str = "(up to " + itos(plyr.ljewels) + ")";
		if (itemRef==1009) str = "(up to " + itos(plyr.lgold) + ")";
		if (itemRef==1010) str = "(up to " + itos(plyr.lsilver) + ")";
		if (itemRef==1011) str = "(up to " + itos(plyr.lcopper) + ")";
		cyText(3, str);

		str = ">" + inputText + "_";
		bText(10,5, str);
		bText(10,9, "Enter amount or press ESC.");
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
		if (key=="ESC") { itemQuantity = 0; enterKeyNotPressed = false; }
	}
	itemQuantity = atoi(inputText.c_str());

	return itemQuantity;
}


void depositObject(int itemRef)
{
	int itemQuantity = inputDepositQuantity(itemRef);
	if (itemQuantity > 0)
	{
		if ((itemRef==1000) && (plyr.food>0))
		{
			if (itemQuantity > plyr.food) { itemQuantity = plyr.food; }
			plyr.food-=itemQuantity;
			plyr.lfood+=itemQuantity;
		}
		if ((itemRef==1001) && (plyr.water>0))
		{
			if (itemQuantity > plyr.water) { itemQuantity = plyr.water; }
			plyr.water-=itemQuantity;
			plyr.lwater+=itemQuantity;
		}
		if ((itemRef==1002) && (plyr.torches>0))
		{
			if (itemQuantity > plyr.torches) { itemQuantity = plyr.torches; }
			plyr.torches-=itemQuantity;
			plyr.ltorches+=itemQuantity;
		}
		if ((itemRef==1003) && (plyr.timepieces>0))
		{
			if (itemQuantity > plyr.timepieces) { itemQuantity = plyr.timepieces; }
			plyr.timepieces-=itemQuantity;
			plyr.ltimepieces+=itemQuantity;
		}
		if ((itemRef==1004) && (plyr.compasses>0))
		{
			if (itemQuantity > plyr.compasses) { itemQuantity = plyr.compasses; }
			plyr.compasses-=itemQuantity;
			plyr.lcompasses+=itemQuantity;
		}
		if ((itemRef==1005) && (plyr.keys>0))
		{
			if (itemQuantity > plyr.keys) { itemQuantity = plyr.keys; }
			plyr.keys-=itemQuantity;
			plyr.lkeys+=itemQuantity;
		}
		if ((itemRef==1006) && (plyr.crystals>0))
		{
			if (itemQuantity > plyr.crystals) { itemQuantity = plyr.crystals; }
			plyr.crystals-=itemQuantity;
			plyr.lcrystals+=itemQuantity;
		}
		if ((itemRef==1007) && (plyr.gems>0))
		{
			if (itemQuantity > plyr.gems) { itemQuantity = plyr.gems; }
			plyr.gems-=itemQuantity;
			plyr.lgems+=itemQuantity;
		}
		if ((itemRef==1008) && (plyr.jewels>0))
		{
			if (itemQuantity > plyr.jewels) { itemQuantity = plyr.jewels; }
			plyr.jewels-=itemQuantity;
			plyr.ljewels+=itemQuantity;
		}
		if ((itemRef==1009) && (plyr.gold>0))
		{
			if (itemQuantity > plyr.gold) { itemQuantity = plyr.gold; }
			plyr.gold-=itemQuantity;
			plyr.lgold+=itemQuantity;
		}
		if ((itemRef==1010) && (plyr.silver>0))
		{
			if (itemQuantity > plyr.silver) { itemQuantity = plyr.silver; }
			plyr.silver-=itemQuantity;
			plyr.lsilver+=itemQuantity;
		}
		if ((itemRef==1011) && (plyr.copper>0))
		{
			if (itemQuantity > plyr.copper) { itemQuantity = plyr.copper; }
			plyr.copper-=itemQuantity;
			plyr.lcopper+=itemQuantity;
		}

	}
}


int inputDepositQuantity(int itemRef)
{
	int itemQuantity = 0;

	string str, key;
	string inputText = "";
	int maxNumberSize = 6;
	bool enterKeyNotPressed = true;
	while ( enterKeyNotPressed )
	{
	cyText (2,"It costs a silver per unit. How many?");


		clearGuildDisplay();
		if (itemRef==1000) { cyText(2, "Deposit how many Food Packets?"); }
		if (itemRef==1001) { cyText(2, "Deposit how many Water Flasks?"); }
		if (itemRef==1002) { cyText(2, "Deposit how many Unlit Torches?"); }
		if (itemRef==1003) { cyText(2, "Deposit how many Timepieces?"); }
		if (itemRef==1004) { cyText(2, "Deposit how many Compasses?"); }
		if (itemRef==1005) { cyText(2, "Deposit how many Keys?"); }
		if (itemRef==1006) { cyText(2, "Deposit how many Crystals?"); }
		if (itemRef==1007) { cyText(2, "Deposit how many Gems?"); }
		if (itemRef==1008) { cyText(2, "Deposit how many Jewels?"); }
		if (itemRef==1009) { cyText(2, "Deposit how much Gold?"); }
		if (itemRef==1010) { cyText(2, "Deposit how much Silver?"); }
		if (itemRef==1011) { cyText(2, "Deposit how much Copper?"); }
		if (itemRef==1012) { cyText(2, "It costs a silver per unit. How many?"); }
		if (itemRef==1000) str = "(up to " + itos(plyr.food) + ")";
		if (itemRef==1001) str = "(up to " + itos(plyr.water) + ")";
		if (itemRef==1002) str = "(up to " + itos(plyr.torches) + ")";
		if (itemRef==1003) str = "(up to " + itos(plyr.timepieces) + ")";
		if (itemRef==1004) str = "(up to " + itos(plyr.compasses) + ")";
		if (itemRef==1005) str = "(up to " + itos(plyr.keys) + ")";
		if (itemRef==1006) str = "(up to " + itos(plyr.crystals) + ")";
		if (itemRef==1007) str = "(up to " + itos(plyr.gems) + ")";
		if (itemRef==1008) str = "(up to " + itos(plyr.jewels) + ")";
		if (itemRef==1009) str = "(up to " + itos(plyr.gold) + ")";
		if (itemRef==1010) str = "(up to " + itos(plyr.silver) + ")";
		if (itemRef==1011) str = "(up to " + itos(plyr.copper) + ")";
		if (itemRef==1012) str = "(up to " + itos(99-plyr.ringCharges) + ")";
		cyText(3, str);

		str = ">" + inputText + "_";
		bText(10,5, str);
		bText(10,9, "Enter amount or press ESC.");
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
		if (key=="ESC") { itemQuantity = 0; enterKeyNotPressed = false; }
	}
	itemQuantity = atoi(inputText.c_str());

	return itemQuantity;
}


int inputItemQuantity(int selectItemMode)
{
	int itemQuantity = 0;

	string str, key;
	string inputText = "";
	int maxNumberSize = 6;
	bool enterKeyNotPressed = true;
	while ( enterKeyNotPressed )
	{

		if (plyr.status == 3 ) { drawEncounterView(); }
		if (plyr.status == 2 ) { clearShopDisplay(); }
		if ((plyr.status !=3) && (plyr.status !=2)) { dispMain(); drawConsoleBackground(); }

		if (selectItemMode==2) { cyText(2, "Drop how many?"); }
		if (selectItemMode==3) { cyText(2, "Offer how many?"); }
		if (selectItemMode==4) { cyText(2, "Deposit how many?"); }

		str = ">" + inputText + "_";
		bText(10,5, str);
		bText(10,9, "Enter amount or press ESC.");
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
		if (key=="ESC") { itemQuantity = 0; enterKeyNotPressed = false; }
	}
	itemQuantity = atoi(inputText.c_str());

	return itemQuantity;
}


void dropObject(int object_ref)
{
	// Turn lit torch to stick when dropped

	if (itemBuffer[object_ref].name == "Lit Torch")
	{
		itemBuffer[object_ref].name = "stick";
		
	}
	itemBuffer[object_ref].location = 1;
	itemBuffer[object_ref].x = plyr.x;
	itemBuffer[object_ref].y = plyr.y;
	itemBuffer[object_ref].level = plyr.map;
	if (plyr.headArmour == object_ref) plyr.headArmour = 255;
	if (plyr.bodyArmour == object_ref) plyr.bodyArmour = 255;
	if (plyr.armsArmour == object_ref) plyr.armsArmour = 255;
	if (plyr.legsArmour == object_ref) plyr.legsArmour = 255;
	if (plyr.priWeapon == object_ref)
	{
	plyr.priWeapon = 0;
	}// Set bufferItem[0] - bare hand
	if (plyr.secWeapon == object_ref)
	{
		plyr.secWeapon = 0;         // Set bufferItem[0] - bare hand
	}
	if (plyr.clothing[0]==object_ref) plyr.clothing[0] = 255; applyEffect(0, clothingItems[itemBuffer[object_ref].index].effect);
	if (plyr.clothing[1]==object_ref) plyr.clothing[1] = 255; applyEffect(0, clothingItems[itemBuffer[object_ref].index].effect);
	if (plyr.clothing[2]==object_ref) plyr.clothing[2] = 255; applyEffect(0, clothingItems[itemBuffer[object_ref].index].effect);
	if (plyr.clothing[3]==object_ref) plyr.clothing[3] = 255; applyEffect(0, clothingItems[itemBuffer[object_ref].index].effect);

	//Check item buffs
	checkplayerLight();
}


void dropVolumeObject(int selectItemMode,int object_ref)
{
	int itemQuantity = 0;
	int existingItem = 9999;
	itemQuantity = inputItemQuantity(selectItemMode);
	if ((object_ref==1000) && (plyr.food>0) && (itemQuantity>0))
	{
		if (itemQuantity > plyr.food) { itemQuantity = plyr.food; }
		existingItem = checkForGenericItemsHere(1);
		if (existingItem==9999) createGenericItem(1,itemQuantity); else itemBuffer[existingItem].hp += itemQuantity;
		plyr.food-=itemQuantity;
	}
	if ((object_ref==1001) && (plyr.water>0) && (itemQuantity>0))
	{
		if (itemQuantity > plyr.water) { itemQuantity = plyr.water; }
		existingItem = checkForGenericItemsHere(2);
		if (existingItem==9999) createGenericItem(2,itemQuantity); else itemBuffer[existingItem].hp += itemQuantity;
		plyr.water-=itemQuantity;
	}
	if ((object_ref==1002) && (plyr.torches>0) && (itemQuantity>0))
	{
		if (itemQuantity > plyr.torches) { itemQuantity = plyr.torches; }
		existingItem = checkForGenericItemsHere(3);
		if (existingItem==9999) createGenericItem(3,itemQuantity); else itemBuffer[existingItem].hp += itemQuantity;
		plyr.torches-=itemQuantity;
	}
	if ((object_ref==1003) && (plyr.timepieces>0) && (itemQuantity>0))
	{
		if (itemQuantity > plyr.timepieces) { itemQuantity = plyr.timepieces; }
		existingItem = checkForGenericItemsHere(4);
		if (existingItem==9999) createGenericItem(4,itemQuantity); else itemBuffer[existingItem].hp += itemQuantity;
		plyr.timepieces-=itemQuantity;
	}
	if ((object_ref==1004) && (plyr.compasses>0) && (itemQuantity>0))
	{
		if (itemQuantity > plyr.compasses) { itemQuantity = plyr.compasses; }
		existingItem = checkForGenericItemsHere(5);
		if (existingItem==9999) createGenericItem(5,itemQuantity); else itemBuffer[existingItem].hp += itemQuantity;
		plyr.compasses-=itemQuantity;
	}
	if ((object_ref==1005) && (plyr.keys>0) && (itemQuantity>0))
	{
		if (itemQuantity > plyr.keys) { itemQuantity = plyr.keys; }
		int existingItem = checkForGenericItemsHere(6);
		if (existingItem==9999) createGenericItem(6,itemQuantity); else itemBuffer[existingItem].hp += itemQuantity;
		plyr.keys-=itemQuantity;
	}
	if ((object_ref==1006) && (plyr.crystals>0) && (itemQuantity>0))
	{
		if (itemQuantity > plyr.crystals) { itemQuantity = plyr.crystals; }
		existingItem = checkForGenericItemsHere(7);
		if (existingItem==9999) createGenericItem(7,itemQuantity); else itemBuffer[existingItem].hp += itemQuantity;
		plyr.crystals-=itemQuantity;
	}
	if ((object_ref==1007) && (plyr.gems>0) && (itemQuantity>0))
	{
		if (itemQuantity > plyr.gems) { itemQuantity = plyr.gems; }
		existingItem = checkForGenericItemsHere(8);
		if (existingItem==9999) createGenericItem(8,itemQuantity); else itemBuffer[existingItem].hp += itemQuantity;
		plyr.gems-=itemQuantity;
	}
	if ((object_ref==1008) && (plyr.jewels>0) && (itemQuantity>0))
	{
		if (itemQuantity > plyr.jewels) { itemQuantity = plyr.jewels; }
		existingItem = checkForGenericItemsHere(9);
		if (existingItem==9999) createGenericItem(9,itemQuantity); else itemBuffer[existingItem].hp += itemQuantity;
		plyr.jewels-=itemQuantity;
	}
	if ((object_ref==1009) && (plyr.gold>0) && (itemQuantity>0))
	{
		if (itemQuantity > plyr.gold) { itemQuantity = plyr.gold; }
		existingItem = checkForGenericItemsHere(10);
		if (existingItem==9999) createGenericItem(10,itemQuantity); else itemBuffer[existingItem].hp += itemQuantity;
		plyr.gold-=itemQuantity;
	}
	if ((object_ref==1010) && (plyr.silver>0) && (itemQuantity>0))
	{
		if (itemQuantity > plyr.silver) { itemQuantity = plyr.silver; }
		existingItem = checkForGenericItemsHere(11);
		if (existingItem==9999) createGenericItem(11,itemQuantity); else itemBuffer[existingItem].hp += itemQuantity;
		plyr.silver-=itemQuantity;
	}
	if ((object_ref==1011) && (plyr.copper>0) && (itemQuantity>0))
	{
		if (itemQuantity > plyr.copper) { itemQuantity = plyr.copper; }
		existingItem = checkForGenericItemsHere(12);
		if (existingItem==9999) createGenericItem(12,itemQuantity); else itemBuffer[existingItem].hp += itemQuantity;
		plyr.copper-=itemQuantity;
	}

}


void useObject(int object_ref)
{
	// Determine object type and pass object_ref to appropriate function
	if (itemBuffer[object_ref].type == 175) { use_item(object_ref); } // Item
	if (itemBuffer[object_ref].type==178) { use_weapon(object_ref); } // weapon
	if (itemBuffer[object_ref].type==177) { use_armor(object_ref); } // armor
	if ((itemBuffer[object_ref].type==176) && (itemBuffer[object_ref].hp==0)) { use_potion(object_ref); } // potion
	if ((itemBuffer[object_ref].type==176) && (itemBuffer[object_ref].hp==1)) { quaffPotion(object_ref); } // potion known
	if (itemBuffer[object_ref].type==180)
	{
		if (!((plyr.clothing[0]==object_ref) || (plyr.clothing[1]==object_ref) || (plyr.clothing[2]==object_ref) || (plyr.clothing[3]==object_ref))) use_clothing(object_ref);
	}
	if (itemBuffer[object_ref].type==199) { use_ammoItem(object_ref); } // ammo item
	if (itemBuffer[object_ref].type==200) { use_questItem(object_ref); } // quest item
	if (itemBuffer[object_ref].type==201) { use_questItem(object_ref); } // guild ring
	checkplayerLight();
	checkItemBuff();
}


void use_questItem(int object_ref)
{
	if (itemBuffer[object_ref].index == 4) { displayLocation(); } // map stone
	checkItemBuff();
}


void use_ammoItem(int object_ref)
{
    // Assume Thunder quarrels for now + leave out ammo type check
	if (itemBuffer[plyr.priWeapon].melee != 0xff)
    {
        itemBuffer[plyr.priWeapon].name = "Crossbow [10]";
        itemBuffer[plyr.priWeapon].ammo = 10;
        itemBuffer[plyr.priWeapon].power = 0x18;
        itemBuffer[object_ref].location = 0; // Destroy the ammo following a reload
    }

}


void use_armor(int object_ref)
{
	// The "melee" attribute is used for bodypart in armour items
	if (itemBuffer[object_ref].melee == 0) {plyr.headArmour = object_ref;}
	if (itemBuffer[object_ref].melee == 1) {plyr.bodyArmour = object_ref;}
	if (itemBuffer[object_ref].melee == 2) {plyr.armsArmour = object_ref;}
	if (itemBuffer[object_ref].melee == 3) {plyr.legsArmour = object_ref;}
	cout << itemBuffer[object_ref].melee << "\n";

	//Add armour buff 


	//int armorRef = itemBuffer[object_ref].index;
	//	if (Armor[armorRef].bodyLocation==1) {plyr.headArmour = object_ref;}
	//	if (Armor[armorRef].bodyLocation==2) {plyr.bodyArmour = object_ref;}
	//	if (Armor[armorRef].bodyLocation==3) {plyr.armsArmour = object_ref;}
	//	if (Armor[armorRef].bodyLocation==4) {plyr.legsArmour = object_ref;}
	checkItemBuff();
}


void use_potion(int object_ref)
{
    // Need to add POOF!
    // Need to add option of identifying potion in response to SIP

    string str, key_value;
	bool keypressed = false;
    int potionType = itemBuffer[object_ref].index;

    while (!keypressed)
    {
        if (plyr.status == 3 ) { drawEncounterView(); }
        else { dispMain(); }

        cyText(1, "POTION");
        //bText (12, 3, "Do you want to:");
        bText (16, 3, "(1) Taste");
        bText (16, 4, "(2) Sip");
        bText (16, 5, "(3) Examine");
        bText (16, 6, "(4) Quaff");
        cyText (8,"Press number or ESC to exit");
        updateDisplay();

			key_value = getSingleKey();
			if (key_value == "1")
			{
                string str = "The potion tastes " + Potions[potionType].taste +".";
                itemMessage(str);
			}
			if (key_value == "2")
			{
                string str = "You take a sip of the potion,@and feel it is " + Potions[potionType].sip +".";
                if (Potions[potionType].sip=="caution") { str = "You take a sip of the potion,@and feel that you@should show caution."; }
                itemMessage(str);
			}
            if (key_value == "3")
			{
                string str = "You open the potion and@see a potion that is " + Potions[potionType].color +"@in colour.";
                itemMessage(str);
			}
			if (key_value == "4") { quaffPotion(object_ref); keypressed=true; }
			if (key_value == "ESC") {keypressed=true; }
    }
	
}


void quaffPotion(int object_ref)
{
    string str, key_value;
	bool keypressed = false;
	int potionType = itemBuffer[object_ref].index;
	string potionName = Potions[potionType].name;

    str = "You drink a@@"+ potionName;
    itemMessage(str);

    // Implement potion effect
    if (potionType==0) { plyr.thirst -= 2; if (plyr.thirst < 0) { plyr.thirst = 0; } }
    if (potionType==1) { plyr.alcohol += 3; }
    if (potionType==2) { plyr.alcohol += 4; }
    if (potionType==3) { plyr.thirst -= 3; if (plyr.thirst < 0) { plyr.thirst = 0; } }
    if (potionType==4) { plyr.thirst -= 3; if (plyr.thirst < 0) { plyr.thirst = 0; } }
    if (potionType==5) { plyr.thirst -= 2; if (plyr.thirst < 0) { plyr.thirst = 0; } }
    if (potionType==6) { plyr.thirst += 3; }
    if (potionType==7) { plyr.invisibility = 1; }
    if (potionType==8) { plyr.thirst += 2; }
    if (potionType==9) { plyr.hp -= 5; }
    if (potionType==10) { plyr.poison[0] = 1; }
    if (potionType==11) { plyr.poison[1] = 1; }
    if (potionType==12) { plyr.poison[2] = 1; }
    if (potionType==13) { plyr.poison[3] = 1; }
    if (potionType==14) { plyr.hp+=5; if (plyr.hp > plyr.maxhp) { plyr.hp = plyr.maxhp; } }
    if (potionType==15) { plyr.hp+=8; if (plyr.hp > plyr.maxhp) { plyr.hp = plyr.maxhp; } }
    if (potionType==16) { plyr.hp+=11; if (plyr.hp > plyr.maxhp) { plyr.hp = plyr.maxhp; } }
    if (potionType==17) { plyr.hp = plyr.maxhp; }
    if (potionType==18) { plyr.poison[0] = 0; plyr.poison[1] = 0; plyr.poison[2] = 0; plyr.poison[3] = 0;}
    if (potionType==19) { plyr.diseases[0] = 0; plyr.diseases[1] = 0; plyr.diseases[2] = 0; plyr.diseases[3] = 0; }
    if (potionType==20) { plyr.delusion = 1; }
    if (potionType==21) { plyr.invulnerability[0] += 8; }
    if (potionType==22) { plyr.invulnerability[1] += 8; }
    if (potionType==23) { plyr.invulnerability[2] += 8; }
    if (potionType==24) { plyr.invulnerability[3] += 8; }
    if (potionType==25) { plyr.invulnerability[4] += 8; }
    if (potionType==26) { plyr.invulnerability[5] += 8; }
    if (potionType==27) { plyr.invulnerability[6] += 8; }
    if (potionType==28) { plyr.invulnerability[7] += 8; }
    if (potionType==29) { plyr.invulnerability[8] += 8; }
    if (potionType==30) { plyr.noticeability += 2;}
    if (potionType==31) { plyr.alcohol+=65; }
    if (potionType==32) { plyr.str+=1; }
    if (potionType==33) { plyr.inte+=1; }
    if (potionType==34) { plyr.chr+=1; }
    if (potionType==35) { plyr.chr-=2; }
    if (potionType==36) { plyr.str-=2; }
    if (potionType==37) { plyr.inte-=2; }
    if (potionType==38) { plyr.speed-=2; }
    if (potionType==39) { plyr.speed-=2; }
    if (potionType==40) { plyr.protection1 += 2; }
    if (potionType==41) { plyr.protection2 += 2; }
    if (potionType==42) { plyr.treasureFinding+=5; }
    if (potionType==43) { plyr.noticeability -= 2; if (plyr.noticeability < 0) { plyr.noticeability = 0; }  }

    itemBuffer[object_ref].location = 0; // Move used potion to void
    tidyObjectBuffer();
}


void use_clothing(int object_ref)
{
	//int clothingRef = itemBuffer[object_ref].index;
	bool allocated = false;
	if (plyr.clothing[0]==255) { plyr.clothing[0] = object_ref; allocated = true; }
	if ((plyr.clothing[1]==255) && (!allocated)) { plyr.clothing[1] = object_ref; allocated = true; }
	if ((plyr.clothing[2]==255) && (!allocated)) { plyr.clothing[2] = object_ref; allocated = true; }
	if ((plyr.clothing[3]==255) && (!allocated)) { plyr.clothing[3] = object_ref; allocated = true; }

	//Update clothing buff

	//	  clothing = itemBuffer[object_ref].index;
	//	  str = clothingItems[clothing].name;

	if ((plyr.clothing[3]!=255) && (!allocated))
	{
		swapClothing(object_ref);
	}

	checkItemBuff();
	//if (object_ref == plyr.secWeapon) { plyr.secWeapon = 255; }    // can't have same pri and sec weapon
}


void swapClothing(int object_ref)
{
	bool keypressed = false;
	string key, str;

	while (!keypressed)
	{
		if (plyr.status == 2) clearGuildDisplay();
		if (plyr.status == 3) drawEncounterView();
		if (plyr.status == 1) dispMain();
		if (plyr.status == 0) dispMain();

		cyText(1, "Wear instead of:");
		str = "(1) " + clothingItems[itemBuffer[plyr.clothing[0]].index].name;
		bText (5, 3, str);
		str = "(2) " + clothingItems[itemBuffer[plyr.clothing[1]].index].name;
		bText (5, 4, str);
		str = "(3) " + clothingItems[itemBuffer[plyr.clothing[2]].index].name;
		bText (5, 5, str);
		str = "(4) " + clothingItems[itemBuffer[plyr.clothing[3]].index].name;
		bText (5, 6, str);
		bText (2, 8,"Item # or ESC to exit");
		SetFontColour(40, 96, 244, 255);
		bText (2, 8,"     #    ESC");
		SetFontColour(215, 215, 215, 255);

		//  clothing = itemBuffer[cur_idx].index;
		//  str = clothingItems[clothing].name;
		updateDisplay();

		key = getSingleKey();
		if (key == "1") { plyr.clothing[0] = object_ref; keypressed=true; }
		if (key == "2") { plyr.clothing[1] = object_ref; keypressed=true; }
		if (key == "3") { plyr.clothing[2] = object_ref; keypressed=true; }
		if (key == "4") { plyr.clothing[3] = object_ref; keypressed=true; }
		if (key == "ESC") keypressed=true;
	}
	checkItemBuff();
}


void use_weapon(int object_ref)
{
		string str, key_value;
	bool keypressed = false;

        while (!keypressed)
		{
			if (plyr.status == 3 ) { drawEncounterView(); }
			else { dispMain(); }

			bText(17, 1, "Use as:");
			bText (12, 4, "1 Primary weapon");
			bText (12, 5, "2 Secondary weapon");
			bText (9, 8,"Press number or ESC to exit");
			updateDisplay();

			key_value = getSingleKey();

			if (key_value == "1")
			{
				plyr.priWeapon = object_ref;
				if (object_ref == plyr.secWeapon) { plyr.secWeapon = 255; }    // can't have same pri and sec weapon
				itemBuffer[plyr.priWeapon].location = 10; // primary was 11
				// remove old primary ref if exists
				keypressed=true;
			}
			if (key_value == "2")
			{
				plyr.secWeapon = object_ref;
				if (object_ref == plyr.priWeapon) { plyr.priWeapon = 255; } // can't have same pri and sec weapon
				itemBuffer[plyr.secWeapon].location = 10; // secondary was 12
				if (plyr.secWeapon)
				{
					plyr.torches--; // ??????
				}
				keypressed=true;
				// remove old secondary ref if exists
			}
			if (key_value == "ESC") {keypressed=true; }
        }
        // use_weapon();
		checkplayerLight();
		checkItemBuff();
}


void use_torch()
{
    // Header - 8B 24 00 00 02 10
    // Text -   4C 69 74 20 54 6F 72 63 68 00
    // Ammo -   00 FF 00
    // Damage - 13 00 00 00 13 00 00 00 00 00 00 (blunt & fire)
    // 04 01 16 16 82 03

	string str, key_value;
	bool keypressed = false;
     if (plyr.torches > 0)
     {

        while (!keypressed)
		{
		dispMain();
        bText(17, 1, "Use as:");
        bText (12, 4, "1 Primary weapon");
        bText (12, 5, "2 Secondary weapon");
        bText (9, 8,"Press number or ESC to exit");
        updateDisplay();

        key_value = getSingleKey();

    // Header - 8B 24 00 00 02 10
    // Text -   4C 69 74 20 54 6F 72 63 68 00
    // Ammo -   00 FF 00
    // Damage - 13 00 00 00 13 00 00 00 00 00 00 (blunt & fire)
    // 04 01 16 16 82 03

		if (key_value == "1")
        {
            plyr.priWeapon = createItem(178,0x0,"Lit Torch",0x16,0x16, 0x82, 0x04, 0x01, 0x0, 0x13,0,0,0,0x13,0,0,0,0,0,0,0,0,0x02,0,0xFF,0,0x03,0);
            //createWeapon(71); // create a new lit torch was 11
            itemBuffer[plyr.priWeapon].location = 10; // primary was 11
            plyr.torches--;
            // remove old primary ref if exists
			keypressed=true;
			checkplayerLight();
        }
        if (key_value == "2")
        {
            plyr.secWeapon = createItem(178,0x0,"Lit Torch",0x16,0x16, 0x82, 0x04, 0x01, 0x0, 0x13,0,0,0,0x13,0,0,0,0,0,0,0,0,0x02,0,0xFF,0,0x03,0);
            itemBuffer[plyr.secWeapon].location = 10; // secondary was 12
            plyr.torches--;
			keypressed=true;
			checkplayerLight();
            // remove old secondary ref if exists
        }
        if (key_value == "ESC") {keypressed=true; }
        }
        // use_weapon();
     }
     else
     {
		 while (!keypressed)
		 {
			dispMain();
			cyText (2,"You have none.");
			cyText (9,"( Press a key )");
			updateDisplay();
			key_value = getSingleKey();
			if ( key_value!="" ) { keypressed=true; }
		 }
     }

}

void use_item(int object_ref)
{
	


	int newSpell_Ref = createSpell(
		newItemArray[itemBuffer[object_ref].index].flags,
		newItemArray[itemBuffer[object_ref].index].index,
		newItemArray[itemBuffer[object_ref].index].name,
		100,  //Percentage
		newItemArray[itemBuffer[object_ref].index].cost,
		newItemArray[itemBuffer[object_ref].index].flags,
		newItemArray[itemBuffer[object_ref].index].elementType,
		newItemArray[itemBuffer[object_ref].index].statType,
		newItemArray[itemBuffer[object_ref].index].negativeValue,
		newItemArray[itemBuffer[object_ref].index].positiveValue,
		newItemArray[itemBuffer[object_ref].index].duration,
		newItemArray[itemBuffer[object_ref].index].damage,
		0, //lessonsno
		0,//LessonBoost
		newItemArray[itemBuffer[object_ref].index].ammo,
		
		100, //maxpercent
		newItemArray[itemBuffer[object_ref].index].effect,
		0, //No guilds
		"" //No cast text
	);

	switch (itemBuffer[object_ref].flags)
	{
	case 1: //Single use
		

		use_singleUse(newSpell_Ref);
		
		break;
	case 2: //

		use_wand(newSpell_Ref);
		break;

	case 3:
		use_eye(newSpell_Ref);
		break;

	}
	
}

void use_singleUse(int object_ref)
{

	
	castSpellAction(object_ref);
	itemBuffer[object_ref].location = 0; // primary was 11
}

void use_wand(int object_ref)
{
	
	if (plyr.crystals > 0)
	{
		castSpellAction(object_ref);
		plyr.crystals -= 1;
	}
	else
	{
		string str = "You need a crystal.";
	}
}

void use_eye(int object_ref)
{

		castSpellAction(object_ref);
		itemBuffer[object_ref].ammo -= 1;
	
}


int createWeapon(int weapon_no)
{
    // Create a new monster weapon on the floor

    // 0 - limbo, 1 - floor, 10 - carried, 11 - primary, 12 - secondary,
    // 13 - worn, 14 - head, 15 - arms, 16 - legs, 17 - body

    tidyObjectBuffer();
    buffer_item new_item;

    // Set weapon type
    new_item.type = monsterWeapons[weapon_no].type;
    new_item.type = 178;                            // type for a weapon that can be wielded (e.g not claws)
    new_item.index          = weapon_no;            // Reference to monsterWeapons - currently index not a binary offset

    // Copy weapon attributes
    new_item.name           = monsterWeapons[weapon_no].name;
    new_item.hp             = monsterWeapons[weapon_no].hp;
    new_item.maxHP          = monsterWeapons[weapon_no].maxHP;
    new_item.flags          = monsterWeapons[weapon_no].flags;
	new_item.minStrength    = monsterWeapons[weapon_no].minStrength;
	new_item.minDexterity   = monsterWeapons[weapon_no].minDexterity;
	new_item.useStrength    = monsterWeapons[weapon_no].useStrength;
	new_item.blunt          = monsterWeapons[weapon_no].blunt;
	new_item.sharp          = monsterWeapons[weapon_no].sharp;
	new_item.earth          = monsterWeapons[weapon_no].earth;
	new_item.air            = monsterWeapons[weapon_no].air;
	new_item.fire           = monsterWeapons[weapon_no].fire;
	new_item.water          = monsterWeapons[weapon_no].water;
	new_item.power          = monsterWeapons[weapon_no].power;
	new_item.magic          = monsterWeapons[weapon_no].magic;
	new_item.good           = monsterWeapons[weapon_no].good;
	new_item.evil           = monsterWeapons[weapon_no].evil;
	new_item.cold           = monsterWeapons[weapon_no].cold;
	new_item.weight         = monsterWeapons[weapon_no].weight;
	new_item.alignment      = monsterWeapons[weapon_no].alignment;
	new_item.melee          = monsterWeapons[weapon_no].melee;          // Don't think needed
	new_item.ammo           = monsterWeapons[weapon_no].ammo;           // Don't think needed
	new_item.parry          = monsterWeapons[weapon_no].parry;

    // Set weapon location
    new_item.location = 1;                          // the floor
    new_item.x = plyr.x;
    new_item.y = plyr.y;
    new_item.level = plyr.map;

    // Update buffer and buffer references
    itemBuffer[plyr.buffer_index] = new_item;
    int new_item_ref = plyr.buffer_index;
    plyr.buffer_index++;
    return new_item_ref; // what was the new items index in the object buffer
}


int createGenericItem(int type, int value)
{
	// generic item type = 1 - food, 2 - water, 3 - torches, 4 - timepieces, 5 - compasses
     // location options:
     // 0 - limbo, 1 - floor, 10 - carried, 11 - primary, 12 - secondary,
    // 13 - worn, 14 - head, 15 - arms, 16 - legs, 17 - body
     //int weapon_no = encounters[monster_no].weapon_no;
    tidyObjectBuffer();
	buffer_item new_item;
     //new_item.index = 0; // should be weapon_no?
     new_item.type = type; // gold, crystals, keys, gems
     //new_item.index = weapon_no;
     new_item.location = 1; // the floor
     new_item.x = plyr.x;
     new_item.y = plyr.y;
     new_item.level = plyr.map;
     //new_item.hp = Weapons[weapon_no].hp;
     new_item.hp = value; // for generic items sets number e.g. 4 food packets, 3 gold
	 itemBuffer[plyr.buffer_index] = new_item;
     int new_item_ref = plyr.buffer_index;
	 plyr.buffer_index++;
     return new_item_ref; // what was the new items index in the object buffer
}


void use_food()
{
     if (plyr.food > 0)
     {
         plyr.hunger-=16;
		 if ( plyr.hunger < 0 ) { plyr.hunger = 0; }
		 plyr.food--;
     }
}


void use_water()
{

     if (plyr.water > 0)
     {
		 plyr.thirst-=15;
		 if ( plyr.thirst < 0 ) { plyr.thirst = 0; }
        plyr.water--;
     }

}


void use_timepiece()
{
	string key;
	string hourtext = "th";
	if ((plyr.hours == 1) || (plyr.hours == 21)) { hourtext = "st"; }
	if ((plyr.hours == 2) || (plyr.hours == 22)) { hourtext = "nd"; }
	if ((plyr.hours == 3) || (plyr.hours == 23)) { hourtext = "rd"; }
	ostringstream myStream;
	myStream << "It is " << plyr.minutes << " minutes@past the " << plyr.hours << hourtext << " hour.@@@@(Press SPACE to continue)";
	string str = myStream.str();

     if (plyr.timepieces > 0)
     {
        key = "";
		while (key!="SPACE")
		{
			dispMain();
			cText (str);
			updateDisplay();
			key = getSingleKey();
		}

     }
     else
     {
        key = "";
		while (key!="SPACE")
		{
			dispMain();
			cText ("You have none.");
			updateDisplay();
			key = getSingleKey();
		}

     }


}


void checkForItemsHere()
{
  // counts number of objects on a map square - equal to 1
  int no_items = 0;
  int cur_idx = 0;
  while (cur_idx < plyr.buffer_index)
  {
    //    if ((itemBuffer[cur_idx].x == plyr.x) && (itemBuffer[cur_idx].y == plyr.y)
    //    && (itemBuffer[cur_idx].location == 1)) { no_items++; }
    if ((itemBuffer[cur_idx].x == plyr.x) && (itemBuffer[cur_idx].y == plyr.y) && (itemBuffer[cur_idx].level == plyr.map) && (itemBuffer[cur_idx].location == 1)) { no_items++; }
	cur_idx++;
  }

  if (no_items == 1) { plyr.status_text = "There is something here."; }
  if (no_items > 1) {plyr.status_text = "There are several things here."; }

}


int checkForGenericItemsHere(int type)
{
  // counts number of objects on a map square - equal to 1
  int value = 9999; // null return
  int cur_idx = 0;
  while (cur_idx < plyr.buffer_index)
  {
    //    if ((itemBuffer[cur_idx].x == plyr.x) && (itemBuffer[cur_idx].y == plyr.y)
    //    && (itemBuffer[cur_idx].location == 1)) { no_items++; }
    if ( (itemBuffer[cur_idx].type == type) && (itemBuffer[cur_idx].x == plyr.x) && (itemBuffer[cur_idx].y == plyr.y) && (itemBuffer[cur_idx].level == plyr.map) && (itemBuffer[cur_idx].location == 1)) { value = cur_idx; }
	cur_idx++;

  }
  return value;
}


bool checkForQuestItem(int itemNo)
{
  // checks through item buffer for carried quest items of type 200
  bool response = false;
  int cur_idx = 0;
  while (cur_idx < plyr.buffer_index)
  {
		if ((itemBuffer[cur_idx].location == 10) && (itemBuffer[cur_idx].type == 200) && (itemBuffer[cur_idx].index == itemNo)) { response = true; }
		if ((itemBuffer[cur_idx].location == 10) && (itemBuffer[cur_idx].type == 201) && (itemBuffer[cur_idx].index == itemNo)) { response = true; }
		cur_idx++;
  }
  return response;
}


int getQuestItemRef(int itemNo)
{
  // checks through item buffer for carried quest items of type 200
  int response = 255; // 255 indicates no match
  int cur_idx = 0;
  while (cur_idx < plyr.buffer_index)
  {
		if ((itemBuffer[cur_idx].location == 10) && (itemBuffer[cur_idx].type == 200) && (itemBuffer[cur_idx].index == itemNo)) { response = cur_idx; }
		if ((itemBuffer[cur_idx].location == 10) && (itemBuffer[cur_idx].type == 201) && (itemBuffer[cur_idx].index == itemNo)) { response = cur_idx; } // guild ring
		cur_idx++;
  }
  return response;
}


void displayLocation()
{
	string str, key, levelDesc; // for message text
	bool keynotpressed = true;
	int squaresNorth = 63-plyr.y;
	while ( keynotpressed )
	{
		if (plyr.map == 0) levelDesc = "the City";
		if (plyr.map == 1) levelDesc = "level 1";
		if (plyr.map == 2) { levelDesc = "level 2"; squaresNorth = 31-plyr.y; }
		if (plyr.map == 3) { levelDesc = "level 3"; squaresNorth = 15-plyr.y; }
		if (plyr.map == 4) { levelDesc = "level 4"; squaresNorth = 7-plyr.y; }

		if (plyr.status == 3) drawEncounterView();
		if (plyr.status == 1) dispMain();
		if (plyr.status == 0) dispMain();
		str = "You are "+itos(squaresNorth)+ " squares North@and "+itos(plyr.x)+ " squares East from the SouthWest@corner of " +levelDesc+".";
		cyText(3, str);
		cyText(8, "<<< Press any key to continue >>>");
		updateDisplay();
		key = getSingleKey();
		if (key!="") { keynotpressed = false; }
	}
}


void tidyObjectBuffer()
{
	// copy itemBuffer[250] to tempBuffer[250] before starting to reorganise entries
	// Maximum of 250 objects in play at the same time
	// 0 - limbo, 1 - floor, 10 - carried, 11 - primary, 12 - secondary,
    // 13 - worn, 14 - head, 15 - arms, 16 - legs, 17 - body

    int bufferSafetyCheckIndex = (itemBufferSize-3);

	if ( plyr.buffer_index > bufferSafetyCheckIndex)
	{
		int idx = plyr.buffer_index; // Number of items in play +1
		int newIndex = 0;
		for (int i=0 ; i<idx ; i++) { tempBuffer[i] = itemBuffer[i]; }

		for (int i=0 ; i<idx ; i++)
		{
			if (tempBuffer[i].location==10)
			{
				itemBuffer[newIndex] = tempBuffer[i];
				if (plyr.priWeapon == i) plyr.priWeapon = newIndex;
				if (plyr.secWeapon == i) plyr.secWeapon = newIndex;
				if (plyr.headArmour ==i) plyr.headArmour = newIndex;
				if (plyr.bodyArmour ==i) plyr.bodyArmour = newIndex;
				if (plyr.armsArmour ==i) plyr.armsArmour = newIndex;
				if (plyr.legsArmour ==i) plyr.legsArmour = newIndex;
				// worn clothing?
				newIndex++;
			}

		}

		// Reset the buffer index value
		plyr.buffer_index = newIndex;
	}
}


int returnCarriedWeight()
{
	int carriedWeight = 0;
	int itemWeight = 0;

	int gold = plyr.gold/16;
	int silver = plyr.silver/16;
	int copper = plyr.copper/16;
	int torches = plyr.torches/16;
	int flasks = plyr.water/16;
	int food = plyr.food/16;
	int crystals = plyr.crystals/16;
	int keys = plyr.keys/16;
	int gems = plyr.gems/16;
	int jewels = plyr.jewels/16;
	int timepieces = plyr.timepieces/16;
	int compasses = plyr.compasses/16;
	carriedWeight = gold+silver+copper+torches+flasks+food+crystals+keys+gems+jewels+timepieces+compasses;
	int idx = plyr.buffer_index;
	for (int i=0 ; i<idx ; i++)
	{
		if (itemBuffer[i].location==10)
		{
//            if (itemBuffer[i].type==177) { itemWeight = itemBuffer[i].weight; }
//            if (itemBuffer[i].type==178) { itemWeight = itemBuffer[i].weight; }
//            if (itemBuffer[i].type==180) { itemWeight = itemBuffer[i].weight; }
			if (itemBuffer[i].weight > 0)
			{
				itemWeight = itemBuffer[i].weight;
				carriedWeight += itemWeight;
			}
			itemWeight = 0;
		}
	}
	return carriedWeight;
}


void cannotCarryMessage()
{
	string key = "";
	while (key!="SPACE")
	{
		dispMain();
		cText ("@You cannot carry any more!@@@@(Press SPACE to continue)");
		updateDisplay();
		key = getSingleKey();
	}
}


void itemMessage(string message)
{
	string str, key; // for message text
	bool keynotpressed = true;
	while ( keynotpressed )
	{
		if (plyr.status == 3) drawEncounterView();
		if (plyr.status == 1) dispMain();

		cText(message);
		cyText(8,"( Press SPACE to continue )");
		updateDisplay();
		key = getSingleKey();
		if (key!="") { keynotpressed = false; }
	}
}


void checkItemBuff()
{

	if (itemBuffer[plyr.priWeapon].effect > 99) { castSpellAction( itemBuffer[plyr.priWeapon].effect); }
	if (itemBuffer[plyr.secWeapon].effect > 99) { castSpellAction( itemBuffer[plyr.secWeapon].effect); }
	if (itemBuffer[plyr.headArmour].effect > 99) { castSpellAction( itemBuffer[plyr.headArmour].effect); }
	if (itemBuffer[plyr.bodyArmour].effect > 99) { castSpellAction( itemBuffer[plyr.bodyArmour].effect); }
	if (itemBuffer[plyr.armsArmour].effect > 99) { castSpellAction( itemBuffer[plyr.armsArmour].effect); }
	if (itemBuffer[plyr.legsArmour].effect > 99) { castSpellAction( itemBuffer[plyr.legsArmour].effect); }
	if (clothingItems[plyr.clothing[0]].effect > 99) { castSpellAction( clothingItems[plyr.clothing[0]].effect); }
	if (clothingItems[plyr.clothing[1]].effect > 99) { castSpellAction( clothingItems[plyr.clothing[1]].effect); }
	if (clothingItems[plyr.clothing[2]].effect > 99) { castSpellAction( clothingItems[plyr.clothing[2]].effect); }
	if (clothingItems[plyr.clothing[3]].effect > 99) { castSpellAction( clothingItems[plyr.clothing[3]].effect); }
	
}


void removeItemBuff(int itemId)
{
	updateWeaponBuff(16383, 0);

	SearchResult WeaponBuffNo = findEffectBySpellTypeOrLastFree(4, 50);

	effectBuffer[WeaponBuffNo.index].effectNo = 0;
	effectBuffer[WeaponBuffNo.index].effectNo = 0;
}


int findlastmonsterWeapon()
{
	std::string targetValue = "";
	int lastIndex = -1;

	for (int i = sizeof(monsterWeapons) / sizeof(monsterWeapons[0]) - 1; i >= 0; i--)
	{
		if (monsterWeapons[i].name != targetValue)
		{
			lastIndex = i;
			
			break;
		}

	}
	return lastIndex;
}


void addNewMonsterItem()
{
	/* need to either write a file reader to get more weapons in the system
	* there are missing weapons, for example the sword of adept
	* also i want to add new weapons when wilderness is added
	* 
	* or modify the exising Bin file to add more weapons.
	* might be easier to make a new Weapon reading system based upon the scenario
	* 
	*/
}


void updateItemBuff(std::bitset<13> binaryItemBuff, int positiveValue) {
	for (int i = 0; i < 13; i++) {
		if (binaryItemBuff.test(i)) {
			*(itemBonusMap[i]) += positiveValue;
		}
	}
}


SearchResult  findItem(int targetNo, int size) {
	SearchResult effectIndex = { -1, false }; // Initialize result

	for (int i = 0; i < size; i++) {
		if (plyr.ActiveSpell[i] == targetNo) {
			// Match found, set the result and return
			effectIndex.index = i;
			effectIndex.found = true;
			//return effectIndex;
			break;
		}

		if (plyr.ActiveSpell[i] == 0) {
			// 0 effectNo indicates a free slot, store the index
			effectIndex.index = i;
			break;
			//return effectIndex;
		}
	}
	return effectIndex;
	// No match found, return the result indicating it's the last free index

}






std::map<std::pair<int, int>, int> countItemTypes(newItem* items, int size) {
	std::map<std::pair<int, int>, int> itemTypeLevelCount;

	// Count occurrences of each itemType with level
	for (int i = 0; i < size; ++i) {
		std::pair<int, int> key = std::make_pair(items[i].itemType, items[i].level);
		itemTypeLevelCount[key]++;
	}

	return itemTypeLevelCount;
}


std::vector<int> randomPick(const newItem* newItemArray, int arraySize, int itemCatToMatch, int levelToMatch, int typeToMatch) {
	std::vector<int> matchingIndices;

	// Find indices matching the criteria
	for (int i = 0; i < arraySize; ++i) {
		if (newItemArray[i].cat == itemCatToMatch && newItemArray[i].level == levelToMatch && newItemArray[i].itemType == typeToMatch) {
			matchingIndices.push_back(i);
		}
	}

	return matchingIndices;
}

int randomItemPicker(int itemCatToMatch, int levelToMatch, int arraySize, int typeToMatch)
{

	
		std::vector<int> matchingIndices = randomPick(newItemArray, plyr.items_index, itemCatToMatch, levelToMatch, typeToMatch);

		// Randomly pick an index from matching indices
		if (!matchingIndices.empty()) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, matchingIndices.size() - 1);

			int randomIndex = dis(gen);
			int selectedIndex = matchingIndices[randomIndex];

			// Display the randomly selected item
			std::cout << "Randomly picked item: " << newItemArray[selectedIndex].name << std::endl;
			return newItemArray[selectedIndex].index;
		}
		else {
			std::cout << "No items match the criteria." << std::endl;
			return 0;
		}
	

}


void checkDurablility(int itemIndex, int itemDamage)
{


	int itemDamaged = randn(0, itemDamage/10);
	
	itemBuffer[itemIndex].hp -= itemDamaged;
	if (itemBuffer[itemIndex].hp < 5 && itemBuffer[itemIndex].hp > 1)
	{
		string key = "";
		while (key != "SPACE")
		{
			dispMain();
			cText("@Your " + itemBuffer[itemIndex].name + " are about to break!@@@@(Press SPACE to continue)");
			updateDisplay();
			key = getSingleKey();
		}
	}
	else if (itemBuffer[itemIndex].hp < 1)
	{
		string key = "";
		while (key != "SPACE")
		{
			dispMain();
			cText("@Your " + itemBuffer[itemIndex].name + " has broken!@@@@(Press SPACE to continue)");
			updateDisplay();
			key = getSingleKey();
			itemBuffer[itemIndex].location = 0;
		}

	}

}

