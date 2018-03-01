#include "Ray.h"
#include "Material.h"


Ray::Ray(const Ray& ray) {
	origin = ray.origin;
	direction = ray.direction;
	depth = ray.depth;
	intensity = ray.intensity;
	positions = ray.positions;
	directions = ray.directions;
}


void Ray::operator=(const Ray& ray) {
	origin = ray.origin;
	direction = ray.direction;
	depth = ray.depth;
	intensity = ray.intensity;
	positions = ray.positions;
	directions = ray.directions;
}


void Ray::setOrigin(Vector3 o, bool push) {
	origin = o;
	if (push) positions.push_back(o);
	else { 
		positions[positions.size()-1] = o; 
	}
}


void Ray::setDirection(Vector3 d, bool push) {
	direction = d;
	if (push) directions.push_back(d);
	else {
		directions[directions.size()-1] = d;
	}
}

void Ray::initialize(Vector3 o, Vector3 d) {
	setOrigin(o);
	setDirection(d);
	depth = 0;
	intensity = 1.0f;
	return;
}


LightRate Ray::transmit(
	Vector3 normal, 
	Vector3* hitPoint, 
	shared_ptr<Material> material, 
	shared_ptr<Ray> refractRay
) {
	// decide the returned ray(fix on current ray) according to the material kind
	return material->transmit(normal, hitPoint, this, refractRay);
}