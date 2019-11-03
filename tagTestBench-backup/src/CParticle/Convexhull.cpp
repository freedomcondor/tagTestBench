/*----------------------------------------*/
/*	Weixu ZHU (Harry)
		zhuweixu_harry@126.com
	Version 1.0
*/
/*----------------------------------------*/

#include "Convexhull.h"

Convexhull::Convexhull():Object()
{
	n = 0;
}

Convexhull::Convexhull(double x,double y,double z)
	:Object(x,y,z)
{
	n = 0;
}

Convexhull::Convexhull(const Vector3& _x)
	:Object(_x)
{
	n = 0;
}

Convexhull::Convexhull(const Vector3& _x, const Quaternion& _y)
	:Object(_x,_y)
{
	n = 0;
}

Convexhull::~Convexhull()
{
	delete[] points;
}

int Convexhull::setPoint(int _x, double _points[])
{
	n = _x;
	points = new Vector3[n];

	int i = 0;
	for (int j = 0; j < n; j++)
	{
		points[j].x = _points[i++];
		points[j].y = _points[i++];
		points[j].z = _points[i++];
	}

	computeHull();
	return 0;
}

int Convexhull::computeHull()
{
	return 0;
}
