#include <SFML\Graphics.hpp>
#include <optional>

#include <SFML\OpenGL.hpp>

#include <string>


#include <openssl/sha.h> // For MD5 hashing
#include <openssl/evp.h>
#include <openssl/md5.h> // For MD5 hashing
#include <cpp-base64/base64.h> // Base64 encoding library


#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cmath>



#include "misc.h"
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
	// Assumption 1 - Goods will be paid for using copper coins if possible as they take up the most weight for least value
	// Assumption 2 - Change will be given using higher value coins
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

string getTextChar()
{
	string keyString;
	while (const std::optional Event = App.pollEvent())
	{
		if (Event->is<sf::Event::TextEntered>())
		{
			const auto* textEntered = Event->getIf<sf::Event::TextEntered>();
			if (textEntered->unicode < 128) { keyString = static_cast<char>(textEntered->unicode); }
			if (textEntered->unicode == 13) { keyString="RETURN"; }
			if (textEntered->unicode == 32) { keyString="SPACE"; }
			if (textEntered->unicode == 8) { keyString="BACKSPACE"; }
		}
	}
	return(keyString);
}



string readKey()
{
	string keyString = "";

		// Process events
		while (const std::optional Event = App.pollEvent())
		{
			if (const auto* keyPressed = Event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->code == sf::Keyboard::Key::Left) keyString="left";
				if (keyPressed->code == sf::Keyboard::Key::Right) keyString="right";
				if (keyPressed->code == sf::Keyboard::Key::Up) keyString="up";
				if (keyPressed->code == sf::Keyboard::Key::Down) keyString="down";

				if (keyPressed->code == sf::Keyboard::Key::F1) keyString="F1";
				if (keyPressed->code == sf::Keyboard::Key::F2) keyString="F2";
				if (keyPressed->code == sf::Keyboard::Key::F3) keyString="F3";
				if (keyPressed->code == sf::Keyboard::Key::F4) keyString="F4";
				if (keyPressed->code == sf::Keyboard::Key::F5) keyString="F5";
				if (keyPressed->code == sf::Keyboard::Key::F6) keyString="F6";
				if (keyPressed->code == sf::Keyboard::Key::F7) keyString="F7";
				if (keyPressed->code == sf::Keyboard::Key::F8) keyString="F8";
				if (keyPressed->code == sf::Keyboard::Key::F10) keyString="F10";
				if (keyPressed->code == sf::Keyboard::Key::F11) keyString="F11";
				if (keyPressed->code == sf::Keyboard::Key::F12) keyString="F12";

				if (keyPressed->code == sf::Keyboard::Key::Num0) keyString="0";
				if (keyPressed->code == sf::Keyboard::Key::Comma) keyString=",";
				if (keyPressed->code == sf::Keyboard::Key::Period) keyString=".";
				if (keyPressed->code == sf::Keyboard::Key::Num1) keyString="1";
				if (keyPressed->code == sf::Keyboard::Key::Num2) keyString="2";
				if (keyPressed->code == sf::Keyboard::Key::Num3) keyString="3";
				if (keyPressed->code == sf::Keyboard::Key::Num4) keyString="4";
				if (keyPressed->code == sf::Keyboard::Key::Num5) keyString="5";
				if (keyPressed->code == sf::Keyboard::Key::Num6) keyString="6";
				if (keyPressed->code == sf::Keyboard::Key::Num7) keyString="7";
				if (keyPressed->code == sf::Keyboard::Key::Num8) keyString="8";
				if (keyPressed->code == sf::Keyboard::Key::Num9) keyString="9";

				if (keyPressed->code == sf::Keyboard::Key::A) keyString="A";
				if (keyPressed->code == sf::Keyboard::Key::B) keyString="B";
				if (keyPressed->code == sf::Keyboard::Key::C) keyString="C";
				if (keyPressed->code == sf::Keyboard::Key::D) keyString="D"; // diag info
				if (keyPressed->code == sf::Keyboard::Key::E) keyString="E"; // force encounterif ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Key::Q)) keyString="Q";
				if (keyPressed->code == sf::Keyboard::Key::F) keyString="F";
				if (keyPressed->code == sf::Keyboard::Key::G) keyString="G";
				if (keyPressed->code == sf::Keyboard::Key::H) keyString="H";
				if (keyPressed->code == sf::Keyboard::Key::I) keyString="I";
				if (keyPressed->code == sf::Keyboard::Key::J) keyString="J";
				if (keyPressed->code == sf::Keyboard::Key::K) keyString="K";
				if (keyPressed->code == sf::Keyboard::Key::L) keyString="L";
				if (keyPressed->code == sf::Keyboard::Key::M) keyString="M";
				if (keyPressed->code == sf::Keyboard::Key::N) keyString="N";
				if (keyPressed->code == sf::Keyboard::Key::O) keyString="O";
				if (keyPressed->code == sf::Keyboard::Key::P) keyString="P";
				if (keyPressed->code == sf::Keyboard::Key::Q) keyString="Q";
				if (keyPressed->code == sf::Keyboard::Key::R) keyString="R";
				if (keyPressed->code == sf::Keyboard::Key::S) keyString="S";
				if (keyPressed->code == sf::Keyboard::Key::T) keyString="T";
				if (keyPressed->code == sf::Keyboard::Key::U) keyString="U";
				if (keyPressed->code == sf::Keyboard::Key::V) keyString="V";
				if (keyPressed->code == sf::Keyboard::Key::W) keyString="W";
				if (keyPressed->code == sf::Keyboard::Key::X) keyString="X";
				if (keyPressed->code == sf::Keyboard::Key::Y) keyString="Y";
				if (keyPressed->code == sf::Keyboard::Key::Z) keyString="Z";
				if (keyPressed->code == sf::Keyboard::Key::Add) keyString = "+";
				if (keyPressed->code == sf::Keyboard::Key::Subtract) keyString = "-";

				if (keyPressed->code == sf::Keyboard::Key::Space) keyString="SPACE";
				if (keyPressed->code == sf::Keyboard::Key::Enter) keyString="RETURN";
				if (keyPressed->code == sf::Keyboard::Key::Backspace) keyString="BACKSPACE";

				// Escape key : exit
				if (keyPressed->code == sf::Keyboard::Key::Escape)
					keyString="ESC";
			}

			// Close window : exit
			if (Event->is<sf::Event::Closed>())
				keyString="QUIT";
		}

	return(keyString);

}

bool keyPressed()
{
    //string key = "";
    //key = readKey();
    if (readKey()!="") return true;
	return false;
}



string getSingleKey() // single pass for a key
{
	string key;
	//key = "";
	key = readKey();

	return key;
}


int randn(int low,int high)
 {
   int result;
   //if (n==0) n = 1; // to temporarily stop divide by 0 errors
   //result = rand()%n;
   //if (result == 0) { result = 1; }
	//if (high == 0) result = 0;
	//else
   //result = (rand()%(high-low))+low;

   if (high >= low) {
	   result = rand() % ((high - low) + 1) + low;
   }
   else {
	   // Handle the case where high is less than low.
	   // You can return an error code or take appropriate action.
	   result = rand() % ((high) + 1) + low;
   }
  
   return result;

}
/*
bool pressSpace()
{
	bool response = false;
	string key ="";
	key = readKey();
	if (key=="SPACE") { response = true; }
	return response;
}
*/



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


std::pair<int, int> getDigits(int number) {
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
		// Handle the error case where dice has 0 or negative sides
		std::cout << "\nInvalid dice with " << dice << " sides!\n";
		return 0; // Return 0 or an appropriate error value
	}

	for (int r = 0; r < rolls; r++)
	{
		result = result + rand() % dice + 1; // Roll the dice
	}

	return result;
}




std::string toCurrency(int i)	// convert int to currency string with commas
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
		// error below?
		clearShopDisplay();
		cyText (0, message);
		if (shopNo==13) displayCoins(); // Bank
		if (shopNo==14) displayCoins(); // City Healer

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
	string key = "";
	while (key!="SPACE")
	{
			clearShopDisplay();
			cText (txt);
			cyText(9,"( Press SPACE to continue )");
			updateDisplay();
			key = getSingleKey();
	}
}

std::string sha256(const std::string& input) {
	EVP_MD_CTX* mdctx;
	const EVP_MD* md;
	unsigned char hash[SHA256_DIGEST_LENGTH];
	unsigned int hash_len; // Change int to unsigned int

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
	EVP_DigestFinal_ex(mdctx, hash, &hash_len); // Pass address of hash_len

	EVP_MD_CTX_free(mdctx);

	// Convert the hash to Base64
	std::string base64_hash = base64_encode(hash, hash_len);

	return base64_hash;
}


std::string trimString(float value, int maxDigits) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(maxDigits) << value; // Convert float to string with fixed precision
	std::string trimmedStr = ss.str(); // Get the string representation

	// Remove trailing zeros after the decimal point
	size_t pos = trimmedStr.find('.');
	if (pos != std::string::npos) {
		size_t endPos = trimmedStr.find_last_not_of('0');
		if (endPos != std::string::npos && endPos > pos) {
			trimmedStr = trimmedStr.substr(0, endPos + 1);
		}
		else {
			// If the decimal point is followed only by zeros, remove it
			trimmedStr.erase(pos);
		}
	}

	return trimmedStr;
}

// Function to copy elements from one array to another based on specified indexes
void copyElements(int source[], int dest[], int indexes[], int numIndexes) {
	for (int i = 0; i < numIndexes; ++i) {
		dest[i] = source[indexes[i]];
	}
}

// Function to convert string to boolean
bool stringToBool(const std::string& str) {
	// Convert the string to lowercase to make the check case-insensitive
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

void replaceSymbol(std::string& origstr, const std::string& replaceWith, const std::string stringToFind) {
	int lengthtofind = stringToFind.size();
	size_t found = origstr.find(stringToFind);
	while (found != std::string::npos) {
		origstr.replace(found, lengthtofind, replaceWith);
		found = origstr.find(stringToFind, found + replaceWith.size());
	}
}





std::string processMessage(std::string unprocessedMessage, std::string ReplacementText)
{
	std::string newMessage = unprocessedMessage;
	std::string genderString = setGenderString(plyr.gender);
	std::string genderIdString = setGenderIdString(plyr.gender);
	std::string genderGreetString = setGenderGreetString(plyr.gender);
	replaceSymbol(newMessage, genderString, "^^");  //Man Woman
	replaceSymbol(newMessage, genderIdString, "��"); // Knave Scullion
	replaceSymbol(newMessage, genderGreetString, ">>"); // Brother Sister
	replaceSymbol(newMessage, plyr.name, "||"); //Player name
	replaceSymbol(newMessage, Monster_Buffer[plyr.encounterRef].name, "$$"); //Text
	if (ReplacementText != "")
	{
		replaceSymbol(newMessage, ReplacementText, "++");
	}
	replaceSymbol(newMessage, "", "\"");
	return newMessage;

	
}

