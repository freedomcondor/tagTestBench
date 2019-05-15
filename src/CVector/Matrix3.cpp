#include "Matrix3.h"
#include <stdio.h>	// for sprintf in toStr and printf

Matrix3::Matrix3()
{
	set(0,0,0,0,0,0,0,0,0);
}

Matrix3::Matrix3(double _x11, double _x12, double _x13,
				double _x21, double _x22, double _x23,
				double _x31, double _x32, double _x33)
{
	set(_x11, _x12, _x13,
		_x21, _x22, _x23,
		_x31, _x32, _x33);
}
Matrix3::Matrix3(const Matrix3& _x)
{
	set(_x);
}

Matrix3::Matrix3(const Vector3& _x, 
				const Vector3& _y, 
				const Vector3& _z)
{
	set(_x, _y, _z);
}

Matrix3::~Matrix3()
{
}

int Matrix3::set(double _x11, double _x12, double _x13,
				double _x21, double _x22, double _x23,
				double _x31, double _x32, double _x33)
{
	mat[0][0] = _x11; mat[0][1] = _x12; mat[0][2] = _x13;
	mat[1][0] = _x21; mat[1][1] = _x22; mat[1][2] = _x23;
	mat[2][0] = _x31; mat[2][1] = _x32; mat[2][2] = _x33;
	return 0;
}

int Matrix3::set(const Matrix3& _x)
{
	mat[0][0] = _x.mat[0][0];
	mat[0][1] = _x.mat[0][1];
	mat[0][2] = _x.mat[0][2];

	mat[0][0] = _x.mat[1][0];
	mat[1][1] = _x.mat[1][1];
	mat[1][2] = _x.mat[1][2];

	mat[2][0] = _x.mat[2][0];
	mat[2][1] = _x.mat[2][1];
	mat[2][2] = _x.mat[2][2];

	return 0;
}

int Matrix3::set(const Vector3& _x, 
				const Vector3& _y, 
				const Vector3& _z)
{
	mat[0][0] = _x.x;
	mat[0][1] = _x.y;
	mat[0][2] = _x.z;

	mat[1][0] = _y.x;
	mat[1][1] = _y.y;
	mat[1][2] = _y.z;

	mat[2][0] = _z.x;
	mat[2][1] = _z.y;
	mat[2][2] = _z.z;

	return 0;
}

/*---------------------  print string ---------------------*/
char* Matrix3::toStr()
{
	sprintf(strForMe,"\n+ %lf, %lf, %lf +\n| %lf, %lf, %lf |\n+ %lf, %lf, %lf +\n",
					  mat[0][0], mat[0][1], mat[0][2],
					  mat[1][0], mat[1][1], mat[1][2],
					  mat[2][0], mat[2][1], mat[2][2]);

	return strForMe;
}
