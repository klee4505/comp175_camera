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
	rotWMat4 = glm::mat4(1.0);
	rotVMat4 = glm::mat4(1.0);
	rotUMat4 = glm::mat4(1.0);
}

//called by main.cpp as a part of the slider callback for controlling rotation
// the reason for computing the diff is to make sure that we are only incrementally rotating the camera
void Camera::setRotUVW(float u, float v, float w) {
	float diffU = u - rotU;
	float diffV = v - rotV;
	float diffW = w - rotW;
	rotateU(u);
	rotateV(v);
	rotateW(w);

	glm::mat4 newrotMat4 = rotationMat4;
	newrotMat4 *= rotUMat4;
	newrotMat4 *= rotVMat4;
	newrotMat4 *= rotWMat4;
	glm::mat4 rotTrans = glm::transpose(newrotMat4);
	glm::vec3 wVec3 = rotTrans[2];

	for (int i = 0; i < 3; i++) {
		lookVector[i] = -wVec3[i];
	}

	upVector = rotTrans[1];
	modelViewMat4 = rotUMat4 * rotWMat4 * rotVMat4 * rotationMat4 * translationMat4;

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
		cout << "LOOK: " << lookVector[i] << endl;
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
		rotationMat4[col][2] = -lookVector[col];
	}



	for (int row = 0; row < 3; row++) {
		translationMat4[3][row] = -1 * eyePosition[row];
	}

	glm::mat4 newrotMat4 = rotationMat4;
	newrotMat4 *= rotUMat4;
	newrotMat4 *= rotVMat4;
	newrotMat4 *= rotWMat4;
	glm::mat4 rotTrans = glm::transpose(newrotMat4);
	glm::vec3 wVec3 = rotTrans[2];

	for (int i = 0; i < 3; i++) {
		lookVector[i] = -wVec3[i];
	}

	upVector = rotTrans[1];
	modelViewMat4 = rotUMat4 * rotWMat4 * rotVMat4 * rotationMat4 * translationMat4;
}


void Camera::orientLookVec(glm::vec3 eyePoint, glm::vec3 lookVec, glm::vec3 upVec) {
	modelViewMat4 = glm::mat4(1.0);
	rotationMat4 = glm::mat4(1.0);
	translationMat4 = glm::mat4(1.0);

	eyePosition = eyePoint;
	upVector = upVec; //But Why

	lookVector = lookVec;
	for (int i = 0; i < 3; i++) {
		lookVector[i] = lookVector[i];
		cout << "LOOK -1: " << lookVector[i] << endl;
	}
	lookVector = glm::normalize(lookVector);

	// glm::vec3 rightVector = glm::cross(upVector, lookVector);
	glm::vec3 rightVector = glm::cross(lookVector, upVector);
	rightVector = glm::normalize(rightVector);

	// upVector = glm::cross(lookVector, rightVector);
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
		rotationMat4[col][2] = -lookVector[col];
	}

	for (int row = 0; row < 3; row++) {
		translationMat4[3][row] = -1 * eyePosition[row];
	}

	// rotationMat4 *= rotWMat4; 
	// rotationMat4 *= rotVMat4;
	// rotationMat4 *= rotUMat4;

	// glm::mat4 rotTrans = glm::transpose(rotationMat4);
	// lookVector = rotTrans[2];
	// upVector = rotTrans[1];

	glm::mat4 newrotMat4 = rotationMat4;
	newrotMat4 *= rotUMat4;
	newrotMat4 *= rotVMat4;
	newrotMat4 *= rotWMat4;
	glm::mat4 rotTrans = glm::transpose(newrotMat4);
	glm::vec3 wVec3 = rotTrans[2];

	for (int i = 0; i < 3; i++) {
		lookVector[i] = -wVec3[i];
	}
	
	upVector = rotTrans[1];
	modelViewMat4 = rotUMat4 * rotWMat4 * rotVMat4 * rotationMat4 * translationMat4;

	cout << rotUMat4[1][1] << endl;
	cout << rotWMat4[1][1] << endl;
	cout << rotVMat4[2][2] << endl;


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
	float radians = glm::radians(degrees);
	glm::mat4 temp = glm::mat4(1.0);
	temp[2][2] = glm::cos(radians);
	temp[2][0] = -glm::sin(radians);
	temp[0][2] = glm::sin(radians);
	temp[0][0] = glm::cos(radians);
	rotVMat4 = temp;
	// rotationMat4 *= rotVMat4;
	// glm::mat4 rotTrans = glm::transpose(rotationMat4);
	// lookVector = rotTrans[2];
	// upVector = rotTrans[1];
}

void Camera::rotateU(float degrees) {
	// float radians = glm::radians(degrees);
	// glm::mat4 temp = glm::mat4(1.0);
	// temp[1][1] = glm::cos(radians);
	// temp[1][2] = -glm::sin(radians);
	// temp[2][1] = glm::sin(radians);
	// temp[2][2] = glm::cos(radians);

	// rotUMat4 = temp;

	rotate(lookVector, glm::transpose(rotationMat4)[0], degrees);
}

void Camera::rotateW(float degrees) {
	float radians = glm::radians(degrees);
	glm::mat4 temp = glm::mat4(1.0);
	temp[0][0] = glm::cos(radians);
	temp[0][1] = glm::sin(radians);
	temp[1][0] = -glm::sin(radians);
	temp[1][1] = glm::cos(radians);

	rotWMat4 = temp;
	// rotationMat4 *= rotWMat4;

	// glm::mat4 rotTrans = glm::transpose(rotationMat4);
	// lookVector = rotTrans[2];
	// upVector = rotTrans[1];
}

void Camera::translate(glm::vec3 v) {

}

void Camera::rotate(glm::vec3 point, glm::vec3 axis, float degrees) {
	float m1theta = -glm::atan(axis[0] / axis[2]);
	float a = point[0];
    float b = point[1];
    float c = point[2];
    float d = glm::sqrt(glm::pow(b, 2) + glm::pow(c, 2));
    float m2theta = glm::asin(axis[0] / d) + PI / 2.0;
    float m3theta = glm::radians(degrees);

    glm::vec4 m1col0 = glm::vec4(glm::cos(m1theta), 0.0, -glm::sin(m1theta), 0.0);
    glm::vec4 m1col1 = glm::vec4(0.0, 1.0, 0.0, 0.0);
    glm::vec4 m1col2 = glm::vec4(glm::sin(m1theta), 0.0, glm::cos(m1theta), 0.0);
    glm::vec4 m1col3 = glm::vec4(0.0, 0.0, 0.0, 1.0);
    glm::vec4 m2col0 = glm::vec4(glm::cos(m2theta), glm::sin(m2theta), 0.0, 0.0);
    glm::vec4 m2col1 = glm::vec4(-glm::sin(m2theta), glm::cos(m2theta), 0.0, 0.0);
    glm::vec4 m2col2 = glm::vec4(0.0, 0.0, 1.0, 0.0);
    glm::vec4 m2col3 = glm::vec4(0.0, 0.0, 0.0, 1.0);
    glm::vec4 m3col0 = glm::vec4(1.0, 0.0, 0.0, 0.0);
    glm::vec4 m3col1 = glm::vec4(0.0, glm::cos(m3theta), glm::sin(m3theta), 0.0);
    glm::vec4 m3col2 = glm::vec4(0.0, -glm::sin(m3theta), glm::cos(m3theta), 0.0);
    glm::vec4 m3col3 = glm::vec4(0.0, 0.0, 0.0, 1.0);

    glm::mat4 m1 = glm::mat4(m1col0, m1col1, m1col2, m1col3);
    glm::mat4 m2 = glm::mat4(m2col0, m2col1, m2col2, m2col3);
    glm::mat4 m3 = glm::mat4(m3col0, m3col1, m3col2, m3col3);

    lookVector = m1 * m2 * m3 * glm::vec4(point, 1.0);
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