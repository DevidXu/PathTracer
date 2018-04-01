// This is the entrance of the whole project
/*
Program Name:	Path Tracer
Starting date:	01/22/2018
Finishing date: ---
Writer:			Xu Dewei
School:			Shanghai Jiao Tong University
*/

#include "Singleton.h"
#include "Debugging.h"
#include "Constants.h"
#include "World.h"
#include "Interface.h"
#include "Shape.h"
#include <thread>
#include <time.h>
using namespace std;


void cvInterface(shared_ptr<Interface> window) {
	window->beginMessageLoop();
}

int main()
{
	//Debug->moduleTest();
	srand((unsigned int)time(NULL));

	World* world = World::getInstance();

	// intialize the bounding box and add two rectangles.s
	world->initialize();

	// call the GUI interface thread to start
	thread window(cvInterface, world->getInterface());

	// render scene
	world->renderScene();

	// wait for the window interface finished
	window.join();
}