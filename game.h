#ifndef GAME_H
#define GAME_H


#include <string>

using std::string;

void initialiseNewGame();
void gameInit();
bool gameTick();   // one in-game frame; false = session over (return to main menu)
bool gameIsRunning();
void gameEndSession();  // stop in-game audio/subsystems when leaving to main menu
void gameLoop();   // native helper: gameInit + while (gameTick())

void quitMenu();
void optionsMenu();
void teleport();

void moveWest();
void moveEast();
void moveNorth();
void moveSouth();
void moveForward();
void moveBack();
void turnLeft();
void turnRight();
void checkTeleport();
//void checkFixedEncounters();
void checkFixedTreasures();
void treasureMessage(std::string str);

void playerDies();
void checkShop();
void leaveShop();
void shopClosed();
void scenarioEntrance(int scenarioNumber);
//void shopDamon();
void shopRetreat();
void shopRathskeller();
void togglePanelsForward();
void togglePanelsBackward();

void pauseGame();
void barredDoor();
void doorMessage(std::string str);
void doorTimedMessage(std::string str);
void updateDoorDetails();
void moveThroughBarredDoor();
bool checkBarredDoor();

bool solidWall(int front);



#endif // GAME_H
// Your existing header content goes here 


