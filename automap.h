#ifndef _automaph 
#define _automaph 

#include <string>

void automap();
void setAutoMapFlag(int mapno, int x, int y);
void clearAutoMaps();
void InitMap();
void DrawImage(int x,int y, int tileNo);
void DrawCell(int x,int y, int pixelx, int pixely);
void drawAutomap();
void drawFullAutomap();


 
// Your existing header content goes here 
 
#endif // _automaph 
