#pragma once

// This file is used to generate the BMP map. It can be adjusted to generate
// graphes of different postfix like JPG, ttp as you like.

#include "Debugging.h"

enum RasterType { RGB, GRAY };

class BMPGraph {
private:
	int	width = 0, height = 0;

public:
	void setWidth(int w)	{ width = w; }
	void setHeight(int h)	{ height = h; }

	void generate(int* begin, RasterType mark, const char* name);
};