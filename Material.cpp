#include "Material.h"
#include <time.h>

// calculate the transmited ray for diffuse material
void Diff::transmit(
	Triangle* triangle, 
	Vector3* hitPoint, 
	Ray* ray, 
	shared_ptr<Ray> refractRay
) {
	// calculate a point randomly on the hemisphere and use as outward direction of ray
	srand((unsigned int) time(NULL));

	Vector3 normal = triangle->getNormal();
	float theta1, theta2;
	if (normal.value[0] == 0.0f) {
		if (normal.value[1] > 0.0f) theta1 = PI;
		else theta1 = 0.0f;
	}
	else {
		theta1 = atan(normal.value[1] / normal.value[0]);
		if (normal.value[1] < 0.0f) theta1 += PI;
	}

	if (normal.value[0] == 0.0f && normal.value[1] == 0.0f)
		theta2 = normal.value[2] > 0.0f ? PI / 2 : -PI / 2;
	else {
		float length = sqrt(normal.value[0] * normal.value[0] + normal.value[1] * normal.value[1]);
		theta2 = atan(length / normal.value[2]);
		if (normal.value[2] < 0.0f) theta2 = 0.0f - theta2;
	}

	theta1 += rand() / (RAND_MAX + 1.0f)*PIDEGREE*180.0f;
	theta2 += rand() / (RAND_MAX + 1.0f)*PIDEGREE*90.0f;

	Vector3 direction(cos(theta1)*cos(theta2), sin(theta1)*cos(theta2), sin(theta2));

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

	direction = ray->getDirection() - triangle->getNormal()*dot * 2;
	ray->setDirection(direction); // ray turns into the reflected ray

	if (dot >= 0.0f) { // if ray goes from dense material to sparse material (glass to air); normal face unit is utilized here

		float theta = acos(dot);
		if (theta >= bounding_angle || dot == 0.0f) {  // if the angle > bounding angle, total reflection
			refractRay = nullptr;
			return;
		}

		float out_theta = asin(refractivity*sin(theta)) - EPISILON;
		_ASSERT(out_theta < PI / 2);

		refractRay = make_shared<Ray>(*ray);

		Vector3 temp = triangle->getNormal()*dot;
		
		direction = (temp + (ray->getDirection() - temp) / tan(theta)*tan(out_theta)).normalize();

		refractRay->setDirection(direction);
	}
	else {
		_ASSERT(refractivity != 0.0f);
		float theta = acos(dot);
		float out_theta = asin(sin(theta)/refractivity);
		
		refractRay = make_shared<Ray>(*ray);

		Vector3 temp = triangle->getNormal()*dot;

		direction = (temp + (ray->getDirection() - temp) / tan(theta)*tan(out_theta)).normalize();

		refractRay->setDirection(direction);
	}

	return;
}