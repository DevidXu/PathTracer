#pragma once

// This stores each point on the screen and their RGB value
// It will be used for generating a RGB file and calculate the value for each raster.
// Some post effects can be added here for more effects.

#include "Debugging.h"
#include "Vector.h"
#include <vector>

#define HEIGHT 128
#define WIDTH 128

class Raster : public Singleton<Raster> {
private:
	Raster();
	~Raster();

	struct Pixel {
		Vector4 color;
		Pixel(float r = 0.0f, float g = 0.0f, float b=0.0f, float a=0.0f):color(r,g,b,a) {};
		Pixel(const Pixel &p) :color(p.color) {};
	};
	std::vector<std::vector<Pixel>> pixels;

public:
	friend Singleton<Raster>;

	void Render();

};