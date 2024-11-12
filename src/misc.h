#ifndef _misch 
#define _misch 

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

bool stringToBool(const std::string& str);

int inputValue(string message,int shopNo);
void moduleMessage(string txt);

std::string trimString(float value, int digits);

std::string sha256(const std::string& input);
// Your existing header content goes here 


std::pair<int, int> getDigits(int number);

void replaceSymbol(std::string& str, const std::string& replaceWith, const std::string stringToFind);

std::string processMessage(std::string unprocessedMessage, std::string ReplacementText);




#endif // _misch 

