#pragma once

// This stores each point on the screen and their RGB value
// It will be used for generating a RGB file and calculate the value for each raster.

#include "Singleton.h"
#include "Vector.h"
#include <vector>
class Raster : public Singleton<Raster> {
private:
	std::vector<Vector4> pixels;
public:
	void Render();
};