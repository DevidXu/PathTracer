#include "Camera.h"


Camera::Camera() {
	raster = make_shared<Raster>(HEIGHT, WIDTH); // initialize the rasetr (each pixel)

	angleView = Vector2(ANGLEVIEWWIDTH, ANGLEVIEWHEIGHT);
};

Camera::Camera(Vector3 m_pos, Vector3 m_towards) :
	position(m_pos), towards(m_towards.normalize()) {

	raster = make_shared<Raster>(HEIGHT, WIDTH); // initialize the rasetr (each pixel)

	angleView = Vector2(ANGLEVIEWWIDTH*0.17444f, ANGLEVIEWHEIGHT*0.17444f);
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
	temp = Vector3(vec[0]*towards[0]/x, vec[0]*towards[1]/x, vec[1]);

	return temp;
}

void Camera::generateRay(shared_ptr<PixelRays> rays, int h, int w) {
	int num = (int)rays->size();
	
	if (num % 4 != 0) throw("Sample num is not a multiple of 4.");

	// calculate the size of the screen to sample evenly
	float screen_width = tan(angleView.value[0] / 2) * 2;
	float screen_height = tan(angleView.value[1] / 2) * 2;

	// divide the pixel into four parts and sample averagely in each part.
	srand(0);
	for (int row=0;row<2;row++)
		for (int column =0;column<2;column++)
			for (int i = 0; i < num/4; i++)
			{
				// sample methods
				shared_ptr<Ray> ray_ptr = rays->at(i);
				float pixel_width = float((rand() + 1) / (RAND_MAX + 2.0) + column * 0.5f);	// ~U(0,1)
				float pixel_height = float((rand() + 1) / (RAND_MAX + 2.0) + row * 0.5f);		// ~U(0,1)

				float yaw = screen_width * (0.5f - w / getWidth() + pixel_width / getWidth());
				float pitch = screen_height * (h / getHeight() + pixel_height / getHeight() - 0.5f);

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

