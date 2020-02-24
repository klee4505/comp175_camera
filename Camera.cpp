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
	cout << "Called orientLookAt" << endl;
	modelViewMat4 = glm::mat4(1.0);
	rotationMat4 = glm::mat4(1.0);
	translationMat4 = glm::mat4(1.0);
	eyePosition = eyePoint;
	upVector = upVec; //But Why

	for (int i = 0; i < 3; i++) {
		lookVector[i] = lookatPoint[i] - eyePosition[i]; //Final - Initial
	}
	/*for (int i = 0; i < 3; i++) {
		lookVector[i] = -1 * lookVector[i];
	}*/
	lookVector = glm::normalize(lookVector);

//	glm::vec3 rightVector = glm::cross(upVector, lookVector);
	glm::vec3 rightVector = glm::cross(lookVector, upVector);
	rightVector = glm::normalize(rightVector);

//	upVector = glm::cross(lookVector, rightVector);
	upVector = glm::cross(rightVector, lookVector);
	upVector = glm::normalize(upVector);

	//@TODO: Go by Columns or by Rows?
	//for (int row = 0; row < 3; row++) {
	//	rotationMat4[0][row] = rightVector[row];
	//	rotationMat4[1][row] = upVector[row];
	//	rotationMat4[2][row] = lookVector[row];
	//}
	for (int col = 0; col < 3; col++) {
		rotationMat4[col][0] = rightVector[col];
		rotationMat4[col][1] = upVector[col];
		rotationMat4[col][2] = lookVector[col];
	}



	for (int row = 0; row < 3; row++) {
		translationMat4[3][row] = -1 * eyePosition[row];
	}

	modelViewMat4 = rotationMat4 * translationMat4;
}


void Camera::orientLookVec(glm::vec3 eyePoint, glm::vec3 lookVec, glm::vec3 upVec) {
	modelViewMat4 = glm::mat4(1.0);
	rotationMat4 = glm::mat4(1.0);
	translationMat4 = glm::mat4(1.0);

	eyePosition = eyePoint;
	upVector = upVec; //But Why

	lookVector = lookVec;
	for (int i = 0; i < 3; i++) {
		lookVector[i] = -1 * lookVector[i];
	}
	lookVector = glm::normalize(lookVector);

	glm::vec3 rightVector = glm::cross(upVector, lookVector);
//	glm::vec3 rightVector = glm::cross(lookVector, upVector);
	rightVector = glm::normalize(rightVector);

	upVector = glm::cross(lookVector, rightVector);
//	upVector = glm::cross(rightVector, lookVector);
	upVector = glm::normalize(upVector);

	//@TODO: Go by Columns or by Rows?
	//for (int row = 0; row < 3; row++) {
	//	rotationMat4[0][row] = rightVector[row];
	//	rotationMat4[1][row] = upVector[row];
	//	rotationMat4[2][row] = lookVector[row];
	//}
	for (int col = 0; col < 3; col++) {
		rotationMat4[col][0] = rightVector[col];
		rotationMat4[col][1] = upVector[col];
		rotationMat4[col][2] = lookVector[col];
	}

	for (int row = 0; row < 3; row++) {
		translationMat4[3][row] = -1 * eyePosition[row];
	}

	modelViewMat4 = rotationMat4 * translationMat4;
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
//	projMat4 = glm::mat4(1.0);
	return projMat4;
}

glm::mat4 Camera::getModelViewMatrix() {
	return modelViewMat4;
}

glm::mat4 Camera::getInverseModelViewMatrix() {
	return invModelViewMat4;
}


void Camera::setViewAngle (float _viewAngle) {
	viewAngle = _viewAngle;

	updateScalingMat();
}

void Camera::setNearPlane (float _nearPlane) {
	nearPlane = _nearPlane;

	updateUnhingeMat();
}

void Camera::setFarPlane (float _farPlane) {
	farPlane = _farPlane;

	updateScalingMat();
	updateUnhingeMat();
}

void Camera::setScreenSize (int _screenWidth, int _screenHeight) {
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;

	updateScalingMat();
}

void Camera::rotateV(float degrees) {
}

void Camera::rotateU(float degrees) {
}

void Camera::rotateW(float degrees) {
}

void Camera::translate(glm::vec3 v) {

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

void Camera::updateScalingMat() {
	//cout << "Updating" << endl;
	screenWidthRatio = (float)screenWidth / (float)screenHeight;
	heightAngle = glm::radians(viewAngle);
	widthAngle = heightAngle * screenWidthRatio;
	scaleMat4 = glm::mat4(1.0);

	scaleMat4[0][0] = 1.0 / (glm::tan(widthAngle / 2.0) * farPlane);
	scaleMat4[1][1] = 1.0 / (glm::tan(heightAngle / 2.0) * farPlane);
	scaleMat4[2][2] = 1.0 / farPlane;

	projMat4 = unhingeMat4 * scaleMat4;
}

void Camera::updateUnhingeMat() {
	float c = -1.0 * nearPlane / farPlane;
	unhingeMat4 = glm::mat4(1.0);

	unhingeMat4[2][2] = -1.0 / (c + 1.0);
	unhingeMat4[2][3] = -1.0;
	unhingeMat4[3][2] = c / (c + 1.0);
	unhingeMat4[3][3] = 0;

	projMat4 = unhingeMat4 * scaleMat4;
}