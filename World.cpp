// This cpp file is the realization of World.h
#include "World.h"
#include <iomanip>

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
	shared_ptr<Object> box, obj1, obj2, light;

	// generate all the objects needed to add into cornell box
	// this part mainly defines the shape, size, material, color and emissive attributes of every objects
	try {
		// The big cornell box
		box = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(0.0, 0.0, 0.0),
				Vector3(BOX_LENGTH, BOX_WIDTH, BOX_HEIGHT)
				),
			make_shared<Diff>(),
			Vector3(0.465f, 0.531f, 0.598f),
			Vector3(0.0f, 0.0f, 0.0f)
			);

		// the sphere of transparent material
		obj1 = make_shared<Object>(
			make_shared<Sphere>(
				Vector3(2.0f, 1.2f, 1.0f), //Vector3(0.0f, 0.0f, 0.0f), //
				1.0f
				),
			make_shared<Refl>(GLASS_REFRACTIVITY),
			Vector3(0.1f, 0.1f, 0.1f),
			Vector3(0.0f, 0.0f, 0.0f)
			);

		obj1->tessellate(2);

		// the rectangle of normal material
		obj2 = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(1.0f, 2.5f, 0.0f),
				Vector3(2.0f, 3.5f, 2.0f)
				),
			make_shared<Diff>(),
			Vector3(0.6f, 0.0f, 0.0f),
			Vector3(0.0f, 0.0f, 0.0f)
			);

		light = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(1.0f, 1.0f, 3.8f),
				Vector3(4.0f, 3.0f, 4.0f)
				),
			make_shared<Diff>(),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(1.0f, 1.0f, 1.0f)
			);
	}
	catch (exception e) {
		LOGPRINT("Meet error when creating the shapes");
	}


	// if all generated successfully, this part will add objects by adding triangles into the bounding box 
	// (triangle contains pointers to vertexs)
	try {
		if (box == nullptr || obj1 == nullptr || obj2 == nullptr || light == nullptr)
			throw("No pointer pointing to the shape created before.");
		addObject(box);
		//addObject(obj1);
		//addObject(obj2);
		addObject(light);
	}
	catch (exception e) {
		LOGPRINT(e.what());
		LOGPRINT("Meet error when adding objects into the box");
	}

	return;
}


// This function will trace a given ray in the bounding box bbox, it will call itself recursively.
Vector3 World::pathTracing(shared_ptr<Ray> ray) {
	if (ray == nullptr) return ENVIRONMENT_COLOR;

	if (ray->getDepth() > MAX_DEPTH) return ENVIRONMENT_COLOR;

	float distance = MAX_DIS;
	Triangle* patch = bbox->intersect(ray, distance); // find the triangle face that intersect with the ray
	if (patch == nullptr) return ENVIRONMENT_COLOR;

	Object* obj = patch->getOwner();

	Vector3 hitPoint = ray->getOrigin() + ray->getDirection()*distance;

	shared_ptr<Ray> refractRay = nullptr;

#ifdef GLOBAL
    ray->transmit(patch, &hitPoint, obj->getMaterial(), refractRay);		// material decide the outward direction
#endif

	if (obj->getColor().magnitude() == 0.0f) 
		return obj->getEmissive();		// if meet the light source

#ifndef GLOBAL
	return obj->getColor();
#endif

	float rate = 1.0f;
	if (refractRay) rate = 0.5f;
	Vector3 rayColor = (pathTracing(ray) + pathTracing(refractRay))*rate;	// if it is refractivity, ray means the reflective ray, refractRay means the refractRay

	for (int i = 0; i < 3; i++) rayColor.value[i] *= obj->getColor().value[i];

	return obj->getEmissive() + rayColor;

}


// This function begins path tracing
RENDERSTATE World::renderScene() {
	int height = camera->getHeight(), width = camera->getWidth();

	float progress = 0.0f, last_progress = 0.0f;
	int sum_pixels = height * width;

	system("cls");
	cout << "Rendering Progress: 0%" << endl;

	for (int i = 0;i<height;i++)
		for (int j = 0; j < width; j++) {
			shared_ptr<PixelRays> rays = make_shared<PixelRays>();
			for (int k = 0; k < SAMPLE_NUM; k++) rays->push_back(make_shared<Ray>());

			camera->generateRay(rays, i, j);

			Vector3 color(0.0f, 0.0f, 0.0f);
			for (auto &ray : *rays) {
				Vector3 pixel_color = pathTracing(ray);
				for (int k = 0; k < 3; k++) 
					pixel_color.value[k] = pixel_color.value[k] > 1.0f ? 1.0f : pixel_color.value[k];
				
				color = color + pixel_color;
			}
			color = color / (float)rays->size();

			camera->Render(color, i, j);

			// show the progress
			progress = (i*width + j)*100.0f / sum_pixels;
			if (progress - last_progress > 1) {
				last_progress = progress;
				system("cls");
				cout << "Rendering Progress: " << setprecision(3) << progress << "%";
			}

		}

	system("cls");
	cout << "Rendering Progress: 100%" << endl;
	cout << "Image has been rendered! Check the local image." << endl;
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