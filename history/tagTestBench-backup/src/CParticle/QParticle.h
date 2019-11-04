/*----------------------------------------*/
/*	Weixu ZHU (Harry)
		zhuweixu_harry@126.com
	Version 2.0
		combine Particle and QParticle
	Version 2.1
		use vector3 to denote rotation, Quaternion not valid
	Version 3.0
		add support of Force and Torque
*/
/*----------------------------------------*/

#ifndef QPARTICLE
#define QPARTICLE

#include "Vector3.h"
#include "Quaternion.h"

								#include<stdio.h>


class QParticle
{
public:
	Vector3 l,v,a;
	Quaternion q;
	Vector3 vq, aq;	
	// you can't use quaternion to count rotation speed
	// if the speed is > 2PI, there will be a problem
	double m,I;
	Vector3 F, T;

	QParticle();
	QParticle(double x,double y,double z);
	QParticle(const Vector3& _x);
	QParticle(double x,double y,double z, 
				double qx,double qy,double qz,double qw);
	QParticle(const Vector3& _x, const Quaternion& _y);
	~QParticle();

	QParticle& setl(const Vector3& _x);
	QParticle& setl(double x,double y,double z);
	QParticle& setv(const Vector3& _x);
	QParticle& setv(double x,double y,double z);
	QParticle& seta(const Vector3& _x);
	QParticle& seta(double x,double y,double z);
	QParticle& setm(double x);
	QParticle& setI(double x);
	QParticle& setF(const Vector3& _x);
	QParticle& setF(double x,double y,double z);
	QParticle& setT(const Vector3& _x);
	QParticle& setT(double x,double y,double z);

	QParticle& setq(const Quaternion& _x);
	QParticle& setq(double qx,double qy,double qz,double qw);
	QParticle& setvq(const Vector3& _x);
	QParticle& setvq(double qx,double qy,double qz,double qw);
	QParticle& setaq(const Vector3& _x);
	QParticle& setaq(double qx,double qy,double qz,double qw);

	QParticle& addF(const Vector3& _x);
	// TODO: add force at a certain point
	QParticle& addT(const Vector3& _x);
	QParticle& clearF();
	QParticle& clearT();

	Vector3 getFront() const;
	Vector3 getUp() const;

	int run(double time);
};

#endif
