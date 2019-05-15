/*----------------------------------------*/
/*	Weixu ZHU (Harry)
		zhuweixu_harry@126.com
	Version 1.0
*/
/*----------------------------------------*/

#include "Sphere.h"

Sphere::Sphere():Object()
{
	setSize(1);
}

Sphere::Sphere(double x,double y,double z)
	:Object(x,y,z)
{
	setSize(1);
}

Sphere::Sphere(const Vector3& _x)
	:Object(_x)
{
	setSize(1);
}

Sphere::Sphere(const Vector3& _x, const Quaternion& _y)
	:Object(_x,_y)
{
	setSize(1);
}

int Sphere::setSize(double _x)
{
	r = _x;
	return 0;
}
