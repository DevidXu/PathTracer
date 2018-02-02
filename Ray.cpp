#include "Ray.h"

void Ray::initialize(Vector3 o, Vector3 d) {
	origin = o;
	direction = d;
	depth = 0;
	intensity = 1.0f;
	positions.push_back(origin);
	return;
}


void Ray::transmit(Triangle* triangle, shared_ptr<Material> material) {

	return;
}