#include "Interface.h"
#include "World.h"
#include <omp.h>

#define WINDOW_NAME "Ray-Trace Debugger"
#define SAVE_PATH "./results"
#define DISTANCE_PARA	1.0f
#define SMOOTH_CONSTANT 10000.0f
#define VARIANCE_SCALE	255.0f // when color enlarge x times, we should rescale the variance to x^2 times
static const char* EnumStrings[] = { "Origin Image", "Normal Map", "Depth Map", "Debug Map" };


Interface::Interface() {
	// create window and set mouse callback to respond event
	cvNamedWindow(WINDOW_NAME, WINDOW_NORMAL);
	cvResizeWindow(WINDOW_NAME, WIDTH * 2, HEIGHT * 2);

	// create CV Mat for rendering and adjust each pixel easily
	image = new Mat(WIDTH, HEIGHT, CV_8UC3, Scalar(0, 0, 0));
	normalMap = new Mat(WIDTH, HEIGHT, CV_8UC3, Scalar(0, 0, 0));
	depthMap = new Mat(WIDTH, HEIGHT, CV_8UC3, Scalar(0, 0, 0)); // grey map
	varianceMap = new Mat(WIDTH, HEIGHT, CV_8UC3, Scalar(0, 0, 0));
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
	delete varianceMap;
	cvDestroyAllWindows();
}


// Update the value of single pixel of those mats
void Interface::updateData(Vector3 color, Vector3 normal, Vector3 variance, float depth, int i, int j) {
	color = Vector3(color.value[2], color.value[1], color.value[0]);
	variance = Vector3(variance.value[2], variance.value[1], variance.value[0]);
	normal = Vector3(normal.value[2], normal.value[1], normal.value[0]);
	for (int k = 0; k < 3; k++) {
		image->at<Vec3b>(i, j)[k] = int(color.value[k] * 255);
		normalMap->at<Vec3b>(i, j)[k] = int((normal.value[k]*0.5+0.5) * 255);
		depthMap->at<Vec3b>(i, j)[k] = int(depth * 255 / BOX_LENGTH);
		varianceMap->at<Vec3b>(i, j)[k] = int(variance.value[k]*255);
	}
}


// called when the rendering is finished to ensure image are shown correctly
void Interface::finishRender() {
	normalize(*depthMap, *depthMap, 230.0, 1.0, NORM_MINMAX);
	//NLMeansProcess(5, 3);

	showImages();

	fstream _file;
	_file.open(SAVE_PATH, ios::in);
	try {
		if (!_file)
			_ASSERT(false);
	}
	catch (exception e) {
		LOGPRINT(e.what());
		return;
	}

	string name;
	name = "/image.bmp";
	imwrite(SAVE_PATH+name, *image);
	name = "/depth.bmp";
	imwrite(SAVE_PATH + name, *depthMap);
	name = "/normal.bmp";
	imwrite(SAVE_PATH + name, *normalMap);
	name = "/debug.bmp";
	imwrite(SAVE_PATH + name, *debugMap);

	return;
}


// more process about debug image and other image can be added here
void Interface::updateDebugImage(Vector3 color, int i, int j) {

	return;
}


// don't call it frequently; it will consume some resource
void Interface::showImages() {
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
	cvWaitKey(50);	// a must for imshow to function properly
}

// rt means ray tracer; this defines the supported orders enum value
enum ORDERS {
	rt_start,
	rt_pause,
	rt_exit, 
	rt_intro, 
	rt_draw, 
	rt_render,  
	rt_progress,
	rt_nlmeans,
	rt_range
};

// this defines the supported command line orders
static const char* ORDERS_STRING[] = {
	"start",
	"pause",
	"exit", 
	"intro",
	"draw", 
	"render", 
	"progress",
	"nlmeans",
	"range",
};

// this complete intro order, it will give an introduction to each command line order
static const char* ORDERS_INTRO[] = {
	"Start the rendering and resume the paused process",
	"Pause the rendering process (at ray level)",
	"Quit the rendering process",
	"An introduction to all the orders and requested parameters",
	"Draw the origin map, depth map, normal map and debug map",
	"Render specified pixel; parameter: PixelW_i, PixelH_j, Sample_Num (Num should be multiple of 4)",
	"Show the rendering progress of whole image",
	"Only called after the rendering process is finished; apply nlmeans to denoise the image",
	"Set the rendering area of interest; para: height_begin, height_end, width_begin, width_end"
};


// process incoming messages; when the process start, the rendering loop will be stopped to
// ensure the efficiency of translating messages like render, draw.
// After the message is process, the rendering process will return to the original state
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

	string name;
	Mat denoise(HEIGHT, WIDTH, CV_8UC3, cvScalar(0, 0, 0));

	switch (o) {
	case rt_start:
		World::getInstance()->rd_pause = false;
		cout << "Rendering process has started. It might affect the fluency of your computer.\n";
		break;

	case rt_pause:
		World::getInstance()->rd_pause = true;
		cout << "Rendering process has been paused.\n";
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
	
	case rt_nlmeans:
		cv::fastNlMeansDenoisingColored(*image, denoise, 7.0f, 3.0f, 7, 21);
		name = "/denoise.bmp";
		denoise.copyTo(*debugMap);
		showImages();
		imwrite(SAVE_PATH + name, denoise);
		World::getInstance()->rd_pause = pauseState;
		cout << "Image denoised by NL Means is shown in debug window and saved locally.\n";
		break;

	case rt_range:
		if (World::getInstance()->rd_begin) 
			cout << "Rendering process has began. Cannot reset rendering range\n";
		else {
			World::getInstance()->height_begin = int(atof(wordList[1].c_str())*HEIGHT);
			World::getInstance()->height_end = int(atof(wordList[2].c_str())*HEIGHT);
			World::getInstance()->width_begin = int(atof(wordList[3].c_str())*WIDTH);
			World::getInstance()->width_end = int(atof(wordList[4].c_str())*WIDTH);
			cout << "Rendering is reset. Please enter start the rendering process.\n";
		}
		break;

	default: // should have been processed before
		_ASSERT(false);
		break;

	}

	return true;

}


// serve as the main loop of the interactive window
bool Interface::beginMessageLoop() {
	cout << "This is ray tracer completed by Dewei Xu, @Shanghai Jiao Tong Univ. The file can only be used for educational purpose. All rights are reserved by Dewei." << endl;
	cout << "Default Rendering Area: \n" << "    Height: 0 to " << HEIGHT << "\n    Width: 0 to " << WIDTH << endl;
	cout << "Entre start to begin the ray tracing process." << endl;
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


// show text on specified position on the images
void Interface::showText(Mat* image, CvPoint point, string s) {
	putText(*image, s, point, CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255));
}


Vector3 Interface::calculateDistance(CvPoint d, CvPoint o) {
	Vector3 temp;
	CvPoint t(o.x + d.x, o.y + d.y);
	if (t.x >= 0 && t.x < HEIGHT && t.y >= 0 && t.y < WIDTH); else t = o;
	for (int i = 0; i < 3; i++) {
		temp.value[i] = (float)pow(image->at<Vec3b>(o)[i] - image->at<Vec3b>(t)[i],2);
		temp.value[i] = (temp.value[i] - (varianceMap->at<Vec3b>(o)[i] + min(varianceMap->at<Vec3b>(t)[i], varianceMap->at<Vec3b>(o)[i]))*VARIANCE_SCALE);
		temp.value[i] /= (EPISILON + 2 * DISTANCE_PARA*DISTANCE_PARA* varianceMap->at<Vec3b>(o)[i]*VARIANCE_SCALE);
	}

	return temp;
}


// easy version of NLMeans process
Vector3 vecToVec(Vec3b v) {
	return Vector3(v[0], v[1], v[2]);
}

Vec3b VecTovec(Vector3 v) {
	return Vec3b((int)v.value[0], (int)v.value[1], (int)v.value[2]);
}

inline Vec3b validMatPos(Mat* mat, int x, int y) {
	if (x >= 0 && x < mat->rows&&y >= 0 && y < mat->cols)
		return mat->at<Vec3b>(x, y);
	else
		return Vec3b(0, 0, 0);
}

void Interface::NLMeansProcessOrigin(int Ds, int ds) {
	Mat *nlMeanMap = new Mat(WIDTH, HEIGHT, CV_8UC3, Scalar(0, 0, 0));

	for (int i=0;i<HEIGHT;i++)
		for (int j = 0; j < WIDTH; j++) {
			Vector3 color, weightSum;
			for (int k1=i-Ds;k1<=i+Ds;k1++)
				for (int k2 = j-Ds; k2 <= j+Ds; k2++) {
					Vector3 disSum;
					int validPoint = 0;
					for (int m1 = -ds; m1 <= ds; m1++)
						for (int m2 = -ds; m2 <= ds; m2++) {
							Vec3b color1 = validMatPos(image, k1 + m1, k2 + m2);
							Vec3b color2 = validMatPos(image, i + m1, j + m2);
							Vector3 tc1 = vecToVec(color1), tc2 = vecToVec(color2);
							Vector3 dis;
							for (int k = 0; k < 3; k++) {
								int varianceDiff = validMatPos(varianceMap, i + m1, j + m2)[k] + min(validMatPos(varianceMap, k1 + m1, k2 + m2)[k], validMatPos(varianceMap, i + m1, j + m2)[k]);
								int variance = validMatPos(varianceMap, i + m1, j + m2)[k];
								if (variance!=0)
									dis.value[k] = ((tc1[k] - tc2[k])*(tc1[k] - tc2[k])-varianceDiff*VARIANCE_SCALE) / (EPISILON + 2 * DISTANCE_PARA*DISTANCE_PARA*variance*VARIANCE_SCALE);
							}
							disSum += dis;
						}
					disSum = disSum / (float)(pow(ds * 2 + 1, 2));
					Vector3 weight = Vector3(exp(-disSum.value[0]), exp(-disSum.value[1]), exp(-disSum.value[2]));
					weightSum += weight;
					color += weight * vecToVec(validMatPos(image, k1, k2));
				}
			for (int k = 0; k < 3; k++)
				color.value[k] = color[k] / weightSum[k];
			nlMeanMap->at<Vec3b>(i, j) = VecTovec(color);
		}

	normalize(*nlMeanMap, *debugMap, 255.0, 1.0, NORM_MINMAX);
	delete nlMeanMap;
}


// Ds, ds must be odd
void Interface::NLMeansProcess(int Ds, int ds) {

	Mat *nlMeanMap = new Mat(WIDTH, HEIGHT, CV_8UC3, Scalar(0, 0, 0));
	Vector3** SW = new Vector3*[HEIGHT];
	Vector3** St = new Vector3*[HEIGHT];
	for (int i = 0; i < HEIGHT; i++) {
		SW[i] = new Vector3[WIDTH];
		St[i] = new Vector3[WIDTH];
	}

#pragma omp parallel for schedule(dynamic, 1)
	for (int t1=-Ds;t1<=Ds;t1++)
		for (int t2 = -Ds; t2 <= Ds; t2++) {
			St[0][0] = Vector3();
			for (int x1 = 1; x1 < HEIGHT; x1++)
				St[x1][0] = St[x1 - 1][0] + calculateDistance(CvPoint(t1, t2), CvPoint(x1, 0));
			for (int x2 = 1; x2 < WIDTH; x2++)
				St[0][x2] = St[0][x2 - 1] + calculateDistance(CvPoint(t1, t2), CvPoint(0, x2));
			for (int x1 = 1; x1 < HEIGHT; x1++)
				for (int x2 = 1; x2 < WIDTH; x2++)
					St[x1][x2] = calculateDistance(CvPoint(t1, t2), CvPoint(x1, x2)) + St[x1 - 1][x2] + St[x1][x2 - 1] - St[x1 - 1][x2 - 1];
			for (int x1 = 0; x1 < HEIGHT; x1++)
				for (int x2 = 0; x2 < WIDTH; x2++) {
					CvPoint y(x1 + t1, x2 + t2);
					Vector3 dis =
						(x1 + ds < HEIGHT&&x2 + ds < WIDTH ? St[x1 + ds][x2 + ds] : Vector3()) +
						(x1 >= ds && x2 >= ds ? St[x1 - ds][x2 - ds] : Vector3()) -
						(x1 + ds < HEIGHT&&x2 >= ds ? St[x1 + ds][x2 - ds] : Vector3()) -
						(x1 >= ds && x2 + ds < WIDTH ? St[x1 - ds][x2 + ds] : Vector3());

					dis = Vector3(sqrt(dis.value[0]), sqrt(dis.value[1]), sqrt(dis.value[2]));
					dis = dis / (float)((2 * ds + 1)*SMOOTH_CONSTANT);
					Vector3 w = Vector3(exp(-dis.value[0]), exp(-dis.value[1]), exp(-dis.value[2]));
					SW[x1][x2] += w;
					if (y.x >= 0 && y.x < HEIGHT&&y.y >= 0 && y.y < WIDTH); else y = CvPoint(x1, x2);
					for (int k = 0; k < 3; k++)
#pragma omp critical
						nlMeanMap->at<Vec3b>(x1, x2)[k] += (int)w[k] * image->at<Vec3b>(y.x,y.y)[k];
				}
		}

	for (int x1 = HEIGHT - 1; x1 >= 0; x1--)
		for (int x2 = WIDTH - 1; x2 >= 0; x2--)
			for (int k = 0; k < 3; k++)
				nlMeanMap->at<Vec3b>(x1, x2)[k] = int(nlMeanMap->at<Vec3b>(x1, x2)[k] * 1.0f / SW[x1][x2][k]);


	normalize(*nlMeanMap, *debugMap, 255.0, 1.0, NORM_MINMAX);

	delete nlMeanMap;

	for (int i = 0; i < HEIGHT; i++) {
		delete[] SW[i];
		delete[] St[i];
	}
	delete[] SW;
	delete[] St;
}