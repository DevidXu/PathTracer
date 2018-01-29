// This file contains the realization of Raster.h

// This is used to generate the bmp file with RGB value for each pixel

#include <stdlib.h>
#include <direct.h> 
#include "Raster.h"

#define RENDERIMAGE "./image.bmp"
#define MAX_PATH 100

// Fill the pixels with vector
Raster::Raster(int w, int h) {
	width = w; 
	height = h;

	for (int i = 0; i < height; i++) {
		vector<Pixel> line;
		line.clear();
		line.reserve(width);

		for (int j = 0; j < width; j++)
			line.push_back(Pixel());
		pixels.push_back(line);
	}
}


Raster::~Raster() {
	for (int i = 0; i < height; i++)
		pixels[i].clear();
	pixels.clear();
}


char* Raster::getRasterType() {
	return (char*)type;
}


void Raster::setRastertype(const char* t) {
	try {
		if (t == "RGB") type = RGB;
		else if (t == "GRAY") type = GRAY;
		else throw("The type is not supported for the raster!");
	}
	catch (exception e) {
		LOGPRINT("You meet error when setting the type for the raster.");
		LOGPRINT(e.what());
	}
}


void Raster::Render() {
	bmpGraph.setHeight(height);
	bmpGraph.setWidth(width);

	// use int to reduce the size of transmission
	int* temp = new int [height*width*3];
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			temp[i*width * 3 + j * 3 + 0] = int(pixels[i][j].color[0]*255);
			temp[i*width * 3 + j * 3 + 1] = int(pixels[i][j].color[1]*255);
			temp[i*width * 3 + j * 3 + 2] = int(pixels[i][j].color[2]*255);
		}
	bmpGraph.generate(temp, type, RENDERIMAGE);
	delete [] temp;


	LOGPRINT("Finish rendering the image. Check the path for result\n");

	char buffer[MAX_PATH] = "";
	_getcwd(buffer, MAX_PATH);
	strcat_s(buffer, RENDERIMAGE);
	LOGPRINT(buffer);
}
