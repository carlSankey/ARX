
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <iostream>
#include <sstream>

#include "audio.h"
#include "createCharacter.h"
#include "player.h"
#include "font.h"
#include "display.h"
#include "misc.h"

// extern Player plyr;
extern sf::Sound cityGate2Sound;
extern sf::Sound cityGate3Sound;

using namespace std;

struct counter
  {
     int value1;
     int value2;
     int x;       // always constant
     int y;      // can be 1-8 , 2 can be 9-16?
     int speed; // decrement from this value until zero to slow down refresh of counter displat
     int speed_initial; // used to reset speed value above when it reaches zero
  };


  counter counters[8] =
  {
  {10,16,64-32, 96 ,11,11},
  {13,21,144-32,96, 9,9},
  {15,10,224-32,96, 8,8},
  {10,10,304-32,96, 13,13},
  {18,11,384-32,96, 10,10},
  {10,12,460-32,96, 8,8},
  {23,15,540-32,96,14,14},
  {54,47,560-32,336,11,11},
  };

  counter dungeonCounters[8] =
  {
  {10, 16,48, 96 ,2,2},
  {13,21,128,96, 0,0},
  {15,10, 208,96, 2,2},
  {10, 10, 288,96, 3,3},
  {18,11,368,96, 4,4},
  {10,12,444,96, 1,1},
  {23,15,524,96,3,3},
  {54,47,544,336,5,5},
  };

  counter cityCounters[8] =
  {
  {8, 16,48, 96 ,2,2},
  {13,21,128,96, 0,0},
  {15,10, 208,96, 2,2},
  {10, 10, 288,96, 4,4},
  {18,11,368,96, 5,5},
  {10,12,444,96, 1,1},
  {23,15,528,96,4,4},
  {54,47,514,370,5,5},
  };


  void cityGate()
{
	bool gateNotDone = true;
	string str,key;
	plyr.scenario=0;

	loadCounterImages();
	for (int i=0 ; i<8 ; i++) // copy city counter data
	{
		counters[i] = cityCounters[i];
	}

	initCityGateSounds();
	playCityGateSound1(); // play sound once only

	while (gateNotDone)
	{

		clearDisplay();
		//displayCityGateImage();
		displayCounters();
		displayCityGateImage();
		updateDisplay();
		//sf::sleep(sf::milliseconds(0.2));

		key = getSingleKey();
		if ( key!="" ) { gateNotDone = false; }

		int counter = 0;
		while ( counter < 8 ) // number of counters should be 8
		{

			if (counters[counter].speed == 0)
			{
				  counters[counter].speed = counters[counter].speed_initial;
				  counters[counter].y--;
				  if ( counters[counter].y == 82 ) // 32
				  {
					  counters[counter].value1 =  counters[counter].value2;
					  counters[counter].value2 = randn(0,12)+10;
					  counters[counter].y = 96; // 40
				  }
				  if (( counter == 7 ) && ( counters[counter].y == 356)) // copper
				  {
					  counters[counter].value1 =  counters[counter].value2;
					  counters[counter].value2 = randn(0,50)+49; // copper is 0-50 + 49 (giving max of 99 coppers)
					  counters[counter].y = 370;
				  }
			}
			else
			{
				 counters[counter].speed--;
			}
			counter++;

		 }
	}

	// key pressed

	stopCityGateSound1(); // stop looping gate sound
	playCityGateSound2(); // start longer gate sound

	// display chosen digits
	int counter = 0;
	while ( counter < 7 ) // number of counters should be 8
	{
		  if (counters[counter].y < 88) { counters[counter].value1 = counters[counter].value2; } //was 36
		  counters[counter].y = 96; // Set vertical position of STR CHA DEX etc
		  counter++;
	}
	if (counters[7].y <362) { counters[7].value1 = counters[7].value2; }
	counters[7].y = 370; // Set vertical position of copper counter when key pressed 354




   // Loop while the sound is playing
    while (cityGate2Sound.getStatus() == sf::Sound::Playing)
    {
        clearDisplay();
		displayCounters();
		displayCityGateImage();
		updateDisplay();
        // Leave some CPU time for other threads
//        sf::sleep(0.1f);
    }

	playCityGateSound3(); // start final gate sound
	while (cityGate3Sound.getStatus() == sf::Sound::Playing)
    {
		clearDisplay();
		drawText (2,11,"You are now joined.  Prepare to enter");
		drawText (7,13,"Alternate Reality, The City.");
		updateDisplay();
		// Leave some CPU time for other threads
//		sf::sleep(0.1f);
	}

	// assign chosen values to stats
	plyr.sta = counters[0].value1;
	plyr.chr = counters[1].value1;
	plyr.str = counters[2].value1;
	plyr.inte = counters[3].value1;
	plyr.wis = counters[4].value1;
	plyr.skl = counters[5].value1;
	plyr.hp = counters[6].value1;
	plyr.maxhp = counters[6].value1;
	plyr.copper = (counters[7].value1)+120;

}





void getPlayerName()
{
 bool details_confirmed = false;
 bool player_chosen = false;
 bool name_completed = false;
 string typed_name;
string sex;
 int name_length;

 while ( !details_confirmed )
 {
	 plyr.fixedEncounter = false;
     player_chosen = false;
     plyr.gender = 0;
     plyr.name = " ";
     typed_name = "";
     string single_key;
     drawPlayerDetails(typed_name);

     while ( !name_completed )
     {
         single_key = "";
		 single_key = getTextChar();
		 if (single_key=="SPACE") { single_key=" "; }
		 if (single_key=="ESC") { single_key=""; }
         if ((single_key=="RETURN") && ( typed_name != "" )) { single_key = ""; name_completed = true; }


         if (single_key=="BACKSPACE")
             {
                  name_length = typed_name.size();
                  if (name_length!=0)
                  {
                      int name_length = typed_name.size();
                      typed_name = typed_name.substr(0,(name_length-1));
                  }
                  single_key = "";
             }
         else
             {
                  if (single_key!="RETURN")
                  {
                      name_length = typed_name.size();
                      if (name_length!=24) // check for limit of name length
                      {
                             typed_name = typed_name + single_key;
                      }
                 }
             }
        drawPlayerDetails(typed_name);
     }

     plyr.name = typed_name;

     drawPlayerDetails(typed_name);

     sex = " ";
     while (plyr.gender == 0)
     {
         single_key = getSingleKey();
         if (single_key=="M")
         {
             plyr.gender=1;
             sex = "male";
         }
         if (single_key=="F")
         {
             plyr.gender=2;
             sex = "female";
         }
     }

     drawPlayerDetails(typed_name);
     while (!player_chosen)
     {
         single_key = getSingleKey();
         if (single_key=="Y")
         {
             details_confirmed = true;
             player_chosen = true;
             // plyr.name = typed_name;
         }
         if (single_key=="N")
         {
             details_confirmed = false;
             player_chosen = true;
             plyr.gender=0;
             plyr.name=" ";
             sex = " ";
             name_completed = false;

         }
     }

    drawPlayerDetails(typed_name);
 }

}


void drawPlayerDetails(string typed_name)
{
	string str;
	clearDisplay();
	drawText(2, 2, "Create a new character");
	str = "Enter name: "+typed_name+"_";
	drawText(2,5,str);

    if (!(plyr.name==" "))
    {
         drawText(2, 16, "Art thou male or female ? (M or F)");
    }

    if (!(plyr.gender==0))
    {
        str = "Thy name is "+typed_name;
		drawText(2, 18,str);
        if (plyr.gender ==1)
        {drawText(12, 20, "Thy sex is male."); }
        else
        {drawText(11, 20, "Thy sex is female."); }
        drawText(8, 22, "Is this correct? (Y or N)");
    }
    updateDisplay();
}



void dungeonGate()
{
	//int healerNo = gethealerNo();
	bool gateNotDone = true;
	string str,key;
	plyr.scenario=1;
	loadCounterImages();

	for (int i=0 ; i<8 ; i++) // copy dungeon counter data
	{
		counters[i] = dungeonCounters[i];
	}

	initDungeonGateSounds();
	playDungeonGateSound1(); // play looped sound while counters spin

	while (gateNotDone)
	{
		clearDisplay();

		displayCounters();
		displayDungeonGateImage();
		updateDisplay();
		//sf::sleep(sf::milliseconds(0.1));


		key = getSingleKey();
		if ( key!="" ) { gateNotDone = false; }

		int counter = 0;
		while ( counter < 8 ) // number of counters should be 8
		{
			if (counters[counter].speed == 0)
			{
				  counters[counter].speed = counters[counter].speed_initial;
				  counters[counter].y--;
				  if ( counters[counter].y == 82 ) // 32
				  {
					  counters[counter].value1 =  counters[counter].value2;
					  counters[counter].value2 = randn(0,13)+10;
					  counters[counter].y = 98; // 40
				  }
				  if (( counter == 7 ) && ( counters[counter].y == 320)) // silver
				  {
					  counters[counter].value1 =  counters[counter].value2;
					  counters[counter].value2 = randn(0,30)+50; // silver is 40 + random number between 1 and 30
					  counters[counter].y = 336;
				  }
			}
			else
			{
				 counters[counter].speed--;
			}
			counter++;

		 }
	}

	// key pressed

	stopDungeonGateSound1(); // stop looping gate sound
	playDungeonGateSound2(); // start longer gate sound

	// display chosen digits
	int counter = 0;
	while ( counter < 7 ) // number of counters should be 8
	{
		  if (counters[counter].y < 90) { counters[counter].value1 = counters[counter].value2; } //was 36
		  counters[counter].y = 96; //was 40
		  counter++;
	}
	if (counters[7].y <344) { counters[7].value1 = counters[7].value2; }
	counters[7].y = 336;

	clearDisplay();
	displayCounters();
	displayDungeonGateImage();
	updateDisplay();

	// pause
	for (int i=0 ; i<3 ; i++) // 1 second
	{
		sf::sleep(sf::seconds(1.0));
	}

	// display "Joined" message

	for (int i=0 ; i<6 ; i++) // 1 second
	{
		sf::sleep(sf::seconds(1));
		clearDisplay();
		drawText (2,11,"You are now joined.  Prepare to enter");
		drawText (6,13,"Alternate Reality, The Dungeon.");
		updateDisplay();

	}

	// assign chosen values to stats
	plyr.sta = counters[0].value1;
	plyr.chr = counters[1].value1;
	plyr.str = counters[2].value1;
	plyr.inte = counters[3].value1;
	plyr.wis = counters[4].value1;
	plyr.skl = counters[5].value1;
	plyr.hp = counters[6].value1;
	plyr.maxhp = counters[6].value1;
	plyr.silver = counters[7].value1;

}






void displayCounters()
{
     string digit1, digit2;
     digit1 = "0";
     digit2 = "0";
     int counter = 0;
     while ( counter < 8 ) // number of counters should be 8
     {
         // Draw the first layer number (upper)
          digit1 = "0";
          digit2 = "0";
         //char counter_filename[40];
         int counter_x = counters[counter].x;
         int counter_y = counters[counter].y;
         ostringstream myStream;
         myStream << counters[counter].value1;
         string digits = myStream.str();
         int digits_number = counters[counter].value1;
         if (digits_number < 10)
         {
            digit1 = "0";
            digit2 = digits.substr(0,1);
         }
         else
         {
             digit1 = digits.substr(0,1);
             digit2 = digits.substr(1,1);
         }

         if (digit1 == "0") {drawImage("0", counter_x, counter_y);}
         if (digit1 == "1") {drawImage("1", counter_x, counter_y);}
         if (digit1 == "2") {drawImage("2", counter_x, counter_y);}
         if (digit1 == "3") {drawImage("3", counter_x, counter_y);}
         if (digit1 == "4") {drawImage("4", counter_x, counter_y);}
         if (digit1 == "5") {drawImage("5", counter_x, counter_y);}
         if (digit1 == "6") {drawImage("6", counter_x, counter_y);}
         if (digit1 == "7") {drawImage("7", counter_x, counter_y);}
         if (digit1 == "8") {drawImage("8", counter_x, counter_y);}
         if (digit1 == "9") {drawImage("9", counter_x, counter_y);}

         if (digit2 == "0") {drawImage("0", counter_x+32, counter_y);}
         if (digit2 == "1") {drawImage("1", counter_x+32, counter_y);}
         if (digit2 == "2") {drawImage("2", counter_x+32, counter_y);}
         if (digit2 == "3") {drawImage("3", counter_x+32, counter_y);}
         if (digit2 == "4") {drawImage("4", counter_x+32, counter_y);}
         if (digit2 == "5") {drawImage("5", counter_x+32, counter_y);}
         if (digit2 == "6") {drawImage("6", counter_x+32, counter_y);}
         if (digit2 == "7") {drawImage("7", counter_x+32, counter_y);}
         if (digit2 == "8") {drawImage("8", counter_x+32, counter_y);}
         if (digit2 == "9") {drawImage("9", counter_x+32, counter_y);}

         // Draw the second layer number (lower)

         digit1 = "0";
         digit2 = "0";
         // myStream = "";
         ostringstream myStream2;
         myStream2 << counters[counter].value2;
         digits = myStream2.str();
         digits_number = counters[counter].value2;
         if (digits_number < 10)
         {
            digit1 = "0";
            digit2 = digits.substr(0,1);
         }
         else
         {
             digit1 = digits.substr(0,1);
             digit2 = digits.substr(1,1);
         }

         if (digit1 == "0") {drawImage("0", counter_x, counter_y+16);}
         if (digit1 == "1") {drawImage("1", counter_x, counter_y+16);}
         if (digit1 == "2") {drawImage("2", counter_x, counter_y+16);}
         if (digit1 == "3") {drawImage("3", counter_x, counter_y+16);}
         if (digit1 == "4") {drawImage("4", counter_x, counter_y+16);}
         if (digit1 == "5") {drawImage("5", counter_x, counter_y+16);}
         if (digit1 == "6") {drawImage("6", counter_x, counter_y+16);}
         if (digit1 == "7") {drawImage("7", counter_x, counter_y+16);}
         if (digit1 == "8") {drawImage("8", counter_x, counter_y+16);}
         if (digit1 == "9") {drawImage("9", counter_x, counter_y+16);}

         if (digit2 == "0") {drawImage("0", counter_x+32, counter_y+16);}
         if (digit2 == "1") {drawImage("1", counter_x+32, counter_y+16);}
         if (digit2 == "2") {drawImage("2", counter_x+32, counter_y+16);}
         if (digit2 == "3") {drawImage("3", counter_x+32, counter_y+16);}
         if (digit2 == "4") {drawImage("4", counter_x+32, counter_y+16);}
         if (digit2 == "5") {drawImage("5", counter_x+32, counter_y+16);}
         if (digit2 == "6") {drawImage("6", counter_x+32, counter_y+16);}
         if (digit2 == "7") {drawImage("7", counter_x+32, counter_y+16);}
         if (digit2 == "8") {drawImage("8", counter_x+32, counter_y+16);}
         if (digit2 == "9") {drawImage("9", counter_x+32, counter_y+16);}

         counter++;
     }

}





