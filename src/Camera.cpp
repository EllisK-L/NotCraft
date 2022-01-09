#include <GL/glut.h>
#include "include/Camera.h"
#include "include/Utils.h"
#include <stdio.h>
#include <math.h>

Camera::Camera() {
	printf("Camera Created!\n");
}
bool Camera::lookAt(GLfloat lookAtVector[3]) {

	return true;
}

void Camera::calcCameraLookMouse(GLfloat xOffset, GLfloat yOffset) {
	lookTheta.x += xOffset * sensitivity * -1;
	lookTheta.y += yOffset * sensitivity;

	if (lookTheta.y > 80) {
		lookTheta.y = 80;
	}
	else if (lookTheta.y < -80) {
		lookTheta.y = -80;
	}

	cameraLook.x = cos(lookTheta.x * DEG_TO_RAD) * cos(lookTheta.y * DEG_TO_RAD) + cameraPos.x;
	cameraLook.y = sin(lookTheta.y * DEG_TO_RAD) + cameraPos.y;
	cameraLook.z = sin(lookTheta.x * DEG_TO_RAD) * cos(lookTheta.y * DEG_TO_RAD) + cameraPos.z;
}

void Camera::moveX(GLfloat amount) {
	cameraPos.x += 10 * amount * cos((lookTheta.x + 90) * DEG_TO_RAD);
	//cameraPosition.y +=  -10 * movement.z * sin(mouseTheta.y * DEG_TO_RAD);
	cameraPos.z += 10 * amount * sin((lookTheta.x + 90) * DEG_TO_RAD);
	calcCameraLookMouse(0, 0);
}

void Camera::moveY(GLfloat amount) {
	cameraPos.y += amount * 5;
	calcCameraLookMouse(0, 0);
}

void Camera::moveZ(GLfloat amount) {
	cameraPos.x += 10 * amount * cos(lookTheta.x * DEG_TO_RAD);
	//cameraPos.y += 10 * amount * sin(lookTheta.y * DEG_TO_RAD);
	cameraPos.z += 10 * amount * sin(lookTheta.x * DEG_TO_RAD);
	calcCameraLookMouse(0, 0);
}

void Camera::update() {
	cameraPos.y -= .1;
	if (cameraPos.y < 1) {
		cameraPos.y = 1;
	}
}

void Camera::render() {
	gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
		cameraLook.x, cameraLook.y, cameraLook.z,
		cameraUp.x, cameraUp.y, cameraUp.z);
}

Utils::point3f Camera::getCurrentPos() {
	Utils::point3f pos = {
		cameraPos.x,
		cameraPos.y,
		cameraPos.z
	};
	return pos;
}

void Camera::calcCameraLookAt() {

}