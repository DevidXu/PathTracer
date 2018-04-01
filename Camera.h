#pragma once

// This file defines one camera in the scene. It will generate rays by working 
// together with the raster class. It contains functions like rotation and generate
// bitmaps. It serves as a private variable of World

#include "Constants.h"
#include "Vector.h"
#include "Raster.h"
#include "Ray.h"

class Camera {
private:
	Vector3 position;
	Vector3 towards;
	Vector2 angleView;

	shared_ptr<Raster> raster; // store the rendered raster

public:
	Camera();

	Camera(
		Vector3 m_pos = Vector3(0.0f, 0.0f, 0.0f),
		Vector3 m_towards = Vector3(0.0f, 1.0f, 0.0f)
	);

	int getWidth() { return raster->getWidth(); }
	int getHeight() { return raster->getHeight(); }
	Vector3 getPosition() { return position; }

	void Render(Vector3 color, int height, int width);

	void drawScene();

	void translate(Vector3 movement);

	// calculate the angle if the camera rotate a certain angle
	// it will simplify the calculation for ray direction
	Vector3 rotate(Vector3 eulerAngle); // the mid value should be 0 to add dead lock; only yaw and pitch; unit is 0-pi

	void generateRay(shared_ptr<PixelRays> rays, int height, int width);
};

// generate ray samples on each pixel
Vector2 hammersley2d(int i, int N); 