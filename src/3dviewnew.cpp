#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glew.h>


#include <gflw/glfw3.h>    //   glfw3.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>

#include "constants.h"
#include "3Dview.h"
#include "display.h"
#include "player.h"
#include "level.h"
#include "globals.h"
#include "3dviewNew.h"

#include <SFML\Graphics.hpp> // Include SFML graphics headers
#include <SFML\Window.hpp>   // Include SFML window headers





int new_depth = 33; // should be 13 was 33
int new_columns = 25; // should be an odd number 25



// Declaration and initialization of a texture array (assuming GLuint is used for texture IDs)
//GLuint new_texture[MAX_TEXTURES]; // Assuming MAX_TEXTURES is defined somewhere
/*
void initializeTextures() {
    // Initialization of texture IDs
    glGenTextures(MAX_TEXTURES, new_texture);

    // Binding and loading textures...
    // glBindTexture, glTexImage2D, etc.
}

*/
/*
void initializeGLEW() {
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        // Handle the error, exit the function, or return
    }
}
*/

/*
void drawCellWalls(int c, int d, float xm, float zm, int frontwall, int leftwall, int rightwall, int frontheight, int leftheight, int rightheight) {
    int texture_no = 0;
    int wall_type;
    float depthdistantfar = (-new_depth * 2) + 1;
    float depthdistantnear = (-new_depth * 2) + 3;




    // Original graphic style for standard height walls?
    if (graphicMode == ATARI_SMALL || graphicMode == A16BIT_SMALL)
    {
        frontheight = 1;
        leftheight = 1;
        rightheight = 1;
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
 

    if ((plyr.zone == 99) && (plyr.map == 1)) { texture_no = 61; } // dungeon level 1 ceiling texture
    if ((plyr.zone == 99) && (plyr.map == 2)) { texture_no = 36; } // dungeon level 2 ceiling texture
    //if ((plyr.zone == 99) && (plyr.map==3)) { texture_no = 42; } // dungeon level 2 ceiling texture
    if ((plyr.zone == 99) && (plyr.map == 4)) { texture_no = 52; } // dungeon level 4 ceiling texture

    if (plyr.zone != 99)
    {
        if (plyr.ceiling == 0) { texture_no = zones[plyr.zoneSet].ceiling; }
        else { texture_no = plyr.ceiling; }
    }
    //texture_no = 0;
    if (texture_no != 0) // 0 = no ceiling texture
    {
        glBindTexture(GL_TEXTURE_2D, new_texture[texture_no]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f + xm, 0.5, depthdistantfar + zm);	// Bottom Left
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-23.0f + xm, 0.5, depthdistantfar + zm);	// Bottom Right
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-23.0f + xm, 0.5, depthdistantnear + zm);	// Top Right
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f + xm, 0.5, depthdistantnear + zm);	// Top Left
        glEnd();
    }

    // Create vertex data for the walls
    GLfloat wallVertices[] = {
        // Left wall
        -25.0f + xm, -0.5f, depthdistantnear + zm,  // Bottom Left
        -25.0f + xm, -0.5f, depthdistantfar + zm,   // Bottom Right
        -25.0f + xm, -0.5f + leftheight, depthdistantfar + zm,  // Top Right
        -25.0f + xm, -0.5f + leftheight, depthdistantnear + zm,  // Top Left

        // Right wall
        -23.0f + xm, -0.5f, depthdistantfar + zm,    // Bottom Left
        -23.0f + xm, -0.5f, depthdistantnear + zm,   // Bottom Right
        -23.0f + xm, -0.5f + rightheight, depthdistantnear + zm,  // Top Right
        -23.0f + xm, -0.5f + rightheight, depthdistantfar + zm   // Top Left
    };

    // Create a VBO and VAO for the walls
    GLuint wallVBO, wallVAO;
    glGenBuffers(1, &wallVBO);
    glGenVertexArrays(1, &wallVAO);

    glBindVertexArray(wallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Render left wall
    if (!(leftwall < 1) && (c <= ((new_columns - 1) / 2))) {
        // Bind VAO and texture for left wall
        glBindVertexArray(wallVAO);
        texture_no = getTextureIndex(leftwall);
        glBindTexture(GL_TEXTURE_2D, new_texture[texture_no]);

        // Draw left wall
        glDrawArrays(GL_QUADS, 0, 4);

        glBindVertexArray(0); // Unbind VAO after drawing
    }

    // Render right wall
    if (!(rightwall < 1) && (c >= ((new_columns - 1) / 2))) {
        // Bind VAO and texture for right wall
        glBindVertexArray(wallVAO);
        texture_no = getTextureIndex(rightwall);
        glBindTexture(GL_TEXTURE_2D, new_texture[texture_no]);

        // Draw right wall
        glDrawArrays(GL_QUADS, 4, 4);

        glBindVertexArray(0); // Unbind VAO after drawing
    }

    if (!(frontwall < 1)) // Ignore wall type o (clear) and 1 (arch)
    {
        glBindVertexArray(wallVAO);
        texture_no = getTextureIndex(leftwall);
        glBindTexture(GL_TEXTURE_2D, new_texture[texture_no]);



        texture_no = 0;
     //   if (wall_type == 3) { texture_no = checkCityDoors(); }
     //   if (texture_no == 0) { texture_no = getTextureIndex(wall_type); }
        glBindTexture(GL_TEXTURE_2D, new_texture[texture_no]);
       
        // Draw left wall
        glDrawArrays(GL_QUADS, 0, 4);

        glBindVertexArray(0); // Unbind VAO after drawing
    

    }// ... rest of the code ...
}
*/