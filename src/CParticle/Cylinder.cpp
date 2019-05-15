/*----------------------------------------*/
/*	Weixu ZHU (Harry)
		zhuweixu_harry@126.com
	Version 1.0
*/
/*----------------------------------------*/

#include "Cylinder.h"

Cylinder::Cylinder():Object()
{
	setSize(1,1);
}

Cylinder::Cylinder(double x,double y,double z)
	:Object(x,y,z)
{
	setSize(1,1);
}

Cylinder::Cylinder(const Vector3& _x)
	:Object(_x)
{
	setSize(1,1);
}

Cylinder::Cylinder(const Vector3& _x, const Quaternion& _y)
	:Object(_x,_y)
{
	setSize(1,1);
}

int Cylinder::setSize(double _r,double _h)
{
	r = _r; h = _h;
	return 0;
}
