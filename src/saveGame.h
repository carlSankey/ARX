#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <string>

using namespace std;
const int saveGameSize = 28541;




void displayLoadGame();
void displaySaveGame();

void initSaveGameDescriptions();
void updateSaveGameDescriptions();

void initcharacter();

bool saveCharacter(int saveSlot);
bool loadCharacter(int saveSlot);


std::string ftos(float i);	// convert float to string
std::string itos(int i);	// convert int to string
//int str2int (const string &str)

#endif
