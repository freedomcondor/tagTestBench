#ifndef MATRIX3
#define MATRIX3

#include "Vector3.h"

class Matrix3
{
public:
	double mat[3][3];
	// creation
	Matrix3();
	Matrix3(double _x11, double _x12, double _x13,
			double _x21, double _x22, double _x23,
			double _x31, double _x32, double _x33);
	Matrix3(const Matrix3& _x);
	Matrix3(const Vector3& _x, 
			const Vector3& _y, 
			const Vector3& _z);

	~Matrix3();

	int set(double _x11, double _x12, double _x13,
			double _x21, double _x22, double _x23,
			double _x31, double _x32, double _x33
			);
	int set(const Matrix3& _x);
	int set(const Vector3& _x, 
			const Vector3& _y, 
			const Vector3& _z);


	//operators

	// for print
public:
	char* toStr();          // use printf("%s",me.toStr());
private:
	char strForMe[300]; // used by toStr
};

#endif	// MATRIX3
