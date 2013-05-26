#include "functions.h"
#include "objects.h"
#include "shadow.h"
#include "tectures.h"
#include "structs.h"
#include "scenario.h"
#include "main.h"
#include <fstream>
#include <iostream>
#include <cmath>
#pragma once

using namespace std;

static SYSTEMTIME tim;

GLint width = 800,
	light_mov=0,
	height = 600;

static GLfloat phase = 0, 
	light_phase = 0;

GLfloat
	light_pos[4] = {-1, 12, 3, 0},
	light_col[4] = {64, 64, 64, 1},
	light_amb[4] = {.01f, .01f, .01f, 1},
	plane_main[4] = {0, 1, 0, 6.99f},
	plane_back[4] = {0, 0, 1, 1},
	shadow_mat_main[4][4];

enum {
  X, Y, Z, W
};

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

void write_to_file() {

	float model[16];
	float pro[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, model);
	glGetFloatv(GL_PROJECTION_MATRIX, pro);

	FILE *f = fopen("temp.txt", "a");

	fprintf(f, "\nModel: ");
	for( int i=0; i<16; i++)
		fprintf(f, " | %f", model[i]);
	fclose(f);
}

void updateCoo(GLfloat x, GLfloat y, GLfloat z) 
{
	GLfloat model[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, model);
	point.x = model[0] * x + model[4] * y + model[8] * z + model[12];
	point.y = model[1] * x + model[5] * y + model[9] * z + model[13];
	point.z = model[2] * x + model[6] * y + model[10] * z + model[14];

	FILE *f = fopen("temp.txt", "a");

	fprintf(f, " | %f", point.x);
	fprintf(f, " | %f", point.y);
	fprintf(f, " | %f", point.z);
	
	fclose(f);
}

void fps_calc(void) {

  static GLint t0=clock(), frames=0;
  GLint t=clock();

  frames++;

  if (t-t0 >= CLOCKS_PER_SEC*3) {
    static char buf[20];
    GLfloat secs=(float)(t-t0)/CLOCKS_PER_SEC;
    GLfloat fps=frames/secs;
    sprintf_s(buf, ": [%3.1f FPS]", fps);
    winTitle(buf);
    t0=t;
    frames=0;
  }
}

void update_progress(void)
{
	GetSystemTime(&tim);

	if (light_mov) {
		light_phase+=.7f;
		if (light_phase>=360) light_phase-=360;
		light_pos[X]+=(float )sin(M_PI*light_phase/180)/50;
		light_pos[Z]+=(float )cos(M_PI*light_phase/180)/50;
	}

	phase=(float)sin(M_PI*((float)tim.wMilliseconds/1000+((tim.wSecond&1)?1:0)));
} // update_timer

void loadTex2D(char *file_name) {
	int w, h;
	GLubyte *t_tex=read_rgb_texture(file_name, &w, &h);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, t_tex);
} // loadTex2D

void loadTex1D(char *file_name) {
	int w, h;
	GLubyte *t_tex=read_rgb_texture(file_name, &w, &h);

	glTexImage1D(GL_TEXTURE_1D, 0, 3, w*h, 0, GL_RGB, GL_UNSIGNED_BYTE, t_tex);
} // loadTex1D

void init(void )
{
	GLfloat
		fog_c[4]={.6f, .6f, .6f, 1};
	GLint first, i;

	shadowmatrix(shadow_mat_main, plane_main, light_pos);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_col);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);  
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	glEnable(GL_FOG);
	glFogf(GL_FOG_DENSITY, 0.004f);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fog_c);
	glHint(GL_FOG_HINT, GL_NICEST);

				/* tryb nak-adania textur */
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

				/* parametry texturowania 2D */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

				/* parametry texturowania 1D */
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				/* inicjalizacja kwadryki */
	qua=gluNewQuadric();
	gluQuadricNormals(qua, GLU_SMOOTH);
	gluQuadricTexture(qua, GLU_TRUE);

	first=glGenLists(OBJ_LIST+TEX_LIST);
	for (i=0; i<OBJ_LIST; i++)
		obj_list[i]=i+first;
	first+=OBJ_LIST;
	for (i=0; i<TEX_LIST; i++)
		tex_list[i]=i+first;


	glNewList(FIELD_OBJ, GL_COMPILE);
		field();
	glEndList();

	glNewList(TABLE_OBJ, GL_COMPILE);
		table(0);
	glEndList();

	glNewList(TABLE_LEG_OBJ, GL_COMPILE);
		table_leg();
	glEndList();

	glNewList(ROOM_OBJ, GL_COMPILE);
		room();
	glEndList();

	glNewList(CHICKEN_BALL_OBJ, GL_COMPILE);
		chicken_ball();
	glEndList();

	glNewList(BOT_BODY_OBJ, GL_COMPILE);
		bot_body();
	glEndList();

	glNewList(BOT_HAT_OBJ, GL_COMPILE);
		bot_hat();
	glEndList();

	glNewList(BOT_BALL_OBJ, GL_COMPILE);
		bot_ball();
	glEndList();

	glNewList(OWEN_OBJ, GL_COMPILE);
		owen();
	glEndList();

	glNewList(PAN_OBJ, GL_COMPILE);
		pan();
	glEndList();

	glNewList(PLATE_TEX, GL_COMPILE);
		loadTex2D("tex/plate.rgb");
	glEndList();

	glNewList(WOOD_D_TEX, GL_COMPILE);
		loadTex2D("tex/wood_d.rgb");
	glEndList();

	glNewList(WOOD_B_TEX, GL_COMPILE);
		loadTex2D("tex/wood_b.rgb");
	glEndList();

	glNewList(PTR_TEX, GL_COMPILE);
		loadTex2D("tex/ptr.rgb");
	glEndList();

	glNewList(BAL_TEX, GL_COMPILE);
		loadTex1D("tex/bal.rgb");
	glEndList();

	glNewList(FIELD_TEX, GL_COMPILE);
		loadTex2D("tex/field.rgb");
	glEndList();

	glNewList(BLACK_TEX, GL_COMPILE);
		loadTex2D("tex/black.rgb");
	glEndList();

	glNewList(CHROME_TEX, GL_COMPILE);
		loadTex2D("tex/chrom.rgb");
	glEndList();

	glNewList(WOOD_TEX, GL_COMPILE);
		loadTex2D("tex/wood.rgb");
	glEndList();

	update_progress();
} // init

void reshape(GLint _w, GLint _h) {
	GLfloat h=(GLfloat)_h/(GLfloat)_w;

	glViewport(0, 0, _w, _h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -h, h, 5.0, 350.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	height=_h; width=_w;
} // reshape

void idle(void ) {
	update_progress();
} // idle

void draw(void ) {
	pcs.progress += pcs.chicken_speed;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(.3f, .3f, 1, 1);

	if (light_mov) {
		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
		shadowmatrix(shadow_mat_main, plane_main, light_pos);
	}

	glPushMatrix();

/////// camera ////////////////////////////////////////////////////////////////////////////

		glTranslatef(0.0, 4.0, -40.0);
		glTranslatef(0, 0, viewed.dist);
		glRotatef(viewed.height, 1, 0, 0);
		glRotatef(viewed.rot, 0, 1, 0);
	
/////// shadow ////////////////////////////////////////////////////////////////////////////

		glPushMatrix();
			glCallList(STATIC_OBJ);
			glMultMatrixf((GLfloat *)shadow_mat_main);
			glCallList(SHADOW_OBJ);
		glPopMatrix();
		
/////// field /////////////////////////////////////////////////////////////////////////////
		
		glPushMatrix();
			glScalef(1000, 1, 1000);
			glTranslatef(-.5f, -2.5f, -.5f);
			glCallList(FIELD_OBJ);
		glPopMatrix();

/////// table /////////////////////////////////////////////////////////////////////////////
		
		glPushMatrix();
			glTranslatef(0, -1.5f, 10.0f);
			glScalef(8.0f, .2f, 4.0f);
			glCallList(TABLE_OBJ);
		glPopMatrix();

/////// legs from table
		
		glPushMatrix();
			glTranslatef(5.0f, -2.5f, 8.0f);
			glRotatef(-90.0,1,0,0);
			glCallList(TABLE_LEG_OBJ);
		glPopMatrix();

/////// legs from table

		glPushMatrix();
			glTranslatef(-5.0f, -2.5f, 8.0f);
			glRotatef(-90.0,1,0,0);
			glCallList(TABLE_LEG_OBJ);
		glPopMatrix();
		
/////// legs from table

		glPushMatrix();
			glTranslatef(5.0f, -2.5f, 12.0f);
			glRotatef(-90.0,1,0,0);
			glCallList(TABLE_LEG_OBJ);
		glPopMatrix();
		
/////// legs from table

		glPushMatrix();
			glTranslatef(-5.0f, -2.5f, 12.0f);
			glRotatef(-90.0,1,0,0);
			glCallList(TABLE_LEG_OBJ);
		glPopMatrix();

/////// room - walls ///////////////////////////////////////////////////////////////////
		
		glPushMatrix();
			glScalef(1.5f, 1.5f, 1.5f);
			glCallList(ROOM_OBJ);
		glPopMatrix();

/////// chicken ////////////////////////////////////////////////////////////////////////
	
		glPushMatrix();
			chicken_move();
			
////////// chicken - body
			
			glPushMatrix();
				glTranslatef(0, 0, 2);
				glScalef(0.6,0.4,0.5);
				glCallList(CHICKEN_BALL_OBJ);
			glPopMatrix();
			
/////////// chicken - head
		
			glPushMatrix();
				glTranslatef(-.4, .3f, 2);
				glScalef(0.2,0.2,0.2);
				glCallList(CHICKEN_BALL_OBJ);
			glPopMatrix();
			
/////////// chicken - wing
		
			glPushMatrix();
				glTranslatef(0, .1f, 2.5f);
				glScalef(0.3, 0.3, 0.1);
				glCallList(CHICKEN_BALL_OBJ);
			glPopMatrix();
			
/////////// chicken - wing
		
			glPushMatrix();
				glTranslatef(0, .1f, 1.5f);
				glScalef(0.3, 0.3, 0.1);
				glCallList(CHICKEN_BALL_OBJ);
			glPopMatrix();
		
		glPopMatrix();
	
/////// bot ////////////////////////////////////////////////////////////////////////

		glPushMatrix();
			glTranslatef(0, 0, 9.f);
			
/////////// bot's chest

			glPushMatrix();
				glScalef(0.5, 0.5, 0.5);
				glCallList(BOT_BALL_OBJ);
			glPopMatrix();
			
			glPushMatrix();
				glTranslatef(0, -1.5f, 0.f);
				glScalef(1.5f, 2.f, 1.5f);
				glRotatef(-90.0,1,0,0);
				glCallList(BOT_BODY_OBJ);
			glPopMatrix();

//////////// bot's right arm
			
			glPushMatrix();
				bot_right_arm();
				glCallList(BOT_BODY_OBJ);;
			glPopMatrix();
			
//////////// obliczanie punktu zaczepienia

			glPushMatrix();
				glLoadIdentity();
				bot_right_arm();
				updateCoo(0,0,2);
			glPopMatrix();

//////////// staw w prawej rece

			glPushMatrix();
				glTranslatef(point.x, point.y, point.z);
				glScalef(.35f, .35f, .35f);
				glCallList(BOT_BALL_OBJ);
			glPopMatrix();

//////////// przedramie bota
			
			glPushMatrix();
				glTranslatef(point.x, point.y, point.z);				
				bot_right_forhand();
				glCallList(BOT_BODY_OBJ);
			glPopMatrix();

/////////// bot's head

			glPushMatrix();
				shake_head();
				glCallList(BOT_BALL_OBJ);
			glPopMatrix();

/////////// disc down

			glPushMatrix();
				down_disc();
				glCallList(BOT_BALL_OBJ);
			glPopMatrix();

/////////// disc up

			glPushMatrix();
				up_disc();
				glCallList(BOT_BALL_OBJ);
			glPopMatrix();

		glPopMatrix();

/////// kitchen /////////////////////////////////////////////////////////////////////////////

		glPushMatrix();
			glTranslatef(0, 0, 12.f);

/////////// owen

			glPushMatrix();
				glTranslatef(0, -1.3f, 0.f);
				glScalef(1.f, .2f, 1.f);
				glCallList(OWEN_OBJ);
			glPopMatrix();

/////////// pan

			glPushMatrix();
				glTranslatef(0, -1.f, 0.f);
				glScalef(.5f, .3f, .5f);
				glRotatef(-90.0,1,0,0);
				glCallList(PAN_OBJ);
			glPopMatrix();

		glPopMatrix();

	glPopMatrix();
	fps_calc();
}

int key(int vkey) 
{
	switch (vkey) 
	{
	case 'q': case 'Q':
		exit(0);
	case 'z': case 'Z':
	    viewed.dist-=1;
		if (viewed.dist <-10) viewed.dist=-10;
		break;
	case 'a': case 'A':
		viewed.dist+=1;
		if (viewed.dist >10) viewed.dist=10;
		break;
	case 's': case 'S':
		pcs.chicken_speed += 1;
		if ( pcs.chicken_speed > 40)
			pcs.chicken_speed = 40;
		break;
	case 'x': case 'X':
		pcs.chicken_speed -= 1;
		if ( pcs.chicken_speed < 1)
			pcs.chicken_speed = 1;
		break;
	case 'w': case 'W':
		pcs.progress = 0;
		break;
	case VK_UP:
		viewed.height+=.5;
		if (viewed.height > 40) viewed.height=40;
		break;
	case VK_DOWN:
		viewed.height-=.5;
		if (viewed.height < 10) viewed.height=10;
		break;
	case VK_LEFT:
		viewed.rot+=2;
		if (viewed.rot >380) viewed.rot-=360;
		break;
	case VK_RIGHT:
		viewed.rot-=2;
		if (viewed.rot <-20) viewed.rot+=360;
		break;
	default:
		return 0;
	}
	return 1;
}

GLvoid bot_right_arm (GLvoid)
{
	GLint angle = 60;
	glTranslatef(0, -1.f, 0.f);

	/// bot w bezruchu ////////////////////////////////////////////////////////////	
	
	if (pcs.progress >= scenario.rozruch_kury && pcs.progress < scenario.wyciag_kromki_na_patelnie && pcs.progress < scenario.koniec) 
	{
		glRotatef(-45.f, 1, 0, 0);
		glRotatef(-45.f, 0, 1, 0);
	}
	
	/// wyciaganie kromki ////////////////////////////////////////////////////////
	
	else if (pcs.progress >= scenario.wyciag_kromki_na_patelnie && pcs.progress < scenario.smazenie)
	{
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}
			
	/// smazenie /////////////////////////////////////////////////////////////////////////////
		
	else if (pcs.progress >= scenario.smazenie && pcs.progress < scenario.podrzut)
	{
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}

	/// podrzut ////////////////////////////////////////////////////////////////////////////
	
	else if (pcs.progress >= scenario.podrzut && pcs.progress < scenario.podanie_podanie_na_talerzu)
	{
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}
			
	/// podanie na talerzu /////////////////////////////////////////////////////////////////

	else if (pcs.progress >= scenario.podanie_podanie_na_talerzu && pcs.progress < scenario.koniec)
	{		
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}						
}
GLvoid bot_left_arm (GLvoid)
{
	glTranslatef(0, -1.f, 0.f);
	GLint angle = 60;

	/// bot w bezruchu ////////////////////////////////////////////////////////////	

	if (pcs.progress >= scenario.rozruch_kury && pcs.progress < scenario.wyciag_kromki_na_patelnie && pcs.progress < scenario.koniec) 
	{
		glRotatef(-45.f, 1, 0, 0);
		glRotatef(-45.f, 0, 1, 0);
	}
	
	/// wyciaganie kromki ////////////////////////////////////////////////////////
	
	else if (pcs.progress >= scenario.wyciag_kromki_na_patelnie && pcs.progress < scenario.smazenie)
	{
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}
			
	/// smazenie /////////////////////////////////////////////////////////////////////////////
		
	else if (pcs.progress >= scenario.smazenie && pcs.progress < scenario.podrzut)
	{
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}

	/// podrzut ////////////////////////////////////////////////////////////////////////////
	
	else if (pcs.progress >= scenario.podrzut && pcs.progress < scenario.podanie_podanie_na_talerzu)
	{
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}
			
	/// podanie na talerzu /////////////////////////////////////////////////////////////////

	else if (pcs.progress >= scenario.podanie_podanie_na_talerzu && pcs.progress < scenario.koniec)
	{		
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}						
}

GLvoid chicken_move (GLvoid)
{
	/// ruch kury wokol osi robota ///////////////////////////////////////////////////////
	
	if (pcs.progress >= scenario.rozruch_kury && pcs.progress < scenario.koniec)
	{		
		glTranslatef(0, 3.f, 9.f);
		glRotatef((float) (pcs.progress % 360), 0, -1, 0);
	}
}

GLvoid shake_head (GLvoid)
{
	
	glTranslatef(0, .5f, 0.f);
	glScalef(1.4f, .7f, .8f);
	
	/// przyszla pora na mieszanie ///////////////////////////////////////////////////////
	
	if (pcs.progress >= scenario.mieszanie && pcs.progress < scenario.pieczenie)
	{		
		glRotatef((float)(pcs.progress % 360), 0, -1, 0);
	}
}

GLvoid bot_left_forhand (GLvoid)
{
	GLint angle = 60;
	glRotatef(45.f, 1, 0, 0);
	glRotatef(45.f, 0, 0, 1);
	
		/// bot w bezruchu ////////////////////////////////////////////////////////////
	
	if (pcs.progress >= scenario.rozruch_kury && pcs.progress < scenario.wyciag_kromki_na_patelnie && pcs.progress < scenario.koniec) 
	{
		glRotatef(-45.f, 1, 0, 0);
		glRotatef(-45.f, 0, 1, 0);
	}
	
	/// wyciaganie kromki ////////////////////////////////////////////////////////
	
	else if (pcs.progress >= scenario.wyciag_kromki_na_patelnie && pcs.progress < scenario.smazenie)
	{
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}
			
	/// smazenie /////////////////////////////////////////////////////////////////////////////
		
	else if (pcs.progress >= scenario.smazenie && pcs.progress < scenario.podrzut)
	{
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}

	/// podrzut ////////////////////////////////////////////////////////////////////////////
	
	else if (pcs.progress >= scenario.podrzut && pcs.progress < scenario.podanie_podanie_na_talerzu)
	{
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}
			
	/// podanie na talerzu /////////////////////////////////////////////////////////////////

	else if (pcs.progress >= scenario.podanie_podanie_na_talerzu && pcs.progress < scenario.koniec)
	{		
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}			
}

GLvoid bot_right_forhand (GLvoid)
{
	GLint angle = 60;
	glRotatef(45.f, 1, 0, 0);
	glRotatef(45.f, 0, 0, 1);
	
		/// bot w bezruchu ////////////////////////////////////////////////////////////
	
	if (pcs.progress >= scenario.rozruch_kury && pcs.progress < scenario.wyciag_kromki_na_patelnie && pcs.progress < scenario.koniec) 
	{
		glRotatef(-45.f, 1, 0, 0);
		glRotatef(-45.f, 0, 1, 0);
	}
	
	/// wyciaganie kromki ////////////////////////////////////////////////////////
	
	else if (pcs.progress >= scenario.wyciag_kromki_na_patelnie && pcs.progress < scenario.smazenie)
	{
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}
			
	/// smazenie /////////////////////////////////////////////////////////////////////////////
		
	else if (pcs.progress >= scenario.smazenie && pcs.progress < scenario.podrzut)
	{
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}

	/// podrzut ////////////////////////////////////////////////////////////////////////////
	
	else if (pcs.progress >= scenario.podrzut && pcs.progress < scenario.podanie_podanie_na_talerzu)
	{
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}
			
	/// podanie na talerzu /////////////////////////////////////////////////////////////////

	else if (pcs.progress >= scenario.podanie_podanie_na_talerzu && pcs.progress < scenario.koniec)
	{		
		if (pcs.progress%angle < angle/2) 
		{
			glRotatef(pcs.progress%angle, 2, 3, 4);
		} 
		else
		{
			glRotatef(angle-pcs.progress%angle, 2, 3, 4);
		}
	}			
}


GLvoid up_disc (GLvoid)
{
	glTranslatef(0, 2.5f, 0.f);
	glScalef(2.3f, .2f, 2.3f);
	
	/// turn disc ////////////////////////////////////////////////////////////////////////////

	if (pcs.progress >= scenario.rozruch_kury && pcs.progress < scenario.koniec)
	{		
		glRotatef((float)(pcs.progress % 360), 0, 1, 0);
	}

	/// move disc down when grean grow up

	if (pcs.progress >= scenario.uprawa && pcs.progress < scenario.pieczenie)
	{		
		glRotatef((float)(pcs.progress % 360), 0, 1, 0);
	}	
}

GLvoid down_disc (GLvoid)
{
	glTranslatef(0, 1.5f, 0.f);
	glScalef(2.3f, .1f, 2.3f);
	
	/// turn disc ////////////////////////////////////////////////////////////////////////////

	if (pcs.progress >= scenario.rozruch_kury && pcs.progress < scenario.koniec)
	{	
		glRotatef((float)(pcs.progress % 360), 0, -1, 0);
	}
}



