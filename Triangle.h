#pragma once

// This defines the class of a triangle. It contains the normal and vertex information
#include "Vector.h"
#include "Ray.h"
#include "Constants.h"
#include <memory>
#include <vector>

class Object;

class Triangle {
private:
	Vector3 * vertex[3];
	Vector3* normal[3]; // each point has a normal
	Vector3 normalVector;
	Object* owner;

public:
	Triangle() {
		for (int i = 0; i < 3; i++) {
			vertex[i] = nullptr;
			normal[i] = nullptr;
		}

	};

	Triangle(Vector3* v0, Vector3* v1, Vector3* v2, Object* m = nullptr) {
		vertex[0] = v0;
		vertex[1] = v1;
		vertex[2] = v2;
		owner = m;
	};

	Triangle(Vector3* v0, Vector3* v1, Vector3* v2,
		Vector3* n0, Vector3* n1, Vector3* n2, Object* m = nullptr) :Triangle(v0, v1, v2, m) {
		normal[0] = n0;
		normal[1] = n1;
		normal[2] = n2;

		normalFaceUnit();
	};

	// used to judge whether the triangle is on one side of the side on some dimension
	bool smallerThan(float mid, int dimension);
	bool largerThan(float mid, int dimension);

	bool isTriangle();

	void setVertex(Vector3* v0, Vector3* v1, Vector3* v2) {
		vertex[0] = v0;
		vertex[1] = v1;
		vertex[2] = v2;
	}

	void setNormal(Vector3* n0, Vector3* n1, Vector3* n2) {
		normal[0] = n0;
		normal[1] = n1;
		normal[2] = n2;
	}

	void setOwner(Object* m) {
		owner = m;
	}

	Object* getOwner() { return owner; }

	Vector3 getNormal() { return normalVector; }

	float averageSideLength();

	Vector3 normalFaceUnit(); // return the normal vector prependicular to the plane

	// return the meeting point of the ray and the triangle, if no, return nullptr;
	bool hitRay(shared_ptr<Ray> ray, shared_ptr<Vector3> hitPoint);

	/* How we tessellate:
	first: we divide the triangle into 4 parts and add three vertexs correspondingly. before
	we call this function, we need to create the correspond space for the triangles, vertexs
	and normals. And we adjust the position of the generated vertexs by taking the midpoint
	of two adjacent vertexs and add len * mid of normal. The length is defined the average
	length of the three sides of the original triangle. For an equal-length triangle we expect
	the generated triangles are all equal-length triangles.
	*/
	void tessellate(
		Triangle** tri, 
		Vector3** v, 
		Vector3** n, 
		float length, 
		Vector3 center = Vector3(0.0f, 0.0f, 0.0f)
	); // expand one triangle to three
};

typedef std::vector<Triangle*> Mesh;