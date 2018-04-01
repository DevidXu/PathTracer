// This cpp file is the realization of World.h
#include "World.h"
#include <omp.h>

#define SAMPLE_NUM	20  // must be times of 4

World::World() {
	// initialize the bounding box (Cornell box)
	bbox = make_shared<BBox>(
		Vector3(0.0f,0.0f,0.0f),
		Vector3(BOX_LENGTH, BOX_WIDTH, BOX_HEIGHT)
		);

	// initialize the camera with position and direction
	camera = make_shared<Camera>(
		CAMERA_POSITION,	// position
		CAMERA_FORWARD	// forward
		);

	// initialize the interactive interface
	winFace = make_shared<Interface>();

	// clear objects and initialize the scene
	objects.clear();

	LOGPRINT("A ray tracer completed by Dewei Xu, @Shanghai Jiao Tong Univ. Rights reserved for educational purpose.");
}

World::~World() {
	LOGPRINT("Congradulations! Path tracer is completed successfully!");
}


// add all objects into the scene and construct the bounding box with small depth
void World::initialize() {

	winFace->initialize();
	
	// manage all usable objects (including un-rendered objects)
	ObjectManager::getInstance()->initialize();
	ObjList* objList = ObjectManager::getInstance()->getObjectList();


	//  adding triangles of model mesh into the bounding box 
	// (triangle contains pointers to vertexs)
	try {
		LOGPRINT("Mesh of all objects loaded here:");
		for (auto obj : *objList) {
			if (obj == nullptr)
				throw("No pointer pointing to the shape created before.");

			addObject(obj);
		}

		LOGPRINT("\n");

		bbox->initializeCube();

		LOGPRINT("Bounding box depth: " + to_string(bbox->getDepth()));
	}
	catch (exception e) {
		LOGPRINT(e.what());
		LOGPRINT("Meet error when adding objects into the box");
	}

	return;
}


// calculate the normal vector of intrsect triangle patch. Special process with sphere shape
Vector3 World::intersectTest(shared_ptr<Ray> ray, Triangle* &patch, float &distance) {
	// find the triangle face that intersect with the ray
	// if no hit, return; if hit sphere, further judge, if normal, go
	ray->incDepth();

	do {
		// calculate the triangle intersection for the ray; if no intersection, return NULL Vector
		distance = MAX_DIS;
		patch = bbox->intersect(ray, distance);
		if (patch == nullptr) return Vector3();

		//
		if (Triangle::intersectSphere(ray, patch, distance)) {
			// calculate the normal vector on the surface of a ball
			return (ray->getOrigin() + ray->getDirection()*distance - patch->getOwner()->getCenter()).normalize();
		}
		else {
			if (patch != nullptr)
				return patch->getNormal();
			else {
				// hit virtual triangle, continue and no add depth
				ray->setOrigin(ray->getOrigin() + ray->getDirection()*distance, false);
				ray->setDirection(ray->getDirection(), false);
			}
		}
	} while (patch == nullptr);

	return Vector3();
}


// This function acts as the central function of ray tracing. It will trace a given ray in 
// the bounding box, it will call itself recursively. It returns the color of traced ray and
// positions, directions info are saved in pointer ray.
Vector3 World::pathTracing(shared_ptr<Ray> ray) {
	// calculate normal vector of intersection
	float distance = MAX_DIS;
	Triangle* patch = nullptr;
	Vector3 normal = intersectTest(ray, patch, distance);

	if (patch == nullptr) return ENVIRONMENT_COLOR;		// ray is nullptr or no hit triangle

	Object* obj = patch->getOwner();

	// calculate the intersection point
	Vector3 hitPoint = ray->getOrigin() + ray->getDirection()*distance;

	// record the intersection object, aborted now
	Debug->recordPath(obj->getName(), &hitPoint);

	Vector3 color = obj->getColor();
	if (ray->getDepth() > MAX_DEPTH) {
#if SMALL_DEPTH
		return ENVIRONMENT_COLOR;
#endif
		// if not small depth, standardlize the color to enlarge later color
		// dark depth will stop the tracing absolutely.
		if (ray->getDepth() > DARK_DEPTH)
			return ENVIRONMENT_COLOR;

		float p = color.max();
		if (rand()/(RAND_MAX+1.0f)>p)
			return ENVIRONMENT_COLOR;
		color = color * (1.0f / p);		// in such case the depth is too big, if you don't enlarge the color, the effect will not be obvious
	}

	_ASSERT(normal.magnitude() > EPISILON);
    ray->transmit(normal, &hitPoint, obj->getMaterial());	// calculate the reflected or refracted ray direction

	if (obj->getColor().magnitude() == 0.0f) 
		return obj->getEmissive();		// if meet the light source

	// call it self with updated ray recursively
	Vector3 rayColor = pathTracing(ray);
	
	for (int i = 0; i < 3; i++) rayColor.value[i] *= color.value[i];	// color = ray * diffuse

	return obj->getEmissive() + rayColor;	//self-emissive considered

}


// This function begins path tracing and acts as core loop of this project
bool World::renderScene() {

	int height = camera->getHeight(), width = camera->getWidth();
	int sum_pixels = height * width;

	Debug->timeCountStart();

	// print log information to logging.txt
	LOGPRINT("Resolution--Height: " + to_string(height));
	LOGPRINT("Resolution--Width:  " + to_string(width));
	LOGPRINT("Num of Rays/Pixel:  " + to_string(SAMPLE_NUM));
	LOGPRINT("Max Depth for ray:  " + to_string(MAX_DEPTH));

	// shared variables and needed to be set private
	Vector3 color, normal;
	float depth;

#ifndef _OPENMP		// OpenMP will accelerate the progress significantly
	LOGPRINT("OpenMP is not supported on your computer");
#else
	LOGPRINT("OpenMP is utilized for accelerating the parallel process.");
#pragma omp parallel for schedule(dynamic, 1) private(color, normal, depth)
#endif
	for (int i = 0; i < height; i++) {
		if (rd_exit) break;		// controlled by interface process
		for (int j = 0; j < width; j++) {
			if (rd_exit) break;
			// initialize the data for each process. Pay attention: all for loops are parallalized here
			color = Vector3(); normal = Vector3();
			depth = 0.0f;

			// generate origin rays
			shared_ptr<PixelRays> rays = make_shared<PixelRays>();
			for (int k = 0; k < SAMPLE_NUM; k++) rays->push_back(make_shared<Ray>());

			Debug->timing("Generate Ray", true);
			// decide rays directions and positions based on camera info
			camera->generateRay(rays, i, j);
			Debug->timing("Generate Ray", false);

			for (auto& ray : *rays) {
				//controlled by interface progress
				if (rd_exit) break;
				while (rd_pause) cvWaitKey(1000);

				Debug->setSample(i, j, (rand() / (RAND_MAX + 1.0f)) < SAMPLE_RATE ? true : false);

				Debug->timing("Path Tracing", true);
				// call recursive progress to trace a ray
				Vector3 pixel_color = pathTracing(ray);
				Debug->timing("Path Tracing", false);

				Debug->recordColor(i, j, &pixel_color);

				// pixel color might be larger than 1 for strong light!
				color = color + pixel_color;
				// calculate the depth and normal of the first hit point of the ray
				normal = normal + ray->getHitInfo().normal;
				depth = depth + (ray->getHitInfo().Zpos - CAMERA_POSITION).dot(CAMERA_FORWARD);
				if ((ray->getHitInfo().Zpos - CAMERA_POSITION).dot(CAMERA_FORWARD) > 7.0f)
					continue;
			}

			// get the average and set 1.0f as maximum
			color = color / (float)rays->size();
			for (int k = 0; k < 3; k++)
				color.value[k] = color.value[k] > 1.0f ? 1.0f : color.value[k];
			normal = normal / (float)rays->size();
			depth = depth / float(rays->size());

			// update the pixel data in camera and interface
			camera->Render(color, i, j);
			winFace->updateData(color, normal, depth, i, j);

			// print the progress
			Debug->setProgress((i*width + j)*100.0f / sum_pixels);
		}
	}

	// save the image and normalize the depth map, update images on the interface
	rd_exit = true;
	camera->drawScene();
	winFace->finishRender();
	Debug->finishRender();

	return true;
}


// this function servers as auxiliary function to debug one pixel; it will show the color
// normal and depth as well as record the path info.
void World::renderPixel(int i, int j, int num, bool showPath = false) {
	shared_ptr<PixelRays> rays = make_shared<PixelRays>();
	for (int k = 0; k < num; k++) rays->push_back(make_shared<Ray>());
	camera->generateRay(rays, i, j);
	int k = 0;
	for (auto& ray : *rays) {
		k += 1;
		Vector3 pixel_color = pathTracing(ray);
		Vector3 normal = ray->getHitInfo().normal;
		Vector3 depth = (ray->getHitInfo().Zpos - CAMERA_POSITION).dot(CAMERA_FORWARD);
		cout << "\n" << left << setw(7) << "Ray " + to_string(k) << "Tracing Data:" << endl;
		cout << "Color:  " << pixel_color << endl;
		cout << "Normal: " << normal << endl;
		cout << "Depth:  " << depth << endl;

		if (showPath) ray->showPath();

	}

	return;
}


// This function saves RGB data and store it as a graph.
void World::drawScene() {
	return camera->drawScene();
}


// add objects into the bbox by analyzing triangles in its mesh
void World::addObject(shared_ptr<Object> object) {
	objects.push_back(object);
	try {
		bbox->addMesh(object->getMesh());
	}
	catch (exception e) {
		LOGPRINT("Meet error when adding triangle mesh to the bounding box. The feature information is: ");
		LOGPRINT(e.what());
	}

	LOGPRINT("    " + object->getName() + " is loaded successfully;");

	return;
}