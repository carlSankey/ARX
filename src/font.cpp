#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include "player.h"
#include "display.h"
#include "font.h"

extern int consoleY; // y position for displaying the bottom screen info panel (in pixels???)
extern int consoleX; // x starting position for displaying the panel for centering
extern int statPanelY; // Used as top of Y for Stats panel
extern int shopConsoleY;
extern int shopStatsY;

sf::Texture FontImage;
sf::Sprite CharImage;
extern int charYBase;
int yBase = 0;

void InitFont()
{
	if (plyr.fontStyle==0) { FontImage.loadFromFile("data/images/arfontSmooth.png"); }
	if (plyr.fontStyle==1) { FontImage.loadFromFile("data/images/arfont.png"); }
	CharImage.setTexture(FontImage);
}


void SetFontColour(int r,int g, int b, int a)
{
	CharImage.setColor(sf::Color(r, g, b, a));
}


void DrawChar(int topY, int x,int y, int initchar_no)
{

	int char_no;
	char_no = initchar_no-32;
	//if ((plyr.scenario==1) || (plyr.scenario==255)) {char_no = initchar_no-32; } // dungeon
	//else {char_no = initchar_no+96; } // city

	int row, column;
	int charsPerRow = 16; // number of chars per row in font image containing all tiles (16 default)

	//Select 16x16 section of tile sheet for tile

	if ( char_no > charsPerRow)
	{
		column = (char_no % charsPerRow); // remainder
		row = ((char_no-column)/charsPerRow);
	}
	else
	{
		column = char_no;
		row = 0; // = row 1 on the actual font sheet at y=0
	}

	int charX = (column)*16;	// x loc on tiles image in pixels
	int charY = ((row)*16);		// y loc on tiles image in pixels
	//std::cout << "tile:" << tile_no << "row:" << spriteRow << "col:" << spriteColumn << "\n";



	CharImage.scale(1.0f, 1.0f);

	CharImage.setTextureRect(sf::IntRect(charX, charY , 16, 16));
	//CharImage.setTextureRect(sf::IntRect(charX, charY , (charX+16), (charY+16)));
	if (char_no==16) { CharImage.setTextureRect(sf::IntRect(0, 16 ,16, 16));} // Dirty fix for printing 0 (zero) - why????


	//CharImage.setPosition(consoleX+((x-1)*16), 1+topY+((y-1)*16)+(y*2));
	CharImage.setPosition(consoleX+((x-1)*16), topY+(y*18));

	//if (plyr.status==0) { App.draw(CharImage); }
	//if (area==2) { App.draw(CharImage); } // stats banner
	//if (area==3) { App.draw(CharImage); } // bottom panels and shop displays

	//if (area==255) { App.draw(CharImage); } // Straight onto App window
	App.draw(CharImage);
}





void bText(int x, int y, std::string text)
{
	int strlen = text.size();
	for(int i = 0; i < strlen; ++i)
    {
		char current_char = text.at(i);
		int char_no = (int(current_char));
        if (plyr.status==2) { DrawChar(shopConsoleY, x,y, char_no); }
		else { DrawChar(consoleY, x,y, char_no); }
		x++;
	}
}

void drawText(int x, int y, std::string text)
{
	int strlen = text.size();
	for(int i = 0; i < strlen; ++i)
    {
		char current_char = text.at(i);
		int char_no = (int(current_char));
		if (plyr.status==2) { DrawChar(shopStatsY, x,y, char_no); }
		else { DrawChar(statPanelY, x,y, char_no); }
		x++;
	}
}






void drawText(int area, int x, int y, std::string text)
{
	int strlen = text.size();
	for(int i = 0; i < strlen; ++i)
    {
		char current_char = text.at(i);
		int char_no = (int(current_char));

			if (plyr.status==2) DrawChar(shopConsoleY, x,y, char_no);
            else DrawChar(consoleY, x,y, char_no); // was 16
			x++;

	}
}


void drawText(int x, int y, int number)
{
	std::string text;
	std::stringstream out;
	out << number;
	text = out.str();


	int strlen = text.size();
	for(int i = 0; i < strlen; ++i)
    {
		char current_char = text.at(i);
		int char_no = (int(current_char));

       if (plyr.status==2) { DrawChar(shopStatsY, x,y, char_no); }
		else { DrawChar(statPanelY, x,y, char_no); }
        x++;
	}
}

void bText(int x, int y, int number)
{
	std::string text;
	std::stringstream out;
	out << number;
	text = out.str();

	int strlen = text.size();
	for(int i = 0; i < strlen; ++i)
    {
		char current_char = text.at(i);
		int char_no = (int(current_char));
		if (plyr.status==2) { DrawChar(shopConsoleY, x,y, char_no); }
		else { DrawChar(consoleY, x,y, char_no); }
		x++;

	}
}

void cText(std::string str)
{

		int string_length, current_string_length;
      int x;
      int y;
      int char_count;
	  std::string current_string;
	  std::string current_char;

      string_length = str.size();
      char_count = 0;
      current_string="";
      y = 1;
      while (char_count <= string_length)
      {
          current_char = str.substr(char_count,1);; //stringa[char_count];

          if ((char_count==string_length) || (current_char=="@"))
          {
               current_string_length = current_string.size();
               x = ((40-current_string_length)/2)+1;

               drawText(0,x,y,current_string);

               y++;
               char_count++;
               current_string="";
          }
          else
          {
               current_string = current_string+current_char;
               char_count++;
          }
       }

}

void cyText(int y, std::string str)
{
	  int string_length, current_string_length;
      int x;
      int char_count;
	  std::string current_string;
	  std::string current_char;

      string_length = str.size();
      char_count = 0;
      current_string="";

      while (char_count <= string_length)
      {
          current_char = str.substr(char_count,1);; //stringa[char_count];

          if ((char_count==string_length) || (current_char=="@"))
          {
               current_string_length = current_string.size();
               x = ((40-current_string_length)/2)+1;

               drawText(consoleY,x,y,current_string);

               y++;
               char_count++;
               current_string="";
          }
          else
          {
               current_string = current_string+current_char;
               char_count++;
          }
       }

}


/* Seem to only be used in item.h */

void text(int x, int y, std::string text)
{
	int strlen = text.size();
	for(int i = 0; i < strlen; ++i)
    {
		char current_char = text.at(i);
		int char_no = (int(current_char));

			DrawChar(16, x,y, char_no); // was 16
			x++;

	}
}


void text(int x, int y, int number)
{
	std::string text;
	std::stringstream out;
	out << number;
	text = out.str();


	int strlen = text.size();
	for(int i = 0; i < strlen; ++i)
    {
		char current_char = text.at(i);
		int char_no = (int(current_char));


			DrawChar(16, x,y, char_no); // was 0
			x++;

	}
}
