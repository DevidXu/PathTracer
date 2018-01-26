#pragma once

#include "Vector.h"
#include "BBox.h"
#include "Triangle.h"

#include <vector>

class Shape {
protected:
	std::vector<Vector3*> vertexs;
	std::vector<Vector3*> normals;
	
	Mesh meshes;	// This defines the triangle construction of the object.
	Vector3 center; // The local space center of the object

public:
	Shape();

	~Shape();

	void setCenter(Vector3 v) { center = v; }

	Mesh getMesh() { return meshes; }

	void translate(Vector3 movement);

	void rotate(float x_angle = 0.0f, float y_angle = 0.0f, float z_angle = 0.0f);

	void rotate(Vector3 eulerAngle);

	void rotate(Vector4 quaternion);

	void scale(float s);

	void scale(Vector3 s);
};


class Rectangle :public Shape {
private:

public:
	Rectangle() {
		setCenter(Vector3(0.0f, 0.0f, 0.0f));
	};

	Rectangle(Vector3 small, Vector3 big);
};