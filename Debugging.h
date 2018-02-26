#pragma once

// This file is used for debugging. It will store all the debugging information to a
// local file.

#include "Singleton.h"
#include "Vector.h"
#include <map>
#include <time.h>
#include <fstream>
#include <sstream>
#include <iomanip>

#define LOGPRINT(x) Debug->print(x)

#define Debug		Debugging::getInstance()

using namespace std;

class Debugging :public Singleton<Debugging> {
private:
	string LOGFILE = "logging.txt";
	ofstream storage;

	// used to calculate progress
	time_t start, current;

	// calculate time consumed for each module
	map<string, clock_t> timing_map;
	map<string, float> time_map;

	// decide whether sample
	bool sample;

	Debugging();

public:
	friend Singleton <Debugging>;
	~Debugging();

	void print(const char* c);

	void setSample(int i, int j, bool s);

	// used to calculate the time needed
	void timeCountStart();
	void timeCountEnd();
	void showProgress(float progress);

	// show color for each pixel
	void recordColor(int i, int j, Vector3* color);

	// show the path for the ray
	void recordPath(string name, Vector3* hitPoint);

	void timing(string moduleName, bool start);
	void showTiming();

	bool moduleTest();
};
