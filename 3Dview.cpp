#define GLUT_DISABLE_ATEXIT_HACK
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <SFML\Graphics.hpp> // Include SFML graphics headers
#include <SFML\Window.hpp>   // Include SFML window headers
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>

#include "constants.h"
#include "3Dview.h"
#include "display.h"
#include "player.h"
#include "level.h"
#include "globals.h"



// Storage for textures

//const int noOfBackgrounds = 61; //was 46


sf::Texture background[noOfBackgrounds];
std::string backgroundNames[noOfBackgrounds];

GLuint texture[noOfTextures];
//sf::Texture background[noOfBackgrounds];
string textureNames[noOfTextures];
//string backgroundNames[noOfBackgrounds];

int filter;                                     // Which Filter To Use
int fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };  // Storage For Three Types Of Fog
int fogfilter= 1;                               // Which Fog To Use
float fogColor[4]= {0.0f, 0.0f, 0.0f, 1.0f};    // Fog Color

int depth = 33; // should be 13 was 33
int columns = 25; // should be an odd number 25
int frontwall = 0;
int leftwall = 0;
int rightwall = 0;
int frontheight = 0;
int leftheight = 0;
int rightheight = 0;
int ceiling = 0;
int floorTexture = 0;
int specialwall = 0;
int zone = 0;

// Draw an sf::Texture as a full-viewport OpenGL quad at maximum depth (z=0.9999)
// so that 3D geometry rendered afterwards will naturally occlude it.
static void drawGLBackground(const sf::Texture& tex)
{
    sf::Texture::bind(&tex);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
        glTexCoord2f(0.f, 1.f); glVertex2f(0.f, 0.f);
        glTexCoord2f(1.f, 1.f); glVertex2f(1.f, 0.f);
        glTexCoord2f(1.f, 0.f); glVertex2f(1.f, 1.f);
        glTexCoord2f(0.f, 0.f); glVertex2f(0.f, 1.f);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    sf::Texture::bind(nullptr);
}

void draw3DView()
{
	// Convert SFML top-left Y to OpenGL bottom-left Y
	int glY = windowHeight - (viewPortY + viewHeight);

	// Exit SFML 2D mode to do raw OpenGL
	App.popGLStates();

	// Set viewport and clear depth
	glViewport(viewPortX, glY, viewWidth, viewHeight);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Re-apply projection each frame
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)viewWidth / (GLfloat)viewHeight, 0.1f, 100.0f);
	if (graphicMode < ALTERNATE_LARGE)
		glTranslatef(0.0f, 0.0f, -1.0f);
	else
		glTranslatef(0.0f, 0.0f, -1.2f);
	glMatrixMode(GL_MODELVIEW);

	// Draw background as GL quad (behind all 3D geometry)
	draw3DBackground();

	buildLevelView(); // Draw the OpenGL 3D corridor and room view

	// Re-enter SFML 2D mode for sprites, text etc.
	App.pushGLStates();
}

void draw3DBackground()
{
    bool drawMountain = false;

    // Determine which background texture to use
    const sf::Texture* bgTex = &background[0];
    const sf::Texture* mtTex = &background[0];

    if (plyr.scenario==2) bgTex = &background[44];

    if ( plyr.zoneSet==0 )  bgTex = &background[15];
    if ( plyr.zoneSet==1 )  bgTex = &background[10];
    if ( plyr.zoneSet==2 )  bgTex = &background[8];
    if ( plyr.zoneSet==5 )  bgTex = &background[46];
    if ( plyr.zoneSet==4 )  bgTex = &background[48];
    if ( plyr.zoneSet==11 ) bgTex = &background[12];
    if ( plyr.zoneSet==14 ) bgTex = &background[15];
    if ( plyr.zoneSet==16 ) bgTex = &background[16];
    if ( plyr.zoneSet==18 ) bgTex = &background[17];
    if ( plyr.zoneSet==21 ) bgTex = &background[44];
    if ( plyr.zoneSet==22 ) bgTex = &background[44];
    if ( plyr.zoneSet==23 ) bgTex = &background[45];
    if ( plyr.zoneSet==24 ) bgTex = &background[8];
    if ( plyr.zoneSet==25 ) bgTex = &background[15];
    if ( plyr.zoneSet==26 ) bgTex = &background[12];
    if ( plyr.zoneSet==27 ) bgTex = &background[16];

    if ((plyr.scenario==0) && (plyr.zone==99))
    {
        if (plyr.timeOfDay==1) // night
        {
            if (plyr.facing== WEST)  bgTex = &background[3];
            if (plyr.facing== NORTH) bgTex = &background[2];
            if (plyr.facing== EAST)  bgTex = &background[0];
            if (plyr.facing== SOUTH) bgTex = &background[1];
        }
        if (plyr.timeOfDay==2) // sunrise1
        {
            if (plyr.facing== WEST)  { bgTex = &background[21]; mtTex = &background[49]; drawMountain = true; }
            if (plyr.facing== NORTH) bgTex = &background[20];
            if (plyr.facing== EAST)  { bgTex = (graphicMode != A16BIT_SMALL) ? &background[18] : &background[21]; mtTex = &background[55]; drawMountain = true; }
            if (plyr.facing== SOUTH) bgTex = &background[19];
        }
        if (plyr.timeOfDay==3) // sunrise2
        {
            if (plyr.facing== WEST)  { bgTex = &background[25]; mtTex = &background[50]; drawMountain = true; }
            if (plyr.facing== NORTH) bgTex = &background[24];
            if (plyr.facing== EAST)  { bgTex = (graphicMode != A16BIT_SMALL) ? &background[22] : &background[25]; mtTex = &background[56]; drawMountain = true; }
            if (plyr.facing== SOUTH) bgTex = &background[23];
        }
        if (plyr.timeOfDay==4) // sunrise3
        {
            if (plyr.facing== WEST)  { bgTex = &background[29]; mtTex = &background[51]; drawMountain = true; }
            if (plyr.facing== NORTH) bgTex = &background[28];
            if (plyr.facing== EAST)  { bgTex = (graphicMode != A16BIT_SMALL) ? &background[26] : &background[29]; mtTex = &background[57]; drawMountain = true; }
            if (plyr.facing== SOUTH) bgTex = &background[27];
        }
        if (plyr.timeOfDay==5)
        {
            if (plyr.facing== WEST)  { bgTex = &background[33]; mtTex = &background[52]; drawMountain = true; }
            if (plyr.facing== NORTH) bgTex = &background[32];
            if (plyr.facing== EAST)  { bgTex = (graphicMode != A16BIT_SMALL) ? &background[30] : &background[33]; mtTex = &background[58]; drawMountain = true; }
            if (plyr.facing== SOUTH) bgTex = &background[31];
        }
        if (plyr.timeOfDay==6)
        {
            if (plyr.facing== WEST)  { bgTex = &background[37]; mtTex = &background[53]; drawMountain = true; }
            if (plyr.facing== NORTH) bgTex = &background[36];
            if (plyr.facing== EAST)  { bgTex = (graphicMode != A16BIT_SMALL) ? &background[34] : &background[37]; mtTex = &background[59]; drawMountain = true; }
            if (plyr.facing== SOUTH) bgTex = &background[35];
        }
        if (plyr.timeOfDay==7)
        {
            if (plyr.facing== WEST)  { bgTex = &background[41]; mtTex = &background[54]; drawMountain = true; }
            if (plyr.facing== NORTH) bgTex = &background[40];
            if (plyr.facing== EAST)  { bgTex = (graphicMode != A16BIT_SMALL) ? &background[38] : &background[41]; mtTex = &background[60]; drawMountain = true; }
            if (plyr.facing== SOUTH) bgTex = &background[39];
        }
        if (plyr.timeOfDay==0)
        {
            if (plyr.facing== WEST)  bgTex = &background[7];
            if (plyr.facing== NORTH) bgTex = &background[6];
            if (plyr.facing== EAST)  bgTex = &background[4];
            if (plyr.facing== SOUTH) bgTex = &background[5];
        }
    }

    // Draw background as OpenGL quad so 3D walls occlude it via depth test
    drawGLBackground(*bgTex);

    // Sun rendering (16bit mode only) - still via SFML after GL pass
    bool drawSun = false;
    int sunY = 0;
    if (plyr.facing== WEST && graphicMode == A16BIT_SMALL && (plyr.hours >= 16 && plyr.hours < 19))
    {
        int time = ((plyr.hours - 16) * 60) + plyr.minutes;
        sunY = ((int)time / 3) - 10;
        drawSun = true;
    }
    else if (plyr.facing== EAST && graphicMode == A16BIT_SMALL && (plyr.hours <= 8 && ((plyr.hours == 4 && plyr.minutes > 29) || plyr.hours > 4)))
    {
        int time = ((plyr.hours - 4) * 60) + plyr.minutes;
        sunY = 61 - ((int)time / 3);
        drawSun = true;
    }

    if (drawSun && sunY < 60)
    {
        // Sun is a small circle - draw as GL point sprite approximation
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, viewWidth, viewHeight, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glColor4f(1.f, 1.f, 0.f, 1.f);
        float sx = 185.f, sy = static_cast<float>(sunY);
        float r = 9.f;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(sx, sy);
        for (int i = 0; i <= 16; i++) {
            float a = i * 3.14159f * 2.f / 16.f;
            glVertex2f(sx + r * cosf(a), sy + r * sinf(a));
        }
        glEnd();
        glColor4f(1.f, 1.f, 1.f, 1.f);
        glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW);  glPopMatrix();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
    }

    if (drawMountain && graphicMode == A16BIT_SMALL)
        drawGLBackground(*mtTex);
}
void loadBackgroundNames() {
	std::string::size_type idx;
	std::string filename;

	for (int i = 0; i < noOfBackgrounds; i++) {
		backgroundNames[i] = "";
	}

	std::cout << "Loading background Texture" << std::endl;

	string graphicType = "";

	
	if (graphicMode == 1)
		graphicType = "16Bit";
	else
		graphicType = "Updated";
	
	filename = "data/map/core/backgrounds" + graphicType + ".txt";

	std::ifstream instream(filename);
	if (!instream) {
		std::cerr << "Error: background file " << filename << " could not be loaded" << std::endl;
		return;
	}

	std::string line, text;
for (int i = 0; i < noOfBackgrounds; i++) {
		if (!std::getline(instream, line)) {
			std::cerr << "Error: not enough lines in file " << filename << std::endl;
			break;
		}

		idx = line.find('=');
		if (idx == std::string::npos || idx + 2 >= line.size()) {
			std::cerr << "Error: invalid format in line " << i + 1 << std::endl;
			continue;
		}

		text = line.substr(idx + 2);
		backgroundNames[i] = text;

		if (!background[i].loadFromFile("data/images/backgrounds/" + graphicType + "/" + text + ".png")) {
			std::cerr << "Error loading texture: data/images/backgrounds/" + graphicType + "/" + text + ".png" << std::endl;
		}
	}

	instream.close();
}

void loadTextureNames()
{
	string::size_type idx;
	string filename;
	for (int i=0 ; i<noOfTextures ; i++) { textureNames[i]=""; }
	if (graphicMode == ATARI_SMALL)
	{
		filename = "data/map/core/textures.txt";
	}
	else if (graphicMode == A16BIT_SMALL)
		filename = "data/map/core/textures16bit.txt";
	else
	{
		filename = "data/map/core/texturesUpdated.txt";
	}
	std::ifstream instream;
	std::string line,text;
	instream.open(filename.c_str());
	if ( !instream )
	{
      //cerr << "Error: textureNames file could not be loaded" << endl;
	}
	for (int i=0 ; i<noOfTextures ; i++)
	{
		getline(instream, line);
		idx = line.find('=');
		text = line.substr(idx+2);
		textureNames[i]= text;
	}
	instream.close();
}




void initTextures()
{
	// Load an OpenGL texture.
    // We could directly use a sf::Image as an OpenGL texture (with its Bind() member function),
    // but here we want more control on it (generate mipmaps, ...) so we create a new one

    sf::Image Image;
	string filename;
    char tempfilename[100];
	glGenTextures(noOfTextures, &texture[0]); // problem line - don't include in loop. Always 0???
	for ( int i=0; i<noOfTextures; i++ )
    {
		filename = textureNames[i];
		if (graphicMode == ATARI_SMALL)
  		{
		 sprintf_s(tempfilename,"%s%s.png","data/images/textures_original/",filename.c_str());
		 Image.loadFromFile(tempfilename);
		 }
		 else if (graphicMode == ALTERNATE_SMALL || graphicMode == ALTERNATE_LARGE)
  		{
		 sprintf_s(tempfilename,"%s%s.png","data/images/textures_alternate/",filename.c_str());
		 Image.loadFromFile(tempfilename);
		 }
		 else
		 {
		 sprintf_s(tempfilename,"%s%s.png","data/images/texture_16bit/",filename.c_str());
		 if (!Image.loadFromFile(tempfilename))
		 {
			//Couldn't locate new alternate texture, try loading old version.
			sprintf_s(tempfilename,"%s%s.png","data/images/textures_original/",filename.c_str());
			if (!Image.loadFromFile(tempfilename))
				std::cout << "Couldn't load original " << tempfilename << std::endl;
		}
		}
		glBindTexture(GL_TEXTURE_2D, texture[i]);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, Image.getSize().x, Image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, Image.getPixelsPtr());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<GLint>(8.0f));
	
	}

	// Need to delete SFML image...

}





void buildLevelView()
{
		float adjustment = 0;
		float tempadj = 0;

	glFogi(GL_FOG_MODE, fogMode[fogfilter]);    // Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);			// Set Fog Color
	if (plyr.darkness == 1 && plyr.light == 0) 
	{ 
		glFogf(GL_FOG_DENSITY, 5.0f); 
	} 
	else 
	{
		glFogf(GL_FOG_DENSITY, 0.0f); 
	};

				// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);          // Fog Hint Value
	glFogf(GL_FOG_START, 0.1f);					// Fog Start Depth
	glFogf(GL_FOG_END, 5.0f);					// Fog End Depth



	// Enable and disable fog based on area and could adjust fog properties here for zones
	if ((plyr.scenario==1) && (graphicMode == ALTERNATE_LARGE)) glEnable(GL_FOG);  // Enables GL_FOG for the Dungeon
	if ((plyr.scenario==1) && (graphicMode == ALTERNATE_SMALL)) glEnable(GL_FOG);  // Enables GL_FOG for the Dungeon
	if ((plyr.scenario==1) && (graphicMode == ATARI_SMALL)) glEnable(GL_FOG);  // Disables GL_FOG for the Dungeon
	if ((plyr.scenario==1) && (graphicMode == A16BIT_SMALL)) glDisable(GL_FOG);  // Disables GL_FOG for the Dungeon
	if (plyr.scenario==0)  glDisable(GL_FOG); // Disable GL_FOG for City


	if ((plyr.scenario==0) && (graphicMode == A16BIT_SMALL))
	{
		bool display = false;
		//16bit adjust the shading of the textures to reflect the time of day.
		if (plyr.hours >= 16  )
			display = true;
		if ((plyr.hours < 7)  )
			display = true;
			
			if (display == true)					
			{
				int time = 0;
				if (plyr.hours >= 16)
				{
					time = ((plyr.hours - 16 )* 60) + plyr.minutes;
//std::cout << "time " << time << "\n";	
					adjustment = 1 -((float)time / 539);
				} else 
				if (plyr.hours < 7)
				{
					time = ((plyr.hours - 4 )* 60) + plyr.minutes;
					adjustment =   (((float)time / 179) );
				}
//std::cout << "adjustment " << adjustment << "\n";	
				if (adjustment < 0.20f)
				   adjustment = 0.20f;
//std::cout << "adjustment " << adjustment << "\n";	
	 			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB,      GL_ADD);
				glColor4f(adjustment,adjustment, adjustment, 0);
			}
		}

	// Start with 5 variables - columns, depth, plyr.x, plyr.y, plyr.facing
	// c and d hold current column and current depth value

	// Draw left hand block of quads
	int leftmostColumn = 0;
	int rightmostColumn = ((columns-1)/2);

	for ( int d=0; d<depth; d++ )
	{
		for ( int c=leftmostColumn; c<rightmostColumn; c++ )
		{
			calculateWallPositions(c,d);
		}
	}

	// Draw right hand block of quads
	leftmostColumn = ((columns-1)/2)+1;
	rightmostColumn = columns;

	for ( int d=0; d<depth; d++ )
	{
		for ( int c=rightmostColumn; c>(leftmostColumn-1); c-- ) //  int c=leftmostColumn; c<rightmostColumn; c++
		{
			   calculateWallPositions(c,d);
		}
	}

	// Draw front block of quads
	int c = ((columns-1)/2); // This should be the central column 13 if columns = 25
	for ( int d=0; d<depth; d++ )
	{
		   calculateWallPositions(c,d);
	}
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}



void calculateWallPositions(int c,int d)
{
	// Calculates the actual positions within OpenGL space to draw the 3 quads that make up a map cell
	int x = 0;
	int y = 0;
	switch (plyr.facing)
	{
		case NORTH: // north
			x = (plyr.x - ((columns-1)/2)) + c; // total colums -1 / 2
			y = ((plyr.y - (depth-1)) + d); // actual depth
			break;

		case WEST: // west
			x = (plyr.x - (depth-1)) + d;
			y = (plyr.y + ((columns-1)/2)) - c;
			break;

		case EAST: // east
			x = (plyr.x + (depth-1)) - d;
			y = (plyr.y -  ((columns-1)/2)) + c;
			break;

		case SOUTH: // south
			x = (plyr.x + ((columns-1)/2)) - c;
			y = ((plyr.y + (depth-1)) - d);
			break;
	}

	if ( (x >= 0) && (x < (plyr.mapWidth)) && (y >= 0) && (y < (plyr.mapHeight)) )  // valid location on map? (64 x 64 in example)
	{
		int ind = getMapIndex( x,y );
//		plyr.current_zone = levelmap[ind].zone;
		transMapIndex (ind);
		frontwall = plyr.front; // front wall texture number
		leftwall = plyr.left;   // left wall texture number
		rightwall = plyr.right; // right wall texture number

		frontheight = plyr.frontheight; // front wall texture number
		leftheight = plyr.leftheight;   // left wall texture number
		rightheight = plyr.rightheight; // right wall texture number

		specialwall = plyr.specialwall; // special used for guild sign etc in City
		float xm = static_cast<float>(c*2);           // x float value to be added to texture positioning co-ords
		float zm = static_cast<float>(d*2);           // z float value to be added to texture positioning co-ords
		zm = (zm+plyr.z_offset)-1.0f; //-1.0f;
		// Draw front, left and right walls for current map cell
		drawCellWalls(c, d, xm, zm, frontwall, leftwall, rightwall,frontheight,leftheight,rightheight); // pass wall numbers and x and z mods
	}
}




int getTextureIndex(int x)
{
	int texture_index;

	switch (x)
	{
		case 1:
		case 2:
			texture_index = zones[plyr.zoneSet].arch; // arch image with transparency2 6
			break;
		case 3:
		case 4:
			texture_index = zones[plyr.zoneSet].door; // door1 5
			break;
		case 7:
		case 8:
		case 9:     // bolted door
		case 10:
		case 11:
		case 12:
			texture_index = zones[plyr.zoneSet].door; // barred door1 5
			break;

		case 5:
		case 6:
			if(plyr.supervision == 1)
			{
				texture_index = zones[plyr.zoneSet].door;
			}
			else
			{
				texture_index = zones[plyr.zoneSet].wall; // secret door1 5
			}
			break;
		case 13:
		case 14:
			texture_index = zones[plyr.zoneSet].wall; // wall0 4
			break;
		case 27:
			texture_index = 27; // City Shop door
			break;
		case 28:
			texture_index = 28; // City Inn door
			break;
		case 29:
			texture_index = 29; // City Tavern door
			break;
		case 30:
			texture_index = 30; // City Smithy door
			break;
		case 31:
			texture_index = 31; // City Bank door
			break;
		case 32:
			texture_index = 32; // City Guild door
			break;
		case 33:
			texture_index = 33; // City Healer door
			break;
		case 68:
			texture_index = 68; // Forest Wall
			break;
		case 69:
			texture_index = 69; // Forest Door
			break;
		case 70:
			texture_index = 70; // Forest Arch
			break;
		default:
			texture_index = x; // use the non-zone value assigned to the wall
			break;
	}
    return texture_index;
}


void drawCellWalls(int c, int d, float xm, float zm, int frontwall, int leftwall, int rightwall, int frontheight, int leftheight,int rightheight)
{
    int texture_no = 0;
    int wall_type;
	float depthdistantfar = static_cast<float>(-depth*2)+1;
	float depthdistantnear = static_cast<float>(-depth*2)+3;

	// Original graphic style for standard height walls?
	if (graphicMode== ATARI_SMALL || graphicMode == A16BIT_SMALL)
	{
		frontheight=1;
		leftheight=1;
		rightheight=1;
	}



	// Draw ceiling

	/*
	if ( (plyr.zone == 99) && (graphicMode==1) && (plyr.scenario==0) && (plyr.ceiling==0) )
	{
		texture_no = 53; // city floor texture - change
	}
	else
	{
		texture_no = plyr.ceiling;
	}
	*/

	if ((plyr.zone == 99) && (plyr.map==1)) { texture_no = 61; } // dungeon level 1 ceiling texture
	if ((plyr.zone == 99) && (plyr.map==2)) { texture_no = 36; } // dungeon level 2 ceiling texture
	//if ((plyr.zone == 99) && (plyr.map==3)) { texture_no = 42; } // dungeon level 2 ceiling texture
	if ((plyr.zone == 99) && (plyr.map==4)) { texture_no = 52; } // dungeon level 4 ceiling texture

	if (plyr.zone != 99)
	{
		if (plyr.ceiling == 0) { texture_no = zones[plyr.zoneSet].ceiling; } else { texture_no = plyr.ceiling; }
	}
	//texture_no = 0;
	if (texture_no != 0) // 0 = no ceiling texture
	{
			glBindTexture(GL_TEXTURE_2D, texture[texture_no]);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f+xm, 0.5,  depthdistantfar+zm);	// Bottom Left
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-23.0f+xm, 0.5,  depthdistantfar+zm);	// Bottom Right
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-23.0f+xm, 0.5,  depthdistantnear+zm);	// Top Right
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f+xm, 0.5,  depthdistantnear+zm);	// Top Left
			glEnd();
	}



    // Draw floor
	texture_no = 0;
	if (zones[plyr.zoneSet].floor > 0) texture_no = zones[plyr.zoneSet].floor;
	if (plyr.floorTexture > 0) texture_no = plyr.floorTexture;
	if ((plyr.scenario==0) && (plyr.floorTexture==0) && (graphicMode== ATARI_SMALL || graphicMode == A16BIT_SMALL)) texture_no = 0;
	if (plyr.zone != 99)
	{
		if (plyr.floorTexture == 0) { texture_no = zones[plyr.zoneSet].floor; } else { texture_no = plyr.floorTexture; }
	}

	if (texture_no != 0) // 0 = no floor texture
	{
		glBindTexture(GL_TEXTURE_2D, texture[texture_no]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f+xm, -0.5,  depthdistantfar+zm);	// Bottom Left
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-23.0f+xm, -0.5,  depthdistantfar+zm);	// Bottom Right
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-23.0f+xm,  -0.5,  depthdistantnear+zm);	// Top Right
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f+xm,  -0.5,  depthdistantnear+zm);	// Top Left
		glEnd();
	}


	int midcol = ((columns-1)/2);


	// left wall

    if ((!(leftwall<1)) && (c <= midcol))
    {
        wall_type = leftwall;
        if ( ( wall_type == 1 ) || ( wall_type == 2) )
        {
			glEnable(GL_BLEND);									// Enable Blending
			//glDisable(GL_DEPTH_TEST);							// Disable Depth Testing
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
			texture_no = getTextureIndex(wall_type);
			glBindTexture(GL_TEXTURE_2D, texture[texture_no]);
			glBegin(GL_QUADS);		                // begin drawing walls
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f+xm, -0.5f, depthdistantnear+zm);	// Bottom Left
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-25.0f+xm, -0.5f, depthdistantfar+zm);	// Bottom Right
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-25.0f+xm,  -0.5f+leftheight, depthdistantfar+zm);	// Top Right
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f+xm,  -0.5f+leftheight, depthdistantnear+zm);	// Top Left
			glEnd();
		 if (( ( wall_type == 1 ) || ( wall_type == 2))) // was 1
		 {
			glEnable(GL_DEPTH_TEST);							// Enable Depth Testing
			glDisable(GL_BLEND);
		 }
}

    if ((!(rightwall<1)) && (c >= midcol))
    {
        wall_type = rightwall;
        if (( wall_type == 1 ) || ( wall_type == 2))
		{
			glEnable(GL_BLEND);									// Enable Blending
			glDisable(GL_DEPTH_TEST);							// Disable Depth Testing
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
	   texture_no = getTextureIndex(wall_type);
		glBindTexture(GL_TEXTURE_2D, texture[texture_no]);
		glBegin(GL_QUADS);		                // begin drawing walls
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-23.0f+xm, -0.5f, depthdistantfar+zm);	// Bottom Left
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-23.0f+xm, -0.5f, depthdistantnear+zm);	// Bottom Right
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-23.0f+xm,  -0.5f+rightheight, depthdistantnear+zm); // Top Right
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-23.0f+xm,  -0.5f+rightheight, depthdistantfar+zm);	// Top Left
		glEnd();
		if (( wall_type == 1 ) || ( wall_type == 2))
		{
			glEnable(GL_DEPTH_TEST);							// Enable Depth Testing
			glDisable(GL_BLEND);
		}


    }


	if (!(frontwall<1)) // Ignore wall type o (clear) and 1 (arch)
	{
		wall_type = frontwall;
		if (( wall_type == 1 ) || ( wall_type == 2))
		{
			glEnable(GL_BLEND);	// Enable Blending
			glDisable(GL_DEPTH_TEST);							// Disable Depth Testing
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
		texture_no = 0;
		if (wall_type==3) { texture_no = checkCityDoors(); }
		if (texture_no==0) { texture_no = getTextureIndex(wall_type); }
		glBindTexture(GL_TEXTURE_2D, texture[texture_no]);
		glBegin(GL_QUADS); // begin drawing walls
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f+xm, -0.5f,  depthdistantfar+zm);	// Bottom Left
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-23.0f+xm, -0.5f,  depthdistantfar+zm);	// Bottom Right
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-23.0f+xm,  -0.5f+frontheight,  depthdistantfar+zm); // Top Right
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f+xm,  -0.5f+frontheight,  depthdistantfar+zm); // Top Left
		glEnd();
		if (( wall_type == 1 ) || ( wall_type == 2))
		{
			glEnable(GL_DEPTH_TEST);	// Enable Depth Testing
			glDisable(GL_BLEND);
		}
    }


}
