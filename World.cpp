// This cpp file is the realization of World.h
#include "World.h"
#include <omp.h>


World::World() {

	bbox = make_shared<BBox>(
		Vector3(0.0f,0.0f,0.0f),
		Vector3(BOX_LENGTH, BOX_WIDTH, BOX_HEIGHT)
		);							// initialize the bounding box (Cornell box of 4:4:4)

	camera = make_shared<Camera>(
		CAMERA_POSITION,	// position
		CAMERA_FORWARD	// forward
		);

	objects.clear();

	LOGPRINT("This is the realization of a path tracer!");
}

World::~World() {
	LOGPRINT("Path tracer is completed successfully!");
}


void World::initialize() {

	ObjectManager::getInstance()->initialize();
	ObjList* objList = ObjectManager::getInstance()->getObjectList();

	// if all generated successfully, this part will add objects by adding triangles into the bounding box 
	// (triangle contains pointers to vertexs)
	try {
		for (auto obj : *objList) {
			if (obj == nullptr)
				throw("No pointer pointing to the shape created before.");

			addObject(obj);
		}
	}
	catch (exception e) {
		LOGPRINT(e.what());
		LOGPRINT("Meet error when adding objects into the box");
	}

	return;
}


// This function will trace a given ray in the bounding box bbox, it will call itself recursively.
Vector3 World::pathTracing(shared_ptr<Ray> ray) {

	float distance = MAX_DIS;
	Debug->timing("Intersect", true);
	Triangle* patch = bbox->intersect(ray, distance);	// find the triangle face that intersect with the ray
	Debug->timing("Intersect", false);
	if (patch == nullptr) return ENVIRONMENT_COLOR;		// ray is nullptr or no hit triangle

	Object* obj = patch->getOwner();

	Vector3 hitPoint = ray->getOrigin() + ray->getDirection()*distance;

	Debug->recordPath(obj->getName(), &hitPoint);
	if (obj->getName() == "Red Cube") {
		int k = 1;
	}

	Vector3 color = obj->getColor();
	if (ray->getDepth() > MAX_DEPTH) {
		if (ray->getDepth() > DARK_DEPTH)
			return ENVIRONMENT_COLOR;

		float p = color.max();
		if (rand()/(RAND_MAX+1.0f)>p)
			return ENVIRONMENT_COLOR;
		color = color * (1.0f / p);  // in such case the depth is too big, if you don't enlarge the color, the effect will not be obvious
	}

	shared_ptr<Ray> refractRay = make_shared<Ray>(*ray);

#ifdef GLOBAL
	Debug->timing("Transmit", true);
    LightRate rate = ray->transmit(patch, &hitPoint, obj->getMaterial(), refractRay);		// material decide the outward direction
	Debug->timing("Transmit", false);
#endif

	// put there to record the hit position
	if (obj->getColor().magnitude() == 0.0f) 
		return obj->getEmissive();		// if meet the light source

#ifndef GLOBAL
	return obj->getColor();
#endif
	Vector3 rayColor; // recursive process here to trace the ray
	rayColor =
		(rate.ray_rate == 0.0f ? 0.0f : pathTracing(ray))*rate.ray_rate 
		+
		(rate.refractRay_rate == 0.0f ? 0.0f : pathTracing(refractRay))*rate.refractRay_rate;
	/*
	if (rate.refractRay_rate == 0.0f)
		rayColor = pathTracing(ray)*rate.ray_rate;	// if it is refractivity, ray means the reflective ray, refractRay means the refractRay
	else {
		// possibility of transmisison, reflective
		float P = 0.25f + 0.5f*rate.ray_rate, RP = rate.ray_rate / P, TP = rate.refractRay_rate / (1 - P);
		if (ray->getDepth() > 2) {
			if (rand() / RAND_MAX < P)
				rayColor = pathTracing(ray)*RP;
			else
				rayColor = pathTracing(refractRay)*TP;
		}
		else rayColor = pathTracing(ray)*rate.ray_rate + pathTracing(refractRay)*rate.refractRay_rate;
	}
	*/
	for (int i = 0; i < 3; i++) rayColor.value[i] *= color.value[i];

	return obj->getEmissive() + rayColor;

}


// This function begins path tracing
RENDERSTATE World::renderScene() {

	int height = camera->getHeight(), width = camera->getWidth();
	int sum_pixels = height * width;

	Debug->timeCountStart();

	Vector3 color;

#ifndef _OPENMP
	LOGPRINT("OpenMP is not supported on your computer");
#else
#pragma omp parallel for  schedule(dynamic, 1) private(color)
#endif
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			shared_ptr<PixelRays> rays = make_shared<PixelRays>();
			for (int k = 0; k < SAMPLE_NUM; k++) rays->push_back(make_shared<Ray>());
			if (i == 70 && j == 40)
				int l = 1;
			Debug->timing("Generate Ray", true);
			camera->generateRay(rays, i, j);
			Debug->timing("Generate Ray", false);

			color = Vector3(0.0f, 0.0f, 0.0f);
			for (auto &ray : *rays) {
				Debug->setSample(i, j, (rand() / (RAND_MAX + 1.0f)) < SAMPLE_RATE ? true : false);

				Debug->timing("Path Tracing", true);
				Vector3 pixel_color = pathTracing(ray);
				Debug->timing("Path Tracing", false);

				Debug->recordColor(i, j, &pixel_color);

				// pixel color might be larger than 1 for strong light
				color = color + pixel_color;
			}

			// get the average and set 1.0f as maximum
			color = color / (float)rays->size();
			for (int k = 0; k < 3; k++)
				color.value[k] = color.value[k] > 1.0f ? 1.0f : color.value[k];

			camera->Render(color, i, j);

			// print the progress
			Debug->showProgress((i*width + j)*100.0f / sum_pixels);
		}

		// after certain time, render the image temporarily
		if (Debug->renderInterval(time(NULL))) camera->drawScene();

	}

	Debug->timeCountEnd();
	Debug->showTiming();

	return true;
}

// This function saves RGB data and store it as a graph.
void World::drawScene() {
	return camera->drawScene();
}


void World::addObject(shared_ptr<Object> object) {
	objects.push_back(object);
	try {
		bbox->addMesh(object->getMesh());
	}
	catch (exception e) {
		LOGPRINT("Meet error when adding triangle mesh to the bounding box. The feature information is: ");
		LOGPRINT(e.what());
	}

	return;
}