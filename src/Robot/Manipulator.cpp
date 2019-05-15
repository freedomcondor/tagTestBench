#include "Manipulator.h"

Manipulator::Manipulator() : Object() 
{
	pos = 0; 
	upLimit = 0.20;
	downLimit = 0;
	posSpeed = 0.02;	// m/s
	posTarget = 0;	// m/s
	structInit();
}

/*--- Struct Define -------------------*/
void Manipulator::structInit()
{
	subN = 2;
	
	/*--- Body ----*/
	subObj[0] = &locBody;
	locBody.l = Vector3(	0,		0,		0.10);
	locBody.q = Quaternion(	0,0,1,	0);
	body.setSize(			0.07,	0.07,	0.20);
	locBody.obj = &body;

	/*--- Manipulator ----*/
	subObj[1] = &locManip;
	locManip.l = Vector3(	0.085,		0,		pos - 0.05);
	locManip.q = Quaternion(	0,0,1,	0);
	manip.structInit();
	locManip.obj = &manip;

	reLocateSub();
}

void Manipulator::Manip::structInit()
{
	subN = 3;

	/*---- holder -------*/
	subObj[0] = &locHolder;
	locHolder.l = Vector3(	-0.045,		0,		0.095);
	locHolder.q = Quaternion(	0,0,1,	0);
	holder.setSize(			0.01,	0.05,	0.01);
	locHolder.obj = &holder;

	/*---- vertical -------*/
	subObj[1] = &locVertical;
	locVertical.l = Vector3(-0.035,		0,		0.05);
	locVertical.q = Quaternion(	0,0,1,	0);
	vertical.setSize(			0.01,	0.05,	0.10);
	locVertical.obj = &vertical;

	/*---- hand -------*/
	subObj[2] = &locHand;
	locHand.l = Vector3(	0,		0,		0.005);
	locHand.q = Quaternion(	0,0,1,	0);
	hand.setSize(			0.06,	0.06,	0.01);
	locHand.obj = &hand;

	reLocateSub();
}

/*--- Manipulator Function-------------------*/
int Manipulator::setPos(double x)
{
	if (x > upLimit)
		posTarget = upLimit;
	else if (x < downLimit)
		posTarget = downLimit;
	else
		posTarget = x;

	return 0;
}

double Manipulator::getPos(double x) {return pos;}

int Manipulator::run(double time)
{
	Object::run(time);

	if (pos < posTarget)
	{
		pos += posSpeed * time;
		if (pos > upLimit) pos = upLimit;
	}
	
	if (pos > posTarget)
	{
		pos -= posSpeed * time;
		if (pos < downLimit) pos = downLimit;
	}

	locManip.l.z = pos - 0.05;

	return 0;
}
