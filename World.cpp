// This cpp file is the realization of World.h
#include "World.h"

World::World() {
	raster = make_shared<Raster>(); // initialize the rasetr (each pixel)
	bbox = make_shared<BBox>();		// initialize the bounding box (Cornell box of 4:4:4)
}

World::~World() {
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