/*----------------------------------------*/
/*	Weixu ZHU (Harry)
		zhuweixu_harry@126.com
	Version 2.0
*/
/*----------------------------------------*/
#include "Vector3.h"
#include <stdio.h>	// for sprintf in toStr and printf
#include <math.h>	// for sqrt


/*-- Constructor Destructor-----------------------------*/
Vector3::Vector3()
	{ set(0,0,0); }

Vector3::Vector3(double x, double y, double z)
	{ set(x,y,z); }

Vector3::Vector3(const Vector3& _x)
	{ set(_x); }

Vector3::~Vector3()
	{}

/*-- set ----------------------------*/
Vector3& Vector3::set(const Vector3& _x)
	{ x = _x.x; y = _x.y; z = _x.z; return *this; }

Vector3& Vector3::set(double _x, double _y, double _z)
{
	x = _x;
	y = _y;
	z = _z;
	return *this;
}

/*-- operator ----------------------------*/
Vector3& Vector3::operator+=(const Vector3& _x)
{
	x += _x.x;
	y += _x.y;
	z += _x.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& _x)
{
	x -= _x.x;
	y -= _x.y;
	z -= _x.z;
	return *this;
}

Vector3 Vector3::operator- () const
{
	Vector3 c(0,0,0);
	c -= *this;
	return c;
}

Vector3 Vector3::operator- (const Vector3& _x) const
{
	Vector3 c(*this);
	c -= _x;
	return c;
}

Vector3 Vector3::operator+ (const Vector3& _x) const
{
	Vector3 c(*this);
	c += _x;
	return c;
}

Vector3& Vector3::operator*=(double _x)
{
	x *= _x;
	y *= _x;
	z *= _x;
	return *this;
}

Vector3& Vector3::operator/=(double _x)
{
	if (_x == 0)
	{
		printf("in Vector3, /= ,tried to divided by 0\n");
		this->set(0,0,0);
		return *this;
	}

	x /= _x;
	y /= _x;
	z /= _x;
	return *this;
}

Vector3 Vector3::operator* (double _x) const
{
	Vector3 c(*this);
	c *= _x;
	return c;
}

Vector3 Vector3::operator/ (double _x) const
{
	Vector3 c(*this);
	c /= _x;
	return c;
}

Vector3 operator*(double _x,const Vector3& _y)
{
	return _y * _x;
}

Vector3 Vector3::operator*(const Vector3& _x) const
{
	Vector3 c(	this->y * _x.z - this->z * _x.y,
				this->z * _x.x - this->x * _x.z,
				this->x * _x.y - this->y * _x.x);
	return c;
}

double Vector3::operator^(double _x) const
{
	double c = 0;
	if (_x == 2)
		c = *this ^ *this;
	else
		printf("in Vector3, invalid ^\n");
	return c;
}
double Vector3::operator^(const Vector3& _x) const
{
	return 	this->x * _x.x +
			this->y * _x.y +
			this->z * _x.z;
}

/*-- operator  == len nor squlen -----------*/

bool Vector3::operator==(const Vector3& _x) const
{
	return (this->x == _x.x && this->y == _x.y && this->z == _x.z);
}

double Vector3::len() const
{
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vector3 Vector3::nor() const
{
	double l = this->len();
	if (l == 0)
		return *this;
	Vector3 c(*this / l);
	return c;
}
Vector3& Vector3::makenor()
{
	double l = this->len();
	this->x /= l;
	this->y /= l;
	this->z /= l;
	return *this;
}

/*-- For Print -----------*/
char* Vector3::toStr()
{
	sprintf(strForMe,"(%lf, %lf, %lf)",x,y,z);
	return strForMe;
}

/* rotation is implemented in Quaternion */

