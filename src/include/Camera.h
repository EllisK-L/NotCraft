#pragma once
#include <GL/glut.h>
#include "Object.h"

class Camera : public Object {

public:
	Camera();
	bool lookAt(GLfloat lookAtVector[3]);
	
	void calcCameraLookMouse(GLfloat xOffset, GLfloat yOffset);

	void update();
	void render();

	void moveX(GLfloat amount);
	void moveY(GLfloat amount);
	void moveZ(GLfloat amount);

	bool advancedCont = false;

	GLfloat* getPos();


	GLfloat* getCameraPos() { return cameraPos; };
	GLfloat* getCameraLook() { return cameraLook; };
	GLfloat* getCameraUp() { return cameraUp; };
	GLfloat* getLookTheta() { return lookTheta; }
	
	


private:
	void calcCameraLookAt();

	GLfloat cameraPos[3] = { 0,1,0 };
	GLfloat cameraLook[3] = { 0,0,0 };
	GLfloat cameraUp[3] = { 0,1,0 };

	GLfloat lookTheta[2] = { 0,0 };

	GLfloat sensitivity = .2f;
};