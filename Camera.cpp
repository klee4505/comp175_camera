#include "Camera.h"

Camera::Camera() {
	modelViewMat4 = glm::mat4(1.0);
	reset();
}

Camera::~Camera() {
}

void Camera::reset() {
	orientLookAt(glm::vec3(0.0f, 0.0f, DEFAULT_FOCUS_LENGTH), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	setViewAngle(VIEW_ANGLE);
	setNearPlane(NEAR_PLANE);
	setFarPlane(FAR_PLANE);
	screenWidth = screenHeight = 200;
	screenWidthRatio = 1.0f;
	rotU = rotV = rotW = 0;
}

//called by main.cpp as a part of the slider callback for controlling rotation
// the reason for computing the diff is to make sure that we are only incrementally rotating the camera
void Camera::setRotUVW(float u, float v, float w) {
	float diffU = u - rotU;
	float diffV = v - rotV;
	float diffW = w - rotW;
	rotateU(diffU);
	rotateV(diffV);
	rotateW(diffW);
	rotU = u;
	rotV = v;
	rotW = w;
}


void Camera::orientLookAt(glm::vec3 eyePoint, glm::vec3 lookatPoint, glm::vec3 upVec) {
	eyePosition = eyePoint;
	lookVector[0] = eyePosition[0] - lookatPoint[0];
	lookVector[1] = eyePosition[1] - lookatPoint[1];
	lookVector[2] = eyePosition[2] - lookatPoint[2];
	
	glm::vec3 rightVector = glm::cross(upVec, lookVector);
	glm::vec3 upVector = glm::cross(lookVector, rightVector);

}


void Camera::orientLookVec(glm::vec3 eyePoint, glm::vec3 lookVec, glm::vec3 upVec) {
	glm::vec3 translateVector = eyePoint;
	eyePosition = eyePoint;
	lookVector = lookVec;
	translate(translateVector);
}

glm::mat4 Camera::getScaleMatrix() {
	return scaleMat4;
}

glm::mat4 Camera::getInverseScaleMatrix() {
	return invScaleMat4;
}

glm::mat4 Camera::getUnhingeMatrix() {
	return unhingeMat4;
}


glm::mat4 Camera::getProjectionMatrix() {
	glm::mat4 projMat4(1.0);
	return projMat4;
}

glm::mat4 Camera::getModelViewMatrix() {
	glm::mat4 tempModelViewMat4 = modelViewMat4;
	cout << "--------------------------" << endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << modelViewMat4[j][i] << ", ";
		}
		cout << endl;
	}
	cout << "--------------------------" << endl;
	modelViewMat4 = glm::mat4(1.0);
	return tempModelViewMat4;
}

glm::mat4 Camera::getInverseModelViewMatrix() {
	return invModelViewMat4;
}


void Camera::setViewAngle (float _viewAngle) {
	viewAngle = _viewAngle;
}

void Camera::setNearPlane (float _nearPlane) {
	nearPlane = _nearPlane;
}

void Camera::setFarPlane (float _farPlane) {
	farPlane = _farPlane;
}

void Camera::setScreenSize (int _screenWidth, int _screenHeight) {
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
}

void Camera::rotateV(float degrees) {
}

void Camera::rotateU(float degrees) {
}

void Camera::rotateW(float degrees) {
}

void Camera::translate(glm::vec3 v) {
	cout << "V: " << v[0] << " " << v[1] << " " << v[2] << endl;
	modelViewMat4[3][0] = v[0];
	modelViewMat4[3][1] = v[1];
	modelViewMat4[3][2] = v[2];
}

void Camera::rotate(glm::vec3 point, glm::vec3 axis, float degrees) {
}


glm::vec3 Camera::getEyePoint() {
	return eyePosition;
}

glm::vec3 Camera::getLookVector() {
	return lookVector;
}

glm::vec3 Camera::getUpVector() {
	return upVector;
}

float Camera::getViewAngle() {
	return viewAngle;
}

float Camera::getNearPlane() {
	return nearPlane;
}

float Camera::getFarPlane() {
	return farPlane;
}

int Camera::getScreenWidth() {
	return screenWidth;
}

int Camera::getScreenHeight() {
	return screenHeight;
}

float Camera::getScreenWidthRatio() {
	return screenWidthRatio;
}
