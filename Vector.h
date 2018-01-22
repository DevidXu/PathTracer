#pragma once
// This file defines all the necessary Vector operations
#include <assert.h>
#include "iostream"
using namespace std;

template <int n>
class Vector {
private:
	float v[n];
public:
	Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) {
		float arr[] = {x, y, z, w}
		for (int i = 0; i < n; i++)
			v[i] = arr[i];
	}

	Vector(const Vector v) {
		assert(typeid(*this) == typeid(v));
		for (int i = 0; i < n; i++)
			v[i] = v.v[i];
	}

	float &operator[](const int k) const {
		return v[k];
	}

	ostream &operator<<(ostream& out, const Vector v) {
		for (int i = 0; i < n; i++)
			out << v[i] << " ";
		return out;
	}
	
	Vector &operator+(const Vector v) {
		Vector temp;
		for (int i = 0; i < n; i++)
			temp.v[i] = v[i] + v.v[i];
		return temp;
	}
	
	Vector &operator-(const Vector v) {
		Vector temp;
		for (int i = 0; i < n; i++)
			temp.v[i] = v[i] - v.v[i];
		return temp;
	}

	Vector &operator=(const Vector v) {
		assert(typeid(*this) == typeid(v));
		for (int i = 0; i < n; i++)
			v[i] = v.v[i];
	}

	float dot(const Vector v) {
		float k = 0.0f;
		for (int i = 0; i < n; i++)
			k += v[i] * v.v[i];
		return k;
	}

	float magnitude() {
		float temp = 0.0f;
		for (int i = 0; i < n; i++)
			temp += v[i] * v[i];
		return sqrt(temp);
	}

	float distance(const Vector v) {
		Vector temp = (*this) - v;
		return temp.magnitude();
	}

	virtual float &operator*();
};


class Vector2: public Vector<2> {
	// override some functions
	virtual float &operator*(const Vector2 v) {
		float temp;
		temp = (*this)[0] * v[1] - (*this)[1] * v[0];
		return temp;
	}

};

class Vector3 : public Vector<3> {
	// override some functions
	virtual float &operator*(const Vector3 v) {
		float temp0, temp1, temp2;
		temp0 = (*this)[1] * v[2] - (*this)[2] * v[1];
		temp1 = (*this)[2] * v[0] - (*this)[0] * v[2];
		temp2 = (*this)[0] * v[1] - (*this)[1] * v[0];
		float temp = sqrt(temp0*temp0 + temp1*temp1 + temp2*temp2);
		return temp;
	}
};

class Vector4 : public Vector<4> {
	virtual float &operator*(const Vector4 v) {
		cout << "Sorry, multiplication cross is not supported currently!" << endl;
		float temp = 0.0f;
		return temp;
	}
};