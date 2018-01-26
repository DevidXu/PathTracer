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

	return true;
}
