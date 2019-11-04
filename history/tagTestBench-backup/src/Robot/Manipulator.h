#ifndef MANIPULATOR
#define MANIPULATOR

#include "Object.h"
#include "Box.h"

class Manipulator : public Object
{
public:
	/*---- struct define -------*/
	Box body;
	struct SubObj locBody;
	class Manip : public Object
	{
		public:
		Box holder, vertical, hand;
		struct SubObj locHolder, locVertical, locHand;
		void structInit();
	} manip;
	struct SubObj locManip;
	void structInit();

	/*---- members -------*/
	Manipulator();
	double pos;
	double posSpeed;
	double posTarget;
	double upLimit, downLimit;

	/*---- control interface -------*/
	int setPos(double x);
	double getPos(double x);

	int run(double time);
};

#endif
