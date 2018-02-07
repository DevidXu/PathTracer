#include "Ray.h"
#include "Material.h"

void Ray::initialize(Vector3 o, Vector3 d) {
	origin = o;
	direction = d;
	depth = 0;
	intensity = 1.0f;
	positions.push_back(origin);
	return;
}


void Ray::transmit(
	Triangle* triangle, 
	Vector3* hitPoint, 
	shared_ptr<Material> material, 
	shared_ptr<Ray> refractRay
) {
	// decide the returned ray(fix on current ray) according to the material kind
	positions.push_back(*hitPoint);

	//shared_ptr<Ray>	ray(this);
	material->transmit(triangle, hitPoint, this, refractRay);

	return;
}