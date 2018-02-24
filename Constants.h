#pragma once

// This h file defines the constants used in the whole project
/*
These variables cannot be changed
*/

#define UNCHANGE	1

#define HEIGHT		64
#define WIDTH		64

#define BOX_LENGTH	5.0  // x max
#define BOX_WIDTH	4.0  // y max
#define BOX_HEIGHT	4.0  // z max

#define CAMERA_POSITION Vector3(4.0f, 2.0f, 2.5f)
#define CAMERA_FORWARD Vector3(-1.0f, 0.0f, 0.0f)

#define ANGLEVIEWWIDTH 120
#define ANGLEVIEWHEIGHT 120

#define SAMPLE_NUM 16

#define MAX_DEPTH 5

#define ENVIRONMENT_COLOR Vector3(0.0f, 0.0f, 0.0f)

#define GLASS_REFRACTIVITY 1.5f

#define MAX_DIS 1000000.0f
#define EPISILON 0.00001f

#define PIDEGREE 0.017444f
#define PI 3.1415926f


// DEBUG DEFINES

//#define TESSELLATE
#define GLOBAL