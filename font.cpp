#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>
#include "player.h"
#include "font.h"
#include "renderer/Sprite2D.h"

extern int consoleY;
extern int consoleX;
extern int statPanelY;
extern int shopConsoleY;
extern int shopStatsY;
extern float uiScale;

arx::Sprite2D FontImage;

extern int charYBase;
int yBase = 0;

void InitFont()
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current Working Directory: " << currentPath.string() << std::endl;

    std::string fontPath = "data/images/core/";
    if (plyr.fontStyle == 0) {
        fontPath += "arfontSmooth.png";
    } else {
        fontPath += "arfont.png";
    }
    if (!FontImage.load(fontPath)) {
        std::cerr << "InitFont: failed to load " << fontPath << std::endl;
    }
}

void releaseFontResources()
{
    // Sprite2D cleanup handled by destructor
}

void SetFontColour(int r, int g, int b, int a)
{
    FontImage.setColor(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), static_cast<uint8_t>(a));
}

void DrawChar(int topY, int x, int y, int initchar_no)
{
    int char_no;
    char_no = initchar_no - 32;

    int row, column;
    int charsPerRow = 16;

    if (char_no > charsPerRow)
    {
        column = (char_no % charsPerRow);
        row = ((char_no - column) / charsPerRow);
    }
    else
    {
        column = char_no;
        row = 0;
    }

    int charX = (column) * 16;
    int charY = ((row) * 16);

    FontImage.setTextureRect(charX, charY, 16, 16);
    if (char_no == 16) {
        FontImage.setTextureRect(0, 16, 16, 16);
    }

    float drawX = static_cast<float>(consoleX + static_cast<int>((x - 1) * 16 * uiScale));
    float drawY = static_cast<float>(topY + static_cast<int>(y * 18 * uiScale));

    FontImage.draw(drawX, drawY, uiScale, uiScale);
}

void bText(int x, int y, std::string text)
{
    int strlen = text.size();
    for (int i = 0; i < strlen; ++i)
    {
        char current_char = text.at(i);
        int char_no = (int(current_char));
        if (plyr.status == 2) { DrawChar(shopConsoleY, x, y, char_no); }
        else { DrawChar(consoleY, x, y, char_no); }
        x++;
    }
}

void drawText(int x, int y, std::string text)
{
    int strlen = text.size();
    for (int i = 0; i < strlen; ++i)
    {
        char current_char = text.at(i);
        int char_no = (int(current_char));
        if (plyr.status == 2) { DrawChar(shopStatsY, x, y, char_no); }
        else { DrawChar(statPanelY, x, y, char_no); }
        x++;
    }
}

void drawText(int area, int x, int y, std::string text)
{
    int strlen = text.size();
    for (int i = 0; i < strlen; ++i)
    {
        char current_char = text.at(i);
        int char_no = (int(current_char));

        if (plyr.status == 2) DrawChar(shopConsoleY, x, y, char_no);
        else DrawChar(consoleY, x, y, char_no);
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
    for (int i = 0; i < strlen; ++i)
    {
        char current_char = text.at(i);
        int char_no = (int(current_char));

        if (plyr.status == 2) { DrawChar(shopStatsY, x, y, char_no); }
        else { DrawChar(statPanelY, x, y, char_no); }
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
    for (int i = 0; i < strlen; ++i)
    {
        char current_char = text.at(i);
        int char_no = (int(current_char));
        if (plyr.status == 2) { DrawChar(shopStatsY, x, y, char_no); }
        else { DrawChar(consoleY, x, y, char_no); }
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
    current_string = "";
    y = 1;
    while (char_count <= string_length)
    {
        current_char = str.substr(char_count, 1);

        if ((char_count == string_length) || (current_char == "@"))
        {
            current_string_length = current_string.size();
            x = ((40 - current_string_length) / 2) + 1;

            drawText(0, x, y, current_string);

            y++;
            char_count++;
            current_string = "";
        }
        else
        {
            current_string = current_string + current_char;
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
    current_string = "";

    while (char_count <= string_length)
    {
        current_char = str.substr(char_count, 1);

        if ((char_count == string_length) || (current_char == "@"))
        {
            current_string_length = current_string.size();
            x = ((40 - current_string_length) / 2) + 1;

            drawText(consoleY, x, y, current_string);

            y++;
            char_count++;
            current_string = "";
        }
        else
        {
            current_string = current_string + current_char;
            char_count++;
        }
    }
}

/* Seem to only be used in item.h */

void text(int x, int y, std::string text)
{
    int strlen = text.size();
    for (int i = 0; i < strlen; ++i)
    {
        char current_char = text.at(i);
        int char_no = (int(current_char));

        DrawChar(16, x, y, char_no);
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
    for (int i = 0; i < strlen; ++i)
    {
        char current_char = text.at(i);
        int char_no = (int(current_char));

        DrawChar(16, x, y, char_no);
        x++;
    }
}