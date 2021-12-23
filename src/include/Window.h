#pragma once
#include <GL/glut.h>
#include <string.h>
#include <iostream>

class Window{
public:
    Window(int width, int height, const std::string& name){
      this->width = width;
      this->height = height;
      this->name = name;
    };

	void initWindow(){
      glutInitWindowSize(this->width,this->height);
      glutInitWindowPosition(100, 150);
      glutCreateWindow(this->name.c_str());
	}

    void resize(int newWidth, int newHeight)
    {

	// update the new width
	width = newWidth;
	// update the new height
	height = newHeight;

	// update the viewport to still be all of the window
	glViewport(0, 0, width, height);

	// change into projection mode so that we can change the camera properties
	glMatrixMode(GL_PROJECTION);

	// load the identity matrix into the projection matrix
	glLoadIdentity();

	// gluPerspective(fovy, aspect, near, far)
	gluPerspective(45, (float)width / (float)height, 0.1, 200);

	// change into model-view mode so that we can change the object positions
	glMatrixMode(GL_MODELVIEW);
}

int* getWidth(){
	return &width;
}

int* getHeight(){
	return &height;
}


private:
    int width, height;
    std::string name;
};