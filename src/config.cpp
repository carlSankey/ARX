
// Load settings from an arx.ini configuration file

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

extern int windowMode, graphicMode, windowWidth, windowHeight;

using std::ofstream;
using namespace std;
using std::cerr;
using std::endl;



bool loadConfig()
{
	std::ifstream instream;
	string filename = "arx.ini";
	instream.open(filename.c_str());
	if( !instream )
	{
		cerr << "Error: arx.ini could not be loaded" << endl;
		return 0;
	}
	string junk, line, text;
	int iniSettings = 4; // number of settings in the ini file
	string::size_type idx;
	getline(instream, junk); // read first line as blank

	for (int a = 0; a < iniSettings; ++a) // number of settings in the ini file
	{
		getline(instream, line);

		idx = line.find('=');
		text = line.substr(idx+2);

		if (a==0) { windowMode = atoi(text.c_str()); }
		if (a==1) { graphicMode = atoi(text.c_str()); }
		if (a==2) { windowWidth = atoi(text.c_str()); }
		if (a==3) { windowHeight = atoi(text.c_str()); }
	}
	instream.close();

	// Minimum window requirement is currently 640 x 480 pixels
	if ((windowWidth < 640) || (windowHeight < 480))
    {
        std::cout << "WARNING: A minimum window size of 640 x 480 pixels is required." << std::endl << std::endl;
        windowWidth = 640;
        windowHeight = 480;
    }
	return 1;
}




