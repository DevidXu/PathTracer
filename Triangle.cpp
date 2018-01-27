#include "Triangle.h"

bool Triangle::smallerThan(float mid, int dimension) {
	for (int i = 0; i < 3; i++)
		if ((*vertex[i])[dimension] > mid) return false;
	
	return true;
}


bool Triangle::largerThan(float mid, int dimension) {
	for (int i = 0; i < 3; i++)
		if ((*vertex[i])[dimension] < mid) return false;
	
	return true;
}


float Triangle::averageSideLength() {
	float sum = 0.0f;
	for (int i = 0; i < 3; i++)
		sum += vertex[i%3]->distance(*vertex[(i + 1)%3]);

	return sum / 3;
}


void Triangle::tessellate(Triangle** tri, Vector3** v, Vector3** n, float length) {
	
	for (int i = 0; i < 3; i++) {
		*v[i] = Vector3((*vertex[i % 3] + *vertex[(i + 1) % 3]) * 0.5f);
		*n[i] = Vector3((*normal[i % 3] + *normal[(i + 1) % 3]).normalize());
		*v[i] = *v[i] + *n[i] * length;
	}

	vertex[1] = v[0]; normal[1] = n[0];
	vertex[2] = v[2]; normal[2] = v[2];

	tri[0]->setVertex(v[0], vertex[1], v[1]);
	tri[0]->setNormal(n[0], normal[0], n[1]);

	tri[1]->setVertex(v[0], v[1], v[2]);
	tri[1]->setNormal(n[0], n[1], n[2]);

	tri[2]->setVertex(v[1], vertex[2], v[2]);
	tri[2]->setNormal(n[1], normal[2], n[2]);

	return;
}