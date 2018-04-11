#include "Debugging.h"
#include "iostream"
#include "Triangle.h"
#include "Ray.h"
#include <exception>
using namespace std;

#define MAX_MODULENAME 15

Debugging::Debugging() {
	storage.open(LOGFILE);
}


Debugging::~Debugging() {
}


void Debugging::print(string c) {
	storage << c << "\n";
}


bool Debugging::moduleTest() {
	try {
		Vector3 a(1.0f, 2.0f, 4.0f);
		Vector3 b(2.0f, 3.5f, 0.2f);
		cout << a << " " << b << endl;
		Vector3 c = a + b;
		cout << "a+b " << c << endl;
		cout << "a==b " << (a == b) << endl;
		cout << "a*b " << a * b << endl;
	}
	catch (exception e) {
		LOGPRINT("Error occurs in Vector operation!");
		return false;
	} 


	try {
		Vector3 v0(0.1f, 0.0f, 0.0f), v1(0.0f, 0.1f, 0.0f), v2(0.0f, 0.0f, 0.1f);
		Triangle tri(&v0,&v1,&v2);
		Vector3 start(0.0f, -0.01f, -0.01f), dir(1.0f, 1.0f, 1.0f);
		dir = dir.normalize();
		shared_ptr<Ray> ray = make_shared<Ray>(start, dir); 
		shared_ptr<Vector3> hitpoint = make_shared<Vector3>();
		cout << "V0: " << v0 << ";   V1: " << v1 << ";   V2: " << v2 << endl;
		cout << "Ray origin: " << ray->getOrigin() << ";    Ray direction: " << ray->getDirection() << endl;
		cout << "Hit result: ";
		if (tri.hitRay(ray, hitpoint)) cout << *hitpoint << endl;
	}
	catch (exception e) {
		LOGPRINT("Error occurs in Ray-Triangle intersection detection!");
		return false;
	}

	return true;
}


void Debugging::timeCountStart() {
	// calculate the time needed
	start = time(NULL);
}


void Debugging::timeCountEnd(){
	end = time(NULL);
	LOGPRINT("Total time consumed:" + to_string(int((float(end - start))*1.0f)) + "s\n");
}


void Debugging::finishRender() {
	timeCountEnd();
	setProgress(100.0f);
	showTiming();
	storage.close();
	cout << "Rendering process has been completed!" << endl;
	cout << "Console: ";
}


void Debugging::setProgress(float current_progress) {
	if (current_progress - progress > 1) {
		progress = current_progress;
	}
}


void Debugging::showProgress() {
	if (progress == 0.0f) {
		cout << "Rendering Progress: 0%" << endl;
		cout << "Estimated Left Time: Unknown." << endl;
		return;
	}

	cout << "Rendering Progress: " << setiosflags(ios::fixed) << setprecision(0) << progress << "%" << endl;
	current = time(NULL);
	int esti_time = int((float(current - start))*1.0f / progress * (100 - progress));

	// standardlize the output
	cout << "Estimated Left Time: ";
	if (esti_time >= 3600) cout << esti_time / 3600 << "h";
	esti_time %= 3600;
	if (esti_time >= 60) cout << esti_time / 60 << "m";
	esti_time %= 60;
	if (esti_time > 0) cout << esti_time << "s";
	cout << endl;

	return;
}


void Debugging::recordColor(int i, int j, Vector3* color) {
#ifndef DEBUG
	return;
#endif
	if (!sample) return;

	stringstream stream;
	stream << "Color (" << i << ", " << j << "): (" << setprecision(2)<<color->value[0] << ", "
		<< color->value[1] << ", " << color->value[2] << ")";
	LOGPRINT(stream.str().c_str());
	return;
}


void Debugging::setSample(int i, int j, bool s) {
#ifndef DEBUG
	return;
#endif
	sample = s;
	if (!s) return;
	stringstream stream;
	stream << "Sample one ray of Pixel: (" << i << ", " << j << ").";
	LOGPRINT("");
	LOGPRINT(stream.str().c_str());
	return;
}


void Debugging::recordPath(string name, Vector3* hitPoint) {
#ifndef DEBUG
	return;
#endif
	if (!sample) return;
	stringstream stream;
	char c[15];
	strcpy_s(c, name.c_str());
	stream << "Hit object: " << c << "; Hit point: (" << setprecision(2) << hitPoint->value[0]
		<< ", " << hitPoint->value[1] << ", " << hitPoint->value[2] << ").";
	LOGPRINT(stream.str().c_str());
	return;
}


// used to monitor the time consumed by each module;
// work by adding the name into the list of the monitored modules and measure the
// difference between the start time and finish time
void Debugging::timing(string moduleName, bool start) {
#ifndef DEBUG
	return; // this function will consume much time (almost 40% increase!)
#endif

	map<string, float>::iterator iter;
	iter = time_map.find(moduleName);
	if (iter == time_map.end()) time_map[moduleName] = 0.0f;

	if (start) {
		timing_map[moduleName] = clock();
	}
	else {
		time_map[moduleName] += float(clock() - timing_map[moduleName]) / CLOCKS_PER_SEC;
	}

	return;
}


void Debugging::showTiming() {
#ifndef DEBUG
	return;
#endif
	cout << "Timing for each module: " << endl;

	for (map<string, float>::iterator iter = time_map.begin(); iter != time_map.end();iter++) {
		cout << "Time consumed for ";
		cout.width(MAX_MODULENAME); cout << iter->first;
		cout << ": " << int(iter->second) << "s"<<endl;
	}
}


bool Debugging::renderInterval(time_t current_time) {
	static time_t last_time = time(NULL);
	if (current_time - last_time > RENDER_INTERVAL) {
		last_time = current_time;
		return true;
	}

	return false;
}