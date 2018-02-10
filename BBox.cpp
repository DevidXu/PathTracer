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

	Vector3 mid = (small + large)*0.5;
	
	int dimension = int(lm);
	Vector3 t_small = small, t_large = large;
	try {
		t_large.value[dimension] = mid[dimension];
		left = new Cube(small, t_large, LayerMark((lm + 1) % 3));

		t_small.value[dimension] = mid[dimension];
		right = new Cube(t_small, large, LayerMark((lm + 1) % 3));
	}
	catch (exception e) {
		LOGPRINT("Meet error when dividing the cube");
		LOGPRINT(e.what());
	}

	return;
}


void Cube::insertTriangle(Triangle* triangle) {
	bool within = true;
	Vector3 mid = (small + large)*0.5;

	if (triangle->smallerThan(mid[lm], lm)) {
		divideCube();
		_ASSERT(left != nullptr);
		left->insertTriangle(triangle);
		return;
	}

	if (triangle->largerThan(mid[lm], lm)) {
		divideCube();
		_ASSERT(right != nullptr);
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
			if (hTriangle == nullptr || temp_distance < distance) {
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

	if (left && left->hitRay(ray, cube_distance) && cube_distance < min_distance) {
		float t_ld = MAX_DIS;
		Triangle* t_left_triangle = left->hitCloestTriangle(ray, t_ld);

		if (t_ld < min_distance) {
			min_distance = t_ld;
			min_Triangle = t_left_triangle;
		}
	}

	if (right && right->hitRay(ray, cube_distance) && cube_distance < min_distance) {
		float t_rd = MAX_DIS;
		Triangle* t_right_triangle = right->hitCloestTriangle(ray, t_rd);

		if (t_rd < min_distance) {
			min_distance = t_rd;
			min_Triangle = t_right_triangle;
		}
	}

	distance = min_distance;

	return min_Triangle;

}


BBox::BBox() {
	small = Vector3(0.0f, 0.0f, 0.0f);
	large = Vector3(1.0f, 1.0f, 1.0f);

	box.setCube(small, large);
}


BBox::BBox(Vector3 s, Vector3 l) {
	small = s;
	large = l;

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

	box.insertTriangle(triangle);
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

	LOGPRINT("Mesh is loaded successfully.");

	return;
}


Triangle* BBox::intersect(shared_ptr<Ray> ray, float &distance) {
	ray->incDepth();
	Triangle* t = box.hitCloestTriangle(ray, distance);

	return t;
}