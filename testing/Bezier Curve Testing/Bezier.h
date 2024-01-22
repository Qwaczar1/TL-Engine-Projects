#pragma once

#include "Vectors.h";
#include "Matrices.h";


struct ControlPoint
{
	Vector posVect;
	float rotation;
};

struct Curve 
{
	// Using 4 point curve

	ControlPoint points[4];
	unsigned int top = 0;

	bool isEmpty() {
		return top == 0;
	}

	bool isFull() {
		return top == 4;
	}

	void newPoint(float x, float y, float z, float rotation = 0) {
		if (!isFull()) {
			points[top++] = { {x,y,z}, rotation };
		}
	}

	ControlPoint top() {
		return points[top];
	}

	Vector posOnCureve(float t) {

		//  P = (1−t)3P1 + 3(1−t)2tP2 +3(1−t)t2P3 + t3P4

		Vector position;
		position.x = (1 - t) * (1 - t) * (1 - t) * points[0].posVect.x + 3 * ((1 - t) * (1 - t)) * t * points[1].posVect.x + 3 * (1 - t) * t * t * points[2].posVect.x + t * t * t * points[3].posVect.x;
		position.y = (1 - t) * (1 - t) * (1 - t) * points[0].posVect.y + 3 * ((1 - t) * (1 - t)) * t * points[1].posVect.y + 3 * (1 - t) * t * t * points[2].posVect.y + t * t * t * points[3].posVect.y;
		position.z = (1 - t) * (1 - t) * (1 - t) * points[0].posVect.z + 3 * ((1 - t) * (1 - t)) * t * points[1].posVect.z + 3 * (1 - t) * t * t * points[2].posVect.z + t * t * t * points[3].posVect.z;
		return position;
	}
};