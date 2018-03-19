#include "BRDF.h"


// Geometrical Attenuation Factor: Kelemen
// pay attention: n.l, n.v has been removed here to simplify calculation
float GFunction(Vector3 l, Vector3 v, float alpha) {
	Vector3 h = (v + l).normalize();
	float dotVH = v.dot(h);

	return 0.25f / float(pow(dotVH, 2));
}


// Normal Distribution Function: D(h)
float DFunction(Vector3 n, Vector3 m, float alpha) {
	return alpha * alpha / PI / float(pow(1.0f + float(pow(n.dot(m), 2))*(alpha*alpha - 1), 2));
}


// Fresnel Equations parameters
// Use Schlick Functions
float FFunction(Vector3 v, Vector3 h, float reflectance) {
	return reflectance + (1 - reflectance)*float(pow(1 - v.dot(h), 5));
}


float BRDF::calculateIntegral() {
	return 1.0f;
}


float BRDF::getBRDF(Vector3 l, Vector3 v, Vector3 n) {
	v = v * -1.0f; // reverse the direction of v to ensure the consistency of v & l
	Vector3 h = (v + l).normalize();
	float alpha = roughness * roughness;

	float F = FFunction(v, h, reflectance);

	float D = DFunction(n, h, alpha);
	
	float G = GFunction(l, v, alpha); // use G(GGX) to calculate

	return F * D * G; // pay attention n.l, n.v has been removed when calculating G
}


float BRDF::getPDF(Vector3 l, Vector3 v, Vector3 n) {

	return k * getBRDF(l, v, n)*n.dot(l);
}


Vector3 rotate(Vector3 dir, Vector3 eulerAngle) {

	Vector3 temp = dir;

	float yaw = eulerAngle[2], pitch = eulerAngle[1];

	Vector2 xy_cord(dir[0], dir[1]);
	xy_cord = xy_cord.rotate(yaw);

	temp.value[0] = xy_cord[0];
	temp.value[1] = xy_cord[1];

	float x = xy_cord.magnitude(), y = temp.value[2];

	Vector2 vec(x, y);
	vec = vec.rotate(pitch);

	if (x < EPISILON) return Vector3(0.0f, 0.0f, vec[1]);
	temp = Vector3(vec[0] * temp.value[0] / x, vec[0] * temp.value[1] / x, vec[1]);
	temp.normalize();

	return temp;
}


Vector3 BRDF::diffReflectDir(Vector3 v, Vector3 n) {
	static int sample_num = 0;
	if (sample_num >= SAMPLE_NUM) sample_num -= SAMPLE_NUM;
	
	//Vector2 point = hammersleyPoints(sample_num, SAMPLE_NUM);
	Vector2 point = Vector2(rand() / (RAND_MAX + 1.0f), rand() / (RAND_MAX + 1.0f));
	sample_num += 1;

	point = importanceSampling(point);

	Vector3 halfWay = rotate(n, Vector3(0.0f, point.value[1], point.value[0]));

	float dot = halfWay.dot(v);

	return v - halfWay * 2.0f*dot;

}


Vector2 BRDF::hammersleyPoints(int i, int N) {
	float sum = 0.0f, parameter = 1.0f;
	while (i > 0) {
		int left = i % 2;
		i /= 2;
		parameter /= 2.0f;
		sum += left * parameter;
	}
	return Vector2(float(i) / float(N), sum);
}


Vector2 BRDF::importanceSampling(Vector2 point) {
	static float glossiness = float(pow(8192, roughness));
	
	float phi = 2.0f*PI*point.value[0];
	float theta = acos(pow(1 - (glossiness + 1.0f)*point.value[1] / (glossiness + 2.0f), 1.0f / (glossiness + 1)));

	return Vector2(phi, theta);
}