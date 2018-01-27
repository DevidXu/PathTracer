#pragma once

// This defines the class of a triangle. It contains the normal and vertex information
#include "Vector.h"
#include <vector>

class Triangle {
private:
	Vector3 * vertex[3];
	Vector3* normal[3]; // each point has a normal

public:
	Triangle() {
		for (int i = 0; i < 3; i++) {
			vertex[i] = nullptr;
			normal[i] = nullptr;
		}

	};

	Triangle(Vector3* v0, Vector3* v1, Vector3* v2) {
		vertex[0] = v0;
		vertex[1] = v1;
		vertex[2] = v2;
	};

	Triangle(Vector3* v0, Vector3* v1, Vector3* v2,
		Vector3* n0, Vector3* n1, Vector3* n2) :Triangle(v0, v1, v2) {
		normal[0] = n0;
		normal[1] = n1;
		normal[2] = n2;
	};

	// used to judge whether the triangle is on one side of the side on some dimension
	bool smallerThan(float mid, int dimension);
	bool largerThan(float mid, int dimension);

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

	float averageSideLength();

	/* How we tessellate:
	first: we divide the triangle into 4 parts and add three vertexs correspondingly. before
	we call this function, we need to create the correspond space for the triangles, vertexs
	and normals. And we adjust the position of the generated vertexs by taking the midpoint
	of two adjacent vertexs and add len * mid of normal. The length is defined the average
	length of the three sides of the original triangle. For an equal-length triangle we expect
	the generated triangles are all equal-length triangles.
	*/
	void tessellate(Triangle** tri, Vector3** v, Vector3** n, float length); // expand one triangle to three
};

typedef std::vector<Triangle*> Mesh;