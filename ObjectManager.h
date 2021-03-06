#pragma once

// This file manage all the objects in the scene.

#include "Singleton.h"
#include "Object.h"

typedef std::vector<shared_ptr<Object>> ObjList;

class ObjectManager :public Singleton<ObjectManager> {
private:
	ObjList objList;

public:
	void initialize();

	ObjList* getObjectList() { return &objList; }

};