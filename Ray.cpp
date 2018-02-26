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


void Ray::setOrigin(Vector3 o) {
	origin = o;
	positions.push_back(o);
}


void Ray::setDirection(Vector3 d) {
	direction = d;
	directions.push_back(d);
}

void Ray::initialize(Vector3 o, Vector3 d) {
	setOrigin(o);
	setDirection(d);
	depth = 0;
	intensity = 1.0f;
	return;
}


void Ray::transmit(
	Triangle* triangle, 
	Vector3* hitPoint, 
	shared_ptr<Material> material, 
	shared_ptr<Ray> refractRay
) {
	// decide the returned ray(fix on current ray) according to the material kind
	material->transmit(triangle, hitPoint, this, refractRay);

	return;
}