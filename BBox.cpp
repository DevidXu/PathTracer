#include "BBox.h"

#define REFINED_CUBE_INTERSECT true


// set the small and large bounding vector of a cube
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


// before call this funciton, you should ensure the dimension and mid variable has been given 
// value correctly. The KD tree will base on these two value to divide the cube
// aborted now because only left or right might be needed. Create them respectively will save
// memory
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
// This function will be called recursively. It will divide the triangleList into three 
// parts: triangles in itself/leftcube/rightcube. It decides the divide boundary based on
// the variance of the centroid of all triangles.
void Cube::insertTriangleList(vector<Triangle*>& triangleList) {
	if (triangleList.size() <= 4) {
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

	// divide triangles into left/right cube by judging whether they are totally in the sub region
	// the leaving triangles will be stored in the current cube
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

	// if it is not the bottom-level, divide itself into smaller cubes.
	Vector3 t_small = small, t_large = large;
	if (leftSequence.size() > 0) {
		t_large.value[lm] = mid;
		left = new Cube(small, t_large);
		left->insertTriangleList(leftSequence);
	}
	if (rightSequence.size() > 0) {
		t_small.value[lm] = mid;
		right = new Cube(t_small, large);
		right->insertTriangleList(rightSequence);
	}

	return;
}


// aborted now; now we consider all triangles are added in a time into the scene
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


// called frequently when tracing ray; decide which triangle in current cube will intersect the ray
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


bool Cube::hitRay(const shared_ptr<Ray> ray, Vector3& inPoint, Vector3& outPoint) {
	float tmin = -MAX_DIS, tmax = MAX_DIS;
	
	Vector3 dir = ray->getDirection(), origin = ray->getOrigin();

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

	inPoint = ray->getOrigin() + ray->getDirection() * tmin;
	outPoint = ray->getOrigin() + ray->getDirection() * tmax;
	return true;
}


// travel down until minimum
// go back and check triangles in the current cube
// only go when there is intersection
Triangle* Cube::hitCloestTriangle(const shared_ptr<Ray> ray, float &distance, Vector3 inPoint, Vector3 outPoint) {
	float min_distance = MAX_DIS, cube_distance = MAX_DIS;

	Triangle* min_Triangle = intersect(ray, &min_distance); // find the intersected triangles in the current cube size
	min_distance = MIN(distance, min_distance);
#if REFINED_CUBE_INTERSECT
	// only called at the beginning of entering the recursive loop;
	if (inPoint == outPoint) {
		bool hit = hitRay(ray, inPoint, outPoint);
		if (!hit) return min_Triangle;
		_ASSERT(inPoint.value[lm] != outPoint.value[lm]);
	}

	Vector3 origin = ray->getOrigin();
	// judge focus is the position of origin and outPoint
	if ((origin.value[lm] - mid)*(outPoint.value[lm] - mid) >= 0) {		// if origin and outPoint on same side of divide plane, just visit one
		Cube* subCube = nullptr;
		if (origin.value[lm] < mid) subCube = left; else subCube = right;
		float t_d = MAX_DIS;
		if (subCube) {
			Triangle* temp_triangle = subCube->hitCloestTriangle(ray, t_d, inPoint, outPoint);
			if (t_d < min_distance) {
				min_distance = t_d;
				min_Triangle = temp_triangle;
			}
		}
	}
	else {	// else: choose the sequence to travel two place; if intersect the first, skip the second
		// interpolate to calculate the intersection point(much better than recalculate in every loop)
		float alpha = (mid - inPoint.value[lm]) / (outPoint.value[lm] - inPoint.value[lm]);
		Vector3 midHit = (outPoint - inPoint)*alpha + inPoint;
		midHit.value[lm] = mid;
		Cube *frontCube = nullptr, *backCube = nullptr;
		if (origin.value[lm] < mid) {
			frontCube = left; backCube = right;
		}
		else {
			frontCube = right; backCube = left;
		}

		Triangle* temp_triangle = nullptr;
		if (frontCube) {
			float t_d = MAX_DIS;
			temp_triangle = frontCube->hitCloestTriangle(ray, t_d, inPoint, midHit);
			if (t_d < min_distance) {
				min_distance = t_d;
				min_Triangle = temp_triangle;
			}
		}
		if (temp_triangle == nullptr && backCube) {
			float t_d = MAX_DIS;
			temp_triangle = backCube->hitCloestTriangle(ray, t_d, midHit, outPoint);
			if (t_d < min_distance) {
				min_distance = t_d;
				min_Triangle = temp_triangle;
			}
		}
	}
#else
	// test the side that intersects first, save some time
	if (left && left->hitRay(ray, cube_distance) && cube_distance < min_distance) {
		float t_d = MAX_DIS;
		Triangle* temp_triangle = left->hitCloestTriangle(ray, t_d, inPoint, outPoint);

		if (t_d < min_distance) {
			min_distance = t_d;
			min_Triangle = temp_triangle;
		}
	}

	if (right && right->hitRay(ray, cube_distance) && cube_distance < min_distance) {
		float t_d = MAX_DIS;
		Triangle* temp_triangle = right->hitCloestTriangle(ray, t_d, inPoint, outPoint);

		if (t_d < min_distance) {
			min_distance = t_d;
			min_Triangle = temp_triangle;
		}
	}
#endif

	distance = min_distance;
	return min_Triangle;

}


int Cube::maxDepth() {
	if (left == nullptr && right == nullptr) return 1;

	int leftDepth = 0, rightDepth = 0;
	if (left) leftDepth = left->maxDepth();
	if (right) rightDepth = right->maxDepth();

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


// add all triangles into triangleList and process them at one time
void BBox::addTriangle(Triangle* triangle) {
	// ensure triangle is smaller than the box
	for (int i = 0; i < 3; i++) {
		if (!triangle->smallerThan(large[i], i) || !triangle->largerThan(small[i], i))
			throw("Triangle is not in the box domain.");
	}

	triangleList.push_back(triangle);
}


// after all triangles are added into the scene, swap triangles into the cube box
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
	// pre-process for unvalid rays
	if (ray == nullptr || ray->getDirection().magnitude() < EPISILON) 
		return nullptr;
	
	Triangle* t = box.hitCloestTriangle(ray, distance, Vector3(), Vector3());

	return t;
}


int BBox::getDepth() {
	return box.maxDepth();
}