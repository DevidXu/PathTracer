#pragma pack(2)// a must; or else the answer of sizeof is incorrect  

#include "BMPGraph.h"
#include <stdlib.h>

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef long    LONG;


typedef struct {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
	DWORD      biSize;
	LONG       biWidth;
	LONG       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
} BITMAPINFOHEADER;


void saveBitmap(const char* name, RasterType mark, int w, int h, int* begin)
{
	// Define BMP Size
	if (mark!=RGB && mark!=GRAY) 
		throw("Unsupported image type or width/height equals 0.");
	
	const int height = h;
	const int width = w;
	int size = w * h * 3;
	int index;

	// Part.1 Create Bitmap File Header  
	BITMAPFILEHEADER fileHeader;

	fileHeader.bfType = 0x4D42;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// Part.2 Create Bitmap Info Header  
	BITMAPINFOHEADER bitmapHeader = { 0 };

	bitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapHeader.biHeight = height;
	bitmapHeader.biWidth = width;
	bitmapHeader.biPlanes = 3;
	bitmapHeader.biBitCount = 24;
	bitmapHeader.biSizeImage = size;
	bitmapHeader.biCompression = 0; //BI_RGB  

	// Part.3 Create Data  
	BYTE *bits = (BYTE *)malloc(size);

	// Clear  
	memset(bits, 0xFF, size);

	// generate the RGB or grayscale, will convert from RGB to BGR
	for (int x = 0; x<h;x++)
		for (int y = 0;y<w;y++){
		index = (int)x * w * 3 + (int)y * 3;

		bits[index + 0] = begin[index+2]; // Blue  
		bits[index + 1] = begin[index+1]; // Green  
		bits[index + 2] = begin[index+0]; // Red  
	}

	// Write to file  
	FILE *output;
	fopen_s(&output, name, "wb");

	if (output == NULL)
		throw("Cannot open the file to output!");
	else {
		fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, output);
		fwrite(&bitmapHeader, sizeof(BITMAPINFOHEADER), 1, output);
		fwrite(bits, size, 1, output);
		fclose(output);
	}
}


void BMPGraph::generate(int* begin, RasterType mark, const char* name) {
	LOGPRINT("Begin to write the image");
	_ASSERT(height != 0);
	_ASSERT(width != 0);

	try {
		//postProcess(begin, DENOISE);
		saveBitmap(name, mark, width, height, begin);
	}
	catch (exception e) {
		LOGPRINT(e.what());
	}

	return;
}


void BMPGraph::postProcess(int* begin, POSTPROCESS pProcess) {
	switch (pProcess) {
	case DENOISE:
		denoiseProcess(begin); break;
	case HDR:
		HDRProcess(begin); break;
	default:
		LOGPRINT("No such post process defined!");
		break;
	}
}


void denoiseProcess(int* begin) {
	// calculate the size of chosen area to denoise
	int size = int(ceil(log(HEIGHT < WIDTH ? HEIGHT : WIDTH)) - 1);
	size = size / 2 * 2 + 1; // ensure it is odd

	for (int i = size / 2; i < HEIGHT - size / 2; i++)
		for (int j = size / 2; j < WIDTH - size / 2; j++) {
			int index = i * WIDTH * 3 + j * 3;
			
			// sum of pixels, sum of squares, mean, variance, denoise parameters
			double sum[3], sum2[3], mean[3], var[3], dk[3];
			
			// initialize all
			for (int k = 0; k < 3; k++) {
				sum[k] = 0.0;
				sum2[k] = 0.0;
				mean[k] = 0.0;
				var[k] = 0.0;
				dk[k] = 0.0;
			}

			for (int m = -size / 2; m <= size / 2; m++)
				for (int n = -size / 2; n <= size / 2; n++) {
					for (int k = 0; k < 3; k++) {
						sum[k] += begin[index+k+m*WIDTH*3+n*3];
						sum2[k] += pow(begin[index+k+m*WIDTH*3+n*3], 2);
					}
				}

			int area = size * size;
			for (int k = 0; k < 3; k++) {
				mean[k] = sum[k] / area;
				var[k] = (sum2[k] - pow(sum[k], 2) / area) / area;
				dk[k] = var[k] / (var[k] + Sigmma);
				begin[index + k] = int(floor((1 - dk[k])*mean[k] + dk[k] * begin[index + k]));
			}

		}
}


void HDRProcess(int* begin) {
	LOGPRINT("HDR is not supported for single image process");
}