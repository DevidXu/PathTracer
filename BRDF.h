#pragma once

#include "Singleton.h"
#include "Constants.h"
#include "Vector.h"

class BRDF {
private:
	// need completed calculation by mathematica
	float k; // 1.0f for energy-conserving surface
	float roughness;
	float reflectance;

public:
	// v:point to the surface
	float getBRDF(Vector3 l, Vector3 v, Vector3 n);

	// probability distribution function of BRDF
	float getPDF(Vector3 l, Vector3 v, Vector3 n);
};