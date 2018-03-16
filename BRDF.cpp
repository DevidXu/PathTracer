#include "BRDF.h"


// Geometrical Attenuation Factor: Kelemen
// pay attention: n.l, n.v has been removed here to simplify calculation
float GFunction(Vector3 l, Vector3 v, float alpha) {
	Vector3 h = (v + l).normalize();
	float dotVH = v.dot(h);

	return 0.25f / float(pow(dotVH, 2));
}


// Normal Distribution Function: D(h)
float DFunction(Vector3 n, Vector3 m, float alpha) {
	return alpha * alpha / PI / float(pow(1.0f + float(pow(n.dot(m), 2))*(alpha*alpha - 1), 2));
}


// Fresnel Equations parameters
// Use Schlick Functions
float FFunction(Vector3 v, Vector3 h, float reflectance) {
	return reflectance + (1 - reflectance)*float(pow(1 - v.dot(h), 5));
}


float BRDF::getBRDF(Vector3 l, Vector3 v, Vector3 n) {
	v = v * -1.0f; // reverse the direction of v to ensure the consistency of v & l
	Vector3 h = (v + l).normalize();
	float alpha = roughness * roughness;

	float F = FFunction(v, h, reflectance);

	float D = DFunction(n, h, alpha);
	
	float G = GFunction(l, v, alpha); // use G(GGX) to calculate

	return F * D * G; // pay attention n.l, n.v has been removed when calculating G
}


float BRDF::getPDF(Vector3 l, Vector3 v, Vector3 n) {

	return k * getBRDF(l, v, n)*n.dot(l);
}