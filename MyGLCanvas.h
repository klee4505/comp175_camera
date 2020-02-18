#pragma once

#ifndef MYGLCANVAS_H
#define MYGLCANVAS_H

#include <FL/gl.h>
#include <FL/glut.h>
#include <FL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <iostream>

#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "Special_Shape.h"

#include "Camera.h"


class MyGLCanvas : public Fl_Gl_Window {
public:
	glm::vec3 rotVec;
	glm::vec3 eyePosition;

	int wireframe;
	int smooth;
	int fill;
	int normal;
	int segmentsX, segmentsY;

	OBJ_TYPE objType;
	Cube* cube;
	Cylinder* cylinder;
	Cone* cone;
	Sphere* sphere;
	Shape* shape;
	Sword* sword;
	float scale;

	Camera* camera;

	MyGLCanvas(int x, int y, int w, int h, const char *l = 0);
	~MyGLCanvas();
	void setShape(OBJ_TYPE type);
	void setSegments();

private:
	void draw();
	void drawScene();

	void drawAxis();

	int handle(int);
	void resize(int x, int y, int w, int h);
	void updateCamera(int width, int height);
};

#endif // !MYGLCANVAS_H