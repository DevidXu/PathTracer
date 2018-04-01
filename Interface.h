#pragma once

// This file runs under multiple threads. It acts to provide command line interaction with 
// users. It allows users to monitor the progress of rendering. Detect the path of each ray
// or pause and resume the rendering progress. It also contains the Mat of origin map, 
// normal map, depth map and debug map(depthMap can be used to store 2D debug information).
// Most functions of this file depend on OpenCV. You need to ensure OpenCV is setup properly
// on your computer for release/debug mode + x64 compiler.

#include "Singleton.h"
#include "Debugging.h"
#include "Constants.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;

enum ImageType { ORIGIN, NORMAL, DEPTH, DEBUG };


class Interface {
public:
	// update the color and depth and normal information of the mat class
	void updateData(Vector3 color, Vector3 normal, float depth, int i, int j);

	// update all the images with information and pixels that have been rendered
	void updateDebugImage();

	// finish the rendering; standard depth map and save images.
	void finishRender();

	// execute at the beginning
	void initialize();

	// present four images on the same page
	void showImages();

	// call at the beginning to deal with console and images
	bool beginMessageLoop();

	// read message from the console and process it
	bool translateMessage(string s);

	Interface();
	virtual ~Interface();


private:
	Mat *depthMap, *normalMap, *image, *debugMap;

	void showText(Mat* image, CvPoint point, string s);
};