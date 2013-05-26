#include <windows.h>
#include "gl.h"
#include "glu.h"
#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

GLvoid shadowmatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4], GLfloat lightpos[4]);
