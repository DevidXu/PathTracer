#pragma once

// This file defines the calculation formulas of BRD, including calculate micro face,
// reflection rate, etc.. It contains Hammersley sample and importance sample.
// Meanwhile, many important material data is stored in this function. BRDF serves as
// a private variable of Material class

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
	BRDF(float rough, float reflect) :roughness(rough), reflectance(reflect) 
	{
		k = calculateIntegral();
	}

	float	getRoughness()					{ return roughness; }
	float	getReflectance()				{ return reflectance; }

	void	setRoughness(float rough)		{ roughness = rough; }
	void	setReflectance(float reflect)	{ reflectance = reflect; }


	float calculateIntegral();

	// v:point to the surface
	float getBRDF(Vector3 l, Vector3 v, Vector3 n);

	// probability distribution function of BRDF
	float getPDF(Vector3 l, Vector3 v, Vector3 n);

	// employ BRDF introducted by Klayge
	Vector3 diffReflectDir(Vector3 v, Vector3 n);
	Vector2 hammersleyPoints(int i, int N);
	Vector2 importanceSampling(Vector2 point);
};