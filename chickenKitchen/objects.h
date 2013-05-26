#include <windows.h>
#include "gl.h"
#include "glu.h"
#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define OBJ_LIST			13
#define BOT_BODY_OBJ		obj_list[1]
#define BOT_BALL_OBJ		obj_list[2]
#define STATIC_OBJ			obj_list[3]
#define SHADOW_OBJ			obj_list[4]
#define TABLE_OBJ			obj_list[5]
#define ROOM_OBJ			obj_list[6]
#define FIELD_OBJ			obj_list[7]
#define TABLE_LEG_OBJ		obj_list[8]
#define CHICKEN_BALL_OBJ	obj_list[9]
#define BOT_HAT_OBJ			obj_list[10]
#define OWEN_OBJ			obj_list[11]
#define PAN_OBJ				obj_list[12]

#define TEX_LIST			9
#define PLATE_TEX			tex_list[0]
#define WOOD_D_TEX			tex_list[1]
#define WOOD_B_TEX			tex_list[2]
#define BAL_TEX				tex_list[3]
#define PTR_TEX				tex_list[4]
#define FIELD_TEX			tex_list[5]
#define BLACK_TEX			tex_list[6]
#define CHROME_TEX			tex_list[7]
#define WOOD_TEX			tex_list[8]

extern GLint obj_list[OBJ_LIST];
extern GLint tex_list[TEX_LIST]; 
extern GLUquadricObj *qua;

GLvoid field (GLvoid);
GLvoid table(int /*is_shadow*/);
GLvoid chicken_ball(GLvoid);
GLvoid room(GLvoid);
GLvoid table_leg(GLvoid);
GLvoid bot_body(GLvoid);
GLvoid bot_ball(GLvoid);
GLvoid bot_hat(GLvoid);

GLvoid owen(GLvoid);
GLvoid pan(GLvoid);
