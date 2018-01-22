// This cpp file is the realization of World.h
#include "World.h"

World::World() {
	raster = Raster::getInstance();
}

// This function begins path tracing
RENDERSTATE World::renderScene() {
	return true;
}

// This function saves RGB data and store it as a graph.
void World::drawScene() {
	return raster->Render();
}

void World::addObject(Object object) {
	
	return;
}