#include "Camera.h"


float radicalInverse_VdC(int bits) {
	float sum = 0.0f, parameter = 1.0f;
	while (bits > 0) {
		int left = bits % 2;
		bits /= 2;
		parameter /= 2.0f;
		sum += left * parameter;
	}
	return sum;
}

Vector2 hammersley2d(int i, int N) {
	return Vector2(float(i) / float(N), radicalInverse_VdC(i));
}


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

	// calculate the size of the screen to sample evenly
	float screen_width = tan(angleView.value[0] / 2) * 2;
	float screen_height = tan(angleView.value[1] / 2) * 2;

#if HAMMERSLEY
	for (int i = 0; i < num; i++) {
		shared_ptr<Ray> ray_ptr = rays->at(i);
		Vector2 hammersleyPoint = hammersley2d(i, num);
		float pixel_width = hammersleyPoint.value[0];
		float pixel_height = hammersleyPoint.value[1];

		float ray_width = screen_width * (0.5f - w * 1.0f / getWidth() + pixel_width / getWidth());
		float ray_height = screen_height * (0.5f - h * 1.0f / getHeight() + pixel_height / getHeight());
		float yaw = atan(ray_width);
		float pitch = atan(ray_height / sqrt(1.0f + pow(ray_width, 2)));

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
	
#else
	for (int i = 0; i < num; i++) {
		// sample methods
		shared_ptr<Ray> ray_ptr = rays->at(i);
		float pixel_width = float((rand() + 1) / (RAND_MAX + 2.0));	// ~U(0,1)
		float pixel_height = float((rand() + 1) / (RAND_MAX + 2.0));		// ~U(0,1)

		float ray_width = screen_width * (0.5f - w * 1.0f / getWidth() + pixel_width / getWidth());
		float ray_height = screen_height * (0.5f - h * 1.0f / getHeight() + pixel_height / getHeight());
		float yaw = atan(ray_width);
		float pitch = atan(ray_height / sqrt(1.0f + pow(ray_width, 2)));

		try {
			Vector3 direction = rotate(Vector3(0.0f, pitch, yaw)).normalize();
			ray_ptr->setOrigin(position);
			ray_ptr->setDirection(direction);
			ray_ptr->setNormal(CAMERA_FORWARD);
			ray_ptr->setIntensity(1.0f);
		}
		catch (exception e) {
			throw("Meet error when generating the direction of sample rays.");
		}
	}
#endif
	return;
}

