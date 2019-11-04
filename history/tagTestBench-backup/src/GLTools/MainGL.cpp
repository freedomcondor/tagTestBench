/*---------------------------------------------------------*/
/*
   	Weixu ZHU (Harry)
   		zhuweixu_harry@126.com
	
	Version 1.0
	Version 1.1 : clean the code a little bit
	Version 1.2 : find a little bug when key 'n' is pressed
	Version 1.3 : improve light and material, 
	              improve view port 2 initialize data
	Version 1.4 : comments about mouse
*/
/*---------------------------------------------------------*/

//#define windows 
//#define ubuntu
//#define mac

/*----------------- OpenGL Include -------------------------*/
#ifdef mac		// get from CMake
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#ifdef windows
#include<windows.h>
#endif

/*----------------- LoopFunction Include -------------------------*/
#include"GLTools.h"

#define pi 3.1415926

/*----------------- OpenGL Functions -------------------------*/
void myDisplay(void);		
void reshape(int w, int h);
void myIdle(void);
void myStep(int id);

void SpecialKeys(int key, int x, int y);
void SpecialUpKeys(int key, int x, int y);
void BoardKeys(unsigned char key, int x, int y);
void BoardUpKeys(unsigned char key, int x, int y);

void SpecialKeysOperate();
void BoardKeysOperate();

void MouseMotion(int x, int y);
void MousePassiveMotion(int x, int y);
void Mouse(int button, int state, int x, int y);
void MouseOperate();

/*----------------- OpenGL Datas -------------------------*/
int KeyStates[256];				// key board states
int KeySpecialStates[256];
int CountIdle = 0;				// idle count for step control 
int Systemwidth, SystemHeight;	// the size of screen
int SystemwidthMiddle, SystemHeightMiddle;
int WindowHeight, Windowwidth;	// the size of window
int WindowHeightMiddle, WindowwidthMiddle;	// the size of window
int WindowX, WindowY;	// the size of window

	/*----------------- Navigations -------------------------*/
//float EyeWInit = 270, 	EyeThInit = -90, 	EyeLInit = 0.001;
float EyeWInit = 270, 	EyeThInit = 0, 	EyeLInit = 0.001;
float EyeXInit = 0, 	EyeYInit = 0, 		EyeZInit = 0;
float EyeW = EyeWInit,	EyeTh = EyeThInit,	EyeL = EyeLInit;				// all unit mm and degree
float EyeX = EyeXInit,	EyeY = EyeYInit,	EyeZ = EyeZInit;					// all unit mm

float EyeW2Init = 270,	EyeTh2Init = 80,	EyeL2Init = 2.5;			// all unit mm and degree
float EyeX2Init = 0.1,	EyeY2Init = 0,		EyeZ2Init = 0;						// all unit mm
float EyeW2 = EyeW2Init,EyeTh2 = EyeTh2Init,EyeL2 = EyeL2Init;			// all unit mm and degree
float EyeX2 = EyeX2Init,EyeY2 = EyeY2Init,	EyeZ2 = EyeZ2Init;						// all unit mm

float RotateStep = 0.500f, ScaleStep = 0.05, MoveStep = 0.03;
//float RotateStep = 0.0300f, ScaleStep = 0.003, MoveStep = 0.001;

	/*----------------- TimeStep -------------------------*/
#define TIMESTEP
int FrameTime = 30;	// draw frequency in ms
int StepTime = 10;		// step frequency in ms

	/*----------------- Pause and Vision -------------------------*/
//int PAUSE = 1;
int PAUSE = 0;

int Vision_type = 0;			// 1 first sight, 0 third sight
int Vision_Control = 1;			// control port 1 or 2

	/*----------------- Mouse -------------------------*/
int MouseNavigation = 0;
int MouseWindowPosX, MouseWindowPosY;

/*	TODO:
int rightmousedown = 0,mousestartx,mousestarty;
GLfloat locx_mouse;
GLfloat locy_mouse;
GLfloat totallength;

float w_saw,h_saw;
float mouselocx_saw = 1,mouselocy_saw = 1;

int Moving_mouse = 0;
*/

/*-----------------------------------------------------------------*/
/*  main start  */
/*-----------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	//-------------------  OpenGL -------------------------//
	printf("----------- openGL -----------------\n");
	glutInit(&argc, argv);      
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);     
	Systemwidth = glutGet(GLUT_SCREEN_WIDTH);		// get screen size
	SystemHeight = glutGet(GLUT_SCREEN_HEIGHT);
	printf("System Screen size : %d %d\n",Systemwidth,SystemHeight);
	WindowHeight = SystemHeight / 2; 
	Windowwidth = Systemwidth / 2;
	SystemwidthMiddle = Windowwidth / 2;
	SystemHeightMiddle = WindowHeight / 2;

	WindowX = Systemwidth / 2, WindowY = SystemHeight / 2;	// the position of window
	glutInitWindowPosition(WindowX, WindowY);     
	glutInitWindowSize(Windowwidth, WindowHeight);      
	glutCreateWindow("Simulator");     

	glutDisplayFunc(myDisplay);     

#ifdef TIMESTEP
	glutTimerFunc(FrameTime,myStep,0);
	glutTimerFunc(StepTime,myStep,1);
#else
	glutIdleFunc(myIdle);
#endif

	glutReshapeFunc(reshape);     
	glutSpecialFunc(SpecialKeys);
	glutSpecialUpFunc(SpecialUpKeys);
	glutKeyboardFunc(BoardKeys);
	glutKeyboardUpFunc(BoardUpKeys);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);

	//------------------ function init --------------------//
	if (function_init(Systemwidth, SystemHeight) != 0)
		return -1;

	//------------------ glut Main Loop -------------------//
	printf("----------- opengl begins ----------\n");
	glutMainLoop();     
	return 0;
}

//------------------------ OpenGL Functions -------------------------//
void myDisplay(void)
{
	GLfloat half;
	
	//------------------------------- view port 1  -----------------------------------//
	//-------- Backgroud -------//
	//glClearColor(0.0f,0.0f,0.0f,1.0f);     	//black
	glClearColor(1.0f,1.0f,1.0f,1.0f);     		//white
	//-------- Depth -----------//
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);     
	glEnable(GL_DEPTH_TEST);

	//--- Light and Material ---//
	const GLfloat light_position[] = { -0.0, -0.0, -100.0, 0.0 };
	const GLfloat light_ambient [] = { 0.1, 0.1, 0.1, 1.0 };
	const GLfloat light_diffuse [] = { 0.1, 0.1, 0.1, 1.0 };
	const GLfloat light_specular[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT , light_ambient );
	glLightfv(GL_LIGHT0, GL_DIFFUSE , light_diffuse );
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	const GLfloat BODY_COLOR[]           = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat SPECULAR[]             = { 0.1f, 0.1f, 0.1f, 1.0f };
	const GLfloat SHININESS[]            = { 0.1f                   };
	const GLfloat EMISSION[]             = { 0.1f, 0.1f, 0.1f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SPECULAR);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, SHININESS);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, EMISSION);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	//------  start to draw ----//
	glViewport(0,0,(float)Windowwidth,(float)WindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,(float)Windowwidth/(float)WindowHeight,0.1f,10000.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();     

	GLfloat eyex,eyey,eyez,eyew,eyeth;

	eyew = EyeW * pi / 180;
	eyeth = EyeTh * pi / 180;
	if (Vision_type == 0)	// 1st/3rd vision
	{
		eyex = EyeL * cos(eyeth) * cos(eyew) + EyeX;
		eyey = EyeL * cos(eyeth) * sin(eyew) + EyeY;
		eyez = EyeL * sin(eyeth) + EyeZ;
		gluLookAt(eyex,eyey,eyez, EyeX,EyeY,EyeZ, 0.0,0.0,1);
	}
	else
	{
		eyex = -EyeL * cos(eyeth) * cos(eyew) + EyeX;
		eyey = -EyeL * cos(eyeth) * sin(eyew) + EyeY;
		eyez = -EyeL * sin(eyeth) + EyeZ;
		gluLookAt(	EyeX,EyeY,EyeZ, eyex,eyey,eyez, 0.0,0.0,1);
	}
	//gluLookAt(0,0,1,0.0,0.0,0.0,0.0,1.0,0);

	//---------------- rec frame ------------------//
	//GLfloat half;
	half = 1.0f;

	glColor4f(0.0f,0.0f,0.0f,0.0f);     	//black			
	//glColor4f(1.0f,1.0f,1.0f,0.0f);

	glBegin(GL_LINES);
	glVertex3f(-half,-half,0.0f);
	glVertex3f(-half,half,0.0f);
	
	glVertex3f(-half,half,0.0f);
	glVertex3f(half,half,0.0f);

	glVertex3f(half,half,0.0f);
	glVertex3f(half,-half,0.0f);

	glVertex3f(half,-half,0.0f);
	glVertex3f(-half,-half,0.0f);
	
	glVertex3f(0,0,0.0f);
	glVertex3f(0,0,half);

	glVertex3f(0,0,0.0f);
	glVertex3f(half,0,0);

	glVertex3f(0,0,0.0f);
	glVertex3f(0,half/2,0);
	glEnd();
	
	//----------------- function draw ----------------//
	function_draw();											//   <-- the draw function
	
	//------------------------------- view port 2  -----------------------------------//
	//------  start to draw ----//
	glViewport(0,0,Windowwidth/4,WindowHeight/4);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,(float)Windowwidth/(float)WindowHeight,0.1f,10000.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();     

	eyew = EyeW2 * pi / 180;
	eyeth = EyeTh2 * pi / 180;
	eyex = EyeL2 * cos(eyeth) * cos(eyew) + EyeX2;
	eyey = EyeL2 * cos(eyeth) * sin(eyew) + EyeY2;
	eyez = EyeL2 * sin(eyeth) + EyeZ2;
	gluLookAt(eyex,eyey,eyez, EyeX2,EyeY2,EyeZ2, 0.0,0.0,1);

	//---------------- rec frame ------------------//
	//GLfloat half;
	half = 1.0f;

	glColor4f(0.0f,0.0f,0.0f,0.0f);     	//black			
	//glColor4f(1.0f,1.0f,1.0f,0.0f);

	glBegin(GL_LINES);
	glVertex3f(-half,0,0.0f);
	glVertex3f(half * 2,0,0.0f);
	
	glVertex3f(0,-half,0.0f);
	glVertex3f(0,half,0.0f);

	glVertex3f(0,0,0.0f);
	glVertex3f(0,0,half);
	glEnd();

	//----------------- function draw ----------------//
	function_draw2();											//   <-- the draw function

	//------------ finish and swap buffer ------------//
	glFlush();
	glutSwapBuffers();
}

#ifdef TIMESTEP
void myStep(int id)
{
	if (id == 0) // frame
	{
		SpecialKeysOperate();
		BoardKeysOperate();
		MouseOperate();
		myDisplay();
		glutTimerFunc(FrameTime,myStep,id);
	}
	else if (id == 1)
	{
		if (PAUSE == 0)
			function_step(StepTime*1.0/1000 * 2);	// StepTime in ms
		else
		{
			int i = 0;	// for debug
		}
		glutTimerFunc(StepTime,myStep,id);
	}
}
#else
void myIdle(void)
{
	SpecialKeysOperate();
	BoardKeysOperate();
	MouseOperate();

	if (PAUSE == 0)
		function_step(StepTime*1.0/1000);	//StepTime in ms
	else
	{
//		int i = 0;	// for debug
//		PAUSE = 0;
	}
	myDisplay();
}
#endif

void reshape(int w, int h)
{
	WindowHeight = h; Windowwidth = w;
}

//----------------------- Key Board ---------------------- //
void SpecialKeys(int key, int x, int y)
{
	KeySpecialStates[key] = 1;
}

void SpecialUpKeys(int key, int x, int y)
{
	KeySpecialStates[key] = 0;
}

void SpecialKeysOperate()
{
	float step = RotateStep, Lstep = ScaleStep;
	if (KeySpecialStates[GLUT_KEY_UP] == 1)
	{
	    if (Vision_Control == 1)
		{
			if (Vision_type == 0)
			{
				if (EyeTh < 90)
					EyeTh += step;
			}
			else if (Vision_type == 1)
			{
				if (EyeTh > -90)
					EyeTh -= step;
			}
		}
		if (Vision_Control == 2)
			if (EyeTh2 < 90)
				EyeTh2 += step;
	}
	if (KeySpecialStates[GLUT_KEY_DOWN] == 1)
	{
	    if (Vision_Control == 1)
		{
			if (Vision_type == 0)
			{
				EyeTh -= step;
				if (EyeTh < -89.9) EyeTh = -89.9;
			}
			else if (Vision_type == 1)
			{
				EyeTh += step;
				if (EyeTh > 89.9) EyeTh = 89.9;
			}
		}
	    if (Vision_Control == 2)
			if (EyeTh2 > -90)
				EyeTh2 -= step;
	}
	if (KeySpecialStates[GLUT_KEY_LEFT] == 1)
	{
	    if (Vision_Control == 1)
		{
			if (Vision_type == 0)
			{
				EyeW -= step;
			}
			else if (Vision_type == 1)
			{
				EyeW += step;
			}
		}
	    if (Vision_Control == 2)
			EyeW2 -= step;
	}
	if (KeySpecialStates[GLUT_KEY_RIGHT] == 1)
	{
	    if (Vision_Control == 1)
		{
			if (Vision_type == 0)
			{
				EyeW += step;
			}
			else if (Vision_type == 1)
			{
				EyeW -= step;
			}
		}
	    if (Vision_Control == 2)
			EyeW2 += step;
	}
	if (KeySpecialStates[GLUT_KEY_PAGE_UP] == 1)
	{
	    if (Vision_Control == 1)
			EyeL -= Lstep;
	    if (Vision_Control == 2)
			EyeL2 -= Lstep;
	}
	if (KeySpecialStates[GLUT_KEY_PAGE_DOWN] == 1)
	{
	    if (Vision_Control == 1)
			EyeL += Lstep;
	    if (Vision_Control == 2)
			EyeL2 += Lstep;
	}
	/*
	if (key == GLUT_KEY_ENTER)
	{
		function_step();
	}
	*/
}

void BoardKeys(unsigned char key, int x, int y)
{
	KeyStates[key] = 1;
}

void BoardUpKeys(unsigned char key, int x, int y)
{
	KeyStates[key] = 0;
}

void BoardKeysOperate()
{
	float mstep = MoveStep;
	//--------------------- Vision contorl -------------------//
	//if (key == 'q')				// switch port
	if (KeyStates['q'] == 1)				// switch port
	{
		Vision_Control = 3 - Vision_Control;
		KeyStates['q'] = 0;
	}
	//if (key == 'v')
	if (KeyStates['v'] == 1)				// switch vision
	{
		Vision_type = 1 - Vision_type;
		KeyStates['v'] = 0;
	}

	//-------------------- navigation -----------------------//
	//if (key == 'w')
	if (KeyStates['w'] == 1)		
	{
		EyeX -= mstep * cos(EyeW*pi/180);
		EyeY -= mstep * sin(EyeW*pi/180);
	}
	//if (key == 's')
	if (KeyStates['s'] == 1)		
	{
		EyeX += mstep * cos(EyeW*pi/180);
		EyeY += mstep * sin(EyeW*pi/180);
	}
	//if (key == 'a')
	if (KeyStates['a'] == 1)		
	{
		EyeX -= mstep * cos(EyeW*pi/180+pi/2);
		EyeY -= mstep * sin(EyeW*pi/180+pi/2);
	}
	//if (key == 'd')
	if (KeyStates['d'] == 1)		
	{
		EyeX += mstep * cos(EyeW*pi/180+pi/2);
		EyeY += mstep * sin(EyeW*pi/180+pi/2);
	}
	//if (key == ' ')
	if (KeyStates[' '] == 1)		
	{
		EyeZ += mstep;
	}
	//if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
	//if (key == 'c')
	if (KeyStates['c'] == 1)		
	{
		EyeZ -= mstep;
	}
	//if (key == 'r')
	if (KeyStates['r'] == 1)		
	{
		EyeW = EyeWInit,	EyeTh = EyeThInit,	EyeL = EyeLInit;
		EyeX = EyeXInit,	EyeY = EyeYInit,	EyeZ = EyeZInit;

		KeyStates['r'] = 0;
	}

	//-------------------- control -------------------//
	if (KeyStates['x'] == 1)		
	{
	}
	if (KeyStates['m'] == 1)		
	{
		MouseNavigation = 1 - MouseNavigation;
		if (MouseNavigation == 1)
		{
			//WindowX = glutGet(GLUT_WINDOW_X);		// get window pos
			//WindowY = glutGet(GLUT_WINDOW_Y);
			//SystemwidthMiddle = WindowX + Windowwidth / 2;	// set window middle
			//SystemHeightMiddle = WindowY + WindowHeight / 2;
			//SetCursorPos(SystemwidthMiddle, SystemHeightMiddle); // windows only
			
			WindowwidthMiddle = Windowwidth / 2;
			WindowHeightMiddle = WindowHeight / 2;
			glutWarpPointer(WindowwidthMiddle , WindowHeightMiddle );	
				// doesn't work in bash on windows
			MouseWindowPosX = WindowwidthMiddle;
			MouseWindowPosY = WindowHeightMiddle;

			glutSetCursor(GLUT_CURSOR_NONE);
		}
		else
		{
			glutSetCursor(!GLUT_CURSOR_NONE);
		}

		KeyStates['m'] = 0;
	}
	if (KeyStates['n'] == 1)		
	{
		function_step(StepTime*1.0/1000);
		KeyStates['n'] = 0;
	}
	if (KeyStates['p'] == 1)		
	{
		PAUSE = 1 - PAUSE;
		KeyStates['p'] = 0;
	}
	//if (key == 27)			// escape key
	if (KeyStates[27] == 1)		
	{
		function_exit();
		exit(0);					// windows or linux or mac
	}

	//--------------  control channel ---------------//
	if (KeyStates['j'] == 1)		
	{
		if (CH1 + CH1_STEP < CH1_MAX)
			CH1 += CH1_STEP;
	}
	if (KeyStates['l'] == 1)		
	{
		if (CH1 - CH1_STEP > CH1_MIN)
			CH1 -= CH1_STEP;
	}
	if (KeyStates['i'] == 1)		
	{
		if (CH2 + CH2_STEP < CH2_MAX)
			CH2 += CH2_STEP;
	}
	if (KeyStates['k'] == 1)		
	{
		if (CH2 - CH2_STEP > CH2_MIN)
			CH2 -= CH2_STEP;
	}

	if (KeyStates['o'] == 1)		
	{
		CMDCH1 = 1 - CMDCH1;
		KeyStates['o'] = 0;
	}
}

//--------------------------- mouse --------------------------------//
// TODO: 
void MouseOperate()
{
	float step = RotateStep, Lstep = ScaleStep;
	int x,y;
	if (MouseNavigation == 1)		// mouse navigation
	{
		/*
		POINT p;					// windows only		get cursor pos
		GetCursorPos(&p);			// windows only
		x = (int)p.x;				// windows only
		y = (int)p.y;				// windows only
		*/
		x = MouseWindowPosX;
		y = MouseWindowPosY;

		//printf("%d %d\n",p.x,p.y);

		x -= WindowwidthMiddle;
		y -= WindowHeightMiddle;

		if (Vision_type == 0)
		{
			EyeW += step * x;
			EyeTh -= step * y;
			if (EyeTh > 89.9) EyeTh = 89.9;
			if (EyeTh < -89.9) EyeTh = -89.9;
		}
		else if (Vision_type == 1)
		{
			EyeW -= step * x;
			EyeTh += step * y;
			if (EyeTh > 89.9) EyeTh = 89.9;
			if (EyeTh < -89.9) EyeTh = -89.9;
		}	

		//SetCursorPos(SystemwidthMiddle, SystemHeightMiddle); // windows only
		glutWarpPointer(WindowwidthMiddle , WindowHeightMiddle );
	}
}

void Mouse(int button, int state, int x, int y)
{
	//happens when a key is hit or release
	//printf("Mouse %d %d\n",x,y);

	/*			right mouse
	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{
		rightmousedown = 1;
		mousestartx = x;
		mousestarty = y;
	}
	else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_UP))
		rightmousedown = 0;
	*/
}

void MouseMotion(int x, int y)
{
	//happens when a key is hold and move
	//printf("Mouse Motion %d %d\n",x,y);
	
	MouseWindowPosX = x;
	MouseWindowPosY = y;

	/*			right mouse navigation
	int hori,vert;
	if (rightmousedown == 1)
	{
		hori = x - mousestartx;
		vert = y - mousestarty;
		if (Vision_Control == 1)
		{
			EyeY += hori * 0.5 /Windowwidth;
			EyeX += vert * 0.5 /WindowHeight;
		}
		if (Vision_Control == 2)
		{
			EyeX2 -= hori * 0.5 /Windowwidth;
			EyeY2 += vert * 0.5 /WindowHeight;
		}
	}
	mousestartx = x;
	mousestarty = y;
	*/
}

void MousePassiveMotion(int x, int y)
{
	//happens when a key is moving without key holding
	//printf("Mouse Passive Motion %d %d\n",x,y);
	MouseWindowPosX = x;
	MouseWindowPosY = y;
}
