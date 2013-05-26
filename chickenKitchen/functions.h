#include <windows.h>
#include "gl.h"
#include "glu.h"
#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#pragma once

#define M_PI 3.14159265358979323846f

extern GLint width, height;

void init(void );
void reshape(GLint /*_w*/, GLint /*_h*/);
void idle(void );
void draw(void );
int key(int /*key*/); //zwraca czy nale¿y przerysowywaæ

void myInit();
void myDisplay();