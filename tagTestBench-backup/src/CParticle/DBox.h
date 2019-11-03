#ifndef DBOX
#define DBOX

#include "Object.h"
#include "Box.h"

class DBox : public Object
{
public:
	DBox();
	DBox(double x,double y,double z);
	DBox(const Vector3& _x);
	DBox(const Vector3& _x, const Quaternion& _y);

	Box left, right;
	class ABox : public Object
	{
		public:
		Box up, down;
		struct SubObj rUp, rDown;
		void structInit();
	} middle;
	struct SubObj rLeft, rRight, rMiddle;
	void structInit();
};

#endif
