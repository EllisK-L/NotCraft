#pragma once
#include <GL/glut.h>

class Object {
public:
	//called once per frame on every object
	virtual void update() = 0;
	virtual void render() = 0;

	virtual void moveX(GLfloat amount) = 0;
	virtual void moveY(GLfloat amount) = 0;
	virtual void moveZ(GLfloat amount) = 0;
};