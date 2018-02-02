#include "Debugging.h"
#include "iostream"
#include <exception>
using namespace std;


void Debugging::print(const char* c) {
	storage << c << "\n";
}


#include "Vector.h"
#include "Triangle.h"
#include "Ray.h"
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
