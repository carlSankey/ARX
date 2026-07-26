#include "automap.h"
#include "display.h"
#include "font.h"
#include "player.h"
#include "level.h"
#include "misc.h"
#include "globals.h"

extern bool autoMapExplored[5][4096];
extern Mapcell levelmap[4096]; // 12288

#ifdef ARX_USE_SDL2
// ============================================================
// SDL2/web implementations using arx::Sprite2D
// ============================================================

#include "renderer/Sprite2D.h"

extern int miniMapX;
extern int miniMapY;

int pixelSize;
int mapLocation;
bool autoMapExplored[5][4096] = {};

// Map tile sprites
static arx::Sprite2D s_mapTiles;
static bool s_mapTilesLoaded = false;
static arx::Sprite2D s_mapLegend;
static bool s_mapLegendLoaded = false;

void automap()
{
    plyr.status = 0;
    bool mapComplete = false;

    while (!mapComplete)
    {
        clearDisplay();
        drawFullAutomap();
        updateDisplay();

        std::string single_key = getSingleKey();
        if (single_key == "SPACE") { mapComplete = true; }
        if (single_key == "RETURN") { mapComplete = true; }
        if (single_key == "M") { mapComplete = true; }
        if (single_key == "ESC") { mapComplete = true; }
    }
    plyr.status = 1;
}

void clearAutoMaps()
{
    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 4096; x++)
        {
            autoMapExplored[y][x] = false;
        }
    }
}

void InitMap()
{
    // Load map tiles spritesheet
    if (!s_mapTilesLoaded) {
        s_mapTilesLoaded = s_mapTiles.load("data/images/core/maptiles.png");
        if (!s_mapTilesLoaded) {
            std::cerr << "InitMap: failed to load maptiles.png" << std::endl;
        }
    }
    
    // Load legend image for current scenario
    if (!s_mapLegendLoaded) {
        std::string legendPath = "data/images/Scenario_" + std::to_string(plyr.scenario) + "/Legend.png";
        s_mapLegendLoaded = s_mapLegend.load(legendPath);
        if (!s_mapLegendLoaded) {
            std::cerr << "InitMap: failed to load legend from " << legendPath << std::endl;
        }
    }
}

void DrawImage(int x, int y, int tileNo)
{
    if (!s_mapTiles.isValid()) return;
    
    x++;
    y++;
    int row, column;
    int tilesPerRow = 8;
    int tileSize = 16;

    if (tileNo >= tilesPerRow)
    {
        column = (tileNo % tilesPerRow);
        row = ((tileNo - column) / tilesPerRow);
    }
    else
    {
        column = tileNo;
        row = 0;
    }

    int tileX = column * tileSize;
    int tileY = row * tileSize;
    s_mapTiles.setTextureRect(tileX, tileY, tileSize, tileSize);
    s_mapTiles.draw(static_cast<float>(x), static_cast<float>(y));
}

void DrawCell(int x, int y, int pixelx, int pixely)
{
    int idx = getMapIndex(x, y);
    int north = levelmap[idx].north;
    int west = levelmap[idx].west;
    int east = levelmap[idx].east;
    int south = levelmap[idx].south;
    int special = levelmap[idx].special;
    int tile;

    if (autoMapExplored[plyr.map][mapLocation])
    {
        DrawImage(pixelx, pixely, 0);
        if (special == 144) DrawImage(pixelx, pixely, 13);
        if (special == 21) DrawImage(pixelx, pixely, 13);
        if (special == 112) DrawImage(pixelx, pixely, 12);
        if (special == 80) DrawImage(pixelx, pixely, 9);
        if (special == 16) DrawImage(pixelx, pixely, 10);
        if (special == 48) DrawImage(pixelx, pixely, 11);
        if ((special == 208) && (plyr.scenario == 0)) DrawImage(pixelx, pixely, 25);
        if (special == 3) DrawImage(pixelx, pixely, 11);
        if (special == 35) DrawImage(pixelx, pixely, 11);
        if ((special == 0xF0) && (plyr.scenario == 0)) DrawImage(pixelx, pixely, 9);
        if (special == 0x0F) DrawImage(pixelx, pixely, 10);
        if (special == 0x1D) DrawImage(pixelx, pixely, 11);
        if (special == 0x0C) DrawImage(pixelx, pixely, 9);
        if (special == 0x0D) DrawImage(pixelx, pixely, 12);
        if (special == 7) DrawImage(pixelx, pixely, 11);
        if (special == 19) DrawImage(pixelx, pixely, 25);
        if (special == 87) DrawImage(pixelx, pixely, 25);
    }

    if (!autoMapExplored[plyr.map][mapLocation]) DrawImage(pixelx, pixely, 24);

    tile = 0;
    if (north == 3 || north == 4 || north == 8 || north == 9 || north == 10 || north > 19) tile = 8;
    else if (north == 5 || north == 6) tile = 4;
    else if (north == 13 || north == 37) tile = 4;
    else if (north == 14) tile = 4;
    if (tile != 0) DrawImage(pixelx, pixely, tile);

    tile = 0;
    if (south == 3 || south == 4 || south == 8 || south == 9 || south == 10 || south > 19) tile = 6;
    else if (south == 5 || south == 6) tile = 2;
    else if (south == 13 || south == 37) tile = 2;
    else if (south == 14) tile = 2;
    if (tile != 0) DrawImage(pixelx, pixely, tile);

    tile = 0;
    if (west == 3 || west == 4 || west == 8 || west == 9 || west == 10 || west > 19) tile = 5;
    else if (west == 5 || west == 6) tile = 1;
    else if (west == 13 || west == 37) tile = 1;
    else if (west == 14) tile = 1;
    if (tile != 0) DrawImage(pixelx, pixely, tile);

    tile = 0;
    if (east == 3 || east == 4 || east == 8 || east == 9 || east == 10 || east > 19) tile = 7;
    else if (east == 5 || east == 6) tile = 3;
    else if (east == 13 || east == 37) tile = 3;
    else if (east == 14) tile = 3;
    if (tile != 0) DrawImage(pixelx, pixely, tile);
}

void drawAutomap()
{
    if (!plyr.miniMapOn) return;

    if ((graphicMode == ALTERNATE_LARGE) && (plyr.status != 2))
    {
        // Draw minimap background box using text
        int boxX = miniMapX / 8;
        int boxY = miniMapY / 8;
        SetFontColour(255, 255, 0, 255);
        drawText(boxX, boxY, "┌────────┐");
        drawText(boxX, boxY + 1, "│AUTOMAP │");
        drawText(boxX, boxY + 2, "│        │");
        drawText(boxX, boxY + 3, "└────────┘");
        SetFontColour(215, 215, 215, 255);
    }

    pixelSize = 16;
    int automapHeight = 9;
    int automapWidth = 9;
    int startx = plyr.x - ((automapWidth - 1) / 2);
    int starty = plyr.y - ((automapHeight - 1) / 2);

    for (int y = 0; y < automapHeight; y++)
    {
        for (int x = 0; x < automapWidth; x++)
        {
            int currentx = startx + x;
            int currenty = starty + y;
            if ((currentx >= 0) && (currentx < plyr.mapWidth) && (currenty >= 0) && (currenty < plyr.mapHeight))
            {
                int pixelx = miniMapX + (x * pixelSize);
                int pixely = miniMapY + (y * pixelSize);
                mapLocation = getMapIndex(currentx, currenty);
                DrawCell(currentx, currenty, pixelx, pixely);
                if (!autoMapExplored[plyr.map][mapLocation]) DrawImage(pixelx, pixely, 24);
            }
        }
    }

    int pixelx = miniMapX + (((automapWidth - 1) / 2) * pixelSize);
    int pixely = miniMapY + (((automapHeight - 1) / 2) * pixelSize);
    if (plyr.facing == WEST) DrawImage(pixelx, pixely, 17);
    if (plyr.facing == NORTH) DrawImage(pixelx, pixely, 14);
    if (plyr.facing == EAST) DrawImage(pixelx, pixely, 16);
    if (plyr.facing == SOUTH) DrawImage(pixelx, pixely, 15);
}

void drawFullAutomap()
{
    plyr.drawingBigAutomap = true;
    pixelSize = 16;
    int automapHeight = 32;
    int automapWidth = 32;
    int cornerX = 0;
    int cornerY = 0;
    int startx, starty;

    if ((plyr.x < 32) && (plyr.y < 32)) { startx = 0; starty = 0; }
    else if ((plyr.x > 31) && (plyr.y < 32)) { startx = 32; starty = 0; }
    else if ((plyr.x > 31) && (plyr.y > 31)) { startx = 32; starty = 32; }
    else { startx = 0; starty = 32; }

    for (int y = 0; y < automapHeight; y++)
    {
        for (int x = 0; x < automapWidth; x++)
        {
            int currentx = startx + x;
            int currenty = starty + y;
            int pixelx = cornerX + (x * pixelSize);
            int pixely = cornerY + (y * pixelSize);
            mapLocation = getMapIndex(currentx, currenty);
            DrawCell(currentx, currenty, pixelx, pixely);
            if (!autoMapExplored[plyr.map][mapLocation]) DrawImage(pixelx, pixely, 24);
        }
    }

    int pixelx = (plyr.x < 32) ? plyr.x * pixelSize : (plyr.x - 32) * pixelSize + 16;
    int pixely = (plyr.y < 32) ? plyr.y * pixelSize : (plyr.y - 32) * pixelSize;
    if (plyr.facing == WEST) DrawImage(pixelx, pixely, 17);
    if (plyr.facing == NORTH) DrawImage(pixelx, pixely, 14);
    if (plyr.facing == EAST) DrawImage(pixelx, pixely, 16);
    if (plyr.facing == SOUTH) DrawImage(pixelx, pixely, 15);

    // Draw legend in top-right corner
    if (s_mapLegend.isValid()) {
        s_mapLegend.draw(512.0f + 16.0f, 16.0f);
    }
    plyr.drawingBigAutomap = false;
}

void setAutoMapFlag(int mapno, int x, int y)
{
    int cellNo = getMapIndex(x, y);
    autoMapExplored[mapno][cellNo] = true;
}

#else
// ============================================================
// SFML implementation (native build only)
// ============================================================
#include <SFML/Graphics.hpp>

extern sf::RenderWindow App;

static sf::Texture mapImage;
static sf::Texture legendImage;
static std::unique_ptr<sf::Sprite> cellImage;
static std::unique_ptr<sf::Sprite> mapLegend;

extern int miniMapY;
extern int miniMapX;
extern int graphicMode;

int pixelSize;
int mapLocation;

void automap()
{
	plyr.status=0;
	bool mapComplete = false;

	while (!mapComplete)
	{
	string single_key;
	clearDisplay();
	drawFullAutomap();
	updateDisplay();

	single_key = getSingleKey();
	if (single_key=="SPACE") { mapComplete = true; }
	if (single_key=="RETURN") { mapComplete = true; }
	if (single_key=="M") { mapComplete = true; }
	if (single_key=="ESC") { mapComplete = true; }
	}
	plyr.status=1;
}

void clearAutoMaps()
{
	for (int y=0 ; y<5 ; y++)
	{
		for (int x=0 ; x<4096 ; x++)
		{
			autoMapExplored[y][x] = false;
		}
	}
}

void InitMap()
{
	if (!mapImage.loadFromFile("data/images/core/maptiles.png")) {}
	cellImage = std::make_unique<sf::Sprite>(mapImage);

	if (!legendImage.loadFromFile("data/images/Scenario_" + std::to_string(plyr.scenario) + "/Legend.png")) {}
	mapLegend = std::make_unique<sf::Sprite>(legendImage);
}

void DrawImage(int x, int y, int tileNo)
{
	x++;
	y++;
	int row, column;
	int tilesPerRow = 8;
	int tileSize = 16;

	if (tileNo >= tilesPerRow)
	{
		column = (tileNo % tilesPerRow);
		row = ((tileNo - column) / tilesPerRow);
	}
	else
	{
		column = tileNo;
		row = 0;
	}

	int tileX = column * tileSize;
	int tileY = row * tileSize;
	cellImage->setTextureRect(sf::IntRect({tileX, tileY}, {tileSize, tileSize}));
	cellImage->setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
	App.draw(*cellImage);
}

void DrawCell(int x, int y, int pixelx, int pixely)
{
	int idx = getMapIndex(x, y);
	int north = levelmap[idx].north;
	int west = levelmap[idx].west;
	int east = levelmap[idx].east;
	int south = levelmap[idx].south;
	int special = levelmap[idx].special;
	int tile;

	if (autoMapExplored[plyr.map][mapLocation])
	{
		DrawImage(pixelx, pixely, 0);
		if (special == 144) DrawImage(pixelx, pixely, 13);
		if (special == 21) DrawImage(pixelx, pixely, 13);
		if (special == 112) DrawImage(pixelx, pixely, 12);
		if (special == 80) DrawImage(pixelx, pixely, 9);
		if (special == 16) DrawImage(pixelx, pixely, 10);
		if (special == 48) DrawImage(pixelx, pixely, 11);
		if ((special == 208) && (plyr.scenario == 0)) DrawImage(pixelx, pixely, 25);
		if (special == 3) DrawImage(pixelx, pixely, 11);
		if (special == 35) DrawImage(pixelx, pixely, 11);
		if ((special == 0xF0) && (plyr.scenario == 0)) DrawImage(pixelx, pixely, 9);
		if (special == 0x0F) DrawImage(pixelx, pixely, 10);
		if (special == 0x1D) DrawImage(pixelx, pixely, 11);
		if (special == 0x0C) DrawImage(pixelx, pixely, 9);
		if (special == 0x0D) DrawImage(pixelx, pixely, 12);
		if (special == 7) DrawImage(pixelx, pixely, 11);
		if (special == 19) DrawImage(pixelx, pixely, 25);
		if (special == 87) DrawImage(pixelx, pixely, 25);
	}

	if (!autoMapExplored[plyr.map][mapLocation]) DrawImage(pixelx, pixely, 24);

	tile = 0;
	if (north == 3 || north == 4 || north == 8 || north == 9 || north == 10 || north > 19) tile = 8;
	else if (north == 5 || north == 6) tile = 4;
	else if (north == 13 || north == 37) tile = 4;
	else if (north == 14) tile = 4;
	if (tile != 0) DrawImage(pixelx, pixely, tile);

	tile = 0;
	if (south == 3 || south == 4 || south == 8 || south == 9 || south == 10 || south > 19) tile = 6;
	else if (south == 5 || south == 6) tile = 2;
	else if (south == 13 || south == 37) tile = 2;
	else if (south == 14) tile = 2;
	if (tile != 0) DrawImage(pixelx, pixely, tile);

	tile = 0;
	if (west == 3 || west == 4 || west == 8 || west == 9 || west == 10 || west > 19) tile = 5;
	else if (west == 5 || west == 6) tile = 1;
	else if (west == 13 || west == 37) tile = 1;
	else if (west == 14) tile = 1;
	if (tile != 0) DrawImage(pixelx, pixely, tile);

	tile = 0;
	if (east == 3 || east == 4 || east == 8 || east == 9 || east == 10 || east > 19) tile = 7;
	else if (east == 5 || east == 6) tile = 3;
	else if (east == 13 || east == 37) tile = 3;
	else if (east == 14) tile = 3;
	if (tile != 0) DrawImage(pixelx, pixely, tile);
}

void drawAutomap()
{
	if (!plyr.miniMapOn) return;

	if ((graphicMode == ALTERNATE_LARGE) && (plyr.status != 2))
	{
		sf::RectangleShape rectangle2;
		rectangle2.setSize(sf::Vector2f(176, 176));
		rectangle2.setOutlineColor(sf::Color::Yellow);
		rectangle2.setFillColor(sf::Color(0, 0, 0, 128));
		rectangle2.setOutlineThickness(1);
		rectangle2.setPosition(sf::Vector2f(static_cast<float>(miniMapX + 1), static_cast<float>(miniMapY + 1)));
		App.draw(rectangle2);
	}

	pixelSize = 16;
	int automapHeight = 9;
	int automapWidth = 9;
	int startx = plyr.x - ((automapWidth - 1) / 2);
	int starty = plyr.y - ((automapHeight - 1) / 2);

	for (int y = 0; y < automapHeight; y++)
	{
		for (int x = 0; x < automapWidth; x++)
		{
			int currentx = startx + x;
			int currenty = starty + y;
			if ((currentx >= 0) && (currentx < plyr.mapWidth) && (currenty >= 0) && (currenty < plyr.mapHeight))
			{
				int pixelx = miniMapX + (x * pixelSize);
				int pixely = miniMapY + (y * pixelSize);
				mapLocation = getMapIndex(currentx, currenty);
				DrawCell(currentx, currenty, pixelx, pixely);
				if (!autoMapExplored[plyr.map][mapLocation]) DrawImage(pixelx, pixely, 24);
			}
		}
	}

	int pixelx = miniMapX + (((automapWidth - 1) / 2) * pixelSize);
	int pixely = miniMapY + (((automapHeight - 1) / 2) * pixelSize);
	if (plyr.facing == WEST) DrawImage(pixelx, pixely, 17);
	if (plyr.facing == NORTH) DrawImage(pixelx, pixely, 14);
	if (plyr.facing == EAST) DrawImage(pixelx, pixely, 16);
	if (plyr.facing == SOUTH) DrawImage(pixelx, pixely, 15);
}

void drawFullAutomap()
{
	plyr.drawingBigAutomap = true;
	pixelSize = 16;
	int automapHeight = 32;
	int automapWidth = 32;
	int cornerX = 0;
	int cornerY = 0;
	int startx, starty;

	if ((plyr.x < 32) && (plyr.y < 32)) { startx = 0; starty = 0; }
	else if ((plyr.x > 31) && (plyr.y < 32)) { startx = 32; starty = 0; }
	else if ((plyr.x > 31) && (plyr.y > 31)) { startx = 32; starty = 32; }
	else { startx = 0; starty = 32; }

	for (int y = 0; y < automapHeight; y++)
	{
		for (int x = 0; x < automapWidth; x++)
		{
			int currentx = startx + x;
			int currenty = starty + y;
			int pixelx = cornerX + (x * pixelSize);
			int pixely = cornerY + (y * pixelSize);
			mapLocation = getMapIndex(currentx, currenty);
			DrawCell(currentx, currenty, pixelx, pixely);
			if (!autoMapExplored[plyr.map][mapLocation]) DrawImage(pixelx, pixely, 24);
		}
	}

	int pixelx = (plyr.x < 32) ? plyr.x * pixelSize : (plyr.x - 32) * pixelSize + 16;
	int pixely = (plyr.y < 32) ? plyr.y * pixelSize : (plyr.y - 32) * pixelSize;
	if (plyr.facing == WEST) DrawImage(pixelx, pixely, 17);
	if (plyr.facing == NORTH) DrawImage(pixelx, pixely, 14);
	if (plyr.facing == EAST) DrawImage(pixelx, pixely, 16);
	if (plyr.facing == SOUTH) DrawImage(pixelx, pixely, 15);

	mapLegend->setPosition(sf::Vector2f(512 + 16, 16));
	App.draw(*mapLegend);
	plyr.drawingBigAutomap = false;
}

void setAutoMapFlag(int mapno, int x, int y)
{
	int cellNo = getMapIndex(x, y);
	autoMapExplored[mapno][cellNo] = true;
}

#endif // ARX_USE_SDL2