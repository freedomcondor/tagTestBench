/*---------------------------------------------------------*/
/*
   	Weixu ZHU (Harry)
   		zhuweixu_harry@126.com
	
	Version 1.0
	Version 1.1 : change opengl debug layout

*/
/*---------------------------------------------------------*/

#include <stdio.h>
#include "GLTools.h"

#include "Vector3.h"
#include "Quaternion.h"

#include "Manipulator.h"

#define PI 3.1415926

/*------------ Function Control Channels ------------*/
double CH1,CH1_MAX,CH1_MIN,CH1_STEP;
double CH2,CH2_MAX,CH2_MIN,CH2_STEP;
int CMDCH1;

/* --------------- MainLoop functions --------------------*/
int function_exit()
{
	return 0;
}

Manipulator a;

int function_init()
{
	a.setPos(0.2);
	return 0;
}

int function_step(double time)	// time in s
{
	a.run(time);
	return 0;
}

/* --------------- OpenGL draw functions --------------------*/
int function_draw()
{
	a.draw();
	return 0;
}

int function_draw2()
{
	return 0;
}

/* --------------- draw obj --------------------*/
#ifdef BOX
void Box::draw()
{
	Vector3 axis = this->q.getAxis();
	double ang = this->q.getAng();

	glTranslatef(this->l.x, this->l.y, this->l.z);
	glRotatef(ang*180/PI,axis.x,axis.y,axis.z);
	if ((this->x != 0) && (this->y != 0) && (this->z != 0))
	{
		glScalef(this->x, this->y, this->z);
		glutSolidCube(1);	
		glScalef(1/this->x, 1/this->y, 1/this->z);
	}
	glRotatef(-ang*180/PI,axis.x,axis.y,axis.z);
	glTranslatef(-this->l.x, -this->l.y, -this->l.z);
}
#endif
