#include "Vector.h"

ostream &operator<<(ostream& out, const Vector2 &v) {
	for (int i = 0; i < 2; i++)
		cout << v.value[i] << " ";
	return out;
}


ostream &operator<<(ostream& out, const Vector3 &v) {
	for (int i = 0; i < 3; i++)
		cout << v.value[i] << " ";
	return out;
}


ostream &operator<<(ostream& out, const Vector4 &v) {
	for (int i = 0; i < 4; i++)
		cout << v.value[i] << " ";
	return out;
}