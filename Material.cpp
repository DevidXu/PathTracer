#include "Material.h"


// calculate the transmited ray for diffuse material
void Diff::transmit(
	Vector3 normal, 
	Vector3* hitPoint, 
	Ray* ray
) {
	// calculate a point randomly on the hemisphere and use as outward direction of ray
	bool sameHemis = true;	// used to judge the random reflected ray and in-ray on the different hemisphere
	Vector3 direction(0.0f, 0.0f, 0.0f);
	float unit = ray->getDirection().dot(normal) > 0 ? -1.0f : 1.0f; // used to judge whether normal on the same face with in-ray
	
	while (sameHemis) {
	    float theta1, theta2;
		theta1 = rand() / (RAND_MAX + 1.0f)*PIDEGREE*360.0f;
		theta2 = (rand() / (RAND_MAX + 1.0f)-0.5f)*PIDEGREE*180.0f;
		direction = Vector3(cos(theta1)*cos(theta2), sin(theta1)*cos(theta2), sin(theta2));
		if (direction.dot(normal*unit) > EPISILON) sameHemis = false;
	}
	
	//direction = brdf.diffReflectDir(ray->getDirection(), normal);
	//if (direction.dot(normal*unit) > EPISILON) sameHemis = false;

	ray->setDirection(direction);
	ray->setOrigin(*hitPoint);

	return;
}


// calculate the transmited ray for specular material (total reflect)
void Spec::transmit(
	Vector3 normal, 
	Vector3* hitPoint, 
	Ray* ray
) {
	float dot=ray->getDirection().dot(normal);
	Vector3 direction = ray->getDirection() - normal * dot * 2;

	ray->setOrigin(*hitPoint, false);
	ray->setDirection(direction, false);

	return;
}


// calculate the transmited ray for reflective material, it will contain the reflect ray and refract ray
// new ray: refractRay is added into the scene
void Refl::transmit(
	Vector3 normal, 
	Vector3* hitPoint, 
	Ray* ray
) {
	_ASSERT(refractivity != 0.0f);

	ray->setOrigin(*hitPoint);

	float dot = ray->getDirection().dot(normal);

#if DIFFUSE_SPEC
	bool sameHemis = true;
	float theta1, theta2;
	Vector3 temp_direction;
	float unit = ray->getDirection().dot(normal) > 0 ? -1.0f : 1.0f;
	while (sameHemis) {
		theta1 = rand() / (RAND_MAX + 1.0f)*PIDEGREE*360.0f;
		theta2 = (rand() / (RAND_MAX + 1.0f) - 0.5f)*PIDEGREE*180.0f;
		temp_direction = Vector3(cos(theta1)*cos(theta2), sin(theta1)*cos(theta2), sin(theta2));
		if (temp_direction.dot(normal*unit) > EPISILON) sameHemis = false;
	}
	Vector3 reflect_direction = temp_direction;
#else
	Vector3 reflect_direction = ray->getDirection() - normal * dot * 2;
#endif

	Vector3 n = normal;
	Vector3 nl = n.dot(ray->getDirection()) < 0 ? n : n * -1;
	bool into = n.dot(nl) > 0;
	float nnt = into ? 1.0f / refractivity : refractivity, ddn = ray->getDirection().dot(nl), cos2t;
	if ((cos2t = 1 - nnt * nnt*(1 - ddn * ddn)) < 0) {  // total internal reflection
		ray->setDirection(reflect_direction);
		return;
	}

	Vector3 refract_dir = (ray->getDirection()*nnt - n * ((into ? 1 : -1)*(ddn*nnt + sqrt(cos2t)))).normalize();
	float a = refractivity - 1.0f, b = refractivity + 1.0f, R0 = a * a / (b*b), c = 1 - (into ? -ddn : refract_dir.dot(n));
	float Re = R0 + (1 - R0)*c*c*c*c*c, Tr = 1 - Re;
	if (rand() / (RAND_MAX + 1.0f) > Re) {
		ray->setDirection(refract_dir);
	}
	else {
		ray->setDirection(reflect_direction);
	}
	return;
}