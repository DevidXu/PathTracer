#pragma once

// This file defines the operation of a ray and about how to generate the new ray when 
// it hits some material.

#include "Vector.h"
#include "Material.h"
#include <vector>
#include <memory>

class Triangle;

class Ray {
private:
	Vector3 origin;		// origin position of ray in each iteration
	Vector3 direction;	// the current direction ray travels along
	float intensity;	// the intensity of ray (1.0 initial value)
	int depth;			// the number of iterations experience by a ray
	vector<Vector3> positions;

public:
	Ray() {};
	~Ray() {};

	Ray(Vector3 o, Vector3 d, float inten = 1.0f) :origin(o), direction(d) {
		intensity = inten;
	};

	void setOrigin(Vector3 o)		{ origin = o; }
	void setDirection(Vector3 d)	{ direction = d; }
	void setIntensity(float i) {
		_ASSERT(i <= 1.0f);
		intensity = i;
	}

	int		getDepth()		{ return depth; }
	Vector3 getOrigin() { return origin; }
	Vector3 getDirection()	{ return direction; }
	float	getIntensity()	{ return intensity; }

	void initialize(Vector3 o, Vector3 d);

	void pushTrace(Vector3 position) {
		positions.push_back(position);
	}

	void transmit(Triangle* triangle, shared_ptr<Material> material);
};

typedef vector<shared_ptr<Ray>> PixelRays;