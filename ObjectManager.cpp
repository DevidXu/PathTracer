#include "ObjectManager.h"

#include "Shape.h"
#include "Vector.h"
#include "Material.h"

void ObjectManager::initialize() {
	shared_ptr<Object> box, left_wall, right_wall, glass_ball, red_cube, purple_cube, light, bunny;

	// generate all the objects needed to add into cornell box
	// this part mainly defines the shape, size, material, color, emissive and name attributes of every objects
	try {
		// The big cornell box
		box = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(BOX_LENGTH, BOX_WIDTH, BOX_HEIGHT)
				),
			make_shared<Diff>(
				Vector3(0.75f, 0.75f, 0.75f), // Vector3(0.465f, 0.531f, 0.598f)
				Vector3(0.0f, 0.0f, 0.0f)
				),
			"Cornell Box"
			);

		// left wall, color pink
		left_wall = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(BOX_LENGTH, 0.01f, BOX_HEIGHT)
				),
			make_shared<Diff>(
				Vector3(0.94f, 0.62f, 0.76f),
				Vector3(0.0f, 0.0f, 0.0f)
				),
			"Left Wall"
			);

		// right wall, color gold
		right_wall = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(0.0f, BOX_WIDTH-0.01f, 0.0f),
				Vector3(BOX_LENGTH, BOX_WIDTH, BOX_HEIGHT)
				),
			make_shared<Diff>(
				Vector3(0.93f, 0.78f, 0.06f),
				Vector3(0.0f, 0.0f, 0.0f)
				),
			"Right Wall"
			);

		// the sphere of transparent material
		glass_ball = make_shared<Object>(
			make_shared<Sphere>(
				Vector3(2.0f, 1.2f, 2.8f),
				0.7f
				),
			make_shared<Refl>(GLASS_REFRACTIVITY,
				Vector3(0.999f, 0.999f, 0.999f),
				Vector3(0.0f, 0.0f, 0.0f)
				),
			"Glass Ball"
			);

		purple_cube = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(1.0f, 1.3f, 1.5f),
				Vector3(1.2f, 2.0f, 2.5f)
				),
			make_shared<Diff>(
				Vector3(0.54f, 0.17f, 0.88f),
				Vector3(0.0f, 0.0f, 0.0f)
				),
			"Purple Cube"
			);

		// the rectangle of normal material
		red_cube = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(1.0f, 2.3f, 0.0f),
				Vector3(2.4f, 3.3f, 1.3f)
				),
			make_shared<Diff>(
				Vector3(0.8f, 0.15f, 0.15f),
				Vector3(0.0f, 0.0f, 0.0f)
				),
			"Red Cube"
			);
		//red_cube->getShape()->rotate(Vector3(0.0f, 0.0f, 30.0f));
		light = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(1.0f, 1.25f, 3.99f),
				Vector3(2.5f, 2.75f, 4.0f)
				),
			make_shared<Diff>(
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(13.0f, 13.0f, 13.0f)
				),
			"Light"
			);
		
		bunny = make_shared<Object>(
			make_shared<Model>(
				Vector3(1.7f, 2.7f, 1.3f),
				0.14f,
				"./bunny.obj"
				),
			make_shared<Diff>(
				Vector3(0.5f, 0.54f, 0.527f),
				Vector3(0.0f, 0.0f, 0.0f)
				),
			"Bunny"
			);
		bunny->getShape()->rotate(Vector3(-90.0f, 0.0f, 90.0f));
		
	}
	catch (exception e) {
		LOGPRINT("Meet error when creating the shapes");
	}

	objList.push_back(box);
	objList.push_back(left_wall);
	objList.push_back(right_wall);
	objList.push_back(glass_ball);
	objList.push_back(red_cube);
	//objList.push_back(purple_cube);
	objList.push_back(bunny);
	objList.push_back(light);

}