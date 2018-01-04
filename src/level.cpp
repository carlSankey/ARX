
// Level handling routines

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "level.h"
#include "player.h"
#include "encounter.h"
#include "automap.h"

// extern Player plyr;

using std::ofstream;
using namespace std;
using std::cerr;
using std::endl;

//int display_row;
//int levelHeight = 64;
//int levelWidth = 64;

Map maps[6];
string descriptions[255];
string roomMessages[255];
zoneRect zones2[255]; // across full first dungeon level
Mapcell levelmap[4096]; // 4096 = 64 by 64 cells, 96x128 - 12288

FILE *fp;               // file pointer - used when reading files
char tempString[100];   // temporary string



zoneRecord zones[28]  =
{
	{9,10,11,3,61}, // Brown walls in Dungeon 1.2
	{4,5,6,8,60}, // Sewer
	{4,5,6,7,0}, // Well lit area
	{4,5,6,7,0}, //
	{12,13,14,18,0}, // 4 KJ Dangerous Area
	{4,5,6,7,0}, // 5 Taurian maze in 1.3
	{4,5,6,7,0}, // 6
	{4,5,6,7,0}, //7
	{4,5,6,7,0}, //8
	{4,5,6,7,0},  // 9
	{4,5,6,7,0}, // 10
	{16,17,2,18,0}, // mausoleum 11
	{12,13,2,15,64}, // goblin lair 12
	{12,13,2,19,63}, // troll lair 13
	{4,5,6,7,0}, // default 1.1, 1.3 & 1.4 14 (4 5 6 7)
	{20,20,20,7,0}, // dark wizards 15
	{21,22,23,24,0}, // light wizards 16
	{25,26,26,7,0}, // 17
	{25,26,26,34,40}, // City enclosed 1? 18
	{35,35,35,3,0}, // Wilderness 19
	{21,22,23,42,36}, // Dungeon level 2 default 20 was 42 & 36
	{43,44,0,45,62}, // Hall of the Adept
	{46,47,0,45,62}, // Hall of the Adept 2
	{48,48,49,50,0}, // Crystal caverns
	{51,51,51,52,0}, // Hall of mirrors
	{55,56,57,42,0}, // Level 3 of the Dungeon
	{58,59,58,8,0}, // Dragon's lair
	{65,66,67,61,52}, // Alien level 4
};

Teleport teleports[32] = {

{0xe0,0x12,0x1E,0,0xFF},
{0xe1,0x12,0x1B,0,0x03},
{0xe2,0x13,0x1B,0,0x01},
{0xe3,0x0F,0x17,0,0x00},
{0xe4,0x1D,0x17,0,0x00},
{0xe5,0x05,0x1A,0,0xFF},
{0xe6,0x14,0x1F,0,0x00},
{0xe7,0x10,0x01,0,0xFF},
{0xe8,0x0B,0x01,1,0xFF},
{0xe9,0x11,0x07,1,0xFF},
{0xea,0x11,0x0A,1,0xFF},
{0xeb,0x0B,0x1C,1,0xFF},
{0xec,0x0B,0x1F,1,0xFF},
{0xed,0x08,0x0F,1,0x80},
{0xee,0x12,0x09,2,0x80},
{0xef,0x00,0x01,2,0xFF},
{0xf0,0x00,0x1E,0,0xFF},
{0xf1,0x01,0x00,1,0xFF},
{0xf2,0x1E,0x00,0,0xFF},
{0xf3,0x1F,0x1E,1,0xFF},
{0xf4,0x1F,0x01,3,0xFF},
{0xf5,0x01,0x1F,3,0xFF},
{0xf6,0x1E,0x1F,2,0xFF},
{0xf7,0x00,0x12,5,0xFF},
{0xf8,0x0D,0x0F,5,0xFF},
{0xf9,0x03,0x01,3,0xFF},
{0xfa,0x03,0x1E,1,0xFF},
{0xfb,0x0B,0x00,4,0xFF},
{0xfc,0x16,0x07,1,0x02},
{0xfd,0x1A,0x05,1,0x00},
{0xfe,0x1C,0x07,1,0x02},
{0xff,0x18,0x1B,2,0x03}
};


void printSpecial();

void moveMapLevelTeleport()
{
	plyr.z_offset=1.0f;
	plyr.mapWidth = maps[plyr.map].width;
	plyr.mapHeight = maps[plyr.map].height;
	loadMapData(plyr.map);
	loadDescriptions(plyr.map);
	loadZoneData(plyr.map);
	loadMessages(plyr.map);
}


void moveMapLevel()
{
	if ((plyr.x==50) && (plyr.y==3) && (plyr.map==1)) // to the city from dungeon 1
	{
		plyr.x=49;
		plyr.y=3;
		plyr.facing=1;
		plyr.map=0; // The City
		plyr.scenario = 0; // The City
		plyr.z_offset=1.0f;
		loadMapData(0);
		loadDescriptions(0);
		//loadCityMonsters();
		loadZoneData(0);
		loadMessages(0);
	}

	if ((plyr.x==59) && (plyr.y==62) && (plyr.map==1)) // to the city from dungeon 1
	{
		plyr.x=59;
		plyr.y=63;
		plyr.facing=4;
		plyr.map=0; // The City
		plyr.scenario = 0; // The City
		plyr.z_offset=1.0f;
		loadMapData(0);
		loadDescriptions(0);
		//loadCityMonsters();
		//loadCityWeapons();
		loadZoneData(0);
		loadMessages(0);
	}

	if ((plyr.x==50)&&(plyr.y==3)&&(plyr.map==0)) // to the dungeon 1 from city
	{
		plyr.x=49;
		plyr.y=3;
		plyr.facing=1;
		plyr.map=1; // The Dungeon
		plyr.scenario = 1; // The Dungeon
		plyr.z_offset=1.0f;
		loadMapData(1);
		loadDescriptions(1);
		//loadEncounters();
		loadZoneData(1);
		loadMessages(1);
	}

	if ((plyr.x==59)&&(plyr.y==62)&&(plyr.map==0)) // to the dungeon 1 from city
	{
		plyr.x=59;
		plyr.y=61;
		plyr.facing=2;
		plyr.map=1; // The Dungeon
		plyr.scenario = 1; // The Dungeon
		plyr.z_offset=1.0f;
		loadMapData(1);
		loadDescriptions(1);
		//loadDungeonMonsters();
		loadZoneData(1);
		loadMessages(1);
	}

	if ((plyr.x==48)&&(plyr.y==48)&&(plyr.map==1)) // from dungeon 1 to dungeon 2 se fix
	{
		plyr.x=30;
		plyr.y=31;
		plyr.facing=1;
		plyr.map=2; // The Dungeon
		plyr.scenario = 1; // The Dungeon
		plyr.z_offset=1.0f;
		plyr.mapWidth=32;
		plyr.mapHeight=32;
		loadMapData(2);
		loadDescriptions(2);
		//loadDungeonMonsters();
		loadZoneData(2);
		loadMessages(2);
	}


	if ((plyr.x==31)&&(plyr.y==31)&&(plyr.map==2)) // from dungeon 2 to dungeon 1 se fix
	{
		plyr.x=47;
		plyr.y=48;
		plyr.facing=1;
		plyr.map=1; // The Dungeon
		plyr.scenario = 1; // The Dungeon
		plyr.z_offset=1.0f;
		plyr.mapWidth=64;
		plyr.mapHeight=64;
		loadMapData(1);
		loadDescriptions(1);
		//loadDungeonMonsters();
		loadZoneData(1);
		loadMessages(1);
	}


		if ((plyr.x==16)&&(plyr.y==48)&&(plyr.map==1)) // from dungeon 1 to dungeon 2 sw fix
	{
		plyr.x=1;
		plyr.y=31;
		plyr.facing=3;
		plyr.map=2; // The Dungeon
		plyr.scenario = 1; // The Dungeon
		plyr.z_offset=1.0f;
		plyr.mapWidth=32;
		plyr.mapHeight=32;
		loadMapData(2);
		loadDescriptions(2);
		//loadDungeonMonsters();
		loadZoneData(2);
		loadMessages(2);
	}


	if ((plyr.x==0)&&(plyr.y==31)&&(plyr.map==2)) // from dungeon 2 to dungeon 1 ne fix
	{
		plyr.x=17;
		plyr.y=48;
		plyr.facing=3;
		plyr.map=1; // The Dungeon
		plyr.scenario = 1; // The Dungeon
		plyr.z_offset=1.0f;
		plyr.mapWidth=64;
		plyr.mapHeight=64;
		loadMapData(1);
		loadDescriptions(1);
		//loadDungeonMonsters();
		loadZoneData(1);
		loadMessages(1);
	}

	if ((plyr.x==49)&&(plyr.y==17)&&(plyr.map==1)) // from dungeon 1 to dungeon 2 fix
	{
		plyr.x=30;
		plyr.y=0;
		plyr.facing=1;
		plyr.map=2; // The Dungeon
		plyr.scenario = 1; // The Dungeon
		plyr.z_offset=1.0f;
		plyr.mapWidth=32;
		plyr.mapHeight=32;
		loadMapData(2);
		loadDescriptions(2);
		//loadDungeonMonsters();
		loadZoneData(2);
		loadMessages(2);
	}

	if ((plyr.x==16)&&(plyr.y==17)&&(plyr.map==1)) // from dungeon 1 to dungeon 2 nw - fix
	{
		plyr.x=0;
		plyr.y=0;
		plyr.facing=2;
		plyr.map=2; // The Dungeon
		plyr.scenario = 1; // The Dungeon
		plyr.z_offset=1.0f;
		plyr.mapWidth=32;
		plyr.mapHeight=32;
		loadMapData(2);
		loadDescriptions(2);
		//loadDungeonMonsters();
		loadZoneData(2);
		loadMessages(2);
	}

	if ((plyr.x==0)&&(plyr.y==1)&&(plyr.map==2)) // from dungeon 2 to dungeon 1 nw fix
	{
		plyr.x=16;
		plyr.y=16;
		plyr.facing=2;
		plyr.map=1; // The Dungeon
		plyr.scenario = 1; // The Dungeon
		plyr.z_offset=1.0f;
		plyr.mapWidth=64;
		plyr.mapHeight=64;
		loadMapData(1);
		loadDescriptions(1);
		//loadDungeonMonsters();
		loadZoneData(1);
		loadMessages(1);
	}

	if ((plyr.x==31)&&(plyr.y==0)&&(plyr.map==2)) // from dungeon 2 to dungeon 1 ne fix
	{
		plyr.x=48;
		plyr.y=17;
		plyr.facing=1;
		plyr.map=1; // The Dungeon
		plyr.scenario = 1; // The Dungeon
		plyr.z_offset=1.0f;
		plyr.mapWidth=64;
		plyr.mapHeight=64;
		loadMapData(1);
		loadDescriptions(1);
		//loadDungeonMonsters();
		loadZoneData(1);
		loadMessages(1);
	}

	if ((plyr.x==17)&&(plyr.y==12)&&(plyr.map==2)) // from dungeon 2 to dungeon 3 fix
	{
		plyr.x=9;
		plyr.y=3;
		plyr.facing=1;
		plyr.map=3; // The Dungeon level 3
		plyr.scenario = 1; // The Dungeon
		plyr.z_offset=1.0f;
		plyr.mapWidth=32; // due to rooms of confusion
		plyr.mapHeight=32; // due to rooms of confusion
		loadMapData(3);
		loadDescriptions(3);
		//loadDungeonMonsters();
		loadZoneData(3); // temp
		loadMessages(2); // temp
	}

	if ((plyr.x==10)&&(plyr.y==3)&&(plyr.map==3)) // from dungeon 3 to dungeon 2
	{
		plyr.x=16;
		plyr.y=12;
		plyr.facing=1;
		plyr.map=2; // The Dungeon level 2
		plyr.scenario = 1; // The Dungeon
		plyr.z_offset=1.0f;
		plyr.mapWidth=32;
		plyr.mapHeight=32;
		loadMapData(2);
		loadDescriptions(2);
		//loadDungeonMonsters();
		loadZoneData(2); // temp
		loadMessages(2); // temp
	}

	if ((plyr.x==6)&&(plyr.y==15)&&(plyr.map==3)) // from dungeon 3 to dungeon 2
	{
		plyr.x=14;
		plyr.y=22;
		plyr.facing=2;
		plyr.map=2; // The Dungeon level 2
		plyr.scenario = 1; // The Dungeon
		plyr.z_offset=1.0f;
		plyr.mapWidth=32;
		plyr.mapHeight=32;
		loadMapData(2);
		loadDescriptions(2);
		//loadDungeonMonsters();
		loadZoneData(2); // temp
		loadMessages(2); // temp
	}

}


void loadDescriptions(int map)
{
	for (int i=0 ; i<255 ; i++) { descriptions[i]=""; }
	string filename = "data/map/"+(maps[map].filename)+"Descriptions.txt";
	std::ifstream instream;
	std::string line;
	instream.open(filename.c_str());
	if ( !instream )
	{
      cerr << "Error: " << filename << " file could not be loaded" << endl;
	}
	int i = 0;
	while (line!="EOF")
	{
		getline(instream, line);
		descriptions[i]= line;
		i++;

	}
	instream.close();
}


void loadMessages(int map)
{
	for (int i=0 ; i<100 ; i++) { roomMessages[i]=""; } // Fix 256 to actual message numbers - Smithy item corruption
	string filename = "data/map/"+(maps[map].filename)+"Messages.txt";
	std::ifstream instream;
	std::string line;
	instream.open(filename.c_str());
	if ( !instream )
	{
      cerr << "Error:" << filename << "could not be loaded" << endl;
	}
	int i = 0;
	while (line!="EOF")
	{
		getline(instream, line);
		roomMessages[i]= line;
		i++;
	}
	instream.close();
}


void initMaps()
{
	std::ifstream instream;
	std::string junk,line, text;

	instream.open("data/map/maps.txt");
	if( !instream )
	{
      cerr << "Error: MAPS.TXT file could not be loaded" << endl;
	}
	int i = 0;
	while (junk!="EOF")
	{
		//string junk, line, text;
		int fields = 5; // should be 45
		string::size_type idx;
		getline(instream, junk); // read first line as blank
		for (int a = 0; a < fields; ++a) // number of attributes - test for 33
		{
			getline(instream, line);
			//cout << line << "\n";
			idx = line.find('=');
			text = line.substr(idx+2);
			if (a==0) { maps[i].filename = text; }
			if (a==1) { maps[i].width = atoi(text.c_str()); }
			if (a==2) { maps[i].height = atoi(text.c_str()); }
			if (a==3) { maps[i].description = text; }
			if (a==4) { maps[i].background = atoi(text.c_str()); }
		}
		getline(instream, junk);
		//cout << "\n\n";
		i++;
	}
	instream.close();
}



void setCurrentZone()
{
	// x >= entry 3 x2
	// x < entry 1 x1
	// y >= entry 0 y1
	// y < entry 2 y2

	int identifiedZone = 99; // 99 used to represent not part of a zone - default
	int x = plyr.x;
	int y = plyr.y;

	for(int z = 0; z < 255; ++z) // 44
	{
		if ( (zones2[z].y1<=y) && (zones2[z].y2>y) && (zones2[z].x1>x) && (zones2[z].x2<=x) )
		{
			identifiedZone = z;
		}
	}

	plyr.zone = identifiedZone;
	plyr.zoneSet = zones2[identifiedZone].zoneRef; // set index for image set
	if (identifiedZone == 99)
	{
		plyr.zoneSet = 0; // set an image set of 0 for default / non zone members
		if ((x>=0) && (x<=31) && (y>=0) && (y<=31)) { plyr.zoneSet = 14; } // map section 1.1
		if ((x>=0) && (x<=31) && (y>=32) && (y<=63)) { plyr.zoneSet = 14; }  // map section 1.3
		if ((x>=32) && (x<=63) && (y>=32) && (y<=63)) { plyr.zoneSet = 14; }  // map section 1.4

		// Dungeon level 2 default
		if (plyr.map==2) { plyr.zoneSet = 20; }

		// Dungeon level 3 default
		if (plyr.map==3) { plyr.zoneSet = 25; } // 25

		// Dungeon level 4 default
		if (plyr.map==4) { plyr.zoneSet = 27; } // 25

		// City override
		if (plyr.map==0) { plyr.zoneSet = 17; }

		// Wilderness override
		if (plyr.map==5) { plyr.zoneSet = 19; }

	}

	//temp for city test
	//if (plyr.scenario==0) { plyr.zoneSet = 17; } // city walls

}



void saveHumanReadableMap()
{
	 ofstream outdata; // outdata is like cin
  outdata.open("data/map/dungeon4.txt"); // opens the file
   if( !outdata )
   { // file couldn't be opened
      cerr << "Error: map file could not be saved" << endl;
      exit(1);
   }


int z = 0; // Up to 4096 - current mapcell
for(int y = 0; y < 32; ++y)
{
	for(int x = 0; x < 32; ++x)
	{

		//levelmap[z].zone = 0;

		outdata << "# Map Cell Item: " << x << "," << y << endl;
		outdata << "East wall = " << levelmap[z].east << endl;
		outdata << "North wall = " << levelmap[z].north << endl;
		outdata << "West wall = " << levelmap[z].west << endl;
		outdata << "South wall = " << levelmap[z].south << endl;
		outdata << "East wall height = " << 1 << endl;
		outdata << "North wall height = " << 1 << endl;
		outdata << "West wall height = " << 1 << endl;
		outdata << "South wall height = " << 1 << endl;
		outdata << "Ceiling = " << 0 << endl;
		outdata << "Floor = " << 0 << endl;
		outdata << "Zone = " << 0 << endl;
		outdata << "Location = " << levelmap[z].location << endl;
		outdata << "Special = " << levelmap[z].special << endl;
		outdata << "      " << endl;


		//outdata << x << " " << y << " " << levelmap[z].east << " " << levelmap[z].north << " " << levelmap[z].west << " " << levelmap[z].south << " "
		//	<< levelmap[z].location << " " << levelmap[z].special << " " << endl;
		z++;
	}
}
   outdata.close();
}




void loadZoneData(int map)
{

	for (int a = 0; a < 255; ++a)
	{
		zones2[a].x1 = 0;
		zones2[a].x2 = 0;
		zones2[a].y1 = 0;
		zones2[a].y2 = 0;
		zones2[a].zoneRef = 0;
	}

	string filename = "data/map/"+(maps[map].filename)+"Zones.txt";
	std::ifstream instream;
	std::string junk,line, text;
	instream.open(filename.c_str());

	if( !instream )
	{
      cerr << "Error:" << filename << " could not be loaded" << endl;
	}
	int i = 0;
	while (junk!="EOF")
	{
		int fields = 5;
		string::size_type idx;
		getline(instream, junk); // read first line as blank
		for (int a = 0; a < fields; ++a) // number of attributes
		{
			getline(instream, line);
			idx = line.find('=');
			text = line.substr(idx+2);
			if (a==0) { zones2[i].y1 = atoi(text.c_str()); }
			if (a==1) { zones2[i].x1 = atoi(text.c_str()); }
			if (a==2) { zones2[i].y2 = atoi(text.c_str()); }
			if (a==3) { zones2[i].x2 = atoi(text.c_str()); }
			if (a==4) { zones2[i].zoneRef = atoi(text.c_str()); }
		}
		getline(instream, junk);
		i++;
	}
	instream.close();
}


void loadMapData(int map)
{
	std::ifstream instream;
	string filename = "data/map/"+(maps[map].filename)+"Cells.txt";
	instream.open(filename.c_str());
	if( !instream )
	{
		cerr << "Error: terrain file could not be loaded" << endl;
	}
	int totalMapCells = plyr.mapWidth*plyr.mapHeight;
	for(int i = 0; i < totalMapCells; ++i)
	{
		string junk, line, text;
		int mapCell_attributes = 13; // should be 12
		string::size_type idx;
		getline(instream, junk); // read first line as blank

		for (int a = 0; a < mapCell_attributes; ++a) // number of attributes - test for 33
		{
			getline(instream, line);

			idx = line.find('=');
			text = line.substr(idx+2);
			//if (a==0) cout << text << " = " << i << ",\n";
			//cout << a << ", " << text << "\n";
			if (a==0) { levelmap[i].east		= atoi(text.c_str()); }
			if (a==1) { levelmap[i].north		= atoi(text.c_str()); }
			if (a==2) { levelmap[i].west		= atoi(text.c_str()); }
			if (a==3) { levelmap[i].south		= atoi(text.c_str()); }
			if (a==4) { levelmap[i].eastHeight	= atoi(text.c_str()); }
			if (a==5) { levelmap[i].northHeight = atoi(text.c_str()); }
			if (a==6) { levelmap[i].westHeight	= atoi(text.c_str()); }
			if (a==7) { levelmap[i].southHeight = atoi(text.c_str()); }
			if (a==8) { levelmap[i].ceiling		= atoi(text.c_str()); }
			if (a==9) { levelmap[i].floor		= atoi(text.c_str()); }
			if (a==10) { levelmap[i].zone		= atoi(text.c_str()); }
			if (a==11) { levelmap[i].location	= atoi(text.c_str()); }
			if (a==12) { levelmap[i].special	= atoi(text.c_str()); }

		}
		getline(instream, junk);
		//cout << "\n\n";
	}
	instream.close();
	//printSpecial(); // generate special lists
}



int getMapIndex(int x,int y)
{
    int tmpIndex = (y*plyr.mapWidth)+x;
    return tmpIndex;
}



void transMapIndex (int idx)
{
     int facing = plyr.facing;
//	 plyr.current_zone = levelmap[idx].zone;

     if (facing == 1) // facing west?
     {
        plyr.back = levelmap[idx].east;
        plyr.left = levelmap[idx].south;
        plyr.right = levelmap[idx].north;
        plyr.front = levelmap[idx].west;
		plyr.leftheight = levelmap[idx].southHeight;
        plyr.rightheight = levelmap[idx].northHeight;
        plyr.frontheight = levelmap[idx].westHeight;

     }

     if (facing == 2) // facing north?
     {
        plyr.back = levelmap[idx].south;
        plyr.left = levelmap[idx].west;
        plyr.right = levelmap[idx].east;
        plyr.front = levelmap[idx].north;
		plyr.leftheight = levelmap[idx].westHeight;
        plyr.rightheight = levelmap[idx].eastHeight;
        plyr.frontheight = levelmap[idx].northHeight;

     }

     if (facing == 3) // facing east?
     {
        plyr.back = levelmap[idx].west;
        plyr.right = levelmap[idx].south;
        plyr.left = levelmap[idx].north;
        plyr.front = levelmap[idx].east;
		plyr.rightheight = levelmap[idx].southHeight;
        plyr.leftheight = levelmap[idx].northHeight;
        plyr.frontheight = levelmap[idx].eastHeight;
     }

     if (facing == 4) // facing south?
     {
        plyr.back = levelmap[idx].north;
        plyr.right = levelmap[idx].west;
        plyr.left = levelmap[idx].east;
        plyr.front = levelmap[idx].south;
		plyr.rightheight = levelmap[idx].westHeight;
        plyr.leftheight = levelmap[idx].eastHeight;
        plyr.frontheight = levelmap[idx].southHeight;
     }

	 plyr.specialwall = levelmap[idx].special;
	 plyr.floorTexture = levelmap[idx].floor;
	 plyr.ceiling = levelmap[idx].ceiling;

}

void loadBinaryLevel()
{
    // Define an array of mapcells used to hold map data converted from binary data
    sprintf(tempString,"%s%s","data/map/","dun4.bin"); //dungeon1
    fp = fopen(tempString, "rb");
    if( fp != NULL )
    {
         for(int i=0;i<4096;i++)
         {
          int tmp = fgetc(fp);
		  if ( tmp==2) { cout << tmp <<" , " << i << "\n"; }
          levelmap[i].east = (tmp&240)>>4;
          levelmap[i].north = tmp&15;
          tmp = fgetc(fp);
          levelmap[i].west = (tmp&240)>>4;
          levelmap[i].south = tmp&15;
          tmp = fgetc(fp);
          levelmap[i].location = tmp;
          tmp = fgetc(fp);
          levelmap[i].special = tmp;
         }
    }
    fclose(fp);

}

void printSpecial()
{
	// 80 - 9F for encounters

	//plyr.mapWidth*plyr.mapHeight
	for(int y=0;y<plyr.mapHeight;y++)
    {
		for(int x=0;x<plyr.mapWidth;x++)
		{
			int mapIdx = getMapIndex(x,y);
			if ((levelmap[mapIdx].special>0x79) && (levelmap[mapIdx].special<0x9F))
				{
					cout << dec << x << "," << y << ",";
					cout << hex << levelmap[mapIdx].special << ",\n";
				}
		}
	}
}
