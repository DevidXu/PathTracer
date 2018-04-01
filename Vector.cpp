#include "Vector.h"


float MAX(float a, float b) {
	float r = a > b ? a : b;
	return r;
}


ostream &operator<<(ostream& out, const Vector2 v) {
	if (&v == nullptr) {
		cout << "None ";
		return out;
	}
	out << "[" << setprecision(3) << v.value[0] << " " << setprecision(3) << v.value[1] << "]";
	return out;
}


ostream &operator<<(ostream& out, const Vector3 v) {
	if (&v == nullptr) {
		cout << "None ";
		return out;
	}
	out << "[";
	for (int i = 0; i < 2; i++)
		out << setprecision(3) << v.value[i] << " ";
	out << setprecision(3) << v.value[2] << "] ";
	return out;
}


ostream &operator<<(ostream& out, const Vector4 v) {
	if (&v == nullptr) {
		cout << "None ";
		return out;
	}
	out << "[";
	for (int i = 0; i < 3; i++)
		out << setprecision(3) << v.value[i] << " ";
	out << setprecision(3) << v.value[3] << "] ";
	return out;
}