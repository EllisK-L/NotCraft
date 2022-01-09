#pragma once
#include <GL/glut.h>
#include "Utils.h"
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

	Utils::point3f getCurrentPos();

	
	Utils::point3f getCameraPos() { return cameraPos; };
	Utils::rotation3f getCameraLook() { return cameraLook; };
	Utils::point3f getCameraUp() { return cameraUp; };
	Utils::rotation3f getLookTheta() { return lookTheta; }
	
	


private:
	void calcCameraLookAt();

	Utils::point3f cameraPos = { 0,1,0 };
	Utils::rotation3f cameraLook = { 0,0,0 };
	Utils::point3f cameraUp = { 0,1,0 };

	Utils::rotation3f lookTheta = { 0,0,0 };

	GLfloat sensitivity = .005f;
};