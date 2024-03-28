#pragma once
#include "Vectors.h"
#include "Matrices.h"
#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

#define CAMSPEED 50
#define ROTATIONSPEED 0.2f
#define PI 3.14159265359f


enum CameraStates {
	FreeMoving, ThirdPerson, FirstPerson, Surveillance
};



struct SnazzyCam {
	ICamera* Camera;
	CameraStates state = FreeMoving;
	Matrix2D matrix;
	IModel* parent;
	I3DEngine* myEngine;

	void setState(CameraStates newState) {
		myEngine->StopMouseCapture();
		Camera->DetachFromParent();
		state = newState;
		switch (state)
		{
		case FreeMoving:
			// Setting default FreeCam Position.
			matrix = { {
			{-0.89172, 0, -0.452573, 0},
			{0.195523, 0.901861, -0.385249, 0},
			{0.408158, -0.432025, -0.804215, 0},
			{-30, 100, 220, 1} } };
			myEngine->StartMouseCapture();
			break;
		case ThirdPerson:
			Camera->AttachToParent(parent);
			matrix = { {
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 10, 10, 1} } };
			matrix = rotateOn(matrix, matrix.vectorizeX(), 0.5f);
			matrix = rotateOn(matrix, matrix.vectorizeY(), PI);
			matrix.matrixToCam(Camera);
			break;
		case FirstPerson:
			Camera->AttachToParent(parent);
			matrix = { {
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 1, 1, 1} } };
			matrix = rotateOn(matrix, matrix.vectorizeY(), PI);
			matrix.matrixToCam(Camera);
			break;
		case Surveillance:
			matrix = { {
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 50, -100, 1} } };
			matrix.matrixToCam(Camera);
			break;
		default:
			break;
		}
	}

	void camKeys() {
		if (myEngine->KeyHit(Key_2)) {
			setState(FreeMoving);
		}
		if (myEngine->KeyHit(Key_3)) {
			setState(ThirdPerson);
		}
		if (myEngine->KeyHit(Key_1)) {
			setState(FirstPerson);
		}
		if (myEngine->KeyHit(Key_4)) {
			setState(Surveillance);
		}

	}

	void useState(float deltaTime) {
		switch (state)
		{
		case FreeMoving:
			if (myEngine->KeyHeld(Key_Up)) {
				matrix = matrix + (matrix.vectorizeZ() * deltaTime * CAMSPEED);
			}
			if (myEngine->KeyHeld(Key_Down)) {
				matrix = matrix + (matrix.vectorizeZ() * deltaTime * CAMSPEED * -1);
			}
			if (myEngine->KeyHeld(Key_Left)) {
				matrix = matrix + (matrix.vectorizeX() * deltaTime * CAMSPEED * -1);
			}
			if (myEngine->KeyHeld(Key_Right)) {
				matrix = matrix + (matrix.vectorizeX() * deltaTime * CAMSPEED);
			}

			matrix = rotateOn(matrix, matrix.vectorizeX(), myEngine->GetMouseMovementY() * deltaTime * -ROTATIONSPEED);
			matrix = rotateY(matrix, myEngine->GetMouseMovementX() * deltaTime * ROTATIONSPEED);
			matrix.matrixToCam(Camera);
			break;
		case ThirdPerson:

			break;
		case FirstPerson:
			break;
		case Surveillance:
			Camera->LookAt(parent);
			break;
		default:
			break;
		}
	}

	void cameraLogic(float deltaTime) {
		camKeys();
		useState(deltaTime);
	}
};