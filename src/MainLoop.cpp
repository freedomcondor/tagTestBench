/*---------------------------------------------------------*/
/*
   	Weixu ZHU (Harry)
   		zhuweixu_harry@126.com
	
	Version 1.0
	Version 1.1 : change opengl debug layout

*/
/*---------------------------------------------------------*/

#include <stdio.h>
#include "GLTools.h"

#include "Vector3.h"
#include "Quaternion.h"

#include "Box.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Convexhull.h"

using namespace cv;

/*------------ Function Control Channels ------------*/
double CH1,CH1_MAX,CH1_MIN,CH1_STEP;
double CH2,CH2_MAX,CH2_MIN,CH2_STEP;
int CMDCH1;

/*------------ Custom ------------*/
// OpenCV
VideoCapture video1;

// apriltag
apriltag_detector* TagDetector;
apriltag_family* TagFamily;
apriltag_detection_info_t CameraInfo;

#define NTAGS 10
int nTags = 0;
Box tags[NTAGS];

// functions
int drawCross(Mat img, int x, int y, const char colour[],int label);
int drawCross(Mat img, int x, int y, const char colour[]);
int setColor(cv::Vec<unsigned char, 3> &pix,int R,int G, int B);
int rotationMatToQuaternion(double data[], Quaternion& q);

/* --------------- MainLoop functions --------------------*/
int function_exit()
{
	apriltag_detector_remove_family(TagDetector, TagFamily);
	apriltag_detector_destroy(TagDetector);
	tag36h11_destroy(TagFamily);
	return 0;
}

int function_init(int SystemWidth, int SystemHeight)
{
	// initalize tags
	for (int i = 0; i < NTAGS; i++)
		tags[i].setSize(0.03, 0.03, 0.001);

	// open camera
	video1.open(0);
	if (video1.isOpened()) printf("camera open successful\n");
	                 else {printf("camera open failed\n"); return -1;}

	namedWindow("camera", WINDOW_NORMAL);
	moveWindow("camera",SystemWidth/2,0);
	resizeWindow("camera",SystemWidth/2,SystemHeight/2);

	// open apriltag
	TagDetector = apriltag_detector_create();
	TagDetector->quad_decimate = 1.0f;
	TagDetector->quad_sigma = 0.0f;
	TagDetector->refine_edges = 1;
	TagDetector->decode_sharpening = 0.25;
	TagDetector->nthreads = 2;
	TagFamily = tag36h11_create();
	apriltag_detector_add_family(TagDetector, TagFamily);

	// apriltag pose estimation
	CameraInfo.tagsize = 0.0235;
	CameraInfo.fx = 939.001439;
	CameraInfo.fy = 939.001439;
	CameraInfo.cx = 320 ;
	CameraInfo.cy = 240;

	return 0;
}

int function_step(double time)	// time in s
{
	Mat imgRGB, imgGRAY;
	// capture image
	video1 >> imgRGB;
	cvtColor(imgRGB, imgGRAY, CV_BGR2GRAY);
	// detect apriltag
	image_u8_t* imgU8Y = image_u8_create(imgGRAY.cols, imgGRAY.rows);
	for (unsigned int row = 0; row < imgU8Y->height; row++)
		memcpy(&imgU8Y->buf[row * imgU8Y->stride],
		       imgGRAY.row(row).data,
		       imgU8Y->width);
	zarray_t* psDetections = apriltag_detector_detect(TagDetector, imgU8Y);
	image_u8_destroy(imgU8Y);

	// get detections
	nTags = zarray_size(psDetections);
	for (int i = 0; i < nTags; i++)
	{
		// get detection
		apriltag_detection_t *detection;
		zarray_get(psDetections, i, &detection);

		// draw on opencv screen
		// for apriltag width first, height second 
		// for opencv height first, width second 
		drawCross(imgRGB, detection->c[1], detection->c[0], "blue");
		drawCross(imgRGB, detection->p[0][1], detection->p[0][0], "blue");
		drawCross(imgRGB, detection->p[1][1], detection->p[1][0], "blue");
		drawCross(imgRGB, detection->p[2][1], detection->p[2][0], "blue");
		drawCross(imgRGB, detection->p[3][1], detection->p[3][0], "blue");

		// pose detection
		apriltag_pose_t pose;
		CameraInfo.det = detection; // don't know what does it mean
		//double err = estimate_tag_pose(&CameraInfo, &pose);	
		estimate_pose_for_tag_homography(&CameraInfo, &pose);	

		tags[i].setl(pose.t->data[0], pose.t->data[1], pose.t->data[2]);
		Quaternion q;
		rotationMatToQuaternion(pose.R->data, q);
		tags[i].setq(q);
	}

	// destroy and draw 
	apriltag_detections_destroy(psDetections);
	imshow("camera", imgRGB);
	waitKey(1);
	return 0;
}

/* --------------- OpenGL draw functions --------------------*/
int function_draw()
{
	for (int i = 0; i < nTags; i++)
		tags[i].draw();
	return 0;
}

int function_draw2()
{
	return 0;
}

/* - Custom ----------------------------------------------------------------  */
int drawCross(Mat img, int x, int y, const char colour[],int label)
{
	label = label % 9;
	if (label == 0) label = 9;
	if (label == 1)
		drawCross(img,x,y,colour);
	else if (label == 2)
	{
		drawCross(img,x,y-2,colour);
		drawCross(img,x,y+2,colour);
	}
	else if (label == 3)
	{
		drawCross(img,x-2,y-2,colour);
		drawCross(img,x-2,y+2,colour);
		drawCross(img,x+2,y,colour);
	}
	else if (label == 4)
	{
		drawCross(img,x-2,y-2,colour);
		drawCross(img,x-2,y+2,colour);
		drawCross(img,x+2,y-2,colour);
		drawCross(img,x+2,y+2,colour);
	}
	else if (label == 5)
	{
		drawCross(img,x-6,y-2,colour);
		drawCross(img,x-2,y-2,colour);
		drawCross(img,x-2,y+2,colour);
		drawCross(img,x+2,y-2,colour);
		drawCross(img,x+2,y+2,colour);
	}
	else if (label == 6)
	{
		drawCross(img,x-6,y-2,colour);
		drawCross(img,x-6,y+2,colour);
		drawCross(img,x-2,y-2,colour);
		drawCross(img,x-2,y+2,colour);
		drawCross(img,x+2,y-2,colour);
		drawCross(img,x+2,y+2,colour);
	}
	else if (label == 7)
	{
		drawCross(img,x-6,y-2,colour);
		drawCross(img,x-6,y+2,colour);
		drawCross(img,x-2,y-2,colour);
		drawCross(img,x-2,y+2,colour);
		drawCross(img,x+2,y-2,colour);
		drawCross(img,x+2,y+2,colour);
		drawCross(img,x+6,y-2,colour);
	}
	else if (label == 8)
	{
		drawCross(img,x-6,y-2,colour);
		drawCross(img,x-6,y+2,colour);
		drawCross(img,x-2,y-2,colour);
		drawCross(img,x-2,y+2,colour);
		drawCross(img,x+2,y-2,colour);
		drawCross(img,x+2,y+2,colour);
		drawCross(img,x+6,y-2,colour);
		drawCross(img,x+6,y+2,colour);
	}
	else if (label == 9)
	{
		drawCross(img,x,y,colour);
		drawCross(img,x-4,y,colour);
		drawCross(img,x+4,y,colour);

		drawCross(img,x,  y-4,colour);
		drawCross(img,x-4,y-4,colour);
		drawCross(img,x+4,y-4,colour);

		drawCross(img,x,  y+4,colour);
		drawCross(img,x-4,y+4,colour);
		drawCross(img,x+4,y+4,colour);
	}
	return 0;
}

int drawCross(Mat img, int x, int y, const char color[])
{
	if ((x > img.rows-1) || (x < 0) || (y > img.cols-1) || (y < 0)) return -1;

	int R,G,B;
	if (strcmp(color,"blue") == 0)
	{ R = 0; G = 0; B = 255; }
	else if (strcmp(color,"green") == 0)
	{ R = 0; G = 255; B = 0; }
	else if (strcmp(color,"red") == 0)
	{ R = 255; G = 0; B = 0; }
	Mat_<Vec3b> _image = img;
	setColor(_image(x,y),R,G,B);
	if (x != 0) 			setColor(_image(x-1,y),R,G,B);
	if (x != img.rows-1) 	setColor(_image(x+1,y),R,G,B);
	if (y != 0) 			setColor(_image(x,y-1),R,G,B);
	if (y != img.cols-1) 	setColor(_image(x,y+1),R,G,B);
	img = _image;
	return 0;
}

int setColor(cv::Vec<unsigned char, 3> &pix,int R,int G, int B)
{
	pix[0] = B;
	pix[1] = G;
	pix[2] = R;
	return 0;
}

int rotationMatToQuaternion(double data[], Quaternion& q)
{
	double w, x, y, z;
	w = sqrt(1.0 + data[3*0+0] + data[3*1+1] + data[3*2+2]) / 2.0;
	double w4 = w * 4.0;
	x = (data[3*2+1] - data[3*1+2]) / w4;
	y = (data[3*0+2] - data[3*2+0]) / w4;
	z = (data[3*1+0] - data[3*0+1]) / w4;
	q.setHardValue(x,y,z,w);
	return 0;
}
