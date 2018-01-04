#include <SFML/Graphics.hpp>

#include "automap.h"
#include "display.h"
#include "font.h"
#include "player.h"
#include "level.h"
#include "misc.h"

extern bool autoMapExplored[5][4096];
extern Mapcell levelmap[4096]; // 12288

sf::Texture mapImage, legendImage;
sf::Sprite cellImage;

sf::Sprite mapLegend;

extern int miniMapY; // y position for displaying the bottom screen info panel
extern int miniMapX; // x starting position for displaying the panel for centering
extern int graphicMode;

//float scale;
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
	if (plyr.scenario==0)
	{
            /*
			text(32, 0, "The City");
			text(35, 4, "Smithy");
			text(35, 6, "Shop");
			text(35, 8, "Bank");
			text(35, 10, "Inn");
			text(35, 12, "Tavern");
			text(35, 14, "Guild");
			text(35, 16, "Healer");
			*/
	}
	else
	{
            /*
			text(32, 0, "The Dungeon");
			text(35, 4, "Smithy");
			text(35, 6, "D & P");
			text(35, 8, "Vault");
			text(35, 10, "Retreat");
			text(35, 12, "Other");
			text(35, 14, "Guild");
			*/
	}

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
	mapImage.loadFromFile("data/images/maptiles.png");
	cellImage.setTexture(mapImage);

	if (plyr.scenario==0) legendImage.loadFromFile("data/images/cityLegend.png");
	if (plyr.scenario==1) legendImage.loadFromFile("data/images/dungeonLegend.png");
	mapLegend.setTexture(legendImage);
}



// Draw an individual image to the display at pixel x,y
void DrawImage(int x,int y, int tileNo)
{
	x++;
	y++;
	int row, column;
	int tilesPerRow = 8; // number of tiles per row in font image containing all tiles (16 default)
	int tileSize = 16; // 16 pixels height and width

	//Select 16x16 pixel section of tile sheet for cell tile
	if ( tileNo >= tilesPerRow)
	{
		column = (tileNo % tilesPerRow); // remainder
		row = ((tileNo-column)/tilesPerRow);
	}
	else
	{
		column = tileNo;
		row = 0; // = row 1 on the actual tile sheet at y=0
	}

	int tileX = (column)*tileSize;	// x loc on tiles image in pixels
	int tileY = ((row)*tileSize);	// y loc on tiles image in pixels
	cellImage.setTextureRect(sf::IntRect(tileX, tileY , tileSize, tileSize));
	cellImage.setPosition(x,y); // simply display at x,y pixel locations
	//cellImage.setScale(scale,scale);
	//if (plyr.drawingBigAutomap) { App.draw(cellImage); }
	//else { App.draw(cellImage); }
	App.draw(cellImage);
}



// Draw all the images required for a single cell on the automap
void DrawCell(int x,int y, int pixelx, int pixely)
{
	int idx = getMapIndex(x,y);
	int north = levelmap[idx].north;
	int west = levelmap[idx].west;
	int east = levelmap[idx].east;
    int south = levelmap[idx].south;
	int special = levelmap[idx].special;
	int tile;

	// Draw cell background colour
	if (autoMapExplored[plyr.map][mapLocation])
	{
        DrawImage(pixelx,pixely,0);
        if (special==144) { DrawImage(pixelx,pixely,13); } // smithy
        if (special==21) { DrawImage(pixelx,pixely,13); } // dwarven smithy
        if (special==112) { DrawImage(pixelx,pixely,12); } // shop
        if (special==80) { DrawImage(pixelx,pixely,9); } // bank
        if (special==16) { DrawImage(pixelx,pixely,10); } // inn
        if (special==48) { DrawImage(pixelx,pixely,11); } // tavern
        if ((special==208) && (plyr.scenario==0)) { DrawImage(pixelx,pixely,25); } // healer
        if (special==3) { DrawImage(pixelx,pixely,11); } // Dungeon level 1 Fountain
        if (special==35) { DrawImage(pixelx,pixely,11); } // Dungeon level 2 Fountain
        if ((special==0xF0)&&(plyr.scenario==0)) { DrawImage(pixelx,pixely,9); } // city guild
        if (special==0x0F) { DrawImage(pixelx,pixely,10); } // retreat
        if (special==0x1D) { DrawImage(pixelx,pixely,11); } // rathskeller
        if (special==0x0C) { DrawImage(pixelx,pixely,9); } // dungeon guild
        if (special==0x0D) { DrawImage(pixelx,pixely,12); } // damon
        if (special==7) { DrawImage(pixelx,pixely,11); } // Goblins & Trolls
        if (special==19) { DrawImage(pixelx,pixely,25); } // ferry
        if (special==87) { DrawImage(pixelx,pixely,25); } // Undead King
	}

	if (!autoMapExplored[plyr.map][mapLocation]) { DrawImage(pixelx,pixely,24);}

	// Standard Dungeon "special" ranges
	//if ( (special>=0x01) && (special<=0x1F) && (plyr.scenario==1) ) { DrawImage(pixelx,pixely,19); } // place of interest
	//if ( (special>=0x20) && (special<=0x7F) && (plyr.scenario==1) ) { DrawImage(pixelx,pixely,23); } // dangerous
	//if ( (special>=0x80) && (special<=0x9F) && (plyr.scenario==1) ) { DrawImage(pixelx,pixely,21); } // encounter
	//if ( (special>=0xA0) && (special<=0xBF) && (plyr.scenario==1) ) { DrawImage(pixelx,pixely,20); } // treasure
	//if ( (special>=0xC0) && (special<=0xDF) && (plyr.scenario==1) ) { DrawImage(pixelx,pixely,18); } // message

	if ( (special>=0xE0) && (special<=0xFF) && (plyr.scenario==1) ) { DrawImage(pixelx,pixely,22); } // blink mine



	// switch statement to set value to image tile
	tile = 0;
	//if (north==0) { tile = 0; }
	if (north==3) { tile = 8; } // door
	if (north==4) { tile = 8; } // door
	if (north==5) { tile = 4; } // secret door
	if (north==6) { tile = 4; }	// secret door
	if (north==8) { tile = 8; }	// barred door
	if (north==9) { tile = 8; }	// barred door
	if (north==10) { tile = 8; }	// barred door
	if (north>19) { tile = 8; } // non standard door
	if (north==13) { tile = 4; } // wall
	if (north==37) { tile = 4; } // wall
	if (north==14) { tile = 4; } // crystal wall


	if (tile != 0) { DrawImage(pixelx,pixely,tile ); }

    // switch statement to set value to image tile
	tile = 0;
	//if (south==0) { tile = 0; }
	if (south==3) { tile = 6; } // door
	if (south==4) { tile = 6; } // door
	if (south==5) { tile = 2; } // secret door
	if (south==6) { tile = 2; }	// secret door
	if (south==8) { tile = 6; }	// barred door
	if (south==9) { tile = 6; }	// barred door
	if (south==10) { tile =6; }	// barred door
	if (south>19) { tile = 6; } // non standard door
	if (south==13) { tile = 2; } // wall
	if (south==37) { tile = 2; } // wall
	if (south==14) { tile = 2; } // crystal wall

	if (tile != 0) { DrawImage(pixelx,pixely,tile ); }

	// switch statement to set value to image tile
	tile = 0;
	//if (west==0) { tile = 0; }
	if (west==3) { tile = 5; } // door
	if (west==4) { tile = 5; } // door
	if (west==5) { tile = 1; } // secret door
	if (west==6) { tile = 1; }	// secret door
	if (west==8) { tile = 5; }	// barred door
	if (west==9) { tile = 5; }	// barred door
	if (west==10) { tile = 5; }	// barred door
	if (west>19) { tile = 5; } // non standard door
	if (west==13) { tile = 1; } // wall
	if (west==37) { tile = 1; } // wall
	if (west==14) { tile = 1; } // crystal wall
	if (tile != 0) { DrawImage(pixelx,pixely,tile ); }

	// switch statement to set value to image tile
	tile = 0;
	//if (east==0) { tile = 0; }
	if (east==3) { tile = 7; } // door
	if (east==4) { tile = 7; } // door
	if (east==5) { tile = 3; } // secret door
	if (east==6) { tile = 3; }	// secret door
	if (east==8) { tile = 7; }	// barred door
	if (east==9) { tile = 7; }	// barred door
	if (east==10) { tile = 7; }	// barred door
	if (east>19) { tile = 7; } // non standard door
	if (east==13) { tile = 3; } // wall
	if (east==37) { tile = 3; } // wall
	if (east==14) { tile = 3; } // crystal wall
	if (tile != 0) { DrawImage(pixelx,pixely,tile ); }
}


void drawAutomap()
{
    if (plyr.miniMapOn)
    {
        if ((graphicMode==2) && (plyr.status!=2)) // shopping?
        {
            //if (graphicMode==2) myminimap.setColor(sf::Color(255, 255, 255, 192));
            sf::RectangleShape rectangle2;
            rectangle2.setSize(sf::Vector2f(176, 176)); // 672, 184
            rectangle2.setOutlineColor(sf::Color::Yellow);
            rectangle2.setFillColor(sf::Color(0, 0, 0, 128));
            rectangle2.setOutlineThickness(1);
            rectangle2.setPosition(miniMapX+1, miniMapY+1);
            App.draw(rectangle2);
        }

        //	scale = 1.0;
        pixelSize = 16;
        int automapHeight = 9; // how many map cells displayed including central player cell + 1 for for loop
        int automapWidth = 9; //was 9

        //int cornerX = 0; // top left pixel coordinate for automap 522
        //int cornerY = 0; // top left pixel coordinate for automap 130
        int startx = 0; // map cell coords for first x
        int starty = 0; // map cell coords for first y
        int currentx = 0;
        int currenty = 0;
        int pixelx = 0;
        int pixely = 0;
        startx = plyr.x - ((automapWidth-1)/2);
        starty = plyr.y - ((automapHeight-1)/2);

        for (int y=0 ; y<(automapHeight) ; y++)
        {
            for (int x=0 ; x<(automapWidth) ; x++)
            {
                // check for valid on map square
                currentx = startx+x;
                currenty = starty+y;
                if ((currentx >=0) && (currentx <plyr.mapWidth) && (currenty >= 0) && (currenty <plyr.mapHeight))
                {
                    pixelx=miniMapX+(x*pixelSize); // 16 = pixels in cell image
                    pixely=miniMapY+(y*pixelSize); // 16 = pixels in cell image
                    mapLocation = getMapIndex(currentx,currenty);
                    DrawCell(currentx,currenty, pixelx, pixely);
                    //if (autoMapExplored[plyr.map][mapLocation]) { DrawCell(currentx,currenty, pixelx, pixely);}
                    if (!autoMapExplored[plyr.map][mapLocation]) { DrawImage(pixelx,pixely,24);} // Added
                }
            }

        }

        // Draw arrow to represent position and direction of player
        pixelx=miniMapX+(((automapWidth-1)/2)*pixelSize);
        pixely=miniMapY+(((automapHeight-1)/2)*pixelSize);
        if (plyr.facing==1) { DrawImage(pixelx,pixely,17); }
        if (plyr.facing==2) { DrawImage(pixelx,pixely,14); }
        if (plyr.facing==3) { DrawImage(pixelx,pixely,16); }
        if (plyr.facing==4) { DrawImage(pixelx,pixely,15); }
    }
}


void drawFullAutomap()
{
	plyr.drawingBigAutomap = true;
//	scale = 1.0;
	pixelSize = 16;
	int automapHeight = 32; // how many map cells displayed including central player cell + 1 for for loop
	int automapWidth = 32;
	int cornerX = 0; // top left pixel coordinate for automap 522
	int cornerY = 0; // top left pixel coordinate for automap
	int startx = 0; // map cell coords for first x
	int starty = 0; // map cell coords for first y
	int currentx = 0;
	int currenty = 0;
	int pixelx = 0;
	int pixely = 0;
	if ((plyr.x<32) && (plyr.y<32))
	{
		startx = 0;
		starty = 0;
	}
	if ((plyr.x>31) && (plyr.y<32))
	{
		startx = 32;
		starty = 0;
	}
	if ((plyr.x>31) && (plyr.y>31))
	{
		startx = 32;
		starty = 32;
	}
	if ((plyr.x<32) && (plyr.y>31))
	{
		startx = 0;
		starty = 32;
	}

	for (int y=0 ; y<(automapHeight) ; y++)
	{
		for (int x=0 ; x<(automapWidth) ; x++)
		{
			// check for valid on map square
			currentx = startx+x;
			currenty = starty+y;
			pixelx=cornerX+(x*pixelSize); // 16 = pixels in cell image
			pixely=cornerY+(y*pixelSize); // 16 = pixels in cell image
			mapLocation = getMapIndex(currentx,currenty);
			DrawCell(currentx,currenty, pixelx, pixely);
			if (!autoMapExplored[plyr.map][mapLocation]) { DrawImage(pixelx,pixely,24);} // Added back
		}

	}

	// Draw arrow to represent position and direction of player
	pixelx=(plyr.x)*pixelSize+16;
	pixely=(plyr.y)*pixelSize;
	if (plyr.y>31) { pixely=(plyr.y-32)*pixelSize; }
	if (plyr.x>31) { pixelx=(plyr.x-32)*pixelSize; }
	if (plyr.facing==1) { DrawImage(pixelx,pixely,17); }
	if (plyr.facing==2) { DrawImage(pixelx,pixely,14); }
	if (plyr.facing==3) { DrawImage(pixelx,pixely,16); }
	if (plyr.facing==4) { DrawImage(pixelx,pixely,15); }

    // Draw legend sprite
    mapLegend.setPosition(512+16, 16);
    App.draw(mapLegend);

	plyr.drawingBigAutomap = false;
}



void setAutoMapFlag(int mapno, int x, int y)
{
	int cellNo = getMapIndex(x,y);
	autoMapExplored[mapno][cellNo] = true;
}
