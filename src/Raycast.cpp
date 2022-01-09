#include "include/Raycast.h"
#include <math.h>
#include <GL/glut.h>


Raycast::Raycast(const Utils::point3f startPosition, const Utils::rotation3f startRotation) :
        startPos(startPosition),
        startRot(startRotation){  
}

void Raycast::cast(float length){
    //rayEndPos.x = (length * cos(startRot.x * DEG_TO_RAD)) + startPos.x;
	// rayEndPos.y = (length * sin(startRot.y * DEG_TO_RAD)) + startPos.y;
    //rayEndPos.y = 0;
    // printf("%f\n", rayEndPos.y);
	//rayEndPos.z = (length * sin(startRot.x * DEG_TO_RAD)) + startPos.z;
    rayEndPos.x = length * cos(startRot.x * DEG_TO_RAD) * cos(startRot.y * DEG_TO_RAD) + startPos.x;
	rayEndPos.y = length *  sin(startRot.y * DEG_TO_RAD) + startPos.y;
	rayEndPos.z = length *  sin(startRot.x * DEG_TO_RAD) * cos(startRot.y * DEG_TO_RAD) + startPos.z;
    hasCast = true;
}

bool Raycast::render(){
    if(hasCast){
        glPushMatrix();
        //glTranslatef(0,.1,0);
        glBegin(GL_LINES);
            glVertex3f(rayEndPos.x, rayEndPos.y, rayEndPos.z);
            // glVertex3f(startPos.x, startPos.y, startPos.z);
            glVertex3f(0,0,0);
        glEnd();
        glPopMatrix();
    }
    return hasCast;
}