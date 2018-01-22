#pragma once

// This file is used for debugging. It will store all the debugging information to a
// local file.

#include "Singleton.h"
#include <fstream>

using namespace std;

class Debugging :public Singleton<Debugging> {
private:
	string LOGFILE = "logging.txt";
	ofstream storage;
	Debugging() {
		storage.open(LOGFILE);
	}
	~Debugging() {
		storage.close();
	}

public:
	void print (const char* c) {
		storage << c;
	}
};