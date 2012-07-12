//
//  OGPhysic.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 11/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGPhysic.h"

OGPhysic::OGPhysic(OGBall* ball, OGTerrain* terrain){
    viscosity = 0.02;
    friction = 0.03;
    gravity = -9.81;
    elasticity = 0.6;
    srand ( time(NULL) );
    const int MAX_WIND = 5;
    wind = Vector3d((rand() % MAX_WIND),0,(rand() % MAX_WIND));
    wind = Vector3d();
    this->ball = ball;
    this->terrain = terrain;
}

void OGPhysic::update(double time){

    Vector3d fp = Vector3d(0,ball->getMass() * gravity,0); //forza peso
    Vector3d faa = 6 * M_PI * viscosity * ball->getMass() * (wind - ball->getSpeed()); //forza attrito aria
    Vector3d fr = fp + faa; //forza attiva sulla pallina
     
    
    Vector3d terrainNorm;
    bool collision = terrainCollision(terrainNorm);
    
    if (collision) {
        Vector3d normV = ball->getSpeed().getNormalized();
        Vector3d fat = -normV * (-fp.y * friction); //attrito radente terreno
        fr += fat;
        //printf("terrain norm: %f",terrainNorm.dot(fr.getNormalized()));
        fr = Vector3d(fr.x,-fr.y,fr.z);
    }
    
    Vector3d acc = fr / ball->getMass();
    
    Vector3d speed = ball->getSpeed() + (acc * time);
    ball->setSpeed(speed);
    ball->setPosition(ball->getPosition() + (ball->getSpeed() * time));
}

bool OGPhysic::terrainCollision(Vector3d &normal){
    Vector3d newPos = ball->getPosition() * terrain->getHScale(); //posizione nell'immagine scalata
    
    Vector3d v1 = terrain->vertex[(int)(round(newPos.z) * terrain->getTerrainWidth() + round(newPos.x))];
    Vector3d v2 = terrain->vertex[(int)(round(newPos.z) * terrain->getTerrainWidth() + round(newPos.x + 1))];
    Vector3d v3 = terrain->vertex[(int)(round(newPos.z +1) * terrain->getTerrainWidth() + round(newPos.x))];
    
    Vector3d center = (v1+v2+v3)/3;
    double distance = (ball->getPosition().y - center.y);
    
    glLineWidth(2);
    glBegin(GL_LINES);
            glVertex3d(v1.x, v1.y, v1.z);
            glVertex3d(v1.x, v1.y+1, v1.z);
            glVertex3d(v2.x, v2.y, v2.z);
            glVertex3d(v2.x, v2.y+1, v2.z);
            glVertex3d(v3.x, v3.y, v3.z);
            glVertex3d(v3.x, v3.y+1, v3.z);
            //glVertex3d(center.x, center.y, center.z);
            //glVertex3d(center.x +0 , center.y +1, center.z +0);
    glEnd();
    glLineWidth(1);
    
    glPushMatrix();
    glTranslated(center.x, center.y, center.z);
    glutSolidSphere(0.002, 10, 10);
    glPopMatrix();
    
    //show red triangle
    glColor3f(0, 1, 0 );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glBegin(GL_TRIANGLES);
    
    glVertex3d(v1.x, v1.y, v1.z);
    glVertex3d(v2.x, v2.y, v2.z);
    glVertex3d(v3.x, v3.y, v3.z);
    
    glEnd();
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    //glutSolidSphere(1, 10, 10);
    if(distance<ball->getRadius()){
        Vector3d n1 = terrain->normals[(int)(round(newPos.z) * terrain->getTerrainWidth() + round(newPos.x))];
        Vector3d n2 = terrain->normals[(int)(round(newPos.z) * terrain->getTerrainWidth() + round(newPos.x + 1))];
        Vector3d n3 = terrain->normals[(int)(round(newPos.z +1) * terrain->getTerrainWidth() + round(newPos.x))];
        normal = (n1+n2+n3).getNormalized(); //
        
        
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex3d(center.x, center.y, center.z);
        glVertex3d(normal.x +center.x , center.y +normal.y, center.z +normal.z);
        glEnd();
        glLineWidth(1);
        return true;
        
    }else{
        normal = Vector3d();
        return false;
    }
    
    
}

void OGPhysic::shoot(float power,Vector3d direction){
    direction.normalize();
    ball->setSpeed(direction * power);
}