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

	//Translation: Initial setting
	modelViewMat4[3][0] = -eyePoint[0];
	modelViewMat4[3][1] = -eyePoint[1];
	modelViewMat4[3][2] = -eyePoint[2];
	eyePosition = eyePoint;

	//Setting lookVector and upVector
	lookVector[0] = eyePosition[0] - lookatPoint[0];
	lookVector[1] = eyePosition[1] - lookatPoint[1];
	lookVector[2] = eyePosition[2] - lookatPoint[2];
	//Only orientLookAt should initialize upVector
	upVector = upVec;
	
	
	
	
	
	cout << "Look Vector: {";
	for (int i = 0; i < 3; i++) {
		cout << lookVector[i] << " ";
	}
	cout << "}" << endl;
	
	glm::vec3 rightVector = glm::cross(lookVector, upVec);
	//The true upVector should only be set when rotV is called; otherwise stay at (0, 1, 0)
	glm::vec3 tempUpVec = glm::cross(rightVector, lookVector);

	cout << "Right Vector: {";
	for (int i = 0; i < 3; i++) {
		cout << rightVector[i] << " ";
	}
	cout << "}" << endl;

	cout << "Up Vector: {";
	for (int i = 0; i < 3; i++) {
		cout << upVector[i] << " ";
	}
	cout << "}" << endl;

	//Normalize?
	lookVector = glm::normalize(lookVector);
	upVector = glm::normalize(upVector);
}


void Camera::orientLookVec(glm::vec3 eyePoint, glm::vec3 lookVec, glm::vec3 upVec) {

	//Rotation?
	//Cross lookVec by upVec to get rightVec
	lookVector = lookVec;
	glm::vec3 rightVector = glm::cross(lookVector, upVec);    //U
	glm::vec3 tempUpVec = glm::cross(rightVector, lookVector); //V

	//Normalize?
	lookVector = glm::normalize(lookVector);
	tempUpVec = glm::normalize(tempUpVec);
	rightVector = glm::normalize(rightVector);
//	upVector = glm::normalize(upVector);

	cout << "Look Vector: {";
	for (int i = 0; i < 3; i++) {
		cout << lookVector[i] << " ";
	}
	cout << "}" << endl;

	cout << "Old Up Vector: {";
	for (int i = 0; i < 3; i++) {
		cout << upVec[i] << " ";
	}
	cout << "}" << endl;
	
	cout << "Right Vector: {";
	for (int i = 0; i < 3; i++) {
		cout << rightVector[i] << " "; 
	}
	cout << "}" << endl;

	cout << "Up Vector: {";
	for (int i = 0; i < 3; i++) {
		cout << upVector[i] << " ";
	}
	cout << "}" << endl;

	for (int row = 0; row < 3; row++) {
		modelViewMat4[0][row] = rightVector[row];
		modelViewMat4[1][row] = tempUpVec[row];
		modelViewMat4[2][row] = lookVector[row];
	}
	
	//Translation: eyePosition is the old eyePoint
	glm::vec3 translateVector;
	for (int i = 0; i < 3; i++) {
		translateVector[i] = -1 * (eyePoint[i] - eyePosition[i]);
	}
	translate(translateVector);
	eyePosition = eyePoint;




	cout << "--------------------------" << endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << modelViewMat4[j][i] << ", ";
		}
		cout << endl;
	}
	cout << "--------------------------" << endl;
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
	//modelViewMat4 = glm::mat4(1.0);
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
	//There's no way we multiply by translate_mat each time... right? 
	//Order of operations: rotate to lookVec, then scale by directly changing -px, -py, -pz?
	glm::mat4 translate_mat(1.0);
	translate_mat[3][0] = v[0];
	translate_mat[3][1] = v[1];
	translate_mat[3][2] = v[2];
	modelViewMat4 *= translate_mat;
	
	/*modelViewMat4[3][0] = v[0];
	modelViewMat4[3][1] = v[1];
	modelViewMat4[3][2] = v[2];*/
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
