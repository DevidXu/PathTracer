#include "Camera.h"


Camera::Camera() {
	raster = make_shared<Raster>(HEIGHT, WIDTH); // initialize the rasetr (each pixel)

	angleView = Vector2(ANGLEVIEWWIDTH, ANGLEVIEWHEIGHT);
};

Camera::Camera(Vector3 m_pos, Vector3 m_towards) :
	position(m_pos), towards(m_towards.normalize()) {

	raster = make_shared<Raster>(HEIGHT, WIDTH); // initialize the rasetr (each pixel)

	angleView = Vector2(ANGLEVIEWWIDTH, ANGLEVIEWHEIGHT);
};


void Camera::Render() {

	return;
}


void Camera::drawScene() {
	raster->Render();

	return;
}


void Camera::translate(Vector3 movement) {
	position = position + movement;

	return;
}


void Camera::rotate(Vector3 eulerAngle) {
	_ASSERT(false);

	float yaw = eulerAngle[2]*0.017444f, pitch = eulerAngle[1]*0.017444f;

	Vector2 xy_cord(towards[0], towards[1]);

	Vector2 temp0(cos(yaw), -sin(yaw)), temp1(sin(yaw), cos(yaw));

	towards.value[0] = xy_cord.dot(temp0);
	towards.value[1] = xy_cord.dot(temp1);




	return;
}

void Camera::generateRay(shared_ptr<PixelRays> rays, int height, int width) {
	_ASSERT(false);

	return;
}

