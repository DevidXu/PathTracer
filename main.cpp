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
#include "World.h"
#include "Shape.h"
using namespace std;

int main()
{
	Debugging::getInstance()->moduleTest();

	cout << "This is the realization of a path tracer!" << endl;
	World* world = World::getInstance();
	
	shared_ptr<Object> obj1 = make_shared<Object>(
		make_shared<Rectangle>(
			Vector3(1.0, 1.0, 1.0),
			Vector3(2.0, 2.0, 2.0)
			)
		);

	world->addObject(obj1);
	// add objects
	// render scene

	// draw each pixel of the raster on an bmp map
	world->drawScene();

	system("pause");
}