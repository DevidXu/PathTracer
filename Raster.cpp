// This file contains the realization of Raster.h

// This is used to generate the bmp file with RGB value for each pixel

#include <stdlib.h>
#include <direct.h> 
#include "Raster.h"

#define RENDERIMAGE "./image.pmm"
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
	FILE* fp;
	fopen_s(&fp, RENDERIMAGE, "wb");
	fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
	static unsigned char fcolor[3];
	
	LOGPRINT("Begin to render the image");
	try {
		for (int i = 0; i < WIDTH; i++)
			for (int j = 0; i < HEIGHT; j++) {
				Vector4 acolor = pixels[HEIGHT][WIDTH].color;
				Vector3 color(acolor[0],acolor[1],acolor[2]);
				color = color * 255;
				for (int i = 0; i < 3; i++) {
					fcolor[0] = unsigned char(int(color[0]));
					if (color[0] > 255) throw(exception("RGB attributes larger than 255"));
				}
				fwrite(fcolor, 1, 3, fp);
			}
	}
	catch (exception e) {
		LOGPRINT("Meet error when rendering the image!");
		LOGPRINT(e.what());
	}

	LOGPRINT("Finish rendering the image. Check the path for result\n");
	
	char buffer[MAX_PATH] = "Current image path:  ";
	_getcwd(buffer, MAX_PATH);
	LOGPRINT(buffer);

	return;
}
