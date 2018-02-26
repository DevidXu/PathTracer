#pragma once

// This file defines the operation of a ray and about how to generate the new ray when 
// it hits some material.

#include "Vector.h"
#include <vector>
#include <memory>

class Triangle;
class Material;

class Ray {
private:
	Vector3 origin;		// origin position of ray in each iteration
	Vector3 direction;	// the current direction ray travels along
	
	float intensity;	// the intensity of ray (1.0 initial value)
	int depth;			// the number of iterations experience by a ray
	vector<Vector3> positions, directions;	// used to monitor the direction of the ray

public:
	Ray() {
		depth = 0;
		//intensity = 1.0f;
	};
	~Ray() {};

	Ray(Vector3 o, Vector3 d, float in = 1.0f) :origin(o), direction(d) {
		intensity = in;
		depth = 0;
	};

	Ray(const Ray& ray); 

	void operator=(const Ray& ray);

	void incDepth() { depth += 1; }

	void setOrigin(Vector3 o);

	void setDirection(Vector3 d);

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

	void transmit(
		Triangle* triangle, 
		Vector3* hitPoint, 
		shared_ptr<Material> material, 
		shared_ptr<Ray> refractRay
	);

};

typedef vector<shared_ptr<Ray>> PixelRays;