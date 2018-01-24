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
		Vector2 a(1.0, 2.0);
		Vector2 b(2.0, 3.5);
		cout << a << " " << b << endl;
		Vector2 c = a + b;
		cout << "a+b " << (a+b) << endl;
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
