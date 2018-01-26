#pragma once

// This file is the base class for all the subjects.
/*
It will has two main properties: material and shape. It supports different
shapes, like cube, sphere, anything that has a triangle construction. Moreover,
it should support tesselation.
*/

#include "Shape.h"
#include "Material.h"

class Object {
private:
	shared_ptr<Shape> shape;
	shared_ptr<Material> material;

public:
	Object() {
		shape = make_shared<Shape>();
		material = make_shared<Material>();
	}

	Object(shared_ptr<Shape> m_shape) :shape(m_shape) {};

	Object(shared_ptr<Shape> m_shape, shared_ptr<Material> m_material) 
		:shape(m_shape), material(m_material) {};
	
	Mesh getMesh() {
		return shape->getMesh();
	}

	~Object() {};

};