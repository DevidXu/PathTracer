// This file contains the realization of Raster.h

// This is used to generate the bmp file with RGB value for each pixel

#include <stdlib.h>
#include <direct.h> 
#include "Raster.h"

#define RENDERIMAGE "./image.bmp"
#define MAX_PATH 100

// Fill the pixels with vector
Raster::Raster() {
	for (int i = 0; i < HEIGHT; i++) {
		vector<Pixel> line;
		line.clear();
		line.reserve(WIDTH);

		for (int j = 0; j < WIDTH; j++)
			line.push_back(Pixel());
		pixels.push_back(line);
	}
}

Raster::~Raster() {
	for (int i = 0; i < HEIGHT; i++)
		pixels[i].clear();
	pixels.clear();
}

void Raster::Render() {
	bmpGraph.setHeight(HEIGHT);
	bmpGraph.setWidth(WIDTH);

	// use int to reduce the size of transmission
	int temp[HEIGHT*WIDTH*3];
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++) {
			temp[i*WIDTH * 3 + j * 3 + 0] = i<100?int(pixels[i][j].color[0]*255):255;
			temp[i*WIDTH * 3 + j * 3 + 1] = int(pixels[i][j].color[1]*255);
			temp[i*WIDTH * 3 + j * 3 + 2] = int(pixels[i][j].color[2]*255);
		}
	bmpGraph.generate(temp, type, RENDERIMAGE);

	LOGPRINT("Finish rendering the image. Check the path for result\n");

	char buffer[MAX_PATH] = "";
	_getcwd(buffer, MAX_PATH);
	strcat_s(buffer, RENDERIMAGE);
	LOGPRINT(buffer);
}
