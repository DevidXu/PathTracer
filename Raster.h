#pragma once

// This stores each point on the screen and their RGB value
// It will be used for generating a RGB file and calculate the value for each raster.
// It can also be used to draw grayscale image for debugging usage. In such case, pixel[0-2] is the same value.
// Some post effects can be added here for more effects.

#include "Debugging.h"
#include "Vector.h"
#include "BMPGraph.h"
#include "Constants.h"

#include <vector>

class Raster{
private:
	int height, width;

	BMPGraph	bmpGraph;
	RasterType	type = RGB; // default value: RGB

	struct Pixel {
		Vector4 color;
		
		Pixel(float r = 0.0f, float g = 0.0f, float b=0.0f, float a=0.0f)
			:color(r,g,b,a) {};

		Pixel(const Pixel &p) :color(p.color) {};
	};

	std::vector<std::vector<Pixel>> pixels;

public:

	Raster(int w = 0, int h = 0);

	~Raster();

	void Render();

	void setRastertype(const char* t);

	char* getRasterType();

	int getWidth() { return width; }

	int getHeight() { return height; }
};