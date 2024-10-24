
#ifndef _levelh 
#define _levelh 

#include <string>
#include <vector>

void loadBinaryLevel();

int getMapIndex(int x,int y);
//void transMapIndex (int idx);
//void updateViewWindow();

void initMaps();
void transMapIndex (int idx);
void setCurrentZone();
//int getZone(int x,int y);

void saveHumanReadableMap();
void loadMapData(int map);
void loadDescriptions(int map);
void loadMessages(int map);
void loadZoneData(int map);
void moveMapLevel();
void moveMapLevelTeleport();
size_t readEncounterData(int map);

void clearArray(std::string roomMessages[], int size);

struct Teleport
{
 int ref;
 int new_x;
 int new_y;
 int new_map;
 int new_facing;
};


struct Mapcell
{
 int north;
 int south;
 int west;
 int east;
 int northHeight;
 int southHeight;
 int westHeight;
 int eastHeight;
 int ceiling;
 int floor;
 int zone;
 int location;
 int special;
};

struct zoneRect
{
 int y1;
 int x1;
 int y2;
 int x2;
 int zoneRef;
};

struct Map
{
 std::string filename;
 int width;
 int height;
 std::string description;
 int background;
 int encounterIndex;
};

struct zoneRecord
{
 int wall;
 int door;
 int arch;
 int floor;
 int ceiling;
 //int background; // if background != 0 then single background used for all 4 directions
 //int backNorth;
 //int backSouth;
 //int backWest;
 //int backEast;
 //bool hasCeiling;
 //bool hasLight;
 //bool hasAntiMagic;
};


struct MapEncounter
{

    int Zone;
    int Encounter;
    int Probability;
    int TimeOfDay;

};
std::vector<MapEncounter> readMapEncounterCSV(const std::string& filename);

extern zoneRecord zones[28];
extern Map maps[];
 
// Your existing header content goes here 
 
#endif // _levelh 
