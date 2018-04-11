#pragma once
#pragma warning(disable: 4199)
// This h file defines the constants used in the whole project

// sample rate for numerous pixels, aborted now
#define SAMPLE_RATE 0.0001

#define RENDER_INTERVAL 300		// save the rendered image at certain intervals

#define HEIGHT		256			// rendered image height
#define WIDTH		256			// rendered image width


#define ENVIRONMENT_COLOR	Vector3(0.1f, 0.1f, 0.1f)		// define ambiant color which will return when no intersect or excess maximum depth

#define BOX_LENGTH	7.0f		// x max of Cornell BOX
#define BOX_WIDTH	4.0f		// y max of Cornell BOX
#define BOX_HEIGHT	4.0f		// z max of Cornell BOX

#define CAMERA_POSITION Vector3(BOX_LENGTH - 0.1f, 2.0f, 2.2f)	// define the position of camera in Cornell BOX
#define CAMERA_FORWARD	Vector3(-1.0f, 0.0f, 0.0f)				// define the forward direction of camera
#define HAMMERSLEY		false	// whether take Hammersley sample for rays in each pixel, aborted now

#define ANGLEVIEWWIDTH	53		// define the horizon angle view of rendering camera
#define ANGLEVIEWHEIGHT 53		// define the prependicular angle view of rendering camera

#define SMALL_DEPTH		true	// whether stop after MAX_DEPTH; if false, standardlize the color until DARK_DEPTH
#define MAX_DEPTH		5		// the max depth after which ray trace stops
#define DARK_DEPTH		10		// postpone the lifetime of ray by standardlize color

#define GLASS_REFRACTIVITY	1.5f	// define for refraction calculation
#define DIFFUSE_SPEC	0			// whether use diffuse way to calculate ray direction on specular surface

#define MAX_DIS		1000000.0f		// some value to calculate intersection
#define EPISILON	0.00001f		// small value for usage

#define PIDEGREE	0.017444f		// pi/180, used to change unit
#define PI			3.1415926f		// pi

#define MAX_ORDER_LENGTH	50		// the maximum command line for Ray-Tracing Debugger console