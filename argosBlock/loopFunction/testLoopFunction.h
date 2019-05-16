#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/simulator/entities/cylinder_entity.h>
#include <argos3/plugins/simulator/entities/box_entity.h>

#include <opencv2/opencv.hpp>
#include <apriltag/apriltag.h>
#include <apriltag/tag36h11.h>
#include <apriltag/apriltag_pose.h>

#include "LEDprojection.h"

using namespace argos;
using namespace cv;

class testLoopFunction : public CLoopFunctions
{
public:
	//testLoopFunction();
	//~testLoopFunction();
	void test();
	void Init(TConfigurationNode& t_tree);
	void PreStep();
	void Destroy();
private:
	CCylinderEntity* pcCylinder;
	CBoxEntity* pcBox[100];
	CBoxEntity* pcTag[100];
};
