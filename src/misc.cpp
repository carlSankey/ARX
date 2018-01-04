#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <string>

#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "misc.h"
#include "display.h"
#include "font.h"
#include "player.h"


using std::ofstream;
using namespace std;


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
	sf::Event Event;
	while (App.pollEvent(Event))
	{
		if (Event.type == sf::Event::TextEntered)
		{
			if (Event.text.unicode < 128) { keyString = Event.text.unicode; }
			if (Event.text.unicode == 13) { keyString="RETURN"; }
			if (Event.text.unicode == 32) { keyString="SPACE"; }
			if (Event.text.unicode == 8) { keyString="BACKSPACE"; }
		}
	}
	return(keyString);
}



string readKey()
{
	string keyString = "";

		// Process events
		sf::Event Event;
		while (App.pollEvent(Event))
		{
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Left)) keyString="left";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Right)) keyString="right";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Up)) keyString="up";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Down)) keyString="down";

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F1)) keyString="F1";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F2)) keyString="F2";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F3)) keyString="F3";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F4)) keyString="F4";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F5)) keyString="F5";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F6)) keyString="F6";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F7)) keyString="F7";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F8)) keyString="F8";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F10)) keyString="F10";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F11)) keyString="F11";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F12)) keyString="F12";

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num0)) keyString="0";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Comma)) keyString=",";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Period)) keyString=".";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num1)) keyString="1";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num2)) keyString="2";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num3)) keyString="3";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num4)) keyString="4";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num5)) keyString="5";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num6)) keyString="6";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num7)) keyString="7";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num8)) keyString="8";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num9)) keyString="9";

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::A)) keyString="A";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::B)) keyString="B";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::C)) keyString="C";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::D)) keyString="D"; // diag info
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::E)) keyString="E"; // force encounterif ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Q)) keyString="Q";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F)) keyString="F";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::G)) keyString="G";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::H)) keyString="H";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::I)) keyString="I";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::J)) keyString="J";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::K)) keyString="K";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::L)) keyString="L";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::M)) keyString="M";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::N)) keyString="N";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::O)) keyString="O";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::P)) keyString="P";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Q)) keyString="Q";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::R)) keyString="R";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::S)) keyString="S";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::T)) keyString="T";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::U)) keyString="U";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::V)) keyString="V";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::W)) keyString="W";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::X)) keyString="X";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Y)) keyString="Y";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Z)) keyString="Z";

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Space)) keyString="SPACE";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Return)) keyString="RETURN";
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::BackSpace)) keyString="BACKSPACE";

			//if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::LShift) && (Event.key.code == sf::Keyboard::T)) keyString="T";

			// Close window : exit
			if (Event.type == sf::Event::Closed)
				keyString="QUIT";

			// Escape key : exit
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				keyString="ESC";


		}

	return(keyString);

}

bool keyPressed()
{
    //string key = "";
    //key = readKey();
    if (readKey()!="") return true;
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
   result = rand() % ((high-low)+1) + low;
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



int rollDice(int rolls, int dice)
{
    int result = 0;

    for( int r=0 ; r<rolls ; r++ )
    {
        result = result + randn( 1 , dice );
    }
    if (result==0) cout << "\nDice roll 0 error!\n";
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

