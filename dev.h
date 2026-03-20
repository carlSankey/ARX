#ifndef _devh 
#define _devh 

/* dev.h
 * Developer Settings to speed up development and testing
 */

 enum OnOff
 {
    OFF,
    ON
 };

struct devSettings
{
    bool CHARACTER_CREATION;         // Skip the full character creation sequence for the City and the Dungeon
    bool TELEPORT_OPTION;            // Option for teleporting around map
};

void setDeveloperFlags();

extern devSettings AR_DEV;

 
// Your existing header content goes here 
 
#endif // _devh 

