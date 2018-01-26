#include "Shape.h"

Shape::Shape() {
	center = Vector3(0.0f, 0.0f, 0.0f);
	vertexs.clear();
	normals.clear();
	meshes.clear();
}


Shape::~Shape() {
	for (auto ptr : vertexs)
		delete ptr;
	for (auto ptr : normals)
		delete ptr;
	for (auto ptr : meshes)
		delete ptr;

	vertexs.clear();
	normals.clear();
	meshes.clear();
}


void Shape::translate(Vector3 movement) {
	try {
		if (vertexs.size() == 0) throw("No vertex information!");
		for (auto ptr : vertexs)
			if (ptr)
				*ptr = (*ptr) + movement;
			else throw ("Invalid vertex information");

		center = center + movement;
	}
	catch (exception e) {
		LOGPRINT(e.what());
	}

	return;
}


void Shape::rotate(float x_angle, float y_angle, float z_angle) {
	Vector3 eulerAngle(x_angle, y_angle, z_angle);
	rotate(eulerAngle);
}


// The eulerAngle means the angle of rotation along each axis. It will be valid for 
// only once. And the unit is degree
void Shape::rotate(Vector3 eulerAngle) {
	float x, y, z, w;
	Vector3 e = eulerAngle * 0.017444f*0.5;
	
	// Store to temp variables can save some sin/cos operations for calculating 
	// the quatertion
	float s[3], c[3];
	s[0] = sin(e[0]); s[1] = sin(e[1]); s[2] = sin(e[2]);
	c[0] = cos(e[0]); c[1] = cos(e[1]); c[2] = cos(e[2]);
	
	w = c[0] * c[1] * c[2] - s[0] * s[1] * s[2];
	x = c[0] * s[1] * s[2] - s[0] * c[1] * c[2];
	y = c[0] * s[1] * c[2] - s[0] * c[1] * s[2];
	z = c[0] * c[1] * s[2] - s[0] * s[1] * c[2];

	Vector4 quaternion(w, x, y, z);
	rotate(quaternion);
}


void Shape::rotate(Vector4 quaternion) {
	Vector4 q = quaternion;

	Vector3 matrix0(
		sqrt(q[0]) + sqrt(q[1]) - sqrt(q[2]) - sqrt(q[3]),
		2 * (q[1] * q[2] - q[0] * q[3]),
		2 * (q[1] * q[3] - q[0] * q[2])
	);

	Vector3 matrix1(
		2 * (q[1] * q[2] + q[0] * q[3]),
		sqrt(q[0]) - sqrt(q[1]) + sqrt(q[2]) - sqrt(q[3]),
		2 * (q[2] * q[3] - q[0] * q[1])
	);

	Vector3 matrix2(
		2 * (q[1] * q[3] - q[0] * q[2]),
		2 * (q[2] * q[3] - q[0] * q[1]),
		sqrt(q[0]) - sqrt(q[1]) - sqrt(q[2]) + sqrt(q[3])
	);

	for (auto ptr : vertexs) {
		Vector3 temp(
			matrix0*(*ptr),
			matrix1*(*ptr),
			matrix2*(*ptr)
		);

		for (int i = 0; i < 3; i++)
			ptr->value[i] = temp[i];
	}

	center = Vector3(matrix0*center, matrix1*center, matrix2*center);

	return;
}


void Shape::scale(float s) {
	Vector3 scaleVector(s, s, s);

	scale(scaleVector);

	return;
}


void Shape::scale(Vector3 s) {
	int i = 0;
	for (auto &ptr : vertexs) {
		ptr->value[i] = (ptr->value[i] - center[i])*s[i] + center[i];
		i += 1;
	}

	return;
}


Rectangle::Rectangle(Vector3 small, Vector3 big) {
	try {
		_ASSERT(small < big);
	}
	catch (exception e) {
		LOGPRINT("Wrong vectors to initialize the rectangle.");
	}

	setCenter((small + big)*0.5);

	Vector3 *v[8];
	Vector3 *n[8];
	v[0] = new Vector3(small);
	v[1] = new Vector3(big.value[0], small.value[1], small.value[2]);
	v[2] = new Vector3(big.value[0], big.value[1], small.value[2]);
	v[3] = new Vector3(small.value[0], big.value[1], small.value[2]);
	v[4] = new Vector3(small.value[0], small.value[1], big.value[2]);
	v[5] = new Vector3(big.value[0], small.value[1], big.value[2]);
	v[6] = new Vector3(big);
	v[7] = new Vector3(small.value[0], big.value[1], big.value[2]);

	n[0] = new Vector3(0.0, -1.0, 0.0);
	n[1] = new Vector3(0.0, -1.0, 0.0);
	n[2] = new Vector3(0.0, 1.0, 0.0);
	n[3] = new Vector3(0.0, 1.0, 0.0);
	n[4] = new Vector3(0.0, -1.0, 0.0);
	n[5] = new Vector3(0.0, -1.0, 0.0);
	n[6] = new Vector3(0.0, 1.0, 0.0);
	n[7] = new Vector3(0.0, 1.0, 0.0);

	int index[36] = { 0,1,4,1,5,4,1,2,6,1,6,5,2,3,7,2,7,6,4,7,3,0,4,3,4,5,6,4,6,7,1,0,3,2,1,3 };
	
	for (int i = 0; i < 8; i++) {
		vertexs.push_back(v[i]);
		normals.push_back(n[i]);
	}

	for (int i = 0; i < 12; i++) {
		Triangle* triangle = new Triangle(
			v[index[i * 3]],
			v[index[i * 3 + 1]],
			v[index[i * 3 + 2]],
			n[index[i * 3]],
			n[index[i * 3 + 1]],
			n[index[i * 3 + 2]]
		);

		meshes.push_back(triangle);
	}

	return;
}

