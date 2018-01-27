#pragma once

/* 
This class is the main class after we start the program. It will construct the world
 and render the scene with path tracer
*/
#include "Singleton.h"
#include "Raster.h"
#include "BBox.h"
#include "Object.h"
#include <memory>
#define RENDERSTATE bool

class World :public Singleton<World> {
private:
	// variables
	shared_ptr<Raster> raster; // store the rendered raster
	shared_ptr<BBox> bbox;

	// functions
	World();
	~World();

public:
	friend Singleton <World>;

	void initialize();

	RENDERSTATE renderScene();

	void drawScene();

	// add an object (like sphere or rectangle into the world)
	void addObject(shared_ptr<Object> object);
};