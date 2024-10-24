#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <string>
#include "player.h"

using namespace std;





void displayLoadGame();
void displaySaveGame();

void initSaveGameDescriptions();
void updateSaveGameDescriptions();

void initcharacter();

//bool saveCharacter(int saveSlot);
bool newsaveCharacter(int saveSlot, const Player& plyr);
bool newloadCharacter(int saveSlot, Player& plyr);
//bool loadCharacter(int saveSlot);


std::string ftos(float i);	// convert float to string
std::string itos(int i);	// convert int to string
//int str2int (const string &str)

#endif

 


