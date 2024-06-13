#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

const std::string progRelease = "0.91";

//3Dview
const int noOfTextures = 68;  //no of image in the images folder
const int noOfBackgrounds = 61; //no of images in the background folder

//SaveGame
const int noOfguildAwards = 12;  //no of guilds that can add a bonus stat
const int noOfFixedEncounters = 32; //no of fixed encounters 
const int noOfGuilds = 14; //total number of guilds
const int noOfSmithyFriendships = 4; //no of smithys
const int noOfBankAccountStatuses = 9; // no of bank statuses
const int noOfBankAccountBalances = 9; // no of bank balances
const int noOfClothing = 4; // number of clothing slots on a player
const int noOfHealerDays = 2; // city healer time to return
const int noOfHealerHours = 2; // city healer time to return
const int noOfHealerMinutes = 2; // city healer time to return
const int noOfDiseases = 4;  // number of diseaese a player can catch
const int noOfPoison = 4; // number of poinsons a player can have active
const int noOfElements = 13; //number of attack/defence elemets a weapon/armour can have
const int noOfItemBuffer = 250; //number of items in the system
const int noOfSpellBuffer = 35; //number of spells in the spell buffer
const int noOfEffects = 50; //number of effects a player can have at once
const int noOfSmithDailyWares = 10; //number of items a smithy can sell. used so hard loading can't change smithy items to better ones
const int noOfAutoMapExplored = 4096; //number of map explorerable tiles
const int noOfSaveGame = 28541; //number of the Save game
const int noOfLevels = 5; //numbe of levels currently coded

//3dviewnew
const int MAX_TEXTURES = 32; //Maximum amount of textures loaded at once

//Actor
const int noOfEncounters = 86; // number of monsters in the game
const int noOfMonsterWeapons = 250; //number of weapons monsters can use  ** think this is redundant as monster weapons use items**
const int noOfWeapons = 111; //number of weapons in the system ** think this is redundant as the weapons are now in items **
const int noOfMonstersFile = 45056; //number of bytes inthe MonsterFile ** think this is redundant as the monsters has its own CSV **

//back_inventory
//const int noOfInventory = 4096;  //not used

//dammon
const size_t noOfDaemon = 12;
const size_t noOfDaemonC = 12;
const int damonFileSize = 916;

//dwarvenSmithy
const int noOfDwarvenFile = 459;


//encounter
const int MAX_OPPONENTS = 8;
const int MAX_CONSOLE_MESSAGES = 20;
const int DUNGEON_TABLE_ENCOUNTERS = 64;

//guild



//items
const int noOfDungeonItems = 10496;
const int noOfFixedTreasures = 64;

//itemselect
const int MAX_MENU_ENTRIES = 4; // Max 4 entries per menu page.

//module
const int MAX_MENU_ITEMS = 6; // Max 6 items per menu page. Should be usable for building any type of general shop menus with a maximum of 20 multi page items

//player



//Smithy
const int noOfCitySmithyFile = 1691;


//spells 
 const int noOfSpellList = 77;

// Other constants...

#endif // CONSTANTS_H