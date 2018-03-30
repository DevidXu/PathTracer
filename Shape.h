#pragma once

#include "Vector.h"
#include "BBox.h"
#include "Triangle.h"
#include "Material.h"

#include <vector>

class Object;

using namespace std;

class Shape {
protected:
	std::vector<Vector3*> vertexs;
	std::vector<Vector3*> normals;
	
	Mesh meshes;	// This defines the triangle construction of the object.
	Vector3 center; // The local space center of the object

public:
	Shape();

	~Shape();

	void	setCenter(Vector3 v)	{ center = v; }
	Vector3 getCenter()				{ return center; }

	void setOwner(Object* m);

	Mesh getMesh() { return meshes; }

	void translate(Vector3 movement);

	void rotate(float x_angle = 0.0f, float y_angle = 0.0f, float z_angle = 0.0f);

	void rotate(Vector3 eulerAngle);

	void rotate(Vector4 quaternion);

	void scale(float s);

	void scale(Vector3 s);

	virtual float getRadius() { return 0.0f; }

	// This defines how to add more vertexs to the model; the iteration means iteration number
	// and the degree control the mutation degree of the model
	virtual void tessellate(float iterations, Object* obj) {};
};


class Rectangle :public Shape {
private:

public:
	Rectangle() {
		setCenter(Vector3(0.0f, 0.0f, 0.0f));
	};

	Rectangle(Vector3 small, Vector3 big);

	virtual float getRadius() { return 0.0f; }

	virtual void tessellate(float iterations, Object* obj);
};


class Sphere :public Shape {
private:
	float radius;

public:
	Sphere() {
		setCenter(Vector3(0.0f, 0.0f, 0.0f));
		radius = 0.0f;
	};

	Sphere(Vector3 m_center, float m_radius);

	virtual float getRadius() { return radius; }

	virtual void tessellate(float iteration, Object* obj);
};


class Model :public Shape {
private:

public:
	Model() {
		setCenter(Vector3(0.0f, 0.0f, 0.0f));
	}

	Model(Vector3 center, float scale, string filename);

	virtual float getRadius() { return 0.0f; }

	virtual void tessellate(float iterations, Object* obj) { return; }
};