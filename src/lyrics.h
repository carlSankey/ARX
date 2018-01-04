#include <string>

void initLyricFont();
void lyricColour(int r,int g, int b, int a);
void drawLyricChar(int x, int char_no);
void lyric(int x, std::string text); // draw string at x,y
void loadLyrics(string filename);
void updateLyrics();


