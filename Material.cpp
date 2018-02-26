#include "Material.h"


// calculate the transmited ray for diffuse material
void Diff::transmit(
	Triangle* triangle, 
	Vector3* hitPoint, 
	Ray* ray, 
	shared_ptr<Ray> refractRay
) {
	// calculate a point randomly on the hemisphere and use as outward direction of ray

	Vector3 normal = triangle->getNormal();
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

	return;
}


// calculate the transmited ray for specular material (total reflect)
void Spec::transmit(
	Triangle* triangle, 
	Vector3* hitPoint, 
	Ray* ray,
	shared_ptr<Ray> refractRay
) {
	float length=(triangle->getNormal()).dot(ray->getDirection());
	Vector3 direction = triangle->getNormal()*length * 2 - ray->getDirection();
	direction = direction * (-1.0f);  // calculate the outward ray direction

	ray->setOrigin(*hitPoint);
	ray->setDirection(direction);

	return;
}


// calculate the transmited ray for reflective material, it will contain the reflect ray and refract ray
// new ray: refractRay is added into the scene
void Refl::transmit(
	Triangle* triangle, 
	Vector3* hitPoint, 
	Ray* ray,
	shared_ptr<Ray> refractRay
) {
	_ASSERT(refractivity != 0.0f);

	ray->setOrigin(*hitPoint);

	Vector3 direction;

	float dot = ray->getDirection().dot(triangle->getNormal());

	if (dot == 1.0f || dot == -1.0f) {
		refractRay = nullptr;
		return;
	}


	// calculate the refracted ray first
	if (dot >= 0.0f) { // if ray goes from dense material to sparse material (glass to air); normal face unit is utilized here

		float theta = acos(dot);
		if (theta >= bounding_angle || dot == 0.0f) {  // if the angle > bounding angle, total reflection
			refractRay = nullptr;
			return;
		}

		float out_theta = asin(refractivity*sin(theta)) - EPISILON;
		_ASSERT(out_theta < PI / 2);

		*refractRay = *ray;

		Vector3 temp = triangle->getNormal()*dot;
		
		direction = (temp + (ray->getDirection() - temp) / tan(theta)*tan(out_theta)).normalize();

		refractRay->setDirection(direction);
	}
	else {
		_ASSERT(refractivity != 0.0f);
		float theta = acos(-dot);
		float out_theta = asin(sin(theta)/refractivity);
		
		*refractRay = *ray;

		Vector3 temp = triangle->getNormal()*dot;

		direction = (temp + (ray->getDirection() - temp) / tan(theta)*tan(out_theta)).normalize();

		refractRay->setDirection(direction);
	}

	// calculate the transmitted reflected ray (change on original ray)
	direction = ray->getDirection() - triangle->getNormal()*dot * 2;
	ray->setDirection(direction); // ray turns into the reflected ray


	return;
}