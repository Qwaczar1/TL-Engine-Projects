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

	ControlPoint points[80];
	unsigned int top_ = 0;

	bool isFull() {
		return top_ == 80;
	}

	void newPoint(float x, float y, float z, float rotation = 0) {
		if (!isFull()) {
			points[top_++] = { {x,y,z}, rotation };
		}
	}


	Vector posOnCureve(float t) {
		int tInt = t;
		t -= tInt;
		tInt *= 3;

		cout << "\nt = " << t << "   tInt = " << tInt;
		Vector position;
		position.x = (1 - t) * (1 - t) * (1 - t) * points[tInt].posVect.x + 3 * ((1 - t) * (1 - t)) * t * points[tInt + 1].posVect.x + 3 * (1 - t) * t * t * points[tInt + 2].posVect.x + t * t * t * points[tInt + 3].posVect.x;
		position.y = (1 - t) * (1 - t) * (1 - t) * points[tInt].posVect.y + 3 * ((1 - t) * (1 - t)) * t * points[tInt + 1].posVect.y + 3 * (1 - t) * t * t * points[tInt + 2].posVect.y + t * t * t * points[tInt + 3].posVect.y;
		position.z = (1 - t) * (1 - t) * (1 - t) * points[tInt].posVect.z + 3 * ((1 - t) * (1 - t)) * t * points[tInt + 1].posVect.z + 3 * (1 - t) * t * t * points[tInt + 2].posVect.z + t * t * t * points[tInt + 3].posVect.z;
		return position;
	}
};