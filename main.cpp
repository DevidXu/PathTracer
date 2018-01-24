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
using namespace std;

int main()
{
	Debugging::getInstance()->moduleTest();

	cout << "This is the realization of a path tracer!" << endl;
	World* world = World::getInstance();
	// initial the cornell box
	// world->addObject();
	// add objects
	// render scene
	//world->drawScene();
	system("pause");
}