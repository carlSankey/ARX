#ifndef _misch 
#define _misch 

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>

// Import selected std symbols to avoid pulling whole namespace (prevents std::byte conflicts)
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::ostringstream;
using std::istringstream;
using std::sort;
using std::ifstream;
using std::ofstream;
using std::to_string;
using std::getline;

int Hex2Dec(std::string s);
int randn(int a,int n);
std::string getTextChar();
std::string readKey();
bool keyPressed();
std::string getSingleKey();

int rollDice(int rolls, int dice);
int oldRollDice(int x, int y);

std::string itos(int i);
std::string ftos(float i);

void displayCoins();
void displaySilverCoins();
std::string toCurrency(int i);
bool checkCoins(int gold,int silver,int copper);
void deductCoins(int gold,int silver, int copper);

bool stringToBool(const std::string& str);

int inputValue(std::string message,int shopNo);
void moduleMessage(std::string txt);

std::string trimString(float value, int digits);

std::string sha256(const std::string& input);
// Your existing header content goes here 


std::pair<int, int> getDigits(int number);

void replaceSymbol(std::string& str, const std::string& replaceWith, const std::string stringToFind);

std::string processMessage(std::string unprocessedMessage, std::string ReplacementText);




#endif // _misch 


