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


bool Triangle::isTriangle() {
	for (int i = 0; i < 3; i++)
		if (*vertex[i] == *vertex[(i + 1) % 3]) return false;

	return true;
}


Vector3 Triangle::normalFaceUnit() {
	_ASSERT(isTriangle());

	Vector3 temp0 = *vertex[2] - *vertex[0];
	Vector3 temp1 = *vertex[2] - *vertex[1];

	Vector3 result = (temp0*temp1).normalize();

	int plus = 0, minus = 0;
	for (int i = 0; i < 3; i++) {
		float dot = result.dot(*normal[i]);
		if (dot>=0.0f) plus += 1;
		if (dot<=0.0f) minus -= 1;
	}

	try {
		_ASSERT(minus == -3 || plus == 3);
	}
	catch (exception e) {
		throw("Wrong normal information! Can't generate normal vector.");
	}

	if (minus == -3) 
		normalVector = Vector3(0.0f, 0.0f, 0.0f) - result;
	else
		normalVector = result;

	return normalVector;
}


// employ ray-triangle intersect detection by Tomas Moller
bool Triangle::hitRay(shared_ptr<Ray> ray, shared_ptr<Vector3> hitPoint) {
	float det, inv_det;
	float u, v;
	Vector3 edge1 = *vertex[1] - *vertex[0], edge2 = *vertex[2] - *vertex[0];

	Vector3 pvec = ray->getDirection()*edge2;
	det = edge1.dot(pvec);

	if (det > -EPISILON && det < EPISILON) return false;
	inv_det = 1.0f / det;

	Vector3 tvec = ray->getOrigin() - *vertex[0];

	u = tvec.dot(pvec)*inv_det;
	if (u<0.0f || u>1.0f) return false;

	Vector3 qvec = tvec * edge1;
	v = ray->getDirection().dot(qvec)*inv_det;
	if (v<0.0f || u + v>1.0f) return false;

	float t = edge2.dot(qvec);
	t *= inv_det;

	if (t <= 0.0f) return false;

	*hitPoint = Vector3(ray->getOrigin() + ray->getDirection()*t);
	return true;
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