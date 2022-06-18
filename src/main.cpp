#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
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
#include "include/Raycast.h"

// Vertex Shader source code
// const char* vertexShaderSource = "#version 330 core\n"
// "layout (location = 0) in vec3 aPos;\n"
// "void main()\n"
// "{\n"
// "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
// "}\0";
// //Fragment Shader source code
// const char* fragmentShaderSource = "#version 330 core\n"
// "out vec4 FragColor;\n"
// "void main()\n"
// "{\n"
// "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
// "}\n\0";
// shader things
// GLuint shaderProgram;

GLuint VAO, VBO;
GLuint VAO2, VBO2;

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


Window* window = new Window(1200,800,"Window 1");
Camera* cam = new Camera(); 
Terrain* terrain = new Terrain(grass, *cam);
//test ray
Raycast* ray = new Raycast({0,0,0},{0,0,0});
Utils::point3f raySphere = {0,0,0};
GLUquadricObj *qobj = 0;

std::chrono::duration<double> calculateFPS() {
	//calculate the frame times

	auto endTime = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsedTime = endTime - startTime;
	//limit the frame times to aprox 60 frames per second
	while (elapsedTime.count() < (double)0.016) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//printf("%f\n", elapsedTime.count());
		endTime = std::chrono::system_clock::now();
		elapsedTime = endTime - startTime;
	}
	startTime = std::chrono::system_clock::now();
	deltaTime = elapsedTime.count();
	return elapsedTime;
}

void displayDraw(void){
	// glUseProgram(shaderProgram);
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

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPushMatrix();
	glTranslated(1,0,0);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
	glPopMatrix();

	ray->render();
	qobj = gluNewQuadric();

	glColor3f(1,0,0);
	glPushMatrix();
	glTranslatef(ray->getRayEnd().x, ray->getRayEnd().y, ray->getRayEnd().z);
	gluSphere(qobj, .1, 30, 30);
	glPopMatrix();

	terrain->render();

	//glFlush();
	glutSwapBuffers();
}

void idle(void){
	fps = 1.0f/(calculateFPS().count());
	//std::this_thread::sleep_for(std::chrono::milliseconds(5));
	printf("%f\n", fps);

	cam->moveZ( (movement[2][0] + movement[2][1]) * playerMovementSpeed * deltaTime);
	cam->moveX( (movement[0][0] + movement[0][1]) * playerMovementSpeed * deltaTime);
	cam->moveY((movement[1][0] + movement[1][1]) * playerMovementSpeed * deltaTime);

	//raycasting test
	delete(ray);
	ray = new Raycast(cam->getCameraPos(), cam->getLookTheta());
	// ray->cast(4);
	//printf("ray Z: %f\n", ray->getRayEnd().z);
	//printf("ray X: %f\n", ray->getRayEnd().x);
	// Block& block = terrain->getBlockAt(ray->getRayEnd());
	// if(block.type != bType_null){
		// block.type = bType_air;
	// }
	//printf("Block: %d\n", .type);

	glutPostRedisplay();
}

void idleMouseFunc(int x, int y) {
	static bool warped = false;
	float offsetX = window->getWidth() / 2 - x;
	float offsetY = window->getHeight() / 2 - y;
	if(offsetX == 0 && offsetY == 0){
		warped = true;
	}
	else{
		warped = false;
	}
	if(!warped){
		glutWarpPointer(window->getWidth() / 2, window->getHeight() / 2);
		glutSetCursor(GLUT_CURSOR_NONE);
		warped = true;
	}
	mousePos[0] = x;
	mousePos[1] = y;
	cam->calcCameraLookMouse(offsetX, offsetY);
}

void initializeGL(void)
{
	GLenum err = glewInit();
if (GLEW_OK != err)
{
  /* Problem: glewInit failed, something is seriously wrong. */
  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
}
fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
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
	gluPerspective(45, (float)(window->getWidth()) / (float)(window->getHeight()), 0.1, 200);
	// change into model-view mode so that we can change the object positions
	glMatrixMode(GL_MODELVIEW);

	// // enabling shaders
	// GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// glCompileShader(vertexShader);

	// GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// glCompileShader(fragmentShader);

	// // shader program
	// shaderProgram = glCreateProgram();
	// glAttachShader(shaderProgram, vertexShader);
	// glAttachShader(shaderProgram, fragmentShader);
	// glLinkProgram(shaderProgram);
	// glDeleteShader(vertexShader);
	// glDeleteShader(fragmentShader);

	float positions[15] = {
	0,10,2,  1,1,
	-10,5,2, 1,0,
	10,5,2,  0,0,
	};

	// buffer tests
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	float positions2[15] = {
	0,5,2,  1,1,
	-10,5,2, 1,0,
	10,5,2,  0,0,
	};

	// buffer tests 2
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO2);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions2), positions2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}

void initializeClasses(){
	terrain->create();
	grass = *Utils::loadImage("resources/grass.ppm");

	float positions[15] = {
	0,10,2,  1,1,
	-10,5,2, 1,0,
	10,5,2,  0,0,
	};



	// GLuint buffer;
	// glGenBuffers(1, &buffer);
	// glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// glBufferData(GL_ARRAY_BUFFER, 15*sizeof(float), positions, GL_STATIC_DRAW);
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	// // glVertexAttribPointer(3,2, GLfloat, GL_FALSE, sizeof(float) * 2, (void*)3*sizeof(float));

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
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
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
