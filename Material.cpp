#include "Material.h"


// calculate the transmited ray for diffuse material
LightRate Diff::transmit(
	Vector3 normal, 
	Vector3* hitPoint, 
	Ray* ray, 
	shared_ptr<Ray> refractRay
) {
	// calculate a point randomly on the hemisphere and use as outward direction of ray

	float theta1, theta2;

	bool sameHemis = true;	// used to judge the random reflected ray and in-ray on the different hemisphere
	Vector3 direction(0.0f, 0.0f, 0.0f);
	float unit = ray->getDirection().dot(normal) > 0 ? -1.0f : 1.0f; // used to judge whether normal on the same face with in-ray

	while (sameHemis) {
		theta1 = rand() / (RAND_MAX + 1.0f)*PIDEGREE*360.0f;
		theta2 = (rand() / (RAND_MAX + 1.0f)-0.5f)*PIDEGREE*180.0f;
		direction = Vector3(cos(theta1)*cos(theta2), sin(theta1)*cos(theta2), sin(theta2));
		if (direction.dot(normal*unit) > EPISILON) sameHemis = false;
	}

	ray->setDirection(direction);
	ray->setOrigin(*hitPoint);

	refractRay->setDirection(Vector3());

	return LightRate(1.0f, 0.0f);
}


// calculate the transmited ray for specular material (total reflect)
LightRate Spec::transmit(
	Vector3 normal, 
	Vector3* hitPoint, 
	Ray* ray,
	shared_ptr<Ray> refractRay
) {
	float dot=ray->getDirection().dot(normal);
	Vector3 direction = ray->getDirection() - normal * dot * 2;

	ray->setOrigin(*hitPoint, false);
	ray->setDirection(direction, false);

	refractRay->setDirection(Vector3());

	return LightRate(1.0f, 0.0f);
}


// calculate the transmited ray for reflective material, it will contain the reflect ray and refract ray
// new ray: refractRay is added into the scene
LightRate Refl::transmit(
	Vector3 normal, 
	Vector3* hitPoint, 
	Ray* ray,
	shared_ptr<Ray> refractRay
) {
	_ASSERT(refractivity != 0.0f);

	ray->setOrigin(*hitPoint);
	refractRay->setOrigin(*hitPoint);

	float dot = ray->getDirection().dot(normal);
	Vector3 reflect_direction = ray->getDirection() - normal * dot * 2;

	Vector3 n = normal;
	Vector3 nl = n.dot(ray->getDirection()) < 0 ? n : n * -1;
	bool into = n.dot(nl) > 0;
	float nnt = into ? 1.0f / refractivity : refractivity, ddn = ray->getDirection().dot(nl), cos2t;
	if ((cos2t = 1 - nnt * nnt*(1 - ddn * ddn)) < 0) {  // total internal reflection
		refractRay->setDirection(Vector3());
		ray->setDirection(reflect_direction);
		return LightRate(1.0f, 0.0f);
	}

	Vector3 refract_dir = (ray->getDirection()*nnt - n * ((into ? 1 : -1)*(ddn*nnt + sqrt(cos2t)))).normalize();
	ray->setDirection(reflect_direction);
	refractRay->setDirection(refract_dir);
	float a = refractivity - 1.0f, b = refractivity + 1.0f, R0 = a * a / (b*b), c = 1 - (into ? -ddn : refract_dir.dot(n));
	float Re = R0 + (1 - R0)*c*c*c*c*c, Tr = 1 - Re;

	return LightRate(Re, Tr);
}