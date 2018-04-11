#pragma once

// This defines the class of a triangle. It contains the normal and vertex information
// Meanwhile, triangle is called frequently by other classes. Therefore, some auxiliary
// functions are added to calculate the updated normal, centroid, etc..
// Remember, if you change some functions inside, you might need to update many related
// values like normals, vertex..

#include "Vector.h"
#include "Ray.h"
#include "Constants.h"
#include <memory>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;

class Object;

class Triangle {
private:
	Vector3 * vertex[3];
	Vector3* normal[3];		// each point has a normal
	Vector2* texcoord[3];	// each point has a texcoord point
	Mat* texture;
	Vector3 centroid;
	Vector3 normalVector;
	Object* owner;
	bool infinite;		// judge whether is an infinite small triangle (useful for sphere)

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

		infinite = false;
		calculateCentroid();
	};

	Triangle(Vector3* v0, Vector3* v1, Vector3* v2,
		Vector3* n0, Vector3* n1, Vector3* n2, Object* m = nullptr){
		normal[0] = n0;
		normal[1] = n1;
		normal[2] = n2;

		vertex[0] = v0;
		vertex[1] = v1;
		vertex[2] = v2;
		owner = m;

		infinite = false;

		normalFaceUnit();
		calculateCentroid();
	};

	// used to judge whether the triangle is on one side of the side on some dimension
	bool smallerThan(float mid, int dimension);
	bool largerThan(float mid, int dimension);
	void calculateCentroid();

	bool isTriangle();

	void setVertex(Vector3* v0, Vector3* v1, Vector3* v2) {
		vertex[0] = v0;
		vertex[1] = v1;
		vertex[2] = v2;
		calculateCentroid();
	}

	void setNormal(Vector3* n0, Vector3* n1, Vector3* n2) {
		normal[0] = n0;
		normal[1] = n1;
		normal[2] = n2;
	}

	void setTexcoord(Vector2* t0, Vector2* t1, Vector2* t2, Mat* tex) {
		texcoord[0] = t0;
		texcoord[1] = t1;
		texcoord[2] = t2;
		setTexture(tex);
	}

	void setNormalVector(Vector3 normalUnit) {
		normalVector = normalUnit;
	}

	void setOwner(Object* m) {
		owner = m;
	}

	void setTexture(Mat* tex) {
		texture = tex;
	}

	void setInfinite(bool m_Infinite) {
		infinite = m_Infinite;
	}


	Object* getOwner() { 
		return owner; 
	}

	Vector3 getNormal() { 
		return normalVector; 
	}

	Vector3 getCentroid() {
		return centroid;
	}

	// adopt uv texture coordinate (u means horizon(column first))
	Vector3 getTexColor(Vector3 hit);

	bool getInfinite() {
		return infinite;
	}

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

	// when ray collides with a ball, it should give specialized process
	static bool intersectSphere(shared_ptr<Ray> ray, Triangle* &patch, float &distance);
};

typedef std::vector<Triangle*> Mesh;