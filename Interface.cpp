#include "Interface.h"
#include "World.h"
#define WINDOW_NAME "Ray-Trace Debugger"

static const char* EnumStrings[] = { "Origin Image", "Normal Map", "Depth Map", "Debug Map" };


Interface::Interface() {
	// create window and set mouse callback to respond event
	cvNamedWindow(WINDOW_NAME, WINDOW_NORMAL);
	cvResizeWindow(WINDOW_NAME, WIDTH * 2, HEIGHT * 2);

	image = new Mat(WIDTH, HEIGHT, CV_8UC3, Scalar(0, 0, 0));
	normalMap = new Mat(WIDTH, HEIGHT, CV_8UC3, Scalar(0, 0, 0));
	depthMap = new Mat(WIDTH, HEIGHT, CV_8UC3, Scalar(0, 0, 0)); // grey map
	debugMap = new Mat(WIDTH, HEIGHT, CV_8UC3, Scalar(0, 0, 0));

}


void Interface::initialize() {
	// print log information
	LOGPRINT("The ray tracing debugger has been started");
}


Interface::~Interface() {
	delete image;
	delete normalMap;
	delete depthMap;
	delete debugMap;
	cvDestroyAllWindows();
}


void Interface::updateData(Vector3 color, Vector3 normal, float depth, int i, int j) {
	color = Vector3(color.value[2], color.value[1], color.value[0]);
	normal = Vector3(normal.value[2], normal.value[1], normal.value[0]);
	for (int k = 0; k < 3; k++) {
		image->at<Vec3b>(i, j)[k] = int(color.value[k] * 255);
		normalMap->at<Vec3b>(i, j)[k] = int((normal.value[k]*0.5+0.5) * 255);
		depthMap->at<Vec3b>(i, j)[k] = int(depth * 255 / BOX_LENGTH);
	}
}


void Interface::finishRender() {
	normalize(*depthMap, *depthMap, 255.0, 1.0, NORM_MINMAX);
	showImages();

	fstream _file;
	_file.open("./results", ios::in);
	try {
		if (!_file)
			_ASSERT(false);
	}
	catch (exception e) {
		LOGPRINT(e.what());
		return;
	}

	imwrite("./results/image.bmp", *image);
	imwrite("./results/depth.bmp", *depthMap);
	imwrite("./results/normal.bmp", *normalMap);
	imwrite("./results/debug.bmp", *debugMap);

	return;
}


// more process about debug image and other image can be added here
void Interface::updateDebugImage() {
	showImages();

	return;
}


// don't call it frequently; it will consume some resource
void Interface::showImages() {
	//normalize(*depthMap, *depthMap, 255.0, 1.0, NORM_MINMAX);

	Mat mergedImage, upline, downline;
	hconcat(*image, *depthMap, upline);
	hconcat(*normalMap, *debugMap, downline);
	vconcat(upline, downline, mergedImage);
	for (int i=0;i<2;i++)
		for (int j = 0; j < 2; j++) {
			showText(
				&mergedImage, 
				CvPoint(int((i+0.5)*WIDTH-30), int((j+0.1)*HEIGHT)), 
				EnumStrings[i * 2 + j]
			);
		}
	imshow(WINDOW_NAME, mergedImage);
	cvWaitKey(50);
}

// rt means ray tracer
enum ORDERS {
	rt_start,
	rt_pause,
	rt_exit, 
	rt_intro, 
	rt_draw, 
	rt_render,  
	rt_progress
};

static const char* ORDERS_STRING[] = {
	"start",
	"pause",
	"exit", 
	"intro",
	"draw", 
	"render", 
	"progress"
};

static const char* ORDERS_INTRO[] = {
	"Start the rendering and resume the paused process",
	"Pause the rendering process (at ray level)",
	"Quit the rendering process",
	"An introduction to all the orders and requested parameters",
	"Draw the origin map, depth map, normal map and debug map",
	"Render specified pixel; parameter: PixelW_i, PixelH_j, Sample_Num (Num should be multiple of 4)",
	"Show the rendering progress of whole image"
};

bool Interface::translateMessage(string s) {
	string word;
	stringstream ss(s);
	vector<string> wordList;
	while (ss >> word) wordList.push_back(word);
	ORDERS o;
	
	// if empty, return directly
	int listLen = int(wordList.size());
	if (listLen == 0) return true;

	// if the first word is supported?
	int k = 0; bool find = false;
	for (auto& order : ORDERS_STRING) {
		if (strcmp(order, wordList[0].c_str()) == 0) {
			o = ORDERS(k);
			find = true;
		}
		k++;
	}
	if (!find) {
		cout << "The order is not supported!" << endl;
		return true;
	}

	int numOrder = 0, i = 0, j = 0, num = 0;
	bool pauseState = World::getInstance()->rd_pause;
	World::getInstance()->rd_pause = true;

	switch (o) {
	case rt_start:
		World::getInstance()->rd_pause = false;
		break;

	case rt_pause:
		World::getInstance()->rd_pause = true;
		break;
	
	case rt_exit:
		if (!World::getInstance()->rd_exit) {
			cout << "The rendering process has not been completed. Are you sure you want to exit? [y/n] ";
			string answer; cin >> answer;
			if (strcmp("y", answer.c_str()) == 0) {
				World::getInstance()->rd_exit = true;
				cout << "Rendering process terminated." << endl;
				return false;
			}
			else {
				World::getInstance()->rd_pause = false;
				return true;
			}
		}
		return false;
		break;
	
	case rt_intro:
		cout << "Supported Orders: \n";
		for (auto& s : ORDERS_STRING) {
			cout << "    " << left << setw(8) << ORDERS_STRING[numOrder] << ": " << ORDERS_INTRO[numOrder] << endl;
			numOrder += 1;
		}
		World::getInstance()->rd_pause = pauseState;
		break;

	case rt_draw:
		showImages();
		cout << "Images are shown on the window." << endl;
		World::getInstance()->rd_pause = pauseState;
		break;
	
	case rt_render:
		World::getInstance()->rd_pause = true;
		i = atoi(wordList[1].c_str()), j = atoi(wordList[2].c_str());
		num = atoi(wordList[3].c_str());
		World::getInstance()->renderPixel(i, j, num, true);
		cvWaitKey(500);
		World::getInstance()->rd_pause = pauseState;
		break;

	case rt_progress:
		Debug->showProgress();
		World::getInstance()->rd_pause = pauseState;
		break;
	
	default: // should have been processed before
		_ASSERT(false);
		break;

	}

	return true;

}


bool Interface::beginMessageLoop() {
	cout << "This is ray tracer completed by Dewei Xu, @Shanghai Jiao Tong Univ. The file can only be used for educational purpose. All rights are reserved by Dewei." << endl;
	bool loopBegin = false;
	while (true) {
		// after certain time, render the image temporarily
		if (Debug->renderInterval(time(NULL))) {
			World::getInstance()->camera->drawScene();
			World::getInstance()->winFace->showImages();
			loopBegin = true;
		}

		cout << "Console: ";
		char order[MAX_ORDER_LENGTH];
		cin.getline(order, MAX_ORDER_LENGTH);
		if (!translateMessage(order)) return false;
		cvWaitKey(200);
	}

	return true;
}


void Interface::showText(Mat* image, CvPoint point, string s) {
	putText(*image, s, point, CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255));
}