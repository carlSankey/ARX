#include <string>

void InitFont();
void DrawChar(int topY, int x,int y, int char_no);
void drawText(int x, int y, std::string text); // draw string at x,y
void drawText(int x, int y, int number); // draw number at x,y
void drawText(int area, int x, int y, std::string text);
void bText(int x, int y, std::string text); // draw string at base (y=17) x,y
void bText(int x, int y, int number); // draw number at base (y=17) x,y
void cText(std::string str); // draw message string at base of screen centred
void cyText(int y, std::string str); // draw message string at base of screen centred
void SetFontColour(int r,int g, int b, int a);

void text(int x, int y, std::string text); // draw text at x,y onto App window
void text(int x, int y, int number); // draw number at x,y onto App window
