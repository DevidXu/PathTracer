#include "Debugging.h"
#include "iostream"
#include <exception>
using namespace std;

void Debugging::print(const char* c) {
	storage << c << "\n";
}

#include "Vector.h"
bool Debugging::moduleTest() {
	try {
		Vector3 a(1.0, 2.0, 4.0);
		Vector3 b(2.0, 3.5, 0.2);
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
	finally: {
		cout << "Vector test finished" << endl;
	}

	return true;
}
