#include "Debugging.h"
#include "iostream"
#include "Triangle.h"
#include "Ray.h"
#include <exception>
using namespace std;


Debugging::Debugging() {
	storage.open(LOGFILE);
}


Debugging::~Debugging() {
	storage.close();
}


void Debugging::print(const char* c) {
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

	system("cls");
	cout << "Rendering Progress: 0%" << endl;
	cout << "Estimated Left Time: 10h" << endl;
}


void Debugging::timeCountEnd(){	
	system("cls");
	cout << "Rendering Progress: 100%" << endl;
	cout << "Image has been rendered! Check the local image." << endl; 
}


void Debugging::showProgress(float progress) {
	static float last_progress = 0.0f;

	if (progress - last_progress > 1) {
		last_progress = progress;
		system("cls");
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
	}

	return;
}


void Debugging::recordColor(int i, int j, Vector3* color) {
	if (!sample) return;

	stringstream stream;
	stream << "Color (" << i << ", " << j << "): (" << setprecision(2)<<color->value[0] << ", "
		<< color->value[1] << ", " << color->value[2] << ")";
	LOGPRINT(stream.str().c_str());
	return;
}


void Debugging::setSample(int i, int j, bool s) {
	sample = s;
	if (!s) return;
	stringstream stream;
	stream << "Sample one ray of Pixel: (" << i << ", " << j << ").";
	LOGPRINT("");
	LOGPRINT(stream.str().c_str());
	return;
}


void Debugging::recordPath(string name, Vector3* hitPoint) {
	if (!sample) return;
	stringstream stream;
	char c[15];
	strcpy_s(c, name.c_str());
	stream << "Hit object: " << c << "; Hit point: (" << setprecision(2) << hitPoint->value[0]
		<< ", " << hitPoint->value[1] << ", " << hitPoint->value[2] << ").";
	LOGPRINT(stream.str().c_str());
	return;
}