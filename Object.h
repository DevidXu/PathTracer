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
	shared_ptr<Shape>		shape;
	shared_ptr<Material>	material;

	Vector3 emissive, color;

public:
	Vector3 setEmissive(Vector3 e)	{ emissive = e; }
	Vector3 setColor(Vector3 c)		{ color = c; }
	Vector3 getEmissive()			{ return emissive; }
	Vector3 getColor()				{ return color; }


	Object() {
		shape = make_shared<Shape>();
		shape->setOwner(this);

		material = make_shared<Material>();
	}

	
	Object(shared_ptr<Shape> m_shape) :shape(m_shape) {
		shape->setOwner(this);
	};

	
	Object(shared_ptr<Shape> m_shape, shared_ptr<Material> m_material) 
		:shape(m_shape), material(m_material) {
		shape->setOwner(this);
	};
	

	Object(shared_ptr<Shape> m_shape, shared_ptr<Material> m_material, Vector3 c, Vector3 e) {
		shape = m_shape;
		material = m_material;
		color = c;
		emissive = e;

		shape->setOwner(this);
	}


	shared_ptr<Shape> getShape() { return shape; }

	Mesh getMesh() {
		return shape->getMesh();
	}

	
	shared_ptr<Material> getMaterial() { return material; }

	
	void tessellate(float iterations) {
#ifndef TESSELLATE
		return;
#endif

		shape->tessellate(iterations, this);

		return;
	};

	
	~Object() {};

	
	friend class Triangle;
};