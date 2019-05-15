/*----------------------------------------*/
/*	Weixu ZHU (Harry)
		zhuweixu_harry@126.com
	Version 1.0
*/
/*----------------------------------------*/

#ifndef CONVEXHULL
#define CONVEXHULL

#include "Object.h"

class Convexhull : public Object
{
public:
	Vector3 *points;
	int n;

	Convexhull();
	Convexhull(double x,double y,double z);
	Convexhull(const Vector3& _x);
	Convexhull(const Vector3& _x, const Quaternion& _y);
	~Convexhull();

	int setPoint(int _x, double _points[]);
	//void draw();	// implement in openGL
	
private:
	int computeHull();
};

#endif
