#pragma once

// This file is used for generating the bounding box for the whole world.
// It will use binary search to find the precise position of the triangle.
/*
When a new triangle comes, it will find the exact position of these triangles
in the bonuding box, as well as divide the bounding box into smaller level so
that each triangle is fixed reasonably. Each box will contains a pointer to an
array which stores all the triangles relate ot this box. Then a ray can visit
through the bounding box to find all possible triangles to hit.
*/
#include "Singleton.h"

class BBox :public Singleton<BBox> {
private:
public:

};