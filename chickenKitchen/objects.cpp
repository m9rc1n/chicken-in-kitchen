#include "objects.h"

GLint obj_list[OBJ_LIST]; /* listy wyÂwietlania element¥w */
GLint tex_list[TEX_LIST]; /* listy wyÂwietlania textur */
GLUquadricObj *qua;

GLfloat
	white_dif_col[4]={.2f, .2f, .2f, 1},
	white_amb_col[4]={-.2f, -.2f, -.2f, 1};

void field (GLvoid) {
	GLint
		s_vec[4]={200, 0, 0, 0},
		t_vec[4]={0, 0, 200, 0};

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeniv(GL_S, GL_OBJECT_PLANE, s_vec);
	glTexGeniv(GL_T, GL_OBJECT_PLANE, t_vec);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white_amb_col);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white_dif_col);
	glEnable(GL_TEXTURE_2D);
	glCallList(FIELD_TEX);
	glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);
		glVertex3f(1, 0, 1);
		glVertex3f(1, 0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

// table ///////////////////////////////////////////////////////////////////////////////////

void table(int is_shadow) 
{
	GLint
		s_vec[4]={1, 1, 1, 1},
		t_vec[4]={1, 0, 1, 0};

	//if (!is_shadow) 
	//{
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);

		glTexGeniv(GL_S, GL_OBJECT_PLANE, s_vec);
		glTexGeniv(GL_T, GL_OBJECT_PLANE, t_vec);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white_amb_col);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white_dif_col);
		glEnable(GL_TEXTURE_2D);
		glCallList(WOOD_TEX);
//	}

	glBegin(GL_QUAD_STRIP);
		glNormal3f(-1, 0, 0);
		glVertex3f(-1, -1, -1);
		glVertex3f(-1, -1, 1);
		glVertex3f(-1, 1, -1);
		glVertex3f(-1, 1, 1);
		glNormal3f(0, 1, 0);
		glVertex3f(1, 1, -1);
		glVertex3f(1, 1, 1);
		glNormal3f(1, 0, 0);
		glVertex3f(1, -1, -1);
		glVertex3f(1, -1, 1);
		glNormal3f(0, -1, 0);
		glVertex3f(-1, -1, -1);
		glVertex3f(-1, -1, 1);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(0, 0, -1);
		glVertex3f(-1, -1, -1);
		glVertex3f(-1, 1, -1);
		glVertex3f(1, 1, -1);
		glVertex3f(1, -1, -1);
	glEnd();

	glBegin(GL_QUADS);		
		glNormal3f(0, 0, 1);
		glVertex3f(1, -1, 1);
		glVertex3f(1, 1, 1);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, -1, 1);
	glEnd();

	if (!is_shadow) 
	{
		glDisable(GL_TEXTURE_2D);

		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
}

// table leg //////////////////////////////////////////////////////////////////////////////

void table_leg() 
{
	GLUquadricObj *podstawa;
	podstawa = gluNewQuadric();
	gluCylinder(podstawa, 1, 1, 1, 40, 40);
	gluDeleteQuadric(podstawa);
}

void bot_body() 
{
		GLint
		s_vec[4]={1, 1, 1, 1},
		t_vec[4]={1, 0, 1, 0};
	//if (!is_shadow) 
	//{
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);

		glTexGeniv(GL_S, GL_OBJECT_PLANE, s_vec);
		glTexGeniv(GL_T, GL_OBJECT_PLANE, t_vec);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white_amb_col);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white_dif_col);
		glEnable(GL_TEXTURE_2D);
		glCallList(CHROME_TEX);
//	}

	GLUquadricObj *podstawa;
	podstawa = gluNewQuadric();
	gluCylinder(podstawa, 0.3, 0.3, 2, 40, 40);
	gluDeleteQuadric(podstawa);
}

void chicken_ball() 
{
	GLUquadricObj *podstawa;
	podstawa = gluNewQuadric();
	gluSphere(podstawa, 1.0 , 40, 40);
	gluDeleteQuadric(podstawa);
}

void bot_ball() 
{
			GLint
		s_vec[4]={1, 1, 1, 1},
		t_vec[4]={1, 0, 1, 0};
	//if (!is_shadow) 
	//{
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);

		glTexGeniv(GL_S, GL_OBJECT_PLANE, s_vec);
		glTexGeniv(GL_T, GL_OBJECT_PLANE, t_vec);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white_amb_col);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white_dif_col);
		glEnable(GL_TEXTURE_2D);
		glCallList(PLATE_TEX);
//	}

	GLUquadricObj *podstawa;
	podstawa = gluNewQuadric();
	gluSphere(podstawa, 1.0 , 40, 40);
	gluDeleteQuadric(podstawa);
}

void bot_hat() 
{
	GLUquadricObj *podstawa;
	podstawa = gluNewQuadric();
	gluDisk(podstawa, 5.0 , 5.0, 40, 40);
	gluDeleteQuadric(podstawa);
}

void pan() 
{
	GLUquadricObj *podstawa;
	podstawa = gluNewQuadric();
	gluCylinder(podstawa, 1.0 , 2.0, 1.0, 40, 40);
	gluDeleteQuadric(podstawa);
}

void owen() 
{
		glBegin(GL_QUAD_STRIP);
		glNormal3f(-1, 0, 0);
		glVertex3f(-1, -1, -1);
		glVertex3f(-1, -1, 1);
		glVertex3f(-1, 1, -1);
		glVertex3f(-1, 1, 1);
		glNormal3f(0, 1, 0);
		glVertex3f(1, 1, -1);
		glVertex3f(1, 1, 1);
		glNormal3f(1, 0, 0);
		glVertex3f(1, -1, -1);
		glVertex3f(1, -1, 1);
		glNormal3f(0, -1, 0);
		glVertex3f(-1, -1, -1);
		glVertex3f(-1, -1, 1);
	glEnd();
	glBegin(GL_QUADS);
		glNormal3f(0, 0, -1);
		glVertex3f(-1, -1, -1);
		glVertex3f(-1, 1, -1);
		glVertex3f(1, 1, -1);
		glVertex3f(1, -1, -1);
	glEnd();
	glBegin(GL_QUADS);		
		glNormal3f(0, 0, 1);
		glVertex3f(1, -1, 1);
		glVertex3f(1, 1, 1);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, -1, 1);
	glEnd();
}

void room()	
{
	glBegin(GL_QUADS);									// Begin Drawing Quads
		// Floor
		glNormal3f(0.0f, 1.0f, 0.0f);					// Normal Pointing Up
		glVertex3f(-10.0f,-10.0f,-10.0f);				// Back Left
		glVertex3f(-10.0f,-10.0f, 10.0f);				// Front Left
		glVertex3f( 10.0f,-10.0f, 10.0f);				// Front Right
		glVertex3f( 10.0f,-10.0f,-10.0f);				// Back Right
		// Ceiling
		glNormal3f(0.0f,-1.0f, 0.0f);					// Normal Point Down
		glVertex3f(-10.0f, 10.0f, 10.0f);				// Front Left
		glVertex3f(-10.0f, 10.0f,-10.0f);				// Back Left
		glVertex3f( 10.0f, 10.0f,-10.0f);				// Back Right
		glVertex3f( 10.0f, 10.0f, 10.0f);				// Front Right
		// Front Wall
		glNormal3f(0.0f, 0.0f, 1.0f);					// Normal Pointing Away From Viewer
		glVertex3f(-10.0f, 10.0f,-10.0f);				// Top Left
		glVertex3f(-10.0f,-10.0f,-10.0f);				// Bottom Left
		glVertex3f( 10.0f,-10.0f,-10.0f);				// Bottom Right
		glVertex3f( 10.0f, 10.0f,-10.0f);				// Top Right
		// Back Wall
		glNormal3f(0.0f, 0.0f,-1.0f);					// Normal Pointing Towards Viewer
		glVertex3f( 10.0f, 10.0f, 10.0f);				// Top Right
		glVertex3f( 10.0f,-10.0f, 10.0f);				// Bottom Right
		glVertex3f(-10.0f,-10.0f, 10.0f);				// Bottom Left
		glVertex3f(-10.0f, 10.0f, 10.0f);				// Top Left
		// Left Wall
		glNormal3f(1.0f, 0.0f, 0.0f);					// Normal Pointing Right
		glVertex3f(-10.0f, 10.0f, 10.0f);				// Top Front
		glVertex3f(-10.0f,-10.0f, 10.0f);				// Bottom Front
		glVertex3f(-10.0f,-10.0f,-10.0f);				// Bottom Back
		glVertex3f(-10.0f, 10.0f,-10.0f);				// Top Back
		// Right Wall
		glNormal3f(-1.0f, 0.0f, 0.0f);					// Normal Pointing Left
		glVertex3f( 10.0f, 10.0f,-10.0f);				// Top Back
		glVertex3f( 10.0f,-10.0f,-10.0f);				// Bottom Back
		glVertex3f( 10.0f,-10.0f, 10.0f);				// Bottom Front
		glVertex3f( 10.0f, 10.0f, 10.0f);				// Top Front
	glEnd();											// Done Drawing Quads
}