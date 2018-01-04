#include <string>

using namespace std;

void initialiseNewGame();
void gameLoop();

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
void treasureMessage(string str);

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
void doorMessage(string str);
void doorTimedMessage(string str);
void updateDoorDetails();
void moveThroughBarredDoor();
bool checkBarredDoor();
