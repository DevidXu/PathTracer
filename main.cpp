// This is the entrance of the whole project
/*
Project Name: Path Tracer
Starting date: 01/22/2018
Finishing date: ---
Writer: Xu Dewei
*/

#include "iostream"
#include "Singleton.h"
#include "Debugging.h"
#include "Constants.h"
#include "World.h"
#include "Shape.h"
using namespace std;

int main()
{
	Debugging::getInstance()->moduleTest();

	World* world = World::getInstance();

	// intialize the bounding box and add two rectangles.s
	world->initialize();

	// render scene
	world->renderScene();

	// draw each pixel of the raster on an bmp map
	world->drawScene();

	system("pause");
}