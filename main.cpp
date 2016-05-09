#   pragma comment (lib, "opengl32.lib")
#   pragma comment (lib, "glu32.lib")     
#   pragma comment (lib, "glaux.lib")

#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <gl\glaux.h>
#include <gl\glut.h>        
#include <math.h>

//#define WIDTH 4.1
#define WIDTH 3.6
//#define DEPTH 3.7
#define DEPTH 2.90
#define PI 3.141592654
HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


static float black[] = { 0,0,0,1 };
static float window_color[] = { 0.8,1.0,0.97 };
static float wood_color[] = { 0.55,0.35,0.11 };

static float woods[] = { 0.71,0.65,0.26 };
static float white[] = { 1,1,1,1 };
static float next_black[] = { 0.25,0.25,0.25,1 };
static float wall_color[] = { 0.8,0.8,0.8,1 };
static float floors_color[] = { 0.73,0.65,0.26 };
static float rod[] = { 0.8,0,0 };
static float gre[] = { 0.5,0.8,0 };
static float gul_color[] = { 1,1,0,1 };
static float skinning[] = { 100 };
static float Darkblue[] = { 0.016,0.216,0.518 };
unsigned int desk, cleanliness;

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active = TRUE;		// Window Active Flag 
bool	fullscreen = TRUE;	// Fullscreen Flag 

float xview = -1, yview = -1, zview = -2.8, xrotate = 0, yrotate = 30;
float trayz = 0;
float sein2 = 1.0;
GLuint lists;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

														// Calculate The Aspect Ratio Of The Window
	gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

}


void layerwindow(float x, float y, bool across)    //window
{
	glMaterialfv(GL_FRONT, GL_DIFFUSE, window_color);
	glMaterialfv(GL_FRONT, GL_AMBIENT, window_color);


	glBegin(GL_QUADS); //window
	glVertex3f(0.0, 0.0, 0);
	glVertex3f(0.0, 0.0, -x);
	glVertex3f(0.0, y, -x);
	glVertex3f(0.0, y, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, wood_color);
	glMaterialfv(GL_FRONT, GL_AMBIENT, wood_color);
	glBegin(GL_QUADS);			//windowsarm
	glVertex3f(0.0, -0.05, 0.05);
	glVertex3f(0.0, -0.05, -x - 0.05);
	glVertex3f(0.0, -0.01, -x - 0.05);
	glVertex3f(0.0, -0.01, 0.05);

	glVertex3f(0.0, y + 0.01, 0.05);
	glVertex3f(0.0, y + 0.01, -x - 0.05);
	glVertex3f(0.0, y + 0.05, -x - 0.05);
	glVertex3f(0.0, y + 0.05, 0.05);

	glVertex3f(0.0, -0.01, 0 + 0.05);
	glVertex3f(0, -0.01, 0.01);
	glVertex3f(0, y + 0.01, 0.01);
	glVertex3f(0, y + 0.01, +0.05);

	glVertex3f(0.0, -0.01, -x - 0.05);
	glVertex3f(0, -0.01, -x - 0.01);
	glVertex3f(0, y + 0.01, -x - 0.01);
	glVertex3f(0, y + 0.01, -x - 0.05);
	glEnd();



}

void layerstick(float h, float thickness)  //leg bed and table
{
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(thickness, 0, 0);
	glVertex3f(thickness, h, 0);
	glVertex3f(0, h, 0);

	glNormal3f(1, 0, 0);
	glVertex3f(thickness, 0, 0);
	glVertex3f(thickness, 0, -thickness);
	glVertex3f(thickness, h, -thickness);
	glVertex3f(thickness, h, 0);

	glNormal3f(0, 0, -1);
	glVertex3f(thickness, 0, -thickness);
	glVertex3f(0, 0, -thickness);
	glVertex3f(0, h, -thickness);
	glVertex3f(thickness, h, -thickness);

	glNormal3f(-1, 0, 0);
	glVertex3f(0, 0, -thickness);
	glVertex3f(0, 0, 0);
	glVertex3f(0, h, 0);
	glVertex3f(0, h, -thickness);
	glEnd();
	glPopMatrix();
}

void layercube(float x, float y, float z)
{
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glVertex3f(x, 0, 0);
	glVertex3f(x, 0, -z);
	glVertex3f(x, y, -z);
	glVertex3f(x, y, 0);

	glNormal3f(0, 1, 0);
	glVertex3f(x, y, 0);
	glVertex3f(x, y, -z);
	glVertex3f(0, y, -z);
	glVertex3f(0, y, 0);

	glNormal3f(-1, 0, 0);
	glVertex3f(0, y, 0);
	glVertex3f(0, y, -z);
	glVertex3f(0, 0, -z);
	glVertex3f(0, 0, 0);

	glNormal3f(0, -1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -z);
	glVertex3f(x, 0, -z);
	glVertex3f(x, 0, 0);
	glEnd();
	glPopMatrix();
}


void layercube2(float x, float y, float z)  // almirah and table top
{
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glVertex3f(x, 0, 0);
	glVertex3f(x, 0, -z);
	glVertex3f(x, y, -z);
	glVertex3f(x, y, 0);

	glNormal3f(0, 1, 0);
	glVertex3f(x, y, 0);
	glVertex3f(x, y, -z);
	glVertex3f(0, y, -z);
	glVertex3f(0, y, 0);

	glNormal3f(-1, 0, 0);
	glVertex3f(0, y, 0);
	glVertex3f(0, y, -z);
	glVertex3f(0, 0, -z);
	glVertex3f(0, 0, 0);

	glNormal3f(0, -1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -z);
	glVertex3f(x, 0, -z);
	glVertex3f(x, 0, 0);

	glNormal3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(x, 0, 0);
	glVertex3f(x, y, 0);
	glVertex3f(0, y, 0);

	glNormal3f(0, 0, -1);
	glVertex3f(0, 0, -z);
	glVertex3f(x, 0, -z);
	glVertex3f(x, y, -z);
	glVertex3f(0, y, -z);

	glEnd();
	glPopMatrix();
}


void layerbed()                //bed
{
	glPushMatrix();
	glPushMatrix();
	glTranslatef(0.2, 0, -0.1);
	layerstick(0.4, 0.05);
	glTranslatef(0, 0, -0.55);
	layerstick(0.4, 0.05);
	glTranslatef(1.65 - 0.15, 0, 0);
	layerstick(0.4, 0.05);
	glTranslatef(0, 0, 0.55);
	layerstick(0.4, 0.05);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0.4, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, Darkblue);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Darkblue);
	layercube2(1.85, 0.1, 0.75);
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, woods);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, woods);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glVertex3f(1.86, -0.08, -0.76);
	glVertex3f(0, -0.08, -0.76);
	glVertex3f(0, 0.04, -0.76);
	glVertex3f(1.86, 0.04, -0.76);

	glNormal3f(1, 0, 0);
	glVertex3f(1.86, -0.08, 0);
	glVertex3f(1.86, -0.08, -0.76);
	glVertex3f(1.86, 0.04, -0.76);
	glVertex3f(1.86, 0.04, 0);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void layershelf()                                        //display shelf
{
	glMaterialfv(GL_FRONT, GL_DIFFUSE, woods);
	glMaterialfv(GL_FRONT, GL_AMBIENT, woods);
	glPushMatrix();
	layercube2(0.03, 0.8, 0.4);
	glTranslatef(0.98, 0, 0);
	layercube2(0.03, 0.8, 0.4);
	glTranslatef(-0.98, 0, 0);
	glPushMatrix();
	glTranslatef(0, 0.1, -0.015);
	layercube2(0.98, 0.03, 0.4 - 0.015);
	glTranslatef(0, 0.26, 0);
	layercube2(0.98, 0.03, 0.4 - 0.015);
	glTranslatef(0, 0.4, 0);
	layercube2(0.98, 0.03, 0.4 - 0.015);
	glPopMatrix();

	glTranslatef(0, 0.8, -0.16);
	layercube2(0.03, 0.96, 0.24);
	glTranslatef(0.98, 0, 0);
	layercube2(0.03, 0.96, 0.24);
	glTranslatef(-0.98, 0, 0);
	glPushMatrix();
	glTranslatef(0, 0.4, 0);
	layercube2(0.98, 0.03, 0.24);
	glTranslatef(0, 0.26, 0);
	layercube2(0.98, 0.03, 0.24);
	glTranslatef(0, 0.26, 0);
	layercube2(0.98, 0.03, 0.24);
	glPopMatrix();

	glPopMatrix();
}


void displaylists()
{
	lists = glGenLists(2);
	cleanliness = lists;
	desk = lists + 1;
	glNewList(desk, GL_COMPILE);					//desk
	glMaterialfv(GL_FRONT, GL_DIFFUSE, woods);
	glMaterialfv(GL_FRONT, GL_AMBIENT, woods);
	glPushMatrix();
	glTranslatef(0, 0.7, 0);
	glBegin(GL_QUADS);              //table top

	glNormal3f(0, 1, 0);
	glVertex3f(0, 0, 0.66);
	glVertex3f(1.25, 0, 0.66);
	glVertex3f(1.25, 0, 0);
	glVertex3f(0, 0, 0);

	glNormal3f(0, -1, 0);
	glVertex3f(0, -0.04, 0.66);
	glVertex3f(1.25, -0.04, 0.66);
	glVertex3f(1.25, -0.04, 0);
	glVertex3f(0, -0.04, 0);

	glNormal3f(0, 0, 1);
	glVertex3f(0, -0.04, 0.66);
	glVertex3f(1.25, -0.04, 0.66);
	glVertex3f(1.25, 0, 0.66);
	glVertex3f(0, 0, 0.66);

	glNormal3f(1, 0, 0);
	glVertex3f(1.25, -0.04, 0.66);
	glVertex3f(1.25, -0.04, 0.0);
	glVertex3f(1.25, 0, 0.0);
	glVertex3f(1.25, 0, 0.66);
	glEnd();

	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.02, 0, 0.78 - 0.15);
	layerstick(0.7, 0.05);
	glPushMatrix();
	glTranslatef(0, +0.2, 0);
	layercube(0.05, 0.05, 0.7 - 0.15);
	glPopMatrix();
	glTranslatef(1.26 - 0.15, 0, 0);
	layerstick(0.7, 0.05);
	glPushMatrix();
	glTranslatef(0, +0.2, 0);
	layercube(0.05, 0.05, 0.7 - 0.15);
	glTranslatef(-0.4, 0.17, -0.03);
	layercube(0.39, 0.31, 0.65 - 0.09);
	glPushMatrix();		//drawer desk
	glTranslatef(0.01, 0.01, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, woods);
	glMaterialfv(GL_FRONT, GL_AMBIENT, woods);
	glTranslatef(0, 0.15, 0);
	//layercube3(0.37, 0.14, 0.65 - 0.09);
	glBegin(GL_QUADS);          //upper drawer
	glNormal3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0.37, 0, 0);
	glVertex3f(0.37, 0.14, 0);
	glVertex3f(0, 0.14, 0);
	glEnd();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glBegin(GL_QUADS);
	glVertex3f(0.14, 0.07, 0.01);
	glVertex3f(0.22, 0.07, 0.01);
	glVertex3f(0.22, 0.09, 0.01);
	glVertex3f(0.14, 0.09, 0.01);
	glEnd();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, woods);
	glMaterialfv(GL_FRONT, GL_AMBIENT, woods);
	glPopMatrix();
	glPopMatrix();
	glTranslatef(0, 0, -0.68 + 0.15);
	layerstick(0.7, 0.05);
	glTranslatef(-1.26 + 0.15, 0, 0);
	layerstick(0.7, 0.05);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2, 1, 2);

	glPopMatrix();
	glEndList();




	glNewList(cleanliness, GL_COMPILE);
	glPushMatrix();			//room start

	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, wall_color);
	glMaterialfv(GL_FRONT, GL_AMBIENT, wall_color);
	
	glBegin(GL_QUADS);    //walls internal
	glNormal3f(0, 0, 1);
	glVertex3f(0.01, 0, 0.01);
	glVertex3f(WIDTH - 0.01, 0, 0.01);
	glVertex3f(WIDTH - 0.01, 2.44 - 0.01, 0.01);
	glVertex3f(0.01, 2.07 - 0.01, 0.01);

	glNormal3f(0, 0, -1);
	glVertex3f(0.01, 0, DEPTH - 0.01);
	glVertex3f(WIDTH - 0.01, 0, DEPTH - 0.01);
	glVertex3f(WIDTH - 0.01, 2.44, DEPTH - 0.01);
	glVertex3f(0.01, 2.07, DEPTH - 0.01);

	glNormal3f(+1, 0, 0);
	glVertex3f(0.01, 0, 0.01);
	glVertex3f(0.01, 0, DEPTH - 0.01);
	glVertex3f(0.01, 2.07, DEPTH - 0.01);
	glVertex3f(0.01, 2.07, 0.01);

	glNormal3f(-1, 0, 0);
	glVertex3f(WIDTH - 0.01, 0, 0.01);
	glVertex3f(WIDTH - 0.01, 0, DEPTH - 0.01);
	glVertex3f(WIDTH - 0.01, 2.44, DEPTH - 0.01);
	glVertex3f(WIDTH - 0.01, 2.44, 0.01);


	glNormal3f(0, -1, 0);   //roof internal
	glVertex3f(0.01, 2.07 - 0.01, 0.01);
	glVertex3f(WIDTH - 0.01, 2.44 - 0.01, 0.01);
	glVertex3f(WIDTH - 0.01, 2.44 - 0.01, DEPTH - 0.01);
	glVertex3f(0.01, 2.07 - 0.01, DEPTH - 0.01);


	glEnd();
	glDisable(GL_POLYGON_STIPPLE);
	glMaterialfv(GL_FRONT, GL_AMBIENT, floors_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, floors_color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, floors_color);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);   //floor
	glVertex3f(0, 0, 0);
	glVertex3f(WIDTH, 0, 0);
	glVertex3f(WIDTH, 0, DEPTH);
	glVertex3f(0, 0, DEPTH);
	glEnd();
	glPushMatrix();
	glTranslatef(0.02, 0.75, DEPTH - 0.1);  //windower
	glNormal3f(1, 0, 0);
	layerwindow(1.15, 1.12, true);
	glTranslatef(-0.02, -0.75, -DEPTH + 0.15);
	glTranslatef(0.02, 0.75, DEPTH - 0.23 - 1.15);
	glNormal3f(1, 0, 0);
	layerwindow(0.4, 1.12, true);
	glTranslatef(-0.02, -0.75, -(DEPTH - 0.2 - 1.3 - 0.1));
	glPopMatrix();
	glPushMatrix();
	
	glTranslatef(0.02, 0.75 + 1.12 + 0.05, DEPTH - 0.01);
	layercube2(0.02, 0.05, DEPTH - 0.02);
	glTranslatef(0, -1.14 - 0.12, 0);
	layercube2(0.02, 0.05, DEPTH - 0.02);
	glPopMatrix();

	
	glMaterialfv(GL_FRONT, GL_AMBIENT, window_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, window_color);
	glLineWidth(5);
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glVertex3f(WIDTH - 0.03, 1, 0.10);
	glVertex3f(WIDTH - 0.03, 1, 0.21);
	glVertex3f(WIDTH - 0.03, 1.02, 0.21);
	glVertex3f(WIDTH - 0.03, 1.02, 0.10);

	glNormal3f(1, 0, 0);
	glVertex3f(WIDTH + 0.03, 1, 0.10);
	glVertex3f(WIDTH + 0.03, 1, 0.21);
	glVertex3f(WIDTH + 0.03, 1.02, 0.21);
	glVertex3f(WIDTH + 0.03, 1.02, 0.10);
	glEnd();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, wall_color);
	glMaterialfv(GL_FRONT, GL_AMBIENT, wall_color);
	glBegin(GL_QUADS); //shelf over dør/wash
	glNormal3f(0, -1, 0);
	glVertex3f(WIDTH - 0.58, 1.95, 0.02);
	glVertex3f(WIDTH - 0.02, 1.95, 0.02);
	glVertex3f(WIDTH - 0.02, 1.95, DEPTH - 0.02);
	glVertex3f(WIDTH - 0.58, 1.95, DEPTH - 0.02);

	glNormal3f(0, -1, 0);
	glVertex3f(WIDTH - 0.58, 2.04, 0.02);
	glVertex3f(WIDTH - 0.02, 2.04, 0.02);
	glVertex3f(WIDTH - 0.02, 2.04, DEPTH - 0.02);
	glVertex3f(WIDTH - 0.58, 2.04, DEPTH - 0.02);

	glNormal3f(-1, 0, 0);
	glVertex3f(WIDTH - 0.58, 1.95, 0.02);
	glVertex3f(WIDTH - 0.58, 1.95, DEPTH - 0.02);
	glVertex3f(WIDTH - 0.58, 2.04, DEPTH - 0.02);
	glVertex3f(WIDTH - 0.58, 2.04, 0.02);
	glEnd();
	glBegin(GL_QUADS);  //the little wall over the shelf
	glNormal3f(-1, 0, 0);
	glVertex3f(WIDTH - 0.30, 2.04, 0.02);
	glVertex3f(WIDTH - 0.30, 2.04, DEPTH - 0.02);
	glVertex3f(WIDTH - 0.30, 2.40, DEPTH - 0.02);
	glVertex3f(WIDTH - 0.30, 2.40, 0.02);
	glEnd();
	

	glMaterialfv(GL_FRONT, GL_DIFFUSE, window_color);
	glMaterialfv(GL_FRONT, GL_AMBIENT, window_color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);

	glMaterialfv(GL_FRONT, GL_SHININESS, skinning);


	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glMaterialfv(GL_FRONT, GL_SHININESS, black);
	glPushMatrix();
	glTranslatef(0.1, 0, 0.1);
	glCallList(desk);  //writing desk


	glTranslatef(3, 0, 1.37);  //shelf
	glRotatef(270, 0, 1, 0);
	layershelf();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.1, 0, DEPTH - 0.05);
	layerbed();
	glPopMatrix();

	glPopMatrix();		//room end
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, rod);
	glMaterialfv(GL_FRONT, GL_AMBIENT, rod);
	
	glPushMatrix();
	glTranslatef(3.33, 0.92, 2);
	glRotatef(90, 0, 1, 0);
	glutSolidTeapot(0.2);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, gre);
	glMaterialfv(GL_FRONT, GL_AMBIENT, gre);
	glTranslatef(1.8, 0.40, 1.3);
	glRotatef(90, 1, 0, 0);
	glutSolidTorus(0.15, 0.2, 30, 50);
	
	//glutSolidCube(0.2);
	
	glPopMatrix();
	glEndList();
}
int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//  Perspective Calculations
	glEnable(GL_LIGHTING);
	static float ambientlys[] = { 0.4,0.4,0.4,1 };
	static float diffuselys[] = { 0.6,0.6,0.6,1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientlys);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuselys);
	glEnable(GL_LIGHT0);
	displaylists();
	return TRUE;										
}

int DrawGLScene(GLvoid)									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

	glLoadIdentity();	// Reset The Current Modelview Matrix

	glRotatef(yrotate, 0, 1, 0);
	glTranslatef(xview, yview, zview);


	glCallList(cleanliness);

	glPushMatrix();			//tray
	glTranslatef(0.1, 0, 0.1);
	glTranslatef(0.02, 0, 0.78);
	glTranslatef(1.26, 0, 0);
	glTranslatef(-0.4, 0.17, -0.03);
	glTranslatef(0.01, 0.01, 0);
	glTranslatef(-0.15, 0.2, -0.15);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, woods);
	glMaterialfv(GL_FRONT, GL_AMBIENT, woods);
	glTranslatef(0, 0, trayz);
	layercube(0.37, 0.14, 0.65 - 0.09);

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0.37, 0, 0);
	glVertex3f(0.37, 0.14, 0);
	glVertex3f(0, 0.14, 0);
	glEnd();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glBegin(GL_QUADS);
	glVertex3f(0.14, 0.07, 0.02);
	glVertex3f(0.22, 0.07, 0.02);
	glVertex3f(0.22, 0.09, 0.02);
	glVertex3f(0.14, 0.09, 0.02);
	glEnd();
	glPopMatrix();

	glTranslatef(0, 0, -DEPTH - 0.3);
	//glCallList(lists);
	glTranslatef(WIDTH + WIDTH + WIDTH, 0, DEPTH);
	glRotatef(180, 0, 1, 0);
	glCallList(cleanliness);


	return TRUE;										
}



BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			
	WNDCLASS	wc;						
	DWORD		dwExStyle;				
	DWORD		dwStyle;				
	RECT		WindowRect;				
	WindowRect.left = (long)0;			
	WindowRect.right = (long)width;		
	WindowRect.top = (long)0;				
	WindowRect.bottom = (long)height;	

	fullscreen = fullscreenflag;			

	hInstance = GetModuleHandle(NULL);				
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc = (WNDPROC)WndProc;					
	wc.cbClsExtra = 0;									
	wc.cbWndExtra = 0;									
	wc.hInstance = hInstance;							
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground = NULL;									
	wc.lpszMenuName = NULL;									
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												
	{
		DEVMODE dmScreenSettings;								
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

																	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		//KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		//KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		//KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		//KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		//KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		//KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		//KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	if (fullscreen) sein2 = 0.2;
	else sein2 = 1.0;
	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_ACTIVATE:							// Watch For Window Activate Message
	{
		if (!HIWORD(wParam))					// Check Minimization State
		{
			active = TRUE;						// Program Is Active
		}
		else
		{
			active = FALSE;						// Program Is No Longer Active
		}

		return 0;								// Return To The Message Loop
	}

	case WM_SYSCOMMAND:							// Intercept System Commands
	{
		switch (wParam)							// Check System Calls
		{
		case SC_SCREENSAVE:					// Screensaver Trying To Start?
		case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
			return 0;							// Prevent Froom Happening
		}
		break;									// Exit
	}

	case WM_CLOSE:								// Did We Receive A Close Message?
	{
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back
	}

	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		keys[wParam] = TRUE;					// If So, Mark It As TRUE
		return 0;								// Jump Back
	}

	case WM_KEYUP:								// Has A Key Been Released?
	{
		keys[wParam] = FALSE;					// If So, Mark It As FALSE
		return 0;								// Jump Back
	}

	case WM_SIZE:								// Resize The OpenGL Window
	{
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done = FALSE;								// Bool Variable To Exit Loop

														// Ask The User Which Screen Mode They Prefer
														//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen = false;							// Windowed Mode
	}

	// Create my OpenGL Window

	if (!CreateGLWindow("Realistic Rendering of indoor scene", 640, 480, 16, fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while (!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = TRUE;							// If So done=TRUE
				glFinish();
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{

			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done = TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
					Sleep(2);
					float sein = 1.1;

					if (keys[VK_SHIFT]) sein = 0.25;
					if (keys[VK_UP]) {
						zview += 0.03*cos(yrotate*PI / 180)*sein*sein2;
						xview -= 0.03*sin(yrotate*PI / 180)*sein*sein2;

					}
					if (keys[VK_DOWN]) {
						zview -= 0.03*cos(yrotate*PI / 180)*sein*sein2;
						xview += 0.03*sin(yrotate*PI / 180)*sein*sein2;

					}

					if (keys[VK_RIGHT]) {
						yrotate += 0.6*sein;
						if (yrotate > 360) yrotate -= 360;
						if (yrotate < -360) yrotate += 360;
					}
					if (keys[VK_LEFT]) {
						yrotate -= 0.6*sein;
						if (yrotate > 360) yrotate -= 360;
						if (yrotate < -360) yrotate += 360;
					}



				}
			}


		}
	}

										
	return (msg.wParam);							
}
