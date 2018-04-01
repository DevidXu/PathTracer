#include "Ray.h"
#include "Material.h"


Ray::Ray(const Ray& ray) {
	origin = ray.origin;
	direction = ray.direction;
	depth = ray.depth;
	intensity = ray.intensity;
	positions = ray.positions;
	directions = ray.directions;
	normals = ray.normals;
}


void Ray::operator=(const Ray& ray) {
	origin = ray.origin;
	direction = ray.direction;
	depth = ray.depth;
	intensity = ray.intensity;
	positions = ray.positions;
	directions = ray.directions;
	normals = ray.normals;
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


void Ray::showPath() {
	cout << "Positions: ";
	for (auto& pos : positions) {
		cout << pos << " ";
	}
	cout << endl << "Direction: ";
	for (auto& dir : directions) {
		cout << dir << " ";
	}
	cout << endl;
	return;
}


void Ray::setNormal(Vector3 n, bool push) {
	if (push) normals.push_back(n);
	else {
		normals[normals.size() - 1] = n;
	}
}


void Ray::initialize(Vector3 o, Vector3 d) {
	setOrigin(o);
	setDirection(d);
	setNormal(CAMERA_FORWARD);
	depth = 0;
	intensity = 1.0f;
	return;
}


void Ray::transmit(
	Vector3 normal, 
	Vector3* hitPoint, 
	shared_ptr<Material> material
) {
	// decide the returned ray(fix on current ray) according to the material kind
	return material->transmit(normal, hitPoint, this);
}