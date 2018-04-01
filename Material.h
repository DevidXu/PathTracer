#pragma once

// This defines the material of the object. It will contain three kinds of material
// Emissive, Matte, Lambertian, Reflective, Perfect Specular
// It serves as a private variable of Object

#include "Singleton.h"
#include "Vector.h"
#include "Ray.h"
#include "Triangle.h"
#include "Constants.h"
#include "BRDF.h"

class Material:public Singleton<Material> {
private:
	Vector3 color;		// this is the diffuse color for the material
	Vector3 emissive;   // defines the self-shining color
	Vector3 specular;   // defines the specular color (like metal or special shiny things) 

public:
	BRDF brdf;

	// if no refract ray, remember to set direction of refract ray as 0.0f
	Material(
		Vector3 c = Vector3(), 
		Vector3 e = Vector3(), 
		Vector3 s = Vector3(), 
		float rough = 0.0f, 
		float reflect = 0.0f
	):
		color(c),emissive(e),specular(s), brdf(rough, reflect)
	{

	}

	Vector3 getColor()		{ return color; }
	Vector3 getEmissive()	{ return emissive; }
	Vector3 getSpecular()	{ return specular; }
	float	getRoughness()	{ return brdf.getRoughness(); }
	float	getReflectance() { return brdf.getReflectance(); }

	void	setColor(Vector3 c)		{ color = c; }
	void	setEmissive(Vector3 e)	{ emissive = e; }
	void	setSpecular(Vector3 s)	{ specular = s; }
	void	setRoughness(float r)	{ brdf.setRoughness(r); }
	void	setReflectance(float r) { brdf.setReflectance(r); }


	virtual void transmit(
		Vector3 normal,
		Vector3* hitPoint,
		Ray* ray
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
	Diff(
		Vector3 c = Vector3(), 
		Vector3 e = Vector3(), 
		Vector3 s = Vector3(), 
		float rough = 0.0f, 
		float reflect = 0.0f
	):
		Material(c, e, s, rough, reflect)
	{
	
	}

	virtual void transmit(
		Vector3 normal, 
		Vector3* hitPoint, 
		Ray* ray
	);

	virtual bool isRefl() { return false; }
};


class Spec :public Material {	// specular material
private:

public:
	Spec(
		Vector3 c = Vector3(), 
		Vector3 e = Vector3(), 
		Vector3 s = Vector3(), 
		float rough = 0.0f,
		float reflect = 0.0f
	):
		Material(c, e, s, rough, reflect)
	{

	}

	virtual void transmit(
		Vector3 normal,
		Vector3* hitPoint, 
		Ray* ray
	);

	virtual bool isRefl() { return false; }
};


class Refl :public Material {	// reflective material
private:
	float refractivity;			// reflectivity rate of the material (air = 1)
	float bounding_angle;		// the maximum angle that allows a ray to flow from the reflective object

public:
	Refl(
		float r = 0.0f, 
		Vector3 c = Vector3(), 
		Vector3 e = Vector3(), 
		Vector3 s = Vector3(), 
		float rough = 0.0f,
		float reflect = 0.0f
	) :
		Material(c, e, s, rough, reflect), refractivity(r)
	{
		_ASSERT(r > 1.0f);
		bounding_angle = asin(1 / r);
	};

	void setRefractivity(float r){
		_ASSERT(r > 1.0f);
		bounding_angle = asin(1 / r);
		return;
	}

	virtual void transmit(
		Vector3 normal, 
		Vector3* hitPoint, 
		Ray* ray
	);

	virtual bool isRefl() { return true; }
};	