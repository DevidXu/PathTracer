#pragma pack(2)// a must; or else the answer of sizeof is incorrect  

#include "BMPGraph.h"

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
		saveBitmap(name, mark, width, height, begin);
	}
	catch (exception e) {
		LOGPRINT(e.what());
	}

	return;
}