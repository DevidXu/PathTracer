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
	for (auto& ele : vertexs)
		delete ele;
	for (auto& ele : normals)
		delete ele;
	for (auto& ele : texcoords)
		delete ele;
	for (auto& ele : meshes)
		delete ele;

	vertexs.clear();
	normals.clear();
	texcoords.clear();
	meshes.clear();

}


void Shape::setOwner(Object* m) {
	for (auto& ptr : meshes)
		ptr->setOwner(m);
}


// don't use it, the later fix is ignored in this function
void Shape::translate(Vector3 movement) {
	try {
		if (vertexs.size() == 0) throw("No vertex information!");
		for (auto ptr : vertexs)
			if (ptr)
				*ptr = (*ptr) + movement;
			else throw ("Invalid vertex information");

		center = center + movement;

		for (auto& ele : meshes)
			ele->calculateCentroid();
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
		ptr->calculateCentroid();
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
	/*
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
	*/
	Vector3 temp(m_radius, m_radius, m_radius);
	Vector3 small = m_center - temp;
	Vector3 big = m_center + temp;
	setCenter(m_center);
	radius = m_radius;

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

		triangle->setInfinite(true);
		meshes.push_back(triangle);
	}
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


Model::Model(Vector3 move, float scale, string filename, string textureName) {
	ifstream inf;
	string s, s1, s2, s3, s4;
	try {
		inf.open(filename);
		_ASSERT(inf.is_open());
	}
	catch (exception e) {
		cout << "The file " << filename << " doesn't exist in current path" << endl;
		return;
	}

	if (textureName != "")
		texture = imread(textureName, CV_LOAD_IMAGE_UNCHANGED);

	while (getline(inf, s))
	{
		istringstream in(s);
		in >> s1 >> s2 >> s3 >> s4;

		if (s[0] == 'v' && s[1]==' ') {
			Vector3 *v = new Vector3(float(atof(s2.c_str())),float(atof(s3.c_str())),float(atof(s4.c_str())));
			*v = *v * scale;
			vertexs.push_back(v);

		};

		if (s[0] == 'v' && s[1] == 'n') {
			Vector3 *n = new Vector3(float(atof(s2.c_str())), float(atof(s3.c_str())), float(atof(s4.c_str())));
			n->normalize();
			normals.push_back(n);
		}

		if (s[0] == 'v'&&s[1] == 't') {
			Vector2 *t = new Vector2(float(atof(s2.c_str())), float(atof(s3.c_str())));
			texcoords.push_back(t);
		}

		if (s[0] == 'f')
		{
			vector<int> index1, index2, index3;
			int pos;
			pos = (int)s2.find("/");
			while (pos != std::string::npos)
			{
				string x = s2.substr(0, pos);
				index1.push_back(atoi(s2.substr(0, pos).c_str()));
				s2 = s2.substr(pos + 1, s2.size());
				pos = (int)s2.find("/");
			}
			index1.push_back(atoi(s2.substr(0, pos).c_str()));

			pos = (int)s3.find("/");
			while (pos != std::string::npos)
			{
				string x = s3.substr(0, pos);
				index2.push_back(atoi(s3.substr(0, pos).c_str()));
				s3 = s3.substr(pos + 1, s3.size());
				pos = (int)s3.find("/");
			}
			index2.push_back(atoi(s3.substr(0, pos).c_str()));

			pos = (int)s4.find("/");
			while (pos != std::string::npos)
			{
				string x = s4.substr(0, pos);
				index3.push_back(atoi(s4.substr(0, pos).c_str()));
				s4 = s4.substr(pos + 1, s4.size());
				pos = (int)s4.find("/");
			}
			index3.push_back(atoi(s4.substr(0, pos).c_str()));

			Triangle* triangle = nullptr;
			if (index1.size() == 1) { // no normal and texcoord
				static bool generate = false;
				if (!generate) {
					for (auto& ele : vertexs) {
						Vector3* n = new Vector3(*ele);
						n->normalize();
						normals.push_back(n);
					}
					generate = true;
				}
				triangle = new Triangle(
					vertexs[index1[0] - 1], vertexs[index2[0] - 1], vertexs[index3[0] - 1],
					vertexs[index1[0] - 1], vertexs[index2[0] - 1], vertexs[index3[0] - 1]
				);
			}
			else {
				triangle = new Triangle(
					vertexs[index1[0] - 1], vertexs[index2[0] - 1], vertexs[index3[0] - 1],
					normals[index1[2] - 1], normals[index2[2] - 1], normals[index3[2] - 1]
				);
				if (textureName != "") triangle->setTexcoord(texcoords[index1[1] - 1], texcoords[index2[1] - 1], texcoords[index3[1] - 1], &texture);
			}

			meshes.push_back(triangle);
		}
	}

	//setCenter(move);
	translate(move);
	inf.close();
}
