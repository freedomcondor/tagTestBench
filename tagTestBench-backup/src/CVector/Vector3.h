/*----------------------------------------*/
/*	Weixu ZHU (Harry)
		zhuweixu_harry@126.com
	Version 2.0
*/
/*----------------------------------------*/
#ifndef VECTOR3
#define VECTOR3
class Vector3
{
public:
	// creation
	double x,y,z;
	Vector3();
	Vector3(double _x, double _y, double _z);
	Vector3(const Vector3& _x);	// use Vector3 b(a);
					// with this, a = b implicitly calls this function
	~Vector3();
	Vector3& set(const Vector3& _x);	// b.set(a);
	Vector3& set(double _x, double _y, double _z);

	// operators
	Vector3& operator+=(const Vector3& _x);
	Vector3& operator-=(const Vector3& _x);
	Vector3  operator-(const Vector3& _x) const;
	Vector3  operator-() const;
	Vector3  operator+(const Vector3& _x) const;

	Vector3& operator*=(double _x);
	Vector3& operator/=(double _x);

	Vector3  operator*(double _x) const;	// const at last means it won't change class members
											// so that friend doube * Vec3 can use it
	Vector3  operator/(double _x) const;
	friend Vector3 operator*(double _x,const Vector3& _y);
	Vector3  operator*(const Vector3& _x) const;

	double operator^(double _x) const;	// only for x^2
	double operator^(const Vector3& _x) const;

	// compare ==  /len/nor/squlen
	bool operator==(const Vector3& _x) const;
	double len() const;
	Vector3 nor() const;
	Vector3& makenor();

public:
	// for print
	char* toStr();			// use printf("%s",me.toStr());
private: 
	char strForMe[100]; // used by toStr

	// should be a rotation nearby
};

#endif
