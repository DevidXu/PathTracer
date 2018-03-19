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

	string name;

public:

	void	setEmissive(Vector3 e)	{ material->setEmissive(e); }
	void	setColor(Vector3 c)		{ material->setColor(c); }
	void	setSpecular(Vector3 s)	{ material->setSpecular(s); }
	void	setRoughness(float r)	{ material->setRoughness(r); }

	Vector3 getEmissive()			{ return material->getEmissive(); }
	Vector3 getColor()				{ return material->getColor(); }
	Vector3 getSpecular()			{ return material->getSpecular(); }
	float	getRoughness()			{ return material->getRoughness(); }
	string	getName()				{ return name; }


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
	

	Object(shared_ptr<Shape> m_shape, shared_ptr<Material> m_material, string n = "Object") {
		shape = m_shape;
		material = m_material;
		name = n;

		shape->setOwner(this);
	}


	shared_ptr<Shape> getShape() { return shape; }

	Mesh getMesh() {
		return shape->getMesh();
	}

	Vector3 getCenter() {
		return shape->getCenter();
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