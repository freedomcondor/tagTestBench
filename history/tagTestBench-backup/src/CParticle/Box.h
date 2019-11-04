/*----------------------------------------*/
/*	Weixu ZHU (Harry)
		zhuweixu_harry@126.com
	Version 1.0
*/
/*----------------------------------------*/

#ifndef BOX
#define BOX

#include "Object.h"

class Box : public Object
{
public:
	double x,y,z;

	Box();
	Box(double x,double y,double z);
	Box(const Vector3& _x);
	Box(const Vector3& _x, const Quaternion& _y);

	int setSize(double x,double y,double z);
	void draw();	// implement in openGL
};

#endif
