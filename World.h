#pragma once

/* 
This class is the main class after we start the program. It will construct the world
 and render the scene with path tracer
*/
#include "Singleton.h"
#include "Raster.h"
#include "Object.h"
#define RENDERSTATE bool

class World :public Singleton<World> {
private:
	// variables
	Raster* raster;

	// functions
	World();

public:
	friend Singleton <World>;

	RENDERSTATE renderScene();
	void drawScene();
	void addObject(Object object);
};