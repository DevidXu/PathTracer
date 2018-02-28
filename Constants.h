#pragma once
#pragma warning(disable: 4199)
// This h file defines the constants used in the whole project
/*
These variables cannot be changed
*/

//#define DEBUG		1

#define SAMPLE_RATE 0.0001

#define HEIGHT		256
#define WIDTH		256

#define SAMPLE_NUM		400  // must be times of 4

//#define TESSELLATE		1

#define ENVIRONMENT_COLOR	Vector3(0.05f, 0.05f, 0.05f)

#define BOX_LENGTH	7.0f  // x max
#define BOX_WIDTH	4.0f  // y max
#define BOX_HEIGHT	4.0f  // z max

#define CAMERA_POSITION Vector3(BOX_LENGTH - 0.1f, 2.0f, 2.2f)
#define CAMERA_FORWARD	Vector3(-1.0f, 0.0f, 0.0f)

#define ANGLEVIEWWIDTH	53
#define ANGLEVIEWHEIGHT 53

#define MAX_DEPTH		5
#define DARK_DEPTH		10

#define GLASS_REFRACTIVITY	1.5f

#define MAX_DIS		1000000.0f
#define EPISILON	0.00001f

#define PIDEGREE	0.017444f
#define PI			3.1415926f

#define GLOBAL