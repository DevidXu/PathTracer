#pragma once

// This file is used for generating the bounding box for the whole world.
// It will use binary search to find the precise position of the triangle.
/*
AABB Bounding Box
When a new triangle comes, it will find the exact position of these triangles
in the bonuding box, as well as divide the bounding box into smaller level so
that each triangle is fixed reasonably. Each box will contains a pointer to an
array which stores all the triangles relate ot this box. Then a ray can visit
through the bounding box to find all possible triangles to hit.
*/
#include "Singleton.h"
#include "Debugging.h"
#include "Triangle.h"
#include "Ray.h"
#include "Constants.h"

// The whole struct like a cube divided layer by layer. Use a KD Tree
enum LayerMark { X, Y, Z };

// AABB box in this project
class Cube {
private:
	Vector3		small, large;
	float		mid;
	LayerMark	lm;

public:
	Cube *left, *right;
	std::vector<Triangle*> triangle_ptr;

	Cube() {
		small = Vector3(0.0f, 0.0f, 0.0f);
		large = Vector3(0.0f, 0.0f, 0.0f);
		mid = 0.0f;

		left = nullptr;
		right = nullptr;
		lm = (LayerMark)0;
	};
	
	void setCube(Vector3 s, Vector3 l);

	Cube(Vector3 s, Vector3 l) {
		setCube(s, l);
		left = nullptr;
		right = nullptr;
	}

	Cube(Vector3 s, Vector3 l, LayerMark m_lm) :lm(m_lm) {
		setCube(s, l);
		left = nullptr;
		right = nullptr;
	};

	~Cube() {
		if (left) delete left;
		if (right) delete right;
	};

	// when insert triangles into the bounding box, it might increase the depth based 
	// on the position of the triangle; currently I'm considering including variance to
	// balance the KD tree or employing red-black tree to balance.
	void divideCube();
	void insertTriangle(Triangle* triangle);
	void insertTriangleList(vector<Triangle*> &triangleList);

	// These functions are used to call recursively to find the triangle which intersect
	// with the ray in the bounding box
	Triangle* hitCloestTriangle(const shared_ptr<Ray> ray, float& distance);
	Triangle* intersect(shared_ptr<Ray> ray, float* distance);
	bool hitRay(const shared_ptr<Ray> ray, float& distance);

	// helper functions 
	int maxDepth();
};


class BBox :public Singleton<BBox> {
private:

	Cube box;
	Vector3 small, large;
	vector<Triangle*> triangleList;

public:
	friend	Singleton<BBox>;
	BBox();
	BBox(Vector3 s, Vector3 l);
	~BBox(); // shared_ptr requirement

	void	addTriangle(Triangle* triangle);  // store all the triangles in a temporary cube

	void	initializeCube();
	void	addMesh(Mesh mesh); // all elements in mesh need to be triangles!

	// find the closest triangle to the ray by visiting through the bounding box
	Triangle* intersect(shared_ptr<Ray> ray, float &distance);

	int getDepth(); // return the depth of the cube constructed in the BBox
};