#include "Camera.h"
#include <time.h>

Camera::Camera() {
	raster = make_shared<Raster>(HEIGHT, WIDTH); // initialize the rasetr (each pixel)

	angleView = Vector2(ANGLEVIEWWIDTH*PIDEGREE, ANGLEVIEWHEIGHT*PIDEGREE);
};

Camera::Camera(Vector3 m_pos, Vector3 m_towards) :
	position(m_pos), towards(m_towards.normalize()) {

	raster = make_shared<Raster>(HEIGHT, WIDTH); // initialize the rasetr (each pixel)

	angleView = Vector2(ANGLEVIEWWIDTH*PIDEGREE, ANGLEVIEWHEIGHT*PIDEGREE);
};


void Camera::Render(Vector3 color, int height, int width) {
	raster->Render(color, height, width);
	return;
}


void Camera::drawScene() {
	raster->Draw();

	return;
}


void Camera::translate(Vector3 movement) {
	position = position + movement;

	return;
}


Vector3 Camera::rotate(Vector3 eulerAngle) {

	Vector3 temp = towards;

	float yaw = eulerAngle[2], pitch = eulerAngle[1];

	Vector2 xy_cord(towards[0], towards[1]);
	xy_cord = xy_cord.rotate(yaw);

	temp.value[0] = xy_cord[0];
	temp.value[1] = xy_cord[1];

	float x = xy_cord.magnitude(), y = temp.value[2];
	
	try {
		_ASSERT(x != 0.0f);
	}
	catch (exception e) {
		LOGPRINT("The camera points directly down after the rotation! It is not allowed");
	}
	Vector2 vec(x, y);
	vec = vec.rotate(pitch);
	temp = Vector3(vec[0]*temp.value[0]/x, vec[0]*temp.value[1]/x, vec[1]);

	return temp;
}

void Camera::generateRay(shared_ptr<PixelRays> rays, int h, int w) {
	int num = (int)rays->size();
	
	if (num % 4 != 0) throw("Sample num is not a multiple of 4.");

	// calculate the size of the screen to sample evenly
	float screen_width = tan(angleView.value[0] / 2) * 2;
	float screen_height = tan(angleView.value[1] / 2) * 2;

	// divide the pixel into four parts and sample averagely in each part.
	srand((unsigned int)time(NULL));

	for (int i = 0; i < num / 4; i++)
		for (int row = 0; row<2; row++)
			for (int column = 0; column<2; column++) {
				// sample methods
				shared_ptr<Ray> ray_ptr = rays->at(i*4+row*2+column);
				float pixel_width = float((rand() + 1) / (RAND_MAX + 2.0) + column * 0.5f);	// ~U(0,1)
				float pixel_height = float((rand() + 1) / (RAND_MAX + 2.0) + row * 0.5f);		// ~U(0,1)

				float yaw = screen_width * (0.5f - w*1.0f / getWidth() + pixel_width / getWidth());
				float pitch = screen_height * (0.5f - h*1.0f / getHeight() + pixel_height / getHeight());

				try {
					Vector3 direction = rotate(Vector3(0.0f, pitch, yaw)).normalize();
					ray_ptr->setOrigin(position);
					ray_ptr->setDirection(direction);
					ray_ptr->setIntensity(1.0f);
				}
				catch (exception e) {
					throw("Meet error when generating the direction of sample rays.");
				}
			}

	return;
}

