#pragma once

// This file defines all the necessary Vector operations
#include <assert.h>
#include "iostream"
using namespace std;

template <int n>
class Vector {
public:
	float value[n];
public:
	Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) {
		float arr[] = { x, y, z, w };
		for (int i = 0; i < n; i++)
			value[i] = arr[i];
	}
};


class Vector2: public Vector<2> {
public:
	Vector2(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) :Vector(x, y, z, w)
	{
	}

	Vector2(const Vector2 &v2) :Vector(v2) {};

	float operator[](const int k) {
		return value[k];
	}

	bool operator==(const Vector2 v) {
		for (int i = 0; i < 2; i++)
			if ((*this)[i] != v.value[i]) return false;
		return true;
	}

	Vector2 operator+(const Vector2 v) {
		Vector2 temp;
		for (int i = 0; i < 2; i++)
			temp.value[i] = (*this)[i] + v.value[i];
		return temp;
	}
	
	Vector2 operator-(const Vector2 v) {
		Vector2 temp;
		for (int i = 0; i < 2; i++)
			temp.value[i] = (*this)[i] - v.value[i];
		return temp;
	}

	void operator=(const Vector2 v) {
		for (int i = 0; i < 2; i++)
			value[i] = v.value[i];
		return;
	}

	bool operator<(const Vector2 v) {
		for (int i = 0; i < 2; i++)
			if (value[i] >= v.value[i]) return false;

		return true;
	}

	float dot(const Vector2 v) {
		float k = 0.0f;
		for (int i = 0; i < 2; i++)
			k += (*this)[i] * v.value[i];
		return k;
	}

	float magnitude() {
		float temp = 0.0f;
		for (int i = 0; i < 2; i++)
			temp += (*this)[i] * (*this)[i];
		return sqrt(temp);
	}

	float distance(const Vector2 v) {
		Vector2 temp = (*this) - v;
		return temp.magnitude();
	}

	Vector2 operator*(float k) {
		Vector2 temp;
		for (int i = 0; i < 2; i++)
			temp.value[i] = (*this)[i] * k;
		return temp;
	}

	// override some functions
	virtual float operator*(const Vector v) {
		float temp;
		temp = (*this)[0] * v.value[1] - (*this)[1] * v.value[0];
		return temp;
	}

};

ostream &operator<<(ostream& out, const Vector2 &v);

class Vector3 : public Vector<3> {
public:
	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) :Vector(x, y, z, w)
	{
	}

	Vector3(const Vector3 &v) :Vector(v) {};

	float operator[](const int k) {
		return value[k];
	}

	ostream &operator<<(ostream& out) {
		for (int i = 0; i < 3; i++)
			cout << (*this)[i] << " ";
		return out;
	}

	bool operator==(const Vector3 &v) {
		for (int i = 0; i < 3; i++)
			if ((*this)[i] != v.value[i]) return false;
		return true;
	}

	Vector3 operator+(const Vector3 v) {
		Vector3 temp;
		for (int i = 0; i < 3; i++)
			temp.value[i] = (*this)[i] + v.value[i];
		return temp;
	}

	Vector3 operator-(const Vector3 v) {
		Vector3 temp;
		for (int i = 0; i < 3; i++)
			temp.value[i] = (*this)[i] - v.value[i];
		return temp;
	}

	void operator=(const Vector3 v) {
		for (int i = 0; i < 3; i++)
			value[i] = v.value[i];
		return;
	}

	bool operator<(const Vector3 v) {
		for (int i = 0; i < 3; i++)
			if (value[i] >= v.value[i]) return false;
		
		return true;
	}

	float dot(const Vector3 v) {
		float k = 0.0f;
		for (int i = 0; i < 3; i++)
			k += (*this)[i] * v.value[i];
		return k;
	}

	float magnitude() {
		float temp = 0.0f;
		for (int i = 0; i < 3; i++)
			temp += (*this)[i] * (*this)[i];
		return sqrt(temp);
	}

	float distance(const Vector3 v) {
		Vector3 temp = (*this) - v;
		return temp.magnitude();
	}

	Vector3 operator*(float k) {
		Vector3 temp;
		for (int i = 0; i < 3; i++)
			temp.value[i] = (*this)[i] * k;
		return temp;
	}

	float operator*(const Vector3 v) {
		float temp0, temp1, temp2;
		temp0 = (*this)[1] * v.value[2] - (*this)[2] * v.value[1];
		temp1 = (*this)[2] * v.value[0] - (*this)[0] * v.value[2];
		temp2 = (*this)[0] * v.value[1] - (*this)[1] * v.value[0];
		float temp = sqrt(temp0*temp0 + temp1*temp1 + temp2*temp2);
		return temp;
	}
};

ostream &operator<<(ostream& out, const Vector3 &v);

class Vector4 : public Vector<4> {
public:
	Vector4 (float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f):Vector(x, y, z, w) 
	{
	}

	Vector4(const Vector4 &v) :Vector(v) {};
	Vector4(const Vector3 &v) :Vector(v.value[0], v.value[1], v.value[2], 0.0) {};

	float operator[](const int k) {
		return value[k];
	}

	ostream &operator<<(ostream& out) {
		for (int i = 0; i < 4; i++)
			cout << (*this)[i] << " ";
		return out;
	}

	bool operator==(const Vector4 &v) {
		for (int i = 0; i < 4; i++)
			if ((*this)[i] != v.value[i]) return false;
		return true;
	}

	Vector4 operator+(const Vector4 v) {
		Vector4 temp;
		for (int i = 0; i < 4; i++)
			temp.value[i] = (*this)[i] + v.value[i];
		return temp;
	}

	Vector4 operator-(const Vector4 v) {
		Vector4 temp;
		for (int i = 0; i < 4; i++)
			temp.value[i] = (*this)[i] - v.value[i];
		return temp;
	}

	void operator=(const Vector4 v) {
		for (int i = 0; i < 4; i++)
			value[i] = v.value[i];
		return;
	}

	bool operator<(const Vector4 v) {
		for (int i = 0; i < 4; i++)
			if (value[i] >= v.value[i]) return false;

		return true;
	}

	float dot(const Vector4 v) {
		float k = 0.0f;
		for (int i = 0; i < 4; i++)
			k += (*this)[i] * v.value[i];
		return k;
	}

	float magnitude() {
		float temp = 0.0f;
		for (int i = 0; i < 4; i++)
			temp += (*this)[i] * (*this)[i];
		return sqrt(temp);
	}

	float distance(const Vector4 v) {
		Vector4 temp = (*this) - v;
		return temp.magnitude();
	}

	Vector4 operator*(float k) {
		Vector4 temp;
		for (int i = 0; i < 4; i++)
			temp.value[i] = (*this)[i] * k;
		return temp;
	}

	virtual float operator*(const Vector v) {
		cout << "Sorry, multiplication cross is not supported currently!" << endl;
		float temp = 0.0f;
		return temp;
	}
};

ostream &operator<<(ostream& out, const Vector4 &v);