﻿#pragma once

#include "Vectors.h";
#include "Matrices.h";



struct CurvePoint
{
	Vector posVect;
	float rotation = 0;
	Vector facingVect;
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


	CurvePoint posOnSegment(float t) {
		int tInt = t;
		t -= tInt;
		tInt *= 3;

		//cout << "\n Inputed t = " << t;
		//cout << "\nControl Point 0: x= " << points[tInt].posVect.x << "  y= " << points[tInt].posVect.y << "  z= " << points[tInt].posVect.z;
		//cout << "\nControl Point 1: x= " << points[tInt + 1].posVect.x << "  y= " << points[tInt + 1].posVect.y << "  z= " << points[tInt + 1].posVect.z;
		//cout << "\nControl Point 2: x= " << points[tInt + 2].posVect.x << "  y= " << points[tInt + 2].posVect.y << "  z= " << points[tInt + 2].posVect.z;
		//cout << "\nControl Point 3: x= " << points[tInt + 3].posVect.x << "  y= " << points[tInt + 3].posVect.y << "  z= " << points[tInt + 3].posVect.z;
		CurvePoint position = { {
		(1 - t) * (1 - t) * (1 - t) * points[tInt].posVect.x + 3 * ((1 - t) * (1 - t)) * t * points[tInt + 1].posVect.x + 3 * (1 - t) * t * t * points[tInt + 2].posVect.x + t * t * t * points[tInt + 3].posVect.x,
		(1 - t) * (1 - t) * (1 - t) * points[tInt].posVect.y + 3 * ((1 - t) * (1 - t)) * t * points[tInt + 1].posVect.y + 3 * (1 - t) * t * t * points[tInt + 2].posVect.y + t * t * t * points[tInt + 3].posVect.y,
		(1 - t) * (1 - t) * (1 - t) * points[tInt].posVect.z + 3 * ((1 - t) * (1 - t)) * t * points[tInt + 1].posVect.z + 3 * (1 - t) * t * t * points[tInt + 2].posVect.z + t * t * t * points[tInt + 3].posVect.z }
		, (1 - t)* (1 - t)* (1 - t)* points[tInt].rotation + 3 * ((1 - t) * (1 - t)) * t * points[tInt + 1].rotation + 3 * (1 - t) * t * t * points[tInt + 2].rotation + t * t * t * points[tInt + 3].rotation };
		
		// Using the .length value to convay the rotation.
		
		//cout << "\nPos: x= " << position.x << "  y= " << position.y << "  z= " << position.z;
		return position;
	}

	Vector derivitive(float t) {
		int tInt = t;
		t -= tInt;
		tInt *= 3;

		return ((points[tInt + 1].posVect - points[tInt].posVect) * (3 * ((1 - t) * (1 - t)))) + ((points[tInt + 2].posVect - points[tInt + 1].posVect) * (6 * (1 - t) * t)) + ((points[3].posVect - points[2].posVect) * ((3 * t * t) * (3 * t * t)));
	}
};

struct Curve
{
	CurveSegment segments[50];
	int size = 0;

	void newSegment(Vector p1, float r1, Vector p2, float r2, Vector p3, float r3, Vector p4, float r4) {
		CurveSegment newSeg;
		newSeg.newPoint(p1.x, p1.z, p1.y, r1);
		newSeg.newPoint(p2.x, p2.z, p2.y, r2);
		newSeg.newPoint(p3.x, p3.z, p3.y, r3);
		newSeg.newPoint(p4.x, p4.z, p4.y, r4);
		segments[size] = newSeg;
		size++;
	}

	void move(Vector input) {
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < segments[i].size; j++)
			{
				segments[i].points[j].posVect = segments[i].points[j].posVect + input;
			}
		}
	}

	void scale (float input) {
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < segments[i].size; j++)
			{
				segments[i].points[j].posVect = segments[i].points[j].posVect * input;
			}
		}
	}

	void rotate(axis xyz, float angle) {

		switch (xyz)
		{
		case X:
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < segments[i].size; j++)
				{
					segments[i].points[j].posVect.rotateX(angle);
				}
			}
			break;
		case Y:
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < segments[i].size; j++)
				{
					segments[i].points[j].posVect.rotateY(angle);
				}
			}
			break;
		case Z:
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < segments[i].size; j++)
				{
					segments[i].points[j].posVect.rotateZ(angle);
				}
			}
			break;
		default:
			break;
		}
		
	}

	CurvePoint posOnCurve(float t) {
		int tInt = t;
		//cout << "\nt = " << t << "  tInt = " << tInt;
		return segments[tInt].posOnSegment(t - tInt);
	}

	CurvePoint closestPoint(Vector pos, float resolution) {
		CurvePoint curvePos = segments[0].posOnSegment(0);
		CurvePoint checkPos;
		float closestDist = pos.findDist(curvePos.posVect);
		float checkDist;
		float closest_i = 0;
		for (int i = 0; i < size * resolution - 1; i++)
		{
			checkPos = posOnCurve(i / resolution);

			checkDist = pos.findDist(checkPos.posVect);
			if (checkDist < closestDist) {
				closest_i = i;
				closestDist = checkDist;
				curvePos = checkPos;
			}
		}
		int tInt = closest_i;
		curvePos = { curvePos.posVect, curvePos.rotation, (curvePos.posVect - posOnCurve(closest_i / resolution - 0.0001).posVect).normalize()};
		cout << "\n rotation = " << curvePos.rotation;
		return curvePos;
	}
};