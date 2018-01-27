#include "BBox.h"

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
	try {
		for (int i = 0; i < 3; i++)
			if (!triangle->smallerThan(large[i], i) || !triangle->largerThan(small[i], i)) 
				throw("Triangle is not in the box domain.");
	}
	catch (exception e) {
		LOGPRINT("Meet error when adding triangles to the bounding box");
		LOGPRINT(e.what());
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