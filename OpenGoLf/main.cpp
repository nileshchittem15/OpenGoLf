//
//  main.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 04/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#ifdef WIN32

#include <GL/gl.h>
#include <GL/glu.h>

#include <GL/glut.h>

#else

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <GLUT/glut.h>
#endif

#include "Vector3d.h"

#include "OGRoom.h"
#include "OGLevel.h"


//----------global var---------------------
//OGLevel *level;
OGRoom *room;
int W_WIDTH = 900;
int W_HEIGHT = 800;
//-----------------------------------------

void init();

int main(int argc, char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(argv[0]);

    init();
    
    room = new OGRoom();
    room->init();

    if (DEBUGGING)
     glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); //enable wireframe

    glutMainLoop();

    return 0;

}

void init(){
    // enable depth testing
	glEnable(GL_DEPTH_TEST);

    //enable lghting
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_COLOR_MATERIAL);
}

