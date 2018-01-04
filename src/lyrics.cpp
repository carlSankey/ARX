#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "player.h"
#include "display.h"
#include "lyrics.h"


//==============================================================================
//                        Declarations / initializations
//==============================================================================
struct lyricElement
    {
	int                         x;
	string                      text;
    };

extern int                      lyricX;
extern int                      lyricY;
extern Player                   plyr;
extern sf::Clock                clock1;
extern sf::RenderWindow         App;
extern sf::RenderTexture        lyricstexture;
extern int                      charYBase;
sf::Clock               clock1;
int                     iCounter;

bool                            wipe;
int                             backInk, ink;
int                             x, backx;
int                             lyricPointer, lyricDuration, sequenceLength;
float                           fSpeedCoefficient = 0;
string                          backText, foreText;

sf::Texture                     lyricFontImage;
sf::Sprite                      lyricCharImage;

lyricElement                    lyrics[2048];


//==============================================================================
//                                Functions
//==============================================================================
void initLyricFont()
    {
	lyricFontImage.loadFromFile("data/images/songFont.png");
	lyricCharImage.setTexture(lyricFontImage);
    }


void lyricColour(int r, int g, int b, int a)
    {
	lyricCharImage.setColor(sf::Color(r, g, b, a));
    }


//================================== lyric =====================================
//
//  Calls the 'drawLyricChar' function, passing it 0 (the upper-leftmost
//  y-component), X (a value passed to 'lyric' that serves a number of
//  functions [explained below]), and CHAR_NO (the ASCII value of the current
//  letter in the string).
//                                              *** CALLED BY 'UPDATE LYRICS'
//------------------------------------------------------------------------------
void lyric(int x, string text)
    {
	int strlen                      =   text.size();

	for(int i = 0; i < strlen; ++i)
        {
		char current_char           =   text.at(i);
		int char_no                 =   int(current_char);

        drawLyricChar(x, char_no);
        x++;
        }
    }


//============================== drawLyricChar =================================
//
//  Accepts as parameters an upper-y position (which is always 0), the
//  current character's x-value (an index which is incremented...), and
//  INITCHAR_NO, an ASCII value derived in the 'lyric' function.
//                                                      *** CALLED BY 'LYRIC'
//------------------------------------------------------------------------------
void drawLyricChar(int x, int initchar_no)
    {
	int char_no                     =   initchar_no - 64;
	if (initchar_no == 39) char_no  =   29;
	if (initchar_no == 32) char_no  =   0;
	if (initchar_no == 33) char_no  =   27;
	if (initchar_no == 63) char_no  =   28;
	if (initchar_no == 45) char_no  =   30;
	if (initchar_no == 46) char_no  =   31;
	int charX                       =   char_no * 32;

	lyricCharImage.setTextureRect(sf::IntRect(charX, 0 , 32, 16));
	lyricCharImage.setPosition(lyricX+((x - 1) * 32), lyricY);
	App.draw(lyricCharImage);
    }


//=============================== loadLyrics ===================================
//
//  A thick function that takes as input the lyrics (stored in .txt files at
//  either one of two locations, depending on if we're using the 'modern' or
//  'classic' soundtrack), which comprise one of two elements: an integer (X,
//  which is used as either an on-screen position indicator, a delay, or a
//  color value) and a string, which itself either indicates a lyric or a
//  color.
//------------------------------------------------------------------------------
void loadLyrics(string filename)
    {
	lyricPointer                    =   0;                              // *** reset for a new set of lyrics
	lyricDuration                   =   0;                              // *** given value only if the lyricElement's current 'x' indicates a delay
	sequenceLength                  =   0;
	foreText                        =   "";
	backText                        =   "";
	int     i                       =   0;                              // *** index used to fill array as sequence data is loaded
    string  line, left, right;                                          // *** LINE stores the current string
    string  lyricsFilename          =   "data/audio/" + filename;

	string::size_type                   idx;
	std::ifstream                       instream;

        if (plyr.musicStyle == 1)                                       // *** selects the 'modern' soundtrack
            {
            lyricsFilename          =   "data/audio/B/" + filename;
            }

	instream.open(lyricsFilename.c_str());

        if (!instream)                                                  // *** obligatory mis-load feedback
            {
            cerr << "Error: lyrics file could not be loaded" << endl;
            }

	while (line != "EOF")
        {


		string::size_type               idx;                            // *** string::size_type is an integer datatype large enough to represent any possible string size
		getline(instream, line);                                        // *** read first line as blank
		idx                         =   line.find(',');                 // *** yields the index in our LINE of the ','.

		if (idx == string::npos)                                        // *** no comma; assume duration value or colour change
            {
			lyrics[i].x             =   atoi(line.c_str());
			lyrics[i].text          =   "ERROR!";

			if (line == "CYAN")
                {
                lyrics[i].x         =   150000;
                lyrics[i].text      =   "COLOUR!";
                }

			if (line == "BLUE")
                {
                lyrics[i].x         =   150001;
                lyrics[i].text      =   "COLOUR!";
                }

			if (line == "GREEN")
                {
                lyrics[i].x         =   150002;
                lyrics[i].text      =   "COLOUR!";
                }

			if (line == "WHITE")
                {
                lyrics[i].x         =   150003;
                lyrics[i].text      =   "COLOUR!";
                }
            }

		else
            {
			left                    =   line.substr(0, idx);
			right                   =   line.substr(idx + 1);
			lyrics[i].x             =   atoi(left.c_str());
			lyrics[i].text          =   right;
            }

        sequenceLength++;
        i++;
        }
	instream.close();
	sequenceLength--;
	backText="";
	foreText="";
	// *lyricstexture.clear(sf::Color::Black);
	wipe = false;
	iCounter =0;
    }


//=============================== updateLyrics =================================
//  Interprets the lyricElement structure and updates accordingly. Assigns a
//  'lyricDuration' value on the basis of a clock that is started whenever
//  'updateLyrics()' is called, which measures the elapsed time between the
//  call and its receipt; this adjusts the lyric duration so that slower
//  computers will see a reduction in the time it takes to update the on-screen
//  lyrics, accounting for the longer time spent in processing.
//                                               *** CALLED BY 'TAVERN.CPP,'
//                                                  'SMITHY.CPP,' 'SHOP.CPP'
//------------------------------------------------------------------------------
void updateLyrics()
    {

        //lyric(backx, backText);
        //lyric(x, foreText);

    if (lyricDuration < 0)
            {
            lyricDuration           =  0;
            }
	if (lyricPointer < sequenceLength)
        {
		if (lyricDuration > 0)
            {

			if (backInk == 1) lyricColour(59, 83, 255, 255);
			if (backInk == 2) lyricColour(54, 127, 40, 255);
			lyric(backx, backText);
			if (ink == 3) lyricColour(161, 238, 255, 255);
			if (ink == 4) lyricColour(192, 192, 192, 255);
			lyric(x, foreText);

            sf::sleep (sf::milliseconds(100));
            lyricDuration          -=  100;

            if (lyricDuration <= 0)
                {
                wipe                =   true;
                lyricDuration       =   0;
                }
            }
		if (wipe)
            {
            // *lyricstexture.clear(sf::Color::Black);
            foreText                =   "";
            //backText                =   "";
            wipe                    =   false;
            }

        //----------------------------------------------------------------------
        //                              COLOR
        //  The following 'if' statement takes care of our color. If the
        //  'lyrics' structure's X value at [n] is greater than 150,000, this
        //  means that X indicates a color value at this index.
        //----------------------------------------------------------------------
		if ((lyrics[lyricPointer].x > 14999) && (lyricDuration == 0))
            {
			if (lyrics[lyricPointer].x == 150000) { ink = 3; }
			if (lyrics[lyricPointer].x == 150001) { ink = 1; backInk = 1; backText=""; }
			if (lyrics[lyricPointer].x == 150002) { ink = 2; backInk = 2; backText=""; }
            //if (lyrics[lyricPointer].x == 150001) { ink = 1; backInk = 1; backText=""; }
			//if (lyrics[lyricPointer].x == 150002) { ink = 2; backInk = 2; backText=""; }
			if (lyrics[lyricPointer].x == 150003) { ink = 4;}
			lyricPointer++;
            }

        //----------------------------------------------------------------------
        //                          ON-SCREEN POSITION
        //  The following 'if' statement takes care of the variable X
        //  when it indicates an on-screen position (its value will always
        //  be less than 20 if this is the case.)
        //----------------------------------------------------------------------
		if ((lyrics[lyricPointer].x < 20) && (lyricDuration == 0))
            {
               // cout<<"\nOnScreen hit\n";
			foreText                =   "";

			if ((ink == 1) || (ink == 2))
                {
                backx               =   lyrics[lyricPointer].x;
                backText            =   lyrics[lyricPointer].text;
                }

			if ((ink == 3) || (ink == 4))
                {
                x                   =   lyrics[lyricPointer].x;
                foreText            =   lyrics[lyricPointer].text;
                }

            if (backInk == 1) lyricColour(59, 83, 255, 255);
            if (backInk == 2) lyricColour(54, 127, 40, 255);
            lyric(backx, backText);

            if (backText != "")
            {
                if (ink == 3) lyricColour(161, 238, 255, 255);
                if (ink == 4) lyricColour(192, 192, 192, 255);
                lyric(x, foreText);
            }

			lyricPointer++;
            }

        //----------------------------------------------------------------------
        //                              DELAY
        //  The following 'if' statement takes care of our delay. It calculates
        //  the elapsed time since the last call, and produces a ratio by which
        //  to multiply the resulting lyricDuration so that differences in
        //  processor speeds are accounted for.
        //----------------------------------------------------------------------
		if ((lyrics[lyricPointer].x > 20) && (lyrics[lyricPointer].x < 150000) && (lyricDuration == 0))
            {
            //std::cout << "\nDelayPoint hit\n";
            sf::Time  timeSinceLast =   clock1.getElapsedTime();
            sf::Time  timeGiven     =   sf::milliseconds(lyrics[lyricPointer].x);
            float     fSinceLast    =   timeSinceLast.asSeconds();
            //std::cout << fSinceLast << "\n";
            float     fGiven        =   timeGiven.asSeconds();
            float     fRatio        =   (fSinceLast / fGiven);
            //std::cout  << "fRatio = " << (1.0 - fRatio) << "\n";
            if ((1.0 - fRatio) > 0.50)
                {
                fSpeedCoefficient   =   1.0;
                }
            else if ((1.0 - fRatio) < 0)
                {
                fSpeedCoefficient   =   0.1;
                }
            else
                {
                fSpeedCoefficient   =   (1.0 - fRatio);
                }
            float     fConverted    =   (lyrics[lyricPointer].x * (fSpeedCoefficient));
            //cout << "\nfConverted = " << fConverted << "\n";
            lyricDuration           =   (int)fConverted;
            //std::cout<<"lyricDuration = " << lyricDuration << "\n\n";
			lyricPointer++;
            }
        }
    }






