#pragma once

// This defines the material of the object. It will contain three kinds of material
// Emissive, Matte, Lambertian, Reflective, Perfect Specular

#include "Singleton.h"
#include "Vector.h"
#include "Ray.h"
#include "Triangle.h"
#include "Constants.h"


class Material:public Singleton<Material> {
private:

public:
	virtual void transmit(
		Triangle* triangle,
		Vector3* hitPoint,
		Ray* ray,
		shared_ptr<Ray> refractRay
	) {
		return;
	}

	virtual bool isRefl() {
		return false;
	}
};


class Diff :public Material {	// diffuse material
private:

public:
	virtual void transmit(
		Triangle* triangle, 
		Vector3* hitPoint, 
		Ray* ray,
		shared_ptr<Ray> refractRay
	);

	virtual bool isRefl() { return false; }
};


class Spec :public Material {	// specular material
private:

public:
	virtual void transmit(
		Triangle* triangle, 
		Vector3* hitPoint, 
		Ray* ray,
		shared_ptr<Ray> refractRay
	);

	virtual bool isRefl() { return false; }
};


class Refl :public Material {	// reflective material
private:
	float refractivity;			// reflectivity rate of the material (air = 1)
	float bounding_angle;		// the maximum angle that allows a ray to flow from the reflective object

public:
	Refl(float r = 0.0f) :refractivity(r) {
		_ASSERT(r > 1.0f);
		bounding_angle = asin(1 / r);
	};

	void setRefractivity(float r){
		_ASSERT(r > 1.0f);
		bounding_angle = asin(1 / r);
		return;
	}

	virtual void transmit(
		Triangle* triangle, 
		Vector3* hitPoint, 
		Ray* ray,
		shared_ptr<Ray> refractRay
	);

	virtual bool isRefl() { return true; }
};	