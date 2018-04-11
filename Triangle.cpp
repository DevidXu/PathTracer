#include "Triangle.h"
#include "Object.h"

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


void Triangle::calculateCentroid() {
	centroid = (*vertex[0] + *vertex[1] + *vertex[2]) / 3.0f;
	return;
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

	if (normalVector.magnitude() == 1.0f) return normalVector;

	Vector3 temp0 = *vertex[2] - *vertex[0];
	Vector3 temp1 = *vertex[2] - *vertex[1];

	Vector3 result = (temp0*temp1).normalize();

	int plus = 0, minus = 0;
	for (int i = 0; i < 3; i++) {
		float dot = result.dot(*normal[i]);
		if (dot>=0.0f) plus += 1;
		if (dot<=0.0f) minus -= 1;
	}

	if (minus == -3 || plus == 3) {
		if (minus == -3)
			normalVector = Vector3() - result;
		else
			normalVector = result;
	}
	else normalVector = result;

	return normalVector;
}


Vector3 Triangle::getTexColor(Vector3 hit) {
	if (!texture || !texcoord) return Vector3(1.0f, 1.0f, 1.0f);

	Vector3 n12 = (*vertex[2] - *vertex[1]).normalize();
	Vector3 n10 = (*vertex[0] - *vertex[1]).normalize();
	Vector3 v13 = hit - *vertex[1];

	float y = 0.0f, x = 0.0f;

	float ytemp = ((*vertex[0] - *vertex[1]) * n12).magnitude();
	if (ytemp < EPISILON) y = 0.0f;
	else
		y = (v13*n12).magnitude() / ytemp;

	float xtemp = ((*vertex[2] - *vertex[1])*n10).magnitude();
	if (xtemp < EPISILON) x = 0.0f;
	else
		x = (v13*n10).magnitude() / xtemp;

	Vector2 interpolateTex = *texcoord[2] * x + *texcoord[0] * y + *texcoord[1] * (1 - x - y);
	CvPoint tex(int(interpolateTex.value[0] * texture->cols), int((1-interpolateTex.value[1]) * texture->rows));
	Vec3b vecColor = texture->at<Vec3b>(tex);
	Vector3 color;
	for (int i = 0; i < 3; i++) color.value[2-i] = vecColor[i] / 255.0f; // mat color is bgr
	return color;
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

	Vector3 temp_origin = ray->getOrigin();

	Vector3 tvec = temp_origin - *vertex[0];

	u = tvec.dot(pvec)*inv_det;
	if (u<0.0f || u>1.0f) return false;

	Vector3 qvec = tvec * edge1;
	v = ray->getDirection().dot(qvec)*inv_det;
	if (v<0.0f || u + v>1.0f) return false;

	float t = edge2.dot(qvec);
	t *= inv_det;

	if (t <= 0.0f) return false;

	*hitPoint = Vector3(temp_origin + ray->getDirection()*t);
	return true;
}


void Triangle::tessellate(Triangle** tri, Vector3** v, Vector3** n, float length, Vector3 center) {
	
	for (int i = 0; i < 3; i++) {
		*v[i] = Vector3((*vertex[i % 3] + *vertex[(i + 1) % 3]) * 0.5f);
		*n[i] = Vector3((*normal[i % 3] + *normal[(i + 1) % 3]).normalize());
		*v[i] = *v[i] + *n[i] * length;
	}

	tri[0]->setVertex(v[0], vertex[1], v[1]);
	tri[0]->setNormal(n[0], normal[1], n[1]);
	tri[0]->normalFaceUnit();

	tri[1]->setVertex(v[0], v[1], v[2]);
	tri[1]->setNormal(n[0], n[1], n[2]);
	tri[1]->normalFaceUnit();

	tri[2]->setVertex(v[1], vertex[2], v[2]);
	tri[2]->setNormal(n[1], normal[2], n[2]);
	tri[2]->normalFaceUnit();

	vertex[1] = v[0]; normal[1] = n[0];
	vertex[2] = v[2]; normal[2] = n[2];
	normalFaceUnit();

	return;
}


bool Triangle::intersectSphere(shared_ptr<Ray> ray, Triangle* &patch, float &distance) {
	// deal with the sphere
	if (patch == nullptr || !patch->getInfinite()) return false;

	Vector3 oc = ray->getOrigin() - patch->getOwner()->getCenter();
	float dotOCD = ray->getDirection().dot(oc);

	if (dotOCD > 0) {
		patch = nullptr;
		return false;
	}

	float dotOC = oc.dot(oc);
	float radius = patch->getOwner()->getShape()->getRadius();;
	_ASSERT(radius != 0.0f);
	float discriminant = dotOCD * dotOCD - dotOC + pow(radius, 2);

	if (discriminant < 0) {
		patch = nullptr;
		return false;
	}
	else {
		if (discriminant < EPISILON) {
			if (-dotOCD < EPISILON) {
				patch = nullptr;
				return false;
			}
			else distance = -dotOCD;
		}
		else
		{
			discriminant = sqrt(discriminant);
			float t0 = -dotOCD - discriminant;
			float t1 = -dotOCD + discriminant;
			if (t0 < EPISILON)
				t0 = t1;
			distance = t0;
		}
	}

	return true;
}
