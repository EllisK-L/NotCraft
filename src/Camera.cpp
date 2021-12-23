#include <GL/glut.h>
#include "include/Camera.h"
#include <stdio.h>
#include <math.h>

#define PI 3.14159265
// conversion multiplier for converting from degrees to Radians
#define DEG_TO_RAD PI/180.0

Camera::Camera() {
	printf("Camera Created!\n");
}
bool Camera::lookAt(GLfloat lookAtVector[3]) {

	return true;
}

void Camera::calcCameraLookMouse(GLfloat xOffset, GLfloat yOffset) {
	lookTheta[0] += xOffset * sensitivity * -1;
	lookTheta[1] += yOffset * sensitivity;

	if (lookTheta[1] > 45) {
		lookTheta[1] = 45;
	}
	else if (lookTheta[1] < -45) {
		lookTheta[1] = -45;
	}

	cameraLook[0] = cos(lookTheta[0] * DEG_TO_RAD) * cos(lookTheta[1] * DEG_TO_RAD) + cameraPos[0];
	cameraLook[1] = sin(lookTheta[1] * DEG_TO_RAD) + cameraPos[1];
	cameraLook[2] = sin(lookTheta[0] * DEG_TO_RAD) * cos(lookTheta[1] * DEG_TO_RAD) + cameraPos[2];
}

void Camera::moveX(GLfloat amount) {
	cameraPos[0] += 10 * amount * cos((lookTheta[0] + 90) * DEG_TO_RAD);
	//cameraPosition[1] +=  -10 * movement[2] * sin(mouseTheta[1] * DEG_TO_RAD);
	cameraPos[2] += 10 * amount * sin((lookTheta[0] + 90) * DEG_TO_RAD);
	calcCameraLookMouse(0, 0);
}

void Camera::moveY(GLfloat amount) {
	cameraPos[1] += amount;
	calcCameraLookMouse(0, 0);
}

void Camera::moveZ(GLfloat amount) {
	cameraPos[0] += 10 * amount * cos(lookTheta[0] * DEG_TO_RAD);
	//cameraPosition[1] +=  -10 * movement[2] * sin(mouseTheta[1] * DEG_TO_RAD);
	cameraPos[2] += 10 * amount * sin(lookTheta[0] * DEG_TO_RAD);
	calcCameraLookMouse(0, 0);
}

void Camera::update() {
	cameraPos[1] -= .1;
	if (cameraPos[1] < 1) {
		cameraPos[1] = 1;
	}
}

void Camera::render() {
	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
		cameraLook[0], cameraLook[1], cameraLook[2],
		cameraUp[0], cameraUp[1], cameraUp[2]);
}

GLfloat* Camera::getPos() {
	return cameraPos;
}

void Camera::calcCameraLookAt() {

}