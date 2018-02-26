#include "ObjectManager.h"

#include "Shape.h"
#include "Vector.h"
#include "Material.h"

void ObjectManager::initialize() {
	shared_ptr<Object> box, left_wall, right_wall, glass_ball, red_cube, light;

	// generate all the objects needed to add into cornell box
	// this part mainly defines the shape, size, material, color, emissive and name attributes of every objects
	try {
		// The big cornell box
		box = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(BOX_LENGTH, BOX_WIDTH, BOX_HEIGHT)
				),
			make_shared<Diff>(),
			Vector3(0.75f, 0.75f, 0.75f), //Vector3(0.465f, 0.531f, 0.598f),
			Vector3(0.0f, 0.0f, 0.0f),
			"Cornell Box"
			);

		// left wall, color pink
		left_wall = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(BOX_LENGTH, 0.01f, BOX_HEIGHT)
				),
			make_shared<Diff>(),
			Vector3(0.94f, 0.62f, 0.76f),
			Vector3(0.0f, 0.0f, 0.0f),
			"Left Wall"
			);

		// right wall, color gold
		right_wall = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(0.0f, BOX_WIDTH-0.01f, 0.0f),
				Vector3(BOX_LENGTH, BOX_WIDTH, BOX_HEIGHT)
				),
			make_shared<Diff>(),
			Vector3(0.93f, 0.78f, 0.06f),
			Vector3(0.0f, 0.0f, 0.0f),
			"Right Wall"
			);

		// the sphere of transparent material
		glass_ball = make_shared<Object>(
			make_shared<Sphere>(
				Vector3(2.0f, 1.2f, 1.0f), //Vector3(0.0f, 0.0f, 0.0f), //
				0.7f
				),
			//make_shared<Refl>(GLASS_REFRACTIVITY),
			make_shared<Spec>(),
			Vector3(0.0f, 0.0f, 0.97f),
			Vector3(0.0f, 0.0f, 0.0f),
			"Glass Ball"
			);

		glass_ball->tessellate(2);

		// the rectangle of normal material
		red_cube = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(1.0f, 2.3f, 0.0f),
				Vector3(2.0f, 3.3f, 1.6f)
				),
			make_shared<Diff>(),
			Vector3(0.7f, 0.0f, 0.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			"Red Cube"
			);

		light = make_shared<Object>(
			make_shared<Rectangle>(
				Vector3(0.5f, 1.25f, 3.99f),
				Vector3(2.0f, 2.75f, 4.0f)
				),
			make_shared<Diff>(),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(35.0f, 35.0f, 35.0f),
			"Light"
			);
	}
	catch (exception e) {
		LOGPRINT("Meet error when creating the shapes");
	}

	objList.push_back(box);
	objList.push_back(left_wall);
	objList.push_back(right_wall);
	objList.push_back(glass_ball);
	objList.push_back(red_cube);
	objList.push_back(light);

}