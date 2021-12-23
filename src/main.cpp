#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <ctime>
#include <unistd.h>
#include "include/Camera.h"
#include "include/Object.h"
#include "include/Window.h"
#include "include/Terrain.h"
#include "include/Utils.h"

double deltaTime = 0;
int mousePos[2] = {0,0};
signed char movement[3][2] = {
	{0,0},
	{0,0},
	{0,0}
};
float playerMovementSpeed = 1.0f;
auto startTime = std::chrono::system_clock::now();
double fps = 0;

Utils::image grass;

Terrain* terrain = new Terrain(grass);
Window* window = new Window(1200,800,"Window 1");
Camera* cam = new Camera(); 

std::chrono::duration<double> calculateFPS() {
	//calculate the frame times

	auto endTime = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsedTime = endTime - startTime;
	//limit the frame times to aprox 60 frames per second
	//while (currentTimeInterval < (double).01666) {
	//	newTime = clock();
	//	currentTimeInterval = (double)(newTime - oldTime) / CLOCKS_PER_SEC;
	//}
	startTime = std::chrono::system_clock::now();
	deltaTime = elapsedTime.count();
	return elapsedTime;
}

void displayDraw(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	cam->render();
	glColor3f(1,1,1);
	int yPos = -1;
	glBegin(GL_QUADS);
		glVertex3f(-10,yPos,10);
		glVertex3f(10,yPos,10);
		glVertex3f(10,yPos,-10);
		glVertex3f(-10,yPos,-10);
	glEnd();

	terrain->render();

	glFlush();
	//glutSwapBuffers();
}

void idle(void){
	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//printf("%f\n",1.0f/(calculateFPS().count()));
	fps = 1.0f/(calculateFPS().count());

	cam->moveZ( (movement[2][0] + movement[2][1]) * playerMovementSpeed * deltaTime);
	cam->moveX( (movement[0][0] + movement[0][1]) * playerMovementSpeed * deltaTime);
	cam->moveY((movement[1][0] + movement[1][1]) * playerMovementSpeed * deltaTime);

	glutPostRedisplay();
}

void idleMouseFunc(int x, int y) {
	float offsetX = mousePos[0] - x;
	float offsetY = mousePos[1] - y;
	
	//glutWarpPointer(windowWidth / 2, windowHeight / 2);
	mousePos[0] = x;
	mousePos[1] = y;
	cam->calcCameraLookMouse(offsetX, offsetY);
}

void initializeGL(void)
{
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// set background color to be black
	glClearColor(0, 0, 0, 1.0);
	// change into projection mode so that we can change the camera properties
	glMatrixMode(GL_PROJECTION);
	// load the identity matrix into the projection matrix
	glLoadIdentity();
	// gluPerspective(fovy, aspect, near, far)
	gluPerspective(45, (float)*(window->getWidth()) / (float)*(window->getHeight()), 0.1, 200);
	// change into model-view mode so that we can change the object positions
	glMatrixMode(GL_MODELVIEW);
}

void initializeClasses(){
	terrain->create();
	grass = *Utils::loadImage("resources/grass.ppm");

}

void resizeWindow(int width, int height){
	window->resize(width, height);
}

void keyboardDown(unsigned char c, int x, int y){
	switch (c){
	case 'w':
		movement[2][0] = 1;
		break;
	case 's':
		movement[2][1] = -1;
		break;
	case 'd':
		movement[0][0] = 1;
		break;
	case 'a':
		movement[0][1] = -1;
		break;
	case 'e':
		movement[1][0] = 1;
		break;
	case 'q':
		movement[1][1] = -1;
		break;
	default:
		break;
	}
}

void keyboardUp(unsigned char c, int x, int y){
	switch (c){
	case 'w':
		movement[2][0] = 0;
		break;
	case 's':
		movement[2][1] = 0;
		break;
	case 'd':
		movement[0][0] = 0;
		break;
	case 'a':
		movement[0][1] = 0;
		break;
	case 'e':
		movement[1][0] = 0;
		break;
	case 'q':
		movement[1][1] = 0;
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	//initialize the toolkit
	glutInit(&argc, argv);
	//set display mode
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	//set window size
	window->initWindow();
	//set window position on screen
	//open the screen window
	//register special keyboard down function
	//glutSpecialFunc(mySpecialKeyboard);
	//register special keyboard up function
	//glutSpecialUpFunc(mySpecialKeyboardUp);
	//register redraw function
	glutDisplayFunc(displayDraw);
	//register the idle function
	glutIdleFunc(idle);
	//register standard keyboard down function
	glutKeyboardFunc(keyboardDown);
	//register standard keyboard up function
	glutKeyboardUpFunc(keyboardUp);

	glutReshapeFunc(resizeWindow);

	glutPassiveMotionFunc(idleMouseFunc);
	//initialize the rendering context
	initializeGL();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//go into a perpetual loop
	initializeClasses();
	glutMainLoop();
	return 0;
}
