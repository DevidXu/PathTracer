#include "BBox.h"

float MIN(float a, float b) {
	return a < b ? a : b;
}

void Cube::setCube(Vector3 s, Vector3 l) {
	try {
		_ASSERT(s < l);
	}
	catch (exception e) {
		LOGPRINT("Wrong vector to initialize the bounding box.");
	}

	small = s;
	large = l;

	return;
}


void Cube::divideCube() {
	if (left != nullptr) return;
	
	int dimension = int(lm);
	Vector3 t_small = small, t_large = large;
	try {
		t_large.value[dimension] = mid;
		left = new Cube(small, t_large);

		t_small.value[dimension] = mid;
		right = new Cube(t_small, large);
	}
	catch (exception e) {
		LOGPRINT("Meet error when dividing the cube");
		LOGPRINT(e.what());
	}

	return;
}


// when insert triangle list, we need to determine the layermark(use x?y?z to divide) as
// well as whether sub-cube is needed.
void Cube::insertTriangleList(vector<Triangle*>& triangleList) {
	if (triangleList.size() <= 1) {
		for (auto ele : triangleList)
			triangle_ptr.push_back(ele);
		return;
	}

	float variance[3], mean[3];
	for (int i = 0; i < 3; i++) {
		variance[i] = 0;
		mean[i] = 0;
	}

	// calculate the mean value of each axis
	for (auto& triangle : triangleList){
		Vector3 centroid = triangle->getCentroid();
		for (int i = 0; i < 3; i++)
			mean[i] += centroid.value[i];
	}

	for (int i = 0; i < 3; i++)
		mean[i] /= triangleList.size();

	for (auto& triangle : triangleList)
		for (int i = 0; i < 3; i++)
			variance[i] += float(pow((triangle->getCentroid().value[i] - mean[i]), 2));

	// determine the axis based on which to choose the variance list
	float max = 0.0f;
	for (int i = 0; i < 3; i++) {
		variance[i] /= triangleList.size();
		if (variance[i] > max) {
			max = variance[i];
			lm = LayerMark(i);
			mid = mean[i];
		}
	}

	vector<Triangle*> leftSequence, rightSequence;
	leftSequence.clear();
	rightSequence.clear();

	while (triangleList.empty() == false) {
		Triangle* triangle = triangleList.at(triangleList.size() - 1);
		triangleList.pop_back();

		if (triangle->smallerThan(mid, lm)) {
			leftSequence.push_back(triangle);
			continue;
		}

		if (triangle->largerThan(mid, lm)) {
			rightSequence.push_back(triangle);
			continue;
		}

		triangle_ptr.push_back(triangle);
	}

	if (leftSequence.size() > 0 || rightSequence.size() > 0) {
		divideCube();
		left->insertTriangleList(leftSequence);
		right->insertTriangleList(rightSequence);
	}

	return;
}


void Cube::insertTriangle(Triangle* triangle) {
	bool within = true;

	if (left && triangle->smallerThan(mid, lm)) {
		left->insertTriangle(triangle);
		return;
	}

	if (right && triangle->largerThan(mid, lm)) {
		right->insertTriangle(triangle);
		return;
	}

	triangle_ptr.push_back(triangle);
	return;
}


Triangle* Cube::intersect(shared_ptr<Ray> ray, float* dis) {
	float distance = MAX_DIS;
	Triangle* hTriangle = nullptr;

	for (auto &ptr : triangle_ptr) {
		shared_ptr<Vector3> hitPoint = make_shared<Vector3>();
		if (ptr->hitRay(ray, hitPoint)) {
			float temp_distance = ray->getOrigin().distance(*hitPoint);
			if (temp_distance > EPISILON && (hTriangle == nullptr || temp_distance < distance)) {
				distance = ray->getOrigin().distance(*hitPoint);
				hTriangle = ptr;
			}
		}
	}

	*dis = distance;

	return hTriangle;
}


// detect whether the ray will cross the cube; if cross, change the distance correspondingly
bool Cube::hitRay(shared_ptr<Ray> ray, float &distance) {
	float tmin = 0.0f, tmax = MAX_DIS;

	Vector3 dir = ray->getDirection(), origin = ray->getOrigin();

	bool inside = true;
	for (int i = 0; i < 3; i++) {
		if (origin.value[i]<small.value[i] || origin.value[i]>large.value[i]) inside = false;
	}
	if (inside) {
		distance = 0.0f;
		return true;
	}

	if (abs(dir.value[0]) < EPISILON) { // if ray parallel to x plane
		if (origin.value[0]<small.value[0] || origin.value[0]>large.value[0])
			return false;
	}
	else {
		float ood = 1.0f / dir.value[0];
		float t1 = (small.value[0] - origin.value[0])*ood;
		float t2 = (large.value[0] - origin.value[0])*ood;

		if (t1 > t2) {
			float temp = t1; t1 = t2; t2 = temp;
		}

		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;

		if (tmin > tmax) return false;
	}

	if (abs(dir.value[1]) < EPISILON) {
		if (origin.value[1]<small.value[1] || origin.value[1]>large.value[1])
			return false;
	}
	else {
		float ood = 1.0f / dir.value[1];
		float t1 = (small.value[1] - origin.value[1])*ood;
		float t2 = (large.value[1] - origin.value[1])*ood;

		if (t1 > t2) { 
			float temp = t1; t1 = t2; t2 = temp; 
		}

		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;

		if (tmin > tmax) return false;
	}

	if (abs(dir.value[2]) < EPISILON) {
		if (origin.value[2]<small.value[2] || origin.value[2]>large.value[2])
			return false;
	}
	else {
		float ood = 1.0f / dir.value[2];
		float t1 = (small.value[2] - origin.value[2])*ood;
		float t2 = (large.value[2] - origin.value[2])*ood;

		if (t1 > t2) {
			float temp = t1; t1 = t2; t2 = temp;
		}

		if (tmin > tmax) return false;
	}

	distance = (dir*tmin).magnitude();
	return true;
}


// travel down until minimum
// go back and check triangles in the current cube
Triangle* Cube::hitCloestTriangle(const shared_ptr<Ray> ray, float &distance) {
	float min_distance = MAX_DIS, cube_distance = MAX_DIS;

	Triangle* min_Triangle = intersect(ray, &min_distance); // find the intersected triangles in the current cube size
	min_distance = MIN(distance, min_distance);

	// test the side that intersects first, save some time
	if (left && left->hitRay(ray, cube_distance) && cube_distance < min_distance) {
		float t_d = MAX_DIS;
		Triangle* temp_triangle = left->hitCloestTriangle(ray, t_d);

		if (t_d < min_distance) {
			min_distance = t_d;
			min_Triangle = temp_triangle;
		}
	}

	if (right && right->hitRay(ray, cube_distance) && cube_distance < min_distance) {
		float t_d = MAX_DIS;
		Triangle* temp_triangle = right->hitCloestTriangle(ray, t_d);

		if (t_d < min_distance) {
			min_distance = t_d;
			min_Triangle = temp_triangle;
		}
	}

	distance = min_distance;
	return min_Triangle;

}


int Cube::maxDepth() {
	if (left == nullptr) return 1;

	int leftDepth = left->maxDepth();
	int rightDepth = right->maxDepth();

	return leftDepth > rightDepth ? leftDepth + 1 : rightDepth + 1;
}


BBox::BBox() {
	small = Vector3(0.0f, 0.0f, 0.0f);
	large = Vector3(1.0f, 1.0f, 1.0f);
	triangleList.clear();

	box.setCube(small, large);
}


BBox::BBox(Vector3 s, Vector3 l) {
	small = s;
	large = l;
	triangleList.clear();

	box.setCube(small, large);
}


BBox::~BBox() {

}


void BBox::addTriangle(Triangle* triangle) {
	// ensure triangle is smaller than the box
	for (int i = 0; i < 3; i++) {
		if (!triangle->smallerThan(large[i], i) || !triangle->largerThan(small[i], i))
			throw("Triangle is not in the box domain.");
	}

	triangleList.push_back(triangle);
}


void BBox::initializeCube() {
	LOGPRINT("Triangle number:    " + to_string(triangleList.size()));

	box.insertTriangleList(triangleList);
	return;
}


void BBox::addMesh(Mesh mesh) {
	// The type should be triangle
	try {
		for (auto ptr : mesh)
			addTriangle(ptr);
	}
	catch (exception e) {
		LOGPRINT("Meet error when loading the mesh!");
		LOGPRINT(e.what());
	}

	return;
}


Triangle* BBox::intersect(shared_ptr<Ray> ray, float &distance) {
	if (ray == nullptr || ray->getDirection().magnitude() < EPISILON) 
		return nullptr;

	Triangle* t = box.hitCloestTriangle(ray, distance);

	return t;
}


int BBox::getDepth() {
	return box.maxDepth();
}