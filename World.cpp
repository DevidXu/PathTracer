// This cpp file is the realization of World.h
#include "World.h"

World::World() {
	raster = make_shared<Raster>(); // initialize the rasetr (each pixel)
	bbox = make_shared<BBox>(
		Vector3(0.0f,0.0f,0.0f),
		Vector3(BOX_LENGTH, BOX_WIDTH, BOX_HEIGHT)
		);		// initialize the bounding box (Cornell box of 4:4:4)

	LOGPRINT("This is the realization of a path tracer!");
}

World::~World() {
	LOGPRINT("Path tracer is completed successfully!");
}


void World::initialize() {
	shared_ptr<Object> box, obj1, obj2;

	try {
		box = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(0.0, 0.0, 0.0),
				Vector3(BOX_LENGTH, BOX_WIDTH, BOX_HEIGHT)
				)
			);

		obj1 = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(1.0, 1.0, 1.0),
				Vector3(2.0, 2.0, 2.0)
				)
			);


		obj2 = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(2.0, 0.0, 2.0),
				Vector3(4.0, 2.0, 3.0)
				)
			);
	}
	catch (exception e) {
		LOGPRINT("Meet error when creating the shapes");
	}

	try {
		if (box == nullptr or obj1 == nullptr or obj2 == nullptr)
			throw("No pointer pointing to the shape created before.");
		addObject(box);
		addObject(obj1);
		addObject(obj2);
	}
	catch (exception e) {
		LOGPRINT(e.what());
		LOGPRINT("Meet error when adding objects into the box");
	}

	return;
}


// This function begins path tracing
RENDERSTATE World::renderScene() {
	return true;
}

// This function saves RGB data and store it as a graph.
void World::drawScene() {
	return raster->Render();
}


void World::addObject(shared_ptr<Object> object) {
	bbox->addMesh(object->getMesh());
	return;
}