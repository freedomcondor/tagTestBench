/*---------------------------------------------------------*/
/*
   	Weixu ZHU (Harry)
   		zhuweixu_harry@126.com
	
	Version 1.0
	Version 1.1 : 	change OpenGl layout
	Version 1.2 : 	add drawDataLog
	Version 1.3 : 	add drawDataLog with location
	Version 1.4 : 	move Box::draw here
	Version 1.5 : 	add Sphere::draw
	             	move CellularAutomaton:draw here
	Version 1.6 : 	add Cylinder::draw
	Version 1.6.1 : add a todo mark
	Version 1.6.2 : use push/pop matrix in drawBox ..
	Version 1.6.3 : add round covers for Cylinder, 32 slices for Sphere
	Version 1.6.4 : fix a bug of cylinder draw
*/
/*---------------------------------------------------------*/

#include"GLTools.h"

#include <math.h>
#include <string.h>

#define pi 3.1415926
#define ZERO 0.0000001

/* --------------- draw obj --------------------*/
#include "Box.h"	// or delete line Box.cpp (any cpp with #include "Box.h") in CMakeList.txt
void Box::draw()
{
	Vector3 axis = this->q.getAxis();
	double ang = this->q.getAng();

	glPushMatrix();
	glTranslatef(this->l.x, this->l.y, this->l.z);
	glRotatef(ang*180/pi,axis.x,axis.y,axis.z);
	if ((this->x != 0) && (this->y != 0) && (this->z != 0))
	{
		glScalef(this->x, this->y, this->z);
		glutSolidCube(1);
	}
	glPopMatrix();
}

#include "Sphere.h"	// or delete line Box.cpp (any cpp with #include "Sphere.h") in CMakeList.txt
void Sphere::draw()
{
	glPushMatrix();
	glTranslatef(this->l.x, this->l.y, this->l.z);
	if (this->r != 0)
		glutSolidSphere(r, 32, 32); 	// slices for longitude and latitude
	glPopMatrix();
}

#include "Cylinder.h"	// or delete line Box.cpp (any cpp with #include "Cylinder.h") in CMakeList.txt
void Cylinder::draw()
{
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();

	Vector3 axis = this->q.getAxis();
	double ang = this->q.getAng();
	glPushMatrix();
	glTranslatef(this->l.x, this->l.y, this->l.z);
	glRotatef(ang*180/pi,axis.x,axis.y,axis.z);
	glTranslatef(0, 0, -this->h/2);
	if ((this->r != 0) && (this->h != 0))
	{
		int slices = 32;

		glScalef(this->r, this->r, this->h);
		gluCylinder(quadratic,1,1,1, slices, slices);

		glBegin(GL_POLYGON);
		for (int i = 0; i < slices; i++)
			glVertex3f(cos(2*pi/slices*i),sin(2*pi/slices*i),0);
		glEnd();

		glTranslatef(0, 0, 1);
		glBegin(GL_POLYGON);
		for (int i = 0; i < slices; i++)
			glVertex3f(cos(2*pi/slices*i),sin(2*pi/slices*i),0);
		glEnd();
	}
	glPopMatrix();
}

#include "CellularAutomaton.h"	// or delete line Box.cpp (any cpp with #include "CellularAutomaton.h") in CMakeList.txt
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
/*------------ Draw Functions -----------------------*/
double dataLog[MAXDATATRACK][MAXLOG];
double plotXRange = 0;	// how many data per 1m when draw
double plotYRange = 30;	// how large is 1m when draw

int drawPlot(int startx, int endx, int plotNumber)
{
	return drawPlot(startx, endx, plotNumber, 0, 0);
}
int drawPlot(int startx, int endx, int plotNumber, double locx, double locy)
{
	if (plotXRange == 0) plotXRange = endx - startx + 1;
	if (startx < 0) startx = 0;
	for (int i = startx+1; i <= endx; i++)
	{
		glBegin(GL_LINES);
			glVertex3f( 	(i-startx-1)/plotXRange 			+ locx,	
							dataLog[plotNumber][i-1]/plotYRange + locy,
							0.0f
					  );
			glVertex3f( 	(i-startx)/plotXRange 				+ locx,	
							dataLog[plotNumber][i] / plotYRange + locy,
							0.0f
					  );
		glEnd();
	}
	return 0;
}

int drawSphere(double x, double y, double z, double r)
{
	glTranslatef(x,y,z);
	glutSolidSphere(r,10,10);
	glTranslatef(-x,-y,-z);

	return 0;
}

int drawCube(double half, double x, double y, double z, 
						  double fx,double fy,double fz,
						  double ux,double uy,double uz)
{
	drawCube(half,x,y,z,fx,fy,fz,ux,uy,uz,"0");
	return 0;
}

int drawCube(double half, double x, double y, double z, 
						  double fx,double fy,double fz,
						  double ux,double uy,double uz, char command[])
	// f is front, u is up
{
	double xaxis,yaxis,zaxis,axis,angleaxis;
	double fxbase,fybase,fzbase;
	double uxbase,uybase,uzbase;
	double fe,ue;
	double fxdiff,fydiff,fzdiff;
	double uxdiff,uydiff,uzdiff;
	double rf,xrf, yrf, zrf;  // rotation from
	double rt,xrt, yrt, zrt;  // rotation to
	double temp;
	double cos;

	int rotateflag = 1;

	if (((fx-0)*(fx-0)<ZERO) && ((fy-0)*(fy-0)<ZERO) && ((fz-0)*(fz-0)<ZERO))
		rotateflag = 0;
	if (((ux-0)*(ux-0)<ZERO) && ((uy-0)*(uy-0)<ZERO) && ((uz-0)*(uz-0)<ZERO))
		rotateflag = 0;
	if ((fx == 1) && (fy == 0) && (fz == 0))
	{
		drawCube(half, x,y,z,  ux,uy,uz,  fx,fy,fz, command);
		return 0;
	}

	//  some problem for specific situation
	// 		only general case and
	// 		only work for right angle and u = 0,0,1
	if (rotateflag == 1)
	{
		fe = sqrt(fx * fx + fy * fy + fz * fz);
		if (fe == 0) return -1;
		ue = sqrt(ux * ux + uy * uy + uz * uz);
		if (ue == 0) return -1;
		fx /= fe; fy /= fe; fz /= fe;
		ux /= ue; uy /= ue; uz /= ue;

		// fbase = (1,0,0), ubase = (0,0,1)
		fxbase = 1; fybase = 0; fzbase = 0;
		uxbase = 0; uybase = 0; uzbase = 1;

		//  fdiff = f - fbase,  udiff = u - ubase
		fxdiff=fx-fxbase; fydiff=fy-fybase; fzdiff=fz-fzbase;
		uxdiff=ux-uxbase; uydiff=uy-uybase; uzdiff=uz-uzbase;

		// xaxis = fdiff * udiff
		xaxis = fydiff * uzdiff - uzdiff * fydiff;
		yaxis = fzdiff * uxdiff - uxdiff * fzdiff;
		zaxis = fxdiff * uydiff - uydiff * fxdiff;

		axis = sqrt(xaxis * xaxis + yaxis * yaxis + zaxis * zaxis);
		if (!(axis*axis < ZERO))
			{xaxis /= axis; yaxis /= axis; zaxis /= axis;}

		// rot_from = base - axis .* f * axis
		temp = xaxis * fx + yaxis * fy + zaxis * fz;
		xrf = fxbase - temp * xaxis;
		yrf = fybase - temp * yaxis;
		zrf = fzbase - temp * zaxis;
		xrt = fx - temp * xaxis;
		yrt = fy - temp * yaxis;
		zrt = fz - temp * zaxis;
		rf = sqrt(xrf*xrf + yrf*yrf + zrf*zrf);
		if (!(rf*rf < ZERO))
			{xrf /= rf; yrf /= rf; zrf /= rf;}
		rt = sqrt(xrt*xrt + yrt*yrt + zrt*zrt);
		if (!(rt*rt < ZERO))
			{xrt /= rt; yrt /= rt; zrt /= rt;}

		cos = xrf * xrt + yrf * yrt + zrf * zrt;
		angleaxis = acos(cos);
		xaxis = yrf * zrt - zrf * yrt;
		yaxis = zrf * xrt - xrf * zrt;
		zaxis = xrf * yrt - yrf * xrt;

		//angleaxis = acos((xbase*ex+ybase*ey+zbase*ez)/e) + pi;
		//printf("%lf %lf %lf %lf\n",angleaxis,xaxis,yaxis,zaxis);
	}

	///*
	glTranslatef(x,y,z);
	if (rotateflag == 1)
		glRotatef(angleaxis*180/pi,xaxis,yaxis,zaxis);

	if (strcmp(command,"wire") == 0)
		glutWireCube(half);
	else
		glutSolidCube(half);

	if (rotateflag == 1)
		glRotatef(-angleaxis*180/pi,xaxis,yaxis,zaxis);
	glTranslatef(-x,-y,-z);
	//*/

	return 0;
}

int drawCylinder(	double base, double top, double height,
				double lx,	double ly, double lz,
				double ex,	double ey, double ez
			)
{
	double xaxis,yaxis,zaxis,angleaxis;
	double xbase,ybase,zbase;
	double e;
	int rotateflag = 1;
	GLUquadricObj *quadratic;

	quadratic = gluNewQuadric();

	//printf("l: %lf %lf %lf\n",lx,ly,lz);
	//printf("e: %lf %lf %lf\n",ex,ey,ez);

	if (((ex-0)*(ex-0)<ZERO) && ((ey-0)*(ey-0)<ZERO) && ((ez-1)*(ez-1)<ZERO))
		rotateflag = 0;

	if (rotateflag == 1)
	{
		e = sqrt(ex * ex + ey * ey + ez * ez);
		if (e == 0) return -1;

		xbase = 0; ybase = 0; zbase = 1;
		xaxis = ybase * ez - zbase * ey;
		yaxis = zbase * ex - xbase * ez;
		zaxis = xbase * ey - ybase * ex;
		//angleaxis = acos((xbase*ex+ybase*ey+zbase*ez)/e) + pi;
		angleaxis = acos((xbase*ex+ybase*ey+zbase*ez)/e);

		//printf("%lf %lf %lf %lf\n",angleaxis,xaxis,yaxis,zaxis);

	}
	if (((ex-0)*(ex-0)<ZERO) && ((ey-0)*(ey-0)<ZERO) && ((ez+1)*(ez+1)<ZERO))
	{
		xaxis = 1; yaxis = 0; zaxis = 0;
		angleaxis = pi;
	}

	glTranslatef(lx,ly,lz);

	if (rotateflag == 1)
		glRotatef(angleaxis*180/pi,xaxis,yaxis,zaxis);

	gluCylinder(quadratic,base,top,height,32,32);//»­Ô²Öù	base top height
	if (rotateflag == 1)
		glRotatef(-angleaxis*180/pi,xaxis,yaxis,zaxis);

	glTranslatef(-lx,-ly,-lz);
	return 0;
}
