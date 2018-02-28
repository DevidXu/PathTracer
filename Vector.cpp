#include "Vector.h"


float MAX(float a, float b) {
	float r = a > b ? a : b;
	return r;
}


ostream &operator<<(ostream& out, const Vector2 &v) {
	if (&v == nullptr) {
		cout << "None ";
		return out;
	}
	for (int i = 0; i < 2; i++)
		cout << v.value[i] << " ";
	return out;
}


ostream &operator<<(ostream& out, const Vector3 &v) {
	if (&v == nullptr) {
		cout << "None ";
		return out;
	}
	for (int i = 0; i < 3; i++)
		cout << v.value[i] << " ";
	return out;
}


ostream &operator<<(ostream& out, const Vector4 &v) {
	if (&v == nullptr) {
		cout << "None ";
		return out;
	}
	for (int i = 0; i < 4; i++)
		cout << v.value[i] << " ";
	return out;
}