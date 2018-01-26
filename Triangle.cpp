#include "Triangle.h"

bool Triangle::smallerThan(float mid, int dimension) {
	for (int i = 0; i < 3; i++)
		if ((*vertex[i])[dimension] > mid) return false;
	
	return true;
}


bool Triangle::largerThan(float mid, int dimension) {
	for (int i = 0; i < 3; i++)
		if ((*vertex[i])[dimension] < mid) return false;
	
	return true;
}