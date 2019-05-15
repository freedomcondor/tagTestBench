/*----------------------------------------*/
/*	Weixu ZHU (Harry)
		zhuweixu_harry@126.com
	Version 2.0
		redesign Constructor
	Version 2.1
		fixed some bugs of the set or Quaternion(xxx)
*/
/*----------------------------------------*/

#ifndef QUATERNION
#define QUATERNION

#include "Vector3.h"

class Quaternion
{
public:
	Vector3 l;
	double w;
	Quaternion();	// default no rotation
	Quaternion(double _x, double _y, double _z, double _w);
	Quaternion(const Vector3& _l, double _w);
		// axis and rotate(rad)
	Quaternion(const Quaternion& _x);
	~Quaternion();

	Quaternion& set(double _x, double _y, double _z, double _w);
	Quaternion& set(const Vector3& _l, double _w);
	Quaternion& set(const Quaternion& _x);
		// axis and rotate(rad)

	Quaternion& setHardValue(double _x, double _y, double _z, double _w);
	Quaternion& setHardValue(const Vector3& _l, double _w);

	/*	not safe ! don't use
	Quaternion& setFrom4Vecs(const Vector3& _abc_o,const Vector3& _pqr_o,
							 const Vector3& _abc,  const Vector3& _pqr);
	*/

	Vector3 getAxis() const;
	double getAng() const;

/*----------------------------------------*/
	// operators
	Quaternion& operator+=(const Quaternion& _x);
	Quaternion& operator-=(const Quaternion& _x);
	Quaternion  operator-() const;
	Quaternion  operator-(const Quaternion& _x) const;
	Quaternion  operator+(const Quaternion& _x) const;
	Quaternion  operator*(double _x) const;
	Quaternion  operator/(double _x) const;
	Quaternion& operator*=(double _x);
	Quaternion& operator/=(double _x);

	Quaternion  operator*(const Quaternion& _x) const;
	Quaternion& operator*=(const Quaternion& _x);


	Quaternion operator^(double _x) const;

	// len & inv
	double len() const;
	Quaternion inv() const;

	// rotate
	Vector3 toRotate(const Vector3& _x) const;

public:
	char* toStr();
private:
	char strForMe[100];
};

#endif
