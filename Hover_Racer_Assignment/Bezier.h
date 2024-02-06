#pragma once

#include "Vectors.h";
#include "Matrices.h";



struct CurvePoint
{
	Vector posVect;
	Vector facingVect;
	float rotation;
};

struct CurveSegment 
{
	// Using 4 point curve

	CurvePoint points[4];
	unsigned int size = 0;

	bool isFull() {
		return size == 4;
	}

	void newPoint(float x, float y, float z, float rotation = 0) {
		if (!isFull()) {
			points[size++] = { {x,y,z}, rotation };
		}
	}


	Vector posOnSegment(float t) {
		int tInt = t;
		t -= tInt;
		tInt *= 3;

		//cout << "\n Inputed t = " << t;
		//cout << "\nControl Point 0: x= " << points[tInt].posVect.x << "  y= " << points[tInt].posVect.y << "  z= " << points[tInt].posVect.z;
		//cout << "\nControl Point 1: x= " << points[tInt + 1].posVect.x << "  y= " << points[tInt + 1].posVect.y << "  z= " << points[tInt + 1].posVect.z;
		//cout << "\nControl Point 2: x= " << points[tInt + 2].posVect.x << "  y= " << points[tInt + 2].posVect.y << "  z= " << points[tInt + 2].posVect.z;
		//cout << "\nControl Point 3: x= " << points[tInt + 3].posVect.x << "  y= " << points[tInt + 3].posVect.y << "  z= " << points[tInt + 3].posVect.z;
		Vector position;
		position.x = (1 - t) * (1 - t) * (1 - t) * points[tInt].posVect.x + 3 * ((1 - t) * (1 - t)) * t * points[tInt + 1].posVect.x + 3 * (1 - t) * t * t * points[tInt + 2].posVect.x + t * t * t * points[tInt + 3].posVect.x;
		position.y = (1 - t) * (1 - t) * (1 - t) * points[tInt].posVect.y + 3 * ((1 - t) * (1 - t)) * t * points[tInt + 1].posVect.y + 3 * (1 - t) * t * t * points[tInt + 2].posVect.y + t * t * t * points[tInt + 3].posVect.y;
		position.z = (1 - t) * (1 - t) * (1 - t) * points[tInt].posVect.z + 3 * ((1 - t) * (1 - t)) * t * points[tInt + 1].posVect.z + 3 * (1 - t) * t * t * points[tInt + 2].posVect.z + t * t * t * points[tInt + 3].posVect.z;
		
		// Using the .length value to convay the rotation.
		position.length = (1 - t) * (1 - t) * (1 - t) * points[tInt].posVect.length + 3 * ((1 - t) * (1 - t)) * t * points[tInt + 1].posVect.length + 3 * (1 - t) * t * t * points[tInt + 2].posVect.length + t * t * t * points[tInt + 3].posVect.length;

		//cout << "\nPos: x= " << position.x << "  y= " << position.y << "  z= " << position.z;
		return position;
	}
};

struct Curve 
{
	CurveSegment segments[10];
	int size = 0;

	void newSegment(Vector p1, float r1, Vector p2, float r2, Vector p3, float r3, Vector p4, float r4) {
		CurveSegment newSeg;
		newSeg.newPoint(p1.x, p1.y, p1.z, r1);
		newSeg.newPoint(p2.x, p2.y, p2.z, r2);
		newSeg.newPoint(p3.x, p3.y, p3.z, r3);
		newSeg.newPoint(p4.x, p4.y, p4.z, r4);
		segments[size] = newSeg;
		size++;
	}

	Vector posOnCurve(float t) {
		int tInt = t;
		//cout << "\nt = " << t << "  tInt = " << tInt;
		return segments[tInt].posOnSegment(t - tInt);
	}

	CurvePoint closestPoint(Vector pos, float resolution) {
		Vector curvePos = posOnCurve(0);
		Vector checkPos;
		float dist = pos.findDist(curvePos);
		float checkDist;
		float closest_t = 0;
		float rotation = curvePos.length;
		for (int i = 0; i < size*resolution - 1; i++)
		{
			checkPos = posOnCurve(i / resolution);
			
			checkDist = pos.findDist(checkPos);
			if (checkDist < dist) {
				closest_t = 1 / resolution;
				dist = checkDist;
				curvePos = checkPos;
				float rotation = curvePos.length;
			}
		}
		return { curvePos, (curvePos - posOnCurve(closest_t + 0.0001)).normalize(), rotation};
	}
};