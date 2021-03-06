#pragma once

// This file defines all the necessary Vector operations. It overrides some operator
// and defines the way to print the Vectors.
// It defines Vector[2,3,4]. THe mostly used vector is Vector3.
#include <assert.h>
#include "iostream"
#include <iomanip>

using namespace std;

float MAX(float a, float b);


template <int n>
class Vector {
public:
	float value[n];

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

	friend ostream& operator<<(ostream& out, const Vector2 v);

	bool operator==(const Vector2 &v) {
		for (int i = 0; i < 2; i++)
			if ((*this)[i] != v.value[i]) return false;
		return true;
	}

	Vector2 operator+(const Vector2 &v) {
		Vector2 temp;
		for (int i = 0; i < 2; i++)
			temp.value[i] = (*this)[i] + v.value[i];
		return temp;
	}
	
	Vector2 operator-(const Vector2 &v) {
		Vector2 temp;
		for (int i = 0; i < 2; i++)
			temp.value[i] = (*this)[i] - v.value[i];
		return temp;
	}

	void operator=(const Vector2 &v) {
		for (int i = 0; i < 2; i++)
			value[i] = v.value[i];
		return;
	}

	bool operator<(const Vector2 &v) {
		for (int i = 0; i < 2; i++)
			if (value[i] >= v.value[i]) return false;

		return true;
	}

	float dot(const Vector2 &v) {
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

	float distance(const Vector2 &v) {
		Vector2 temp = (*this) - v;
		return temp.magnitude();
	}

	Vector2 normalize() {
		float length = magnitude();
		_ASSERT(length != 0.0f);
		for (int i = 0; i < 2; i++)
			value[i] = value[i] / length;
		return *this;
	}

	Vector2 operator*(float k) {
		Vector2 temp;
		for (int i = 0; i < 2; i++)
			temp.value[i] = (*this)[i] * k;
		return temp;
	}

	Vector2 operator/(float k) {
		_ASSERT(k != 0.0f);
		return operator*(1.0f / k);
	}

	Vector2 rotate(float angle) {
		Vector2 temp0(cos(angle), -sin(angle)), temp1(sin(angle), cos(angle));
		Vector2 result(this->dot(temp0), this->dot(temp1));
		
		return result;
	}

	float max() {
		return MAX(value[0], value[1]);
	}

	// override some functions
	virtual float operator*(const Vector &v) {
		float temp;
		temp = (*this)[0] * v.value[1] - (*this)[1] * v.value[0];
		return temp;
	}

};

ostream &operator<<(ostream& out, const Vector2 v);

class Vector3 : public Vector<3> {
public:
	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) :Vector(x, y, z, w)
	{
	}

	Vector3(const Vector3 &v) :Vector(v) {};

	float operator[](const int k) {
		return value[k];
	}

	friend ostream &operator<<(ostream& out, const Vector3 v);

	bool operator==(const Vector3 &v) {
		for (int i = 0; i < 3; i++)
			if ((*this)[i] != v.value[i]) return false;
		return true;
	}

	Vector3 operator+(const Vector3 &v) {
		Vector3 temp;
		for (int i = 0; i < 3; i++)
			temp.value[i] = (*this)[i] + v.value[i];
		return temp;
	}

	Vector3 operator+=(const Vector3 &v) {
		for (int i = 0; i < 3; i++)
			value[i] += v.value[i];
		return *this;
	}

	Vector3 operator-(const Vector3 &v) {
		Vector3 temp;
		for (int i = 0; i < 3; i++)
			temp.value[i] = (*this)[i] - v.value[i];
		return temp;
	}

	void operator=(const Vector3 &v) {
		for (int i = 0; i < 3; i++)
			value[i] = v.value[i];
		return;
	}

	bool operator<(const Vector3 &v) {
		for (int i = 0; i < 3; i++)
			if (value[i] >= v.value[i]) return false;
		
		return true;
	}

	float max() {
		return MAX(MAX(value[0], value[1]), value[2]);
	}

	float dot(const Vector3 &v) {
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

	float distance(const Vector3 &v) {
		Vector3 temp = (*this) - v;
		return temp.magnitude();
	}

	Vector3 operator*(float k) {
		Vector3 temp;
		for (int i = 0; i < 3; i++)
			temp.value[i] = (*this)[i] * k;
		return temp;
	}

	Vector3 operator/(float k) {
		_ASSERT(k != 0.0f);
		return operator*(1.0f / k);
	}

	Vector3 normalize() {
		float length = magnitude();
		_ASSERT(length != 0.0f);
		for (int i = 0; i < 3; i++)
			value[i] = value[i] / length;
		return *this;
	}

	Vector3 operator*(const Vector3 &v) {
		float temp0, temp1, temp2;
		temp0 = (*this)[1] * v.value[2] - (*this)[2] * v.value[1];
		temp1 = (*this)[2] * v.value[0] - (*this)[0] * v.value[2];
		temp2 = (*this)[0] * v.value[1] - (*this)[1] * v.value[0];

		return Vector3(temp0, temp1, temp2);
	}
};

ostream &operator<<(ostream& out, const Vector3 v);

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

	friend ostream &operator<<(ostream& out, const Vector4 v);

	bool operator==(const Vector4 &v) {
		for (int i = 0; i < 4; i++)
			if ((*this)[i] != v.value[i]) return false;
		return true;
	}

	Vector4 operator+(const Vector4 &v) {
		Vector4 temp;
		for (int i = 0; i < 4; i++)
			temp.value[i] = (*this)[i] + v.value[i];
		return temp;
	}

	Vector4 operator-(const Vector4 &v) {
		Vector4 temp;
		for (int i = 0; i < 4; i++)
			temp.value[i] = (*this)[i] - v.value[i];
		return temp;
	}

	void operator=(const Vector4 &v) {
		for (int i = 0; i < 4; i++)
			value[i] = v.value[i];
		return;
	}

	bool operator<(const Vector4 &v) {
		for (int i = 0; i < 4; i++)
			if (value[i] >= v.value[i]) return false;

		return true;
	}

	float max() {
		return MAX(MAX(MAX(value[0], value[1]), value[2]), value[3]);
	}

	float dot(const Vector4 &v) {
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

	float distance(const Vector4 &v) {
		Vector4 temp = (*this) - v;
		return temp.magnitude();
	}

	Vector4 normalize() {
		float length = magnitude();
		_ASSERT(length != 0.0f);
		for (int i = 0; i < 4; i++)
			value[i] = value[i] / length;
		return *this;
	}

	Vector4 operator*(float k) {
		Vector4 temp;
		for (int i = 0; i < 4; i++)
			temp.value[i] = (*this)[i] * k;
		return temp;
	}

	Vector4 operator/(float k) {
		_ASSERT(k != 0.0f);
		return operator*(1.0f / k);
	}

	virtual float operator*(const Vector &v) {
		cout << "Sorry, multiplication cross is not supported currently!" << endl;
		float temp = 0.0f;
		return temp;
	}
};

ostream &operator<<(ostream& out, const Vector4 v);