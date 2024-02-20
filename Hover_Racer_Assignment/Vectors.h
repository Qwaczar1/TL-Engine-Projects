#pragma once
#include <TL-Engine.h>
#include <iostream>
using namespace std;
using namespace tle;


#define PI 3.14159265359f



struct Vector
{
	float x;
	float y;
	float z;
	float length;

	void vectorSet(float newX, float newY, float newZ) {
		x = newX;
		y = newY;
		z = newZ;
		length = sqrt((x * x) + (y * y) + (z * z));
	}

	void getLength() {
		length = sqrt((x * x) + (y * y) + (z * z));
	}

	void setLength(float newLength) {

		length = sqrt((x * x) + (y * y) + (z * z));
		x *= newLength / length;
		y *= newLength / length;
		z *= newLength / length;
		length = newLength;
	}

	float findDist(Vector vect) {
		return sqrt(((x - vect.x) * (x - vect.x)) + ((y - vect.y) * (y - vect.y)) + ((z - vect.z) * (z - vect.z)));
	}

	Vector addVector(Vector vect) {
		return { x + vect.x, y + vect.y, z + vect.z };
	}

	Vector multiplyVector(float a) {
		return { x * a, y * a, z * a };
	}

	Vector normalize() {
		getLength();
		if (length != 0 && length != -0) {
			Vector normalized{ x / length, y / length, z / length, 1 };
			//cout << "Normalized Vector x : " << normalized.x << "  y : " << normalized.y << "  z : " << normalized.z;
			return normalized;
		}
		return { 0,0,0,0 };
	}

	float dotProduct(Vector w) {
		return (x * w.x + y * w.y + z * w.z);
	}

	Vector crossProduct(Vector w) {
		//Vector newVector{ y * w.z - z * w.y, z * w.x - x * w.z, x * w.y - y * w.x, 1 };
		return { y * w.z - z * w.y, z * w.x - x * w.z, x * w.y - y * w.x, 1 };
	}

	float angleDiffrence(Vector w) {
		return acos(dotProduct(w));
	}

	void rotateX(float angle) {
		z = z * cos(-angle) - y * sin(-angle);
		y = z * sin(-angle) + y * cos(-angle);
		setLength(length);
	}

	void rotateY(float angle) {
		x = x * cos(-angle) - z * sin(-angle);
		z = x * sin(-angle) + z * cos(-angle);
		setLength(length);
	}

	void rotateZ(float angle) {
		x = x * cos(-angle) - y * sin(-angle);
		y = x * sin(-angle) + y * cos(-angle);
		setLength(length);
	}

	void rotateOn(Vector axis, float angle) {
		Vector newVect = addVector(axis.multiplyVector(1 - cos(angle)).crossProduct(crossProduct(axis))).addVector(crossProduct(axis.multiplyVector(sin(angle))));
		x = newVect.x;
		y = newVect.y;
		z = newVect.z;
	}

	Vector operator - (Vector vect) {
		return { x - vect.x, y - vect.y, z - vect.z };
	}

	Vector operator + (Vector vect) {
		return { x + vect.x, y + vect.y, z + vect.z };
	}

	Vector operator * (float a) {
		return { x * a, y * a, z * a };
	}

	Vector operator *(Vector vect) {
		return { x * vect.x, y * vect.y, z * vect.z };
	}

	void move(IModel* model, float factor = 1) {
		model->Move(x * factor, y * factor, z * factor);
	}

	void directionalAline(Vector vect) {
		if (!((x >= 0 && vect.x >= 0) || (x <= 0 && vect.x <= 0))) {
			x = -x;
		}
		if (!((y >= 0 && vect.y >= 0) || (y <= 0 && vect.y <= 0))) {
			y = -y;
		}
		if (!((z >= 0 && vect.z >= 0) || (z <= 0 && vect.z <= 0))) {
			z = -z;
		}
	}

	void print() {
		cout << "\n x= " << x << "  y= " << y << "  z= " << z;
	}
};
