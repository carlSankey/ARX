#include <optional>
#include <string>

#ifndef ARX_USE_SDL2
#include <openssl/sha.h> // For MD5 hashing
#include <openssl/evp.h>
#include <openssl/md5.h> // For MD5 hashing
#include <cpp-base64/base64.h> // Base64 encoding library
#endif


#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cmath>



#include "misc.h"
#include "platform/Window.h"
#include "platform/InputQueue.h"

#include "display.h"
#include "font.h"
#include "player.h"
#include "actor.h"



using std::ofstream;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;


int Hex2Dec(std::string s)
{
    std::stringstream ss(s);
    int i;
    ss >> std::hex >> i;
    return i;
}

// =======================================================================
// INPUT FUNCTIONS - available in both SFML and SDL2 builds
// =======================================================================

#ifdef ARX_USE_SDL2

// -----------------------------------------------------------------------
// SDL2 input path: drain events from g_window->pollEvent() and push
// them into the InputQueue. No SFML dependency.
// -----------------------------------------------------------------------

string getTextChar()
{
	// Drain text input events into the text queue
	if (arx::g_window) {
		while (std::optional<arx::Event> event = arx::g_window->pollEvent()) {
			if (event->type == arx::EventType::TextInput) {
				// Push to text input queue
				arx::textPush(event->text);
			} else if (event->type == arx::EventType::KeyPressed) {
				// Also push key events into the key queue for readKey
				switch (event->key) {
				case arx::Key::Escape:    arx::inputPush("ESC");      break;
				case arx::Key::Space:     arx::inputPush("SPACE");    break;
				case arx::Key::Enter:     arx::inputPush("RETURN");   break;
				case arx::Key::Backspace: arx::inputPush("BACKSPACE");break;
				case arx::Key::Left:      arx::inputPush("LEFT");     break;
				case arx::Key::Right:     arx::inputPush("RIGHT");    break;
				case arx::Key::Up:        arx::inputPush("UP");       break;
				case arx::Key::Down:      arx::inputPush("DOWN");     break;
				case arx::Key::Num0:      arx::inputPush("0");        break;
				case arx::Key::Num1:      arx::inputPush("1");        break;
				case arx::Key::Num2:      arx::inputPush("2");        break;
				case arx::Key::Num3:      arx::inputPush("3");        break;
				case arx::Key::Num4:      arx::inputPush("4");        break;
				case arx::Key::Num5:      arx::inputPush("5");        break;
				case arx::Key::Num6:      arx::inputPush("6");        break;
				case arx::Key::Num7:      arx::inputPush("7");        break;
				case arx::Key::Num8:      arx::inputPush("8");        break;
				case arx::Key::Num9:      arx::inputPush("9");        break;
				case arx::Key::F1:        arx::inputPush("F1");       break;
				case arx::Key::F2:        arx::inputPush("F2");       break;
				case arx::Key::F3:        arx::inputPush("F3");       break;
				case arx::Key::F4:        arx::inputPush("F4");       break;
				case arx::Key::F5:        arx::inputPush("F5");       break;
				case arx::Key::F6:        arx::inputPush("F6");       break;
				case arx::Key::F7:        arx::inputPush("F7");       break;
				case arx::Key::F8:        arx::inputPush("F8");       break;
				case arx::Key::F10:       arx::inputPush("F10");      break;
				case arx::Key::F11:       arx::inputPush("F11");      break;
			case arx::Key::F12:       arx::inputPush("F12");      break;
			case arx::Key::Comma:     arx::inputPush(",");        break;
			case arx::Key::Period:    arx::inputPush(".");        break;
			default: break;
			}
			} else if (event->type == arx::EventType::Closed) {
				arx::inputPush("QUIT");
			}
		}
	}

	// Poll from the text queue - return the first typed character
	string result;
	while (true) {
		result = arx::textPoll();
		if (result.empty()) break;

		// Filter special keys the game expects from text input
		if (result == "\r" || result == "\n") { result = "RETURN"; break; }
		if (result == "\b") { result = "BACKSPACE"; break; }
		if (result == " ") { result = "SPACE"; break; }
		if (result == "\x1b") { result = "ESC"; break; }

		// Only return printable ASCII characters
		if (result.size() == 1 && result[0] >= 32 && result[0] < 127) {
			// Convert to uppercase for game consistency
			if (result[0] >= 'a' && result[0] <= 'z') {
				result[0] = result[0] - 32;
			}
			break;
		}
		// Skip non-printable characters and continue polling
	}
	return result;
}



string readKey()
{
	string keyString = "";

	// Use g_window for the isOpen check so we're not tied to App directly.
	if (!arx::g_window || !arx::g_window->isOpen()) {
		return "QUIT";
	}

	// Drain all pending events from the SDL window event queue
	while (std::optional<arx::Event> event = arx::g_window->pollEvent())
	{
		if (event->type == arx::EventType::KeyPressed)
		{
			string k = "";
			switch (event->key) {
			case arx::Key::Left:      k = "left";    break;
			case arx::Key::Right:     k = "right";   break;
			case arx::Key::Up:        k = "up";      break;
			case arx::Key::Down:      k = "down";    break;
			case arx::Key::F1:        k = "F1";      break;
			case arx::Key::F2:        k = "F2";      break;
			case arx::Key::F3:        k = "F3";      break;
			case arx::Key::F4:        k = "F4";      break;
			case arx::Key::F5:        k = "F5";      break;
			case arx::Key::F6:        k = "F6";      break;
			case arx::Key::F7:        k = "F7";      break;
			case arx::Key::F8:        k = "F8";      break;
			case arx::Key::F10:       k = "F10";     break;
			case arx::Key::F11:       k = "F11";     break;
			case arx::Key::F12:       k = "F12";     break;
			case arx::Key::Num0:      k = "0";       break;
			case arx::Key::Num1:      k = "1";       break;
			case arx::Key::Num2:      k = "2";       break;
			case arx::Key::Num3:      k = "3";       break;
			case arx::Key::Num4:      k = "4";       break;
			case arx::Key::Num5:      k = "5";       break;
			case arx::Key::Num6:      k = "6";       break;
			case arx::Key::Num7:      k = "7";       break;
			case arx::Key::Num8:      k = "8";       break;
			case arx::Key::Num9:      k = "9";       break;
			case arx::Key::A:         k = "A";       break;
			case arx::Key::B:         k = "B";       break;
			case arx::Key::C:         k = "C";       break;
			case arx::Key::D:         k = "D";       break;
			case arx::Key::E:         k = "E";       break;
			case arx::Key::F:         k = "F";       break;
			case arx::Key::G:         k = "G";       break;
			case arx::Key::H:         k = "H";       break;
			case arx::Key::I:         k = "I";       break;
			case arx::Key::J:         k = "J";       break;
			case arx::Key::K:         k = "K";       break;
			case arx::Key::L:         k = "L";       break;
			case arx::Key::M:         k = "M";       break;
			case arx::Key::N:         k = "N";       break;
			case arx::Key::O:         k = "O";       break;
			case arx::Key::P:         k = "P";       break;
			case arx::Key::Q:         k = "Q";       break;
			case arx::Key::R:         k = "R";       break;
			case arx::Key::S:         k = "S";       break;
			case arx::Key::T:         k = "T";       break;
			case arx::Key::U:         k = "U";       break;
			case arx::Key::V:         k = "V";       break;
			case arx::Key::W:         k = "W";       break;
			case arx::Key::X:         k = "X";       break;
			case arx::Key::Y:         k = "Y";       break;
			case arx::Key::Z:         k = "Z";       break;
			case arx::Key::Space:     k = "SPACE";   break;
			case arx::Key::Enter:     k = "RETURN";  break;
			case arx::Key::Backspace: k = "BACKSPACE"; break;
			case arx::Key::Escape:    k = "ESC";     break;
			case arx::Key::Comma:     k = ",";       break;
			case arx::Key::Period:    k = ".";       break;
			default: break;
			}

			if (!k.empty()) {
				arx::inputPush(k);
				if (keyString.empty()) keyString = k; // return first key this poll
			}
		}

		if (event->type == arx::EventType::TextInput) {
			// Route < and > (shift+comma / shift+period) to key queue for panel switching
			if (event->text == "<" || event->text == ">") {
				arx::inputPush(event->text);
				if (keyString.empty()) keyString = event->text;
			} else {
				arx::textPush(event->text);
			}
		}

		if (event->type == arx::EventType::Closed) {
			arx::inputPush("QUIT");
			keyString = "QUIT";
		}
	}

	// Also drain the InputQueue for any keys pushed by other means
	if (keyString.empty()) {
		keyString = arx::inputPoll();
	}

	return keyString;
}

bool keyPressed()
{
    if (readKey()!="") return true;
	return false;
}

string getSingleKey()
{
	readKey();
	return arx::inputPoll();
}

#endif // ARX_USE_SDL2

// =======================================================================
// GAME UTILITY FUNCTIONS (SFML/player-dependent)
// =======================================================================

#ifndef ARX_USE_SDL2

bool checkCoins(int gold,int silver,int copper)
{
	bool sufficientFunds = false;
	int itemCostInCoppers = (gold*100)+(silver*10)+copper;
	int playerTotalCoppers = (plyr.gold*100)+(plyr.silver*10)+plyr.copper;
	if (itemCostInCoppers <= playerTotalCoppers) { sufficientFunds = true; }
	return sufficientFunds;
}

void deductCoins(int gold,int silver, int copper)
{
	bool deductionCompleted = false;
	int itemCost = (gold*100)+(silver*10)+copper;

	if (itemCost <= plyr.copper)
	{
		plyr.copper-=itemCost;
		deductionCompleted = true;
	}
	else
	{
	itemCost-=plyr.copper;
	plyr.copper = 0;
	}

	if (!deductionCompleted)
	{
		int copperChange = (itemCost % 10);
		int numberOfSilversRequired = (itemCost/10);
		if (!copperChange==0) { numberOfSilversRequired++; }
		if (numberOfSilversRequired <= plyr.silver)
		{
			plyr.silver-=numberOfSilversRequired;
			if (!copperChange==0) { plyr.copper+=(10-copperChange); }
			deductionCompleted = true;
		}
		else
		{
			itemCost-=(plyr.silver*10);
			plyr.silver = 0;
		}
	}

	if (!deductionCompleted)
	{
		int copperChange = 0;
		int	silverChange = 0;
		int change = 0;
		change = (itemCost % 100);
		if (!change==0)
		{
			copperChange = (change%10);
			silverChange = (change/10);
		}
		int numberOfGoldsRequired = (itemCost/100);
		if (!copperChange==0) { silverChange++; }
		if (!silverChange==0) { numberOfGoldsRequired++; }

		plyr.gold-=numberOfGoldsRequired;
		if (!copperChange==0) { plyr.copper+=(10-copperChange); }
		if (!silverChange==0) { plyr.silver+=(10-silverChange); }
		deductionCompleted = true;
	}
}

void displayCoins()
{
	string str;
	int coinsCopper = (plyr.gold*100)+(plyr.silver*10)+plyr.copper;
	str = "Your coins in copper " + toCurrency(coinsCopper);
	cyText (9,str);
}

void displaySilverCoins()
{
	string str;
	int coinsSilver = (plyr.gold*10)+plyr.silver+(plyr.copper/10);
	str = "Your coins in silver " + toCurrency(coinsSilver);
	cyText (9,str);
}

int inputValue(string message, int shopNo)
{
	int itemQuantity = 0;
	string str, key;
	string inputText = "";
	int maxNumberSize = 6;
	bool enterKeyNotPressed = true;
	while ( enterKeyNotPressed )
	{
		clearShopDisplay();
		cyText (0, message);
		if (shopNo==13) displayCoins();
		if (shopNo==14) displayCoins();

		str = ">" + inputText + "_";
		bText(17,5, str);
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

void moduleMessage(string txt)
{
	clearShopDisplay();
	cText(txt);
	cyText(9,"( Press SPACE to continue )");
	updateDisplay();
	getSingleKey();
}

std::string processMessage(std::string unprocessedMessage, std::string ReplacementText)
{
	std::string newMessage = unprocessedMessage;
	std::string genderString = setGenderString(plyr.gender);
	std::string genderIdString = setGenderIdString(plyr.gender);
	std::string genderGreetString = setGenderGreetString(plyr.gender);
	replaceSymbol(newMessage, genderString, "^^");
	replaceSymbol(newMessage, genderIdString, "");
	replaceSymbol(newMessage, genderGreetString, ">>");
	replaceSymbol(newMessage, plyr.name, "||");
	replaceSymbol(newMessage, Monster_Buffer[plyr.encounterRef].name, "$$");
	if (ReplacementText != "")
	{
		replaceSymbol(newMessage, ReplacementText, "++");
	}
	replaceSymbol(newMessage, "", "\"");
	return newMessage;
}

#else // ARX_USE_SDL2 - stub unimplemented game utility functions (not called in web build)

bool checkCoins(int, int, int) { return false; }
void deductCoins(int, int, int) {}
void displayCoins() {}
void displaySilverCoins() {}
int inputValue(string, int) { return 0; }
void moduleMessage(string) {}
std::string processMessage(std::string, std::string) { return ""; }

#endif // ARX_USE_SDL2

// =======================================================================
// MATH / STRING UTILITIES (available in both builds)
// =======================================================================

int randn(int low,int high)
{
   int result;
   if (high >= low) {
	   result = rand() % ((high - low) + 1) + low;
   }
   else {
	   result = rand() % ((high) + 1) + low;
   }
   return result;
}

int oldRollDice(int x, int y)
{
    int result = 0;
    if (x != 0)
    {
         int i = 1;
         while (i <= x)
         {
             int roll = randn(0,y);
             result = result + roll;
             i++;
         }
    }
     return result;
}

std::pair<int, int> getDigits(int number)
{
	std::string numStr = std::to_string(number);
	int firstDigit = numStr.empty() ? 0 : (numStr[0] - '0');
	int remainingDigits = numStr.size() > 1 ? std::stoi(numStr.substr(1)) : 0;
	return std::make_pair(firstDigit, remainingDigits);
}

int rollDice(int rolls, int dice)
{
	int result = 0;
	if (dice <= 0)
	{
		std::cout << "\nInvalid dice with " << dice << " sides!\n";
		return 0;
	}
	for (int r = 0; r < rolls; r++)
	{
		result = result + rand() % dice + 1;
	}
	return result;
}

std::string toCurrency(int i)
{
	string temp, formatedNumber, low, high;
	std::stringstream s;
	s << i;
	temp = s.str();
	if (i<1000)
	{
		formatedNumber = temp;
	}
	if (i>999)
	{
		int c;
		int zLength = temp.length();
		low = temp.substr(zLength-3,3);
		if (zLength==6) { c = 3; }
		if (zLength==5) { c = 2; }
		if (zLength==4) { c = 1; }
		high = temp.substr(0,c);
		formatedNumber = high + "," + low;
	}
	return formatedNumber;
}

std::string sha256(const std::string& input)
{
#ifndef ARX_USE_SDL2
	EVP_MD_CTX* mdctx;
	const EVP_MD* md;
	unsigned char hash[SHA256_DIGEST_LENGTH];
	unsigned int hash_len;

	OpenSSL_add_all_digests();
	md = EVP_get_digestbyname("sha256");
	if (md == NULL) {
		std::cerr << "Error: SHA-256 not supported" << std::endl;
		return "";
	}

	mdctx = EVP_MD_CTX_new();
	if (mdctx == NULL) {
		std::cerr << "Error: Unable to create hash context" << std::endl;
		return "";
	}

	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, input.c_str(), input.length());
	EVP_DigestFinal_ex(mdctx, hash, &hash_len);
	EVP_MD_CTX_free(mdctx);

	std::string base64_hash = base64_encode(hash, hash_len);
	return base64_hash;
#else
	// OpenSSL not available in Emscripten build; return empty string
	return "";
#endif
}

std::string trimString(float value, int maxDigits)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(maxDigits) << value;
	std::string trimmedStr = ss.str();
	size_t pos = trimmedStr.find('.');
	if (pos != std::string::npos) {
		size_t endPos = trimmedStr.find_last_not_of('0');
		if (endPos != std::string::npos && endPos > pos) {
			trimmedStr = trimmedStr.substr(0, endPos + 1);
		}
		else {
			trimmedStr.erase(pos);
		}
	}
	return trimmedStr;
}

void copyElements(int source[], int dest[], int indexes[], int numIndexes)
{
	for (int i = 0; i < numIndexes; ++i) {
		dest[i] = source[indexes[i]];
	}
}

bool stringToBool(const std::string& str)
{
	std::string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
	if (lowerStr == "true" || lowerStr == "1") {
		return true;
	}
	else if (lowerStr == "false" || lowerStr == "0") {
		return false;
	}
	else {
		throw std::invalid_argument("Invalid input string for boolean conversion: " + str);
	}
}

void replaceSymbol(std::string& origstr, const std::string& replaceWith, const std::string stringToFind)
{
	int lengthtofind = stringToFind.size();
	size_t found = origstr.find(stringToFind);
	while (found != std::string::npos) {
		origstr.replace(found, lengthtofind, replaceWith);
		found = origstr.find(stringToFind, found + replaceWith.size());
	}
}
