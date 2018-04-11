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
#include "Interface.h"
#include <memory>
#include <sstream>

class World :public Singleton<World> {
private:
	// important variables
	shared_ptr<BBox> bbox;
	shared_ptr<Camera> camera;
	shared_ptr<Interface> winFace; // windows Interface
	vector<shared_ptr<Object>> objects;

	// auxiliary variables
	bool rd_exit = false, rd_pause = true;	// pause at the begining
	bool rd_begin = false, rd_finish = false;

	int height_begin = 0, height_end = HEIGHT, width_begin = 0, width_end = WIDTH;

	// functions
	World();
	~World();

public:
	friend Singleton <World>;
	friend class Interface;

	void initialize();

	bool renderScene();
	void renderPixel(int i, int j, int num, bool showPath);

	void drawScene();

	// add an object (like sphere or rectangle into the world)
	void addObject(shared_ptr<Object> object);

	Vector3 intersectTest(shared_ptr<Ray> ray, Triangle* &patch, float &distance);

	Vector3 pathTracing(shared_ptr<Ray> ray);

	shared_ptr<Interface> getInterface() { return winFace; }
};
