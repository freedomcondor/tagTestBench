/*---------------------------------------------------------*/
/*
   	Weixu ZHU (Harry)
   		zhuweixu_harry@126.com
	
	Version 1.0
	Version 1.1 : change opengl debug layout

*/
/*---------------------------------------------------------*/
#include <opencv2/opencv.hpp>
#include <apriltag/apriltag.h>
#include <apriltag/tag36h11.h>
#include <apriltag/apriltag_pose.h>

extern "C" {
#include "solve_square.h"
}

#include <stdio.h>
#include "GLTools.h"

#include "Vector3.h"
#include "Quaternion.h"

#include "Box.h"

//#include "LuaCtrl.h"

using namespace cv;

// opencv
VideoCapture video1;
VideoCapture video2;

// apriltag
apriltag_detector* TagDetector;
apriltag_family* TagFamily;
apriltag_detection_info_t CameraInfo;

// functions
int drawCross(Mat img, int x, int y, const char colour[],int label);
int drawCross(Mat img, int x, int y, const char colour[]);
int setColor(cv::Vec<unsigned char, 3> &pix,int R,int G, int B);
int rotationMatToQuaternion(double data[], Quaternion& q);

// tags
#define NTAGS 20
int nTags = 0;
Box lib_tags[NTAGS];
Box lib_tagDir[NTAGS];
Box c_tags[NTAGS];
Box c_tagDir[NTAGS];

/* --------------- MainLoop functions --------------------*/
int simu_exit()
{
	return 0;
}

int simu_init(int ScreenWidth, int ScreenHeight)
{
	EyeWInit = 270; EyeThInit = -90; EyeLInit = 0.015;
	EyeXInit = 0;   EyeYInit = 0;  EyeZInit = 0;

	// initalize tags
	Vector3 tagSize = Vector3(0.03, 0.03, 0.001);
	Vector3 tagDirSize = Vector3(0.005, 0.005, 0.03);
	for (int i = 0; i < NTAGS; i++)
	{
		lib_tags[i].setSize(tagSize);
		lib_tagDir[i].setSize(tagDirSize);
		c_tags[i].setSize(tagSize);
		c_tagDir[i].setSize(tagDirSize);
	}

	// ------------------------ opencv --------------------------- //
	video1.open(0);
	if (video1.isOpened()) printf("camera open successful\n");
	                 else {printf("camera open failed\n"); return -1;}

	video1.set(CV_CAP_PROP_FRAME_WIDTH,800);
	video1.set(CV_CAP_PROP_FRAME_HEIGHT,600);
	//video1.set(CV_CAP_PROP_FRAME_WIDTH,1024);
	//video1.set(CV_CAP_PROP_FRAME_HEIGHT,768);

	namedWindow("camera1", WINDOW_NORMAL);
	moveWindow("camera1",ScreenWidth/2,ScreenHeight/2);
	resizeWindow("camera1",ScreenWidth/4,ScreenHeight/2.2);

	video2.open(1);
	if (video2.isOpened()) printf("camera open successful\n");
	                 else {printf("camera open failed\n"); return -1;}

	namedWindow("camera2", WINDOW_NORMAL);
	moveWindow("camera2",ScreenWidth/2,0);
	resizeWindow("camera2",ScreenWidth/4,ScreenHeight/2.2);

	video2.set(CV_CAP_PROP_FRAME_WIDTH,800);
	video2.set(CV_CAP_PROP_FRAME_HEIGHT,600);
	//video2.set(CV_CAP_PROP_FRAME_WIDTH,1024);
	//video2.set(CV_CAP_PROP_FRAME_HEIGHT,768);

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
	/*
	CameraInfo.fx = 939.001439;
	CameraInfo.fy = 939.001439;
	CameraInfo.cx = 320 ;
	CameraInfo.cy = 240;
	*/

	CameraInfo.fx = 796.9096;
	CameraInfo.fy = 796.9096;
	CameraInfo.cx = 400;
	CameraInfo.cy = 300;

	/*
	CameraInfo.fx = 1039.4975;
	CameraInfo.fy = 1039.4975;
	CameraInfo.cx = 512;
	CameraInfo.cy = 384;
	*/

	return 0;
}

int simu_step(double time)	// time in s
{
	printf("--------step----------\n");
	// ----------------- opencv capture  ------------------------- //
	Mat imgRGB800, imgRGB, imgGRAY;
	// capture image
	video1 >> imgRGB800;
	//imgRGB = imread("../failure.jpg");
	
	Rect crop(100, 0, 600, 600);
	imgRGB = imgRGB800(crop);

	cvtColor(imgRGB, imgGRAY, CV_BGR2GRAY);

	Mat imgRGB8002, imgRGB2, imgGRAY2;
	video2 >> imgRGB8002;
	imgRGB2 = imgRGB8002(crop);

	// ----------------- apriltag detect ------------------------- //
	image_u8_t* imgU8Y = image_u8_create(imgGRAY.cols, imgGRAY.rows);
	for (unsigned int row = 0; row < imgU8Y->height; row++)
		memcpy(&imgU8Y->buf[row * imgU8Y->stride],
		       imgGRAY.row(row).data,
		       imgU8Y->width);
	zarray_t* psDetections = apriltag_detector_detect(TagDetector, imgU8Y);
	image_u8_destroy(imgU8Y);

	// get detections
	nTags = zarray_size(psDetections);
	printf("detect number: %d\n", nTags);
	for (int i = 0; i < nTags; i++)
	{
		// get detection
		apriltag_detection_t *detection;
		zarray_get(psDetections, i, &detection);

		// draw 4 corners on opencv screen
		// for apriltag width first, height second 
		// for opencv height first, width second 
		drawCross(imgRGB, detection->c[1], detection->c[0], "blue");
		drawCross(imgRGB, detection->p[0][1], detection->p[0][0], "red");
		drawCross(imgRGB, detection->p[1][1], detection->p[1][0], "green");
		drawCross(imgRGB, detection->p[2][1], detection->p[2][0], "blue");
		drawCross(imgRGB, detection->p[3][1], detection->p[3][0], "blue");

		// pose detection
		apriltag_pose_t pose;
		CameraInfo.det = detection;

		// iteration
		double err = estimate_tag_pose(&CameraInfo, &pose);	

		lib_tags[i].setl(pose.t->data[0], pose.t->data[1], pose.t->data[2]);
		Quaternion q;
		rotationMatToQuaternion(pose.R->data, q);
		lib_tags[i].setq(q * Quaternion(1,0,0, 3.1415926));
		lib_tagDir[i].setl(lib_tags[i].l + lib_tags[i].q.toRotate(Vector3(0,0,0.015)));
		lib_tagDir[i].setq(lib_tags[i].q);

		matd_destroy(pose.t);
		matd_destroy(pose.R);

		// solveSquare in C
		err = estimate_tag_pose_solve_square(&CameraInfo, &pose);	

		c_tags[i].setl(pose.t->data[0], pose.t->data[1], pose.t->data[2]);
		rotationMatToQuaternion(pose.R->data, q);
		c_tags[i].setq(q * Quaternion(1,0,0, 3.1415926));
		c_tagDir[i].setl(c_tags[i].l + c_tags[i].q.toRotate(Vector3(0,0,0.015)));
		c_tagDir[i].setq(c_tags[i].q);

		matd_destroy(pose.t);
		matd_destroy(pose.R);

		lib_tags[i].l -= Vector3(0.15,0,0);
		lib_tagDir[i].l -= Vector3(0.15,0,0);
		c_tags[i].l += Vector3(0.15,0,0);
		c_tagDir[i].l += Vector3(0.15,0,0);
	}

	// ----------------- opencv show ----------------------------- //
	imshow("camera1", imgRGB);
	imshow("camera2", imgRGB2);
	waitKey(1);
	return 0;
}

/* --------------- OpenGL draw functions --------------------*/
int simu_draw()
{
	for (int i = 0; i < nTags; i++)
	{
		lib_tags[i].draw();
		lib_tagDir[i].draw();

		c_tags[i].draw();
		c_tagDir[i].draw();
	}

	return 0;
}

int simu_draw2()
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
	/*
	double w, x, y, z;
	w = sqrt(1.0 + data[3*0+0] + data[3*1+1] + data[3*2+2]) / 2.0;
	double w4 = w * 4.0;
	x = (data[3*2+1] - data[3*1+2]) / w4;
	y = (data[3*0+2] - data[3*2+0]) / w4;
	z = (data[3*1+0] - data[3*0+1]) / w4;
	printf("xyzw = %lf %lf %lf %lf\n", x + y + z + w);
	q.setHardValue(x,y,z,w);
	printf("length = %lf\n", x*x + y*y + z*z + w*w);
	return 0;
	*/
	double x, y, z, w;
	double trace = data[3*0+0] + data[3*1+1] + data[3*2+2]; // I removed + 1.0f; see discussion with Ethan
	if( trace > 0 ) {// I changed M_EPSILON to 0
    	double s = 0.5f / sqrtf(trace+ 1.0f);
		w = 0.25f / s;
		x = ( data[3*2+1] - data[3*1+2] ) * s;
		y = ( data[3*0+2] - data[3*2+0] ) * s;
		z = ( data[3*1+0] - data[3*0+1] ) * s;
	} 
	else 
	{
		if ( data[3*0+0] > data[3*1+1] && data[3*0+0] > data[3*2+2] ) {
			double s = 2.0f * sqrtf( 1.0f + data[3*0+0] - data[3*1+1] - data[3*2+2]);
			w = (data[3*2+1] - data[3*1+2] ) / s;
			x = 0.25f * s;
			y = (data[3*0+1] + data[3*1+0] ) / s;
			z = (data[3*0+2] + data[3*2+0] ) / s;
		} else if (data[3*1+1] > data[3*2+2]) {
			float s = 2.0f * sqrtf( 1.0f + data[3*1+1] - data[3*0+0] - data[3*2+2]);
			w = (data[3*0+2] - data[3*2+0] ) / s;
			x = (data[3*0+1] + data[3*1+0] ) / s;
			y = 0.25f * s;
			z = (data[3*1+2] + data[3*2+1] ) / s;
		} else {
			float s = 2.0f * sqrtf( 1.0f + data[3*2+2] - data[3*0+0] - data[3*1+1] );
			w = (data[3*1+0] - data[3*0+1] ) / s;
			x = (data[3*0+2] + data[3*2+0] ) / s;
			y = (data[3*1+2] + data[3*2+1] ) / s;
			z = 0.25f * s;
		}
	}
	q.setHardValue(x,y,z,w);
	return 0;
}

