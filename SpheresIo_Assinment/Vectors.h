#pragma once
#include <TL-Engine.h>
#include <iostream>
#include <sstream>
using namespace std;
using namespace tle;


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

	void addVector(Vector vect) {
		x += vect.x;
		y += vect.y;
		z += vect.z;
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

	}

	void move(IModel* model, float factor = 1) {
		model->Move(x * factor, y * factor, z * factor);
	}

	Vector normalize() {
		getLength();
		Vector normalized;
		normalized.x = x / length;
		normalized.y = y / length;
		normalized.z = z / length;
		return normalized;
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
};