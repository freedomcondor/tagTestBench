#include "DBox.h"

#include <stdlib.h>

#define PI 3.1415926
DBox::DBox() : Object() {structInit();}
DBox::DBox(double x,double y,double z) : Object(x,y,z) {structInit();}
DBox::DBox(const Vector3& _x) : Object(_x) {structInit();}
DBox::DBox(const Vector3& _x, const Quaternion& _y) : Object(_x,_y) {structInit();}

void DBox::structInit()
{
	subN = 3;

	subObj[0] = &rLeft;
	subObj[0]->l = Vector3(0,0.2,0);
	subObj[0]->q = Quaternion(0,0,1,-PI/4);
	left.setSize(0.1,0.2,0.3);
	subObj[0]->obj = &left;

	subObj[1] = &rRight; 
	subObj[1]->l = Vector3(0,-0.2,0);
	subObj[1]->q = Quaternion(0,0,1,PI/4);
	right.setSize(0.1,0.2,0.3);
	subObj[1]->obj = &right;

	subObj[2] = &rMiddle;
	subObj[2]->l = Vector3(0.0,0,0);
	subObj[2]->q = Quaternion(0,0,1,0);
	middle.structInit();
	subObj[2]->obj = &middle;

	reLocateSub();
}

void DBox::ABox::structInit()
{
	subN = 2;

	subObj[0] = &rUp;
	subObj[0]->l = Vector3(0,0,0.1);
	subObj[0]->q = Quaternion(0,0,1,0);
	up.setSize(0.1,0.1,0.1);
	subObj[0]->obj = &up;

	subObj[1] = &rDown; 
	subObj[1]->l = Vector3(0,0,-0.1);
	subObj[1]->q = Quaternion(0,0,1,0);
	down.setSize(0.1,0.1,0.1);
	subObj[1]->obj = &down;

	reLocateSub();
}
