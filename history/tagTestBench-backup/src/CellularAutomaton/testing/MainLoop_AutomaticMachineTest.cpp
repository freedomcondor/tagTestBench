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

#include "CellularAutomaton.h"

/*------------ Function Control Channels ------------*/
double CH1,CH1_MAX,CH1_MIN,CH1_STEP;
double CH2,CH2_MAX,CH2_MIN,CH2_STEP;
int CMDCH1;

CellularAutomaton ca;

/* --------------- MainLoop functions --------------------*/
int function_exit()
{
	return 0;
}

int function_init()
{
	ca.set(0,0,0,1);
	ca.set(0,1,0,1);
	ca.set(1,0,0,1);

	printf("%lf\n",ca.get(0,0,0));
	printf("%lf\n",ca.get(0,1,0));
	printf("%lf\n",ca.get(0,0,1));
	return 0;
}

int function_step(double time)	// time in s
{
	ca.run(-50,-50,0, 100, 100, 1);
	return 0;
}

/* --------------- OpenGL draw functions --------------------*/
int function_draw()
{
	ca.draw(-50,-50,0, 100, 100, 1);
	return 0;
}

int function_draw2()
{
	return 0;
}

/* --------------- draw other things --------------------*/
#ifdef CELLULARAUTOMATON
int CellularAutomaton::draw(int x, int y, int z, int xl, int yl, int zl)
{
	double length = 0.01;
	for (int i = x; i < x+xl; i++)
		for (int j = y; j < y+yl; j++)
			for (int k = z; k < z+zl; k++)
				if (world->get(i,j,k) > 0.5)
					drawCube(length*0.95, i * length, j*length, k*length,
					                      1,          0,        0,
					                      0,          0,        1);

	return 0;
}
#endif
