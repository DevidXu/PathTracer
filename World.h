#pragma once

/* 
This class is the main class after we start the program. It will construct the world
 and render the scene with path tracer
*/
#include "Singleton.h"
#include "BBox.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Ray.h"
#include "Constants.h"
#include <memory>
#include <sstream>
#define RENDERSTATE bool

class World :public Singleton<World> {
private:
	// variables
	shared_ptr<BBox> bbox;
	shared_ptr<Camera> camera;
	vector<shared_ptr<Object>> objects;

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

	Vector3 pathTracing(shared_ptr<Ray> ray);
};