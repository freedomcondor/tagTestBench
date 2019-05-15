/*----------------------------------------*/
/*	Weixu ZHU (Harry)
		zhuweixu_harry@126.com
	Version 1.0
*/
/*----------------------------------------*/

#ifndef SPHERE
#define SPHERE

#include "Object.h"
class Sphere : public Object
{
public:
	double r;

	Sphere();
	Sphere(double x,double y,double z);
	Sphere(const Vector3& _x);
	Sphere(const Vector3& _x, const Quaternion& _y);

	int setSize(double x);
	void draw();	// implement in openGL
};

#endif
