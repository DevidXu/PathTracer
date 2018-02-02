// This cpp file is the realization of World.h
#include "World.h"

World::World() {

	bbox = make_shared<BBox>(
		Vector3(0.0f,0.0f,0.0f),
		Vector3(BOX_LENGTH, BOX_WIDTH, BOX_HEIGHT)
		);							// initialize the bounding box (Cornell box of 4:4:4)

	camera = make_shared<Camera>(
		Vector3(0.0f, 1.0f, 1.5f),	// position
		Vector3(0.0f, 1.0f, 0.0f)	// forward
		);

	LOGPRINT("This is the realization of a path tracer!");
}

World::~World() {
	LOGPRINT("Path tracer is completed successfully!");
}


void World::initialize() {
	shared_ptr<Object> box, obj1, obj2;

	try {
		// The big cornell box
		box = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(0.0, 0.0, 0.0),
				Vector3(BOX_LENGTH, BOX_WIDTH, BOX_HEIGHT)
				)
			);

		// the sphere of transparent material
		obj1 = make_shared<Object>(
			make_shared<Sphere>(
				Vector3(1.0f, 3.5f, 1.0f),
				1.0f
				)
			);
		
		obj1->tessellate(2);

		// the rectangle of normal material
		obj2 = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(2.0, 0.0, 2.0),
				Vector3(4.0, 2.0, 3.0)
				)
			);
	}
	catch (exception e) {
		LOGPRINT("Meet error when creating the shapes");
	}

	try {
		if (box == nullptr or obj1 == nullptr or obj2 == nullptr)
			throw("No pointer pointing to the shape created before.");
		addObject(box);
		addObject(obj1);
		addObject(obj2);
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

	if (ray->getDepth() >= MAX_DEPTH) return ENVIRONMENT_COLOR;

	Triangle* patch = bbox->intersect(ray); // complete
	if (patch == nullptr) return ENVIRONMENT_COLOR;

	Object* obj = patch->getOwner();

	ray->transmit(patch, obj->getMaterial()); // material
	
	// if reflective, another light created
	shared_ptr<Ray> reflectRay = nullptr;

	return obj->getEmissive() + obj->getColor()*(pathTracing(ray) + pathTracing(reflectRay));

}


// This function begins path tracing
RENDERSTATE World::renderScene() {
	int height = camera->getHeight(), width = camera->getWidth();

	for (int i = 0;i<height;i++)
		for (int j = 0; j < width; j++) {
			shared_ptr<PixelRays> rays = make_shared<PixelRays>();
			for (int k = 0; k < SAMPLE_NUM; k++) rays->push_back(make_shared<Ray>());

			camera->generateRay(rays, i, j);

			Vector3 color(0.0f, 0.0f, 0.0f);
			for (auto &ray : *rays) {
				color = color + pathTracing(ray);
			}
			color = color * (1.0f / rays->size());
		}

	return true;
}

// This function saves RGB data and store it as a graph.
void World::drawScene() {
	return camera->drawScene();
}


void World::addObject(shared_ptr<Object> object) {
	bbox->addMesh(object->getMesh());

	return;
}