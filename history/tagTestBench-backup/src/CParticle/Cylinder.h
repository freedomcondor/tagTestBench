/*----------------------------------------*/
/*	Weixu ZHU (Harry)
		zhuweixu_harry@126.com
	Version 1.0
*/
/*----------------------------------------*/

#ifndef CYLINDER
#define CYLINDER

#include "Object.h"

class Cylinder : public Object
{
public:
	double r,h;

	Cylinder();
	Cylinder(double x,double y,double z);
	Cylinder(const Vector3& _x);
	Cylinder(const Vector3& _x, const Quaternion& _y);

	int setSize(double _r,double _h);
	void draw();	// implement in openGL
};
#endif
