#include "Shape.h"

float square(float num) {
	return pow(num, 2);
}

Shape::Shape() {
	center = Vector3(0.0f, 0.0f, 0.0f);
	vertexs.clear();
	normals.clear();
	meshes.clear();
}


Shape::~Shape() {

	vertexs.clear();
	normals.clear();
	meshes.clear();
}


void Shape::setOwner(Object* m) {
	for (auto& ptr : meshes)
		ptr->setOwner(m);
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


// unit is degree
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
	Vector3 uv, uuv;
	Vector3 qvec(quaternion.value[0], quaternion.value[1], quaternion.value[2]);


	for (auto ptr : vertexs) {
		*ptr = *ptr - center;
		uv = qvec * (*ptr);
		uuv = qvec * uv;
		uv = uv * (2.0f*quaternion.value[3]);
		uuv = uuv * 2.0f;

		*ptr = (*ptr) + uv + uuv + center;
	}

	for (auto ptr : normals) {
		uv = qvec * (*ptr);
		uuv = qvec * uv;
		uv = uv * (2.0f*quaternion.value[3]);
		uuv = uuv * 2.0f;

		*ptr = (*ptr) + uv + uuv;
	}

	for (auto ptr : meshes) {
		Vector3 normalVector = ptr->getNormal();
		uv = qvec * normalVector;
		uuv = qvec * uv;
		uv = uv * (2.0f*quaternion.value[3]);
		uuv = uuv * 2.0f;

		normalVector = normalVector + uv + uuv;
		ptr->setNormalVector(normalVector);
	}
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

	setCenter((small + big)*0.5f);

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

	for (int i = 0; i < 8; i++) {
		float k[3] = { 0.0f, 0.0f, 0.0f };
		for (int j = 0; j < 3; j++) {
			if (v[i]->value[j] < big.value[j]) k[j] = -1.0f;
			else k[j] = 1.0f;
		}

		n[i] = new Vector3(k[0], k[1], k[2]);
		n[i]->normalize();
	}

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


void Rectangle::tessellate(float iteration, Object* obj) {
	// Rectangle has no need to tessellate here
	return;
}


Sphere::Sphere(Vector3 m_center, float m_radius) {
	try {
		_ASSERT(m_radius > 0);
	}
	catch (exception e) {
		LOGPRINT("Wrong radius to initialize the sphere");
	}

	setCenter(Vector3(0.0f,0.0f,0.0f));
	radius = m_radius;

	Vector3* v[6];
	Vector3* n[6];

	v[0] = new Vector3(0.0f, 0.0f, m_radius);
	v[1] = new Vector3(-m_radius, 0.0f, 0.0f);
	v[2] = new Vector3(0.0f, -m_radius, 0.0f);
	v[3] = new Vector3(m_radius, 0.0f, 0.0f);
	v[4] = new Vector3(0.0f, m_radius, 0.0f);
	v[5] = new Vector3(0.0f, 0.0f, -m_radius);

	float rate = 1 / m_radius;
	for (int i = 0; i < 6; i++) {
		n[i] = new Vector3((*v[i]) * rate);
		vertexs.push_back(v[i]);
		normals.push_back(n[i]);
	}

	int index[24] = {1,2,0,2,3,0,3,4,0,4,1,0,5,2,1,5,3,2,5,4,3,5,1,4};

	for (int i = 0; i < 8; i++) {
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

	translate(m_center);
}


void Sphere::tessellate(float iteration, Object* obj) {
	for (int num = 0; num < iteration; num++) {
		int size = (int)meshes.size();

		for (int i = 0; i < size; i++) {
			Triangle* triangle = meshes[i];

			// add new triangles, vertexs and normals into the shape
			Triangle* tri[3];
			Vector3 *v[3], *n[3];
			for (int k = 0; k < 3; k++) {
				tri[k] = new Triangle();
				tri[k]->setOwner(obj);

				v[k] = new Vector3();
				n[k] = new Vector3();
				vertexs.push_back(v[k]);
				normals.push_back(n[k]);
				meshes.push_back(tri[k]);
			}

			float len = radius - sqrt(pow(radius, 2) - pow(triangle->averageSideLength() / 2, 2));
			triangle->tessellate(tri, v, n, len, center);
		}
	}

	return;
}