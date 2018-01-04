#include <string>

using namespace std;

int Hex2Dec(std::string s);
int randn(int a,int n);
string getTextChar();
string readKey();
bool keyPressed();
string getSingleKey();

int rollDice(int rolls, int dice);
int oldRollDice(int x, int y);

string itos(int i);
string ftos(float i);

void displayCoins();
void displaySilverCoins();
std::string toCurrency(int i);
bool checkCoins(int gold,int silver,int copper);
void deductCoins(int gold,int silver, int copper);

int inputValue(string message,int shopNo);
void moduleMessage(string txt);
