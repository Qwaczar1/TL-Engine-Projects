// Hover_Racer_Assignment.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

#include "Bezier.h"
#include "Vectors.h"
#include "Matrices.h"

#define PI 3.14159265359f

void viewVector(IModel* model1, IModel* model2, IModel* model3 , Vector vect, Vector pos) {
	model1->SetPosition(pos.x, pos.y, pos.z);
	vect.move(model1);
	model2->SetPosition(pos.x, pos.y, pos.z);
	vect.move(model2);
	vect.move(model2);
	model3->SetPosition(pos.x, pos.y, pos.z);
	vect.move(model3);
	vect.move(model3);
	vect.move(model3);

}

struct Racer {
	IModel* model;
	IModel* cubes[6];
	Matrix2D matrix;
	Vector movementVector = { 0,0,0,0 };
	Vector rotationVector = { 0,0,0,0 };
	Vector temp;
	float baseSpeed = 0.2;


	void accMovement(float factor) {
		movementVector = movementVector + matrix.vectorizeZ() * baseSpeed * factor;
		movementVector.getLength();
	}

	void accRotation(float factor) {
	    //rotationVector.rotateOn(matrix.vectorizeY(), factor * ((1 / ((rotationVector.length * (movementVector.length + 1)) / 10 + 0.1))));

		rotationVector = rotationVector + matrix.vectorizeY() * ((1 / (rotationVector.length * 0.1 + 0.1)) * factor);
		rotationVector.getLength();
	}

	// Applies a downwords vector
	void gravity(float factor) {
		movementVector.y -= factor * 0.1;
		movementVector.getLength();
	}

	void friction(float factor) {
		// motion friction
		float force = (factor * movementVector.length * movementVector.length) + (factor * 0.05);
		if (movementVector.length > force) {
			movementVector.setLength(movementVector.length - force);
		}
		else {
			movementVector = { 0,0,0,0 };
		}
		// rotation friction
		force = factor * rotationVector.length;
		if (rotationVector.length > force) {
			rotationVector.setLength(rotationVector.length - force);
		}
		else {
			rotationVector = { 0,0,0,0 };
		}
	}

	void trackColision(Curve track, float factor) {

		//  Finds out if te racer is in colision with the Bezier curve Track, and applies an outwards force to get the racer out of the track.

		float targetDistance = 10;
		CurvePoint closestPoint = track.closestPoint(matrix.vectorizePos(), 500);

		// Getting a track normal vector for the closest track point.



		Vector trackNormal = { -closestPoint.facingVect.z, 0, closestPoint.facingVect.x };
		trackNormal.rotateOn(closestPoint.facingVect, closestPoint.rotation);
		trackNormal = trackNormal.normalize().crossProduct(closestPoint.facingVect.normalize());

		viewVector(cubes[0], cubes[1], cubes[2], trackNormal, closestPoint.posVect);
		viewVector(cubes[3], cubes[4], cubes[5], closestPoint.facingVect, closestPoint.posVect);

		cout << "\n dotProduct: " << matrix.vectorizeY().normalize().dotProduct(trackNormal.normalize());
		if (matrix.vectorizeY().normalize().dotProduct(trackNormal.normalize()) < 0.9999) {
			matrix = rotateTo(matrix, Y, trackNormal.normalize());

			cout << "\nmatrix: \n";
			matrix.print();
		}

		/*cout << "\n Current Normal : x= " << trackNormal.x << "  y= " << trackNormal.y << "  z= " << trackNormal.z;*/

		// Checking if the racer is too close to track

		trackNormal.setLength(targetDistance);
		closestPoint.posVect.addVector(trackNormal);
		Vector racerPos = { model->GetX(), model->GetY(), model->GetZ() };
		racerPos = racerPos - closestPoint.posVect;
		float dotProduct = trackNormal.normalize().dotProduct(racerPos.normalize());

		//cout << "\n Dot product = " << dotProduct;
		//cout << "\n angle math crap = " << abs(dotProduct) * racerPos.length;

		racerPos.getLength();
		if (racerPos.length < 10) {
			if (dotProduct < 0.0) {
				float pushPower = abs(dotProduct) * racerPos.length;
				//cout << "\n Push Power = " << pushPower;
				trackNormal.setLength(pushPower);
				//cout << "\n Current Normal : x= " << trackNormal.x << "  y= " << trackNormal.y << "  z= " << trackNormal.z;
				gravity(-factor);
				movementVector = movementVector + trackNormal;

				
			}
		}
	}

	void applyVectors(float factor) {

		//   Turns all of the acting vectors to the racer making it move/rotate according to the vectors.
		matrix = matrix + movementVector;
		matrix = rotateBy(matrix, rotationVector * (PI / 180));
		
		matrix.matrixToModel(model);
	}
};



void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );
	myEngine->AddMediaFolder("./Media");

	/**** Set up your scene here ****/

	ICamera* mycamera = myEngine->CreateCamera(kFPS);

	IMesh* testMesh = myEngine->LoadMesh("MainTrack.x");
	IModel* TestModel = testMesh->CreateModel(0,0,0);

	float trackScale = 60;

	TestModel->Scale(60);

	IMesh* cubeMesh = myEngine->LoadMesh("Cube.x");
	IMesh* racerMesh = myEngine->LoadMesh("racer.x");
	/*IModel* racerModel = racerMesh->CreateModel();
	racerModel->RotateLocalY(180);*/

	Racer myRacer;
	myRacer.model = racerMesh->CreateModel();
	for (int i = 0; i < 6; i++)
	{
		myRacer.cubes[i] = cubeMesh->CreateModel();
		myRacer.cubes[i]->Scale(0.02);
	}
	

	Curve trackCurve;
	trackCurve.newSegment({ 0,0,0 }, 0, { 0, 0, 55.2125 }, 0, { -43.7876, 50, 100 }, 0, { -100, 0, 100 }, 0);
	trackCurve.newSegment({ -100, 0, 100}, 0, { -154.213, 0, 100}, 0, { -200, 0, 55.2125 }, 0, { -200, 0, 0}, 0);
	trackCurve.newSegment({ -200,0,0 }, 0, { -200, 0, -55.2125 }, 0, { -154.213, 0, -100 }, 0, { -100, 0, -100 }, 0);
	trackCurve.newSegment({ -100, 0, -100}, 0, { -43.7876, 0, -100}, 0, { 0, 0, -55.2125 }, 0, { 0, 0, 0}, 0);

	IModel* cubes[4000];
	Vector temp;

	for (int i = 0, o = 0; i <= trackCurve.size * 50; i++)
	{
		cubes[i] = cubeMesh->CreateModel();
		o = i / 50.0f;
		temp = trackCurve.posOnCurve(i / 50.0f);
		cout << "\n";
		temp.print();
		temp.move(cubes[i]);
		cubes[i]->Scale(0.05);
	}

	float deltaTime = myEngine->Timer();

	mycamera->RotateLocalX(20);

	/*myRacer.model->RotateY(45);
	myRacer.matrix.martixFromModel(myRacer.model);
	myRacer.matrix.print();

	myRacer.model->RotateLocalY(45);
	myRacer.matrix.martixFromModel(myRacer.model);
	myRacer.matrix.print();

	myRacer.model->MoveX(5);
	myRacer.matrix.martixFromModel(myRacer.model);
	myRacer.matrix.print();

	myRacer.model->MoveLocalX(5);
	myRacer.matrix.martixFromModel(myRacer.model);
	myRacer.matrix.print();

	float matrix44[4][4] = {
		{0,0,-1,0},
		{0,1,0,0},
		{1,0,0,0},
		{5,0,-5,1}
	};
	myRacer.model->SetMatrix(matrix44[0]);*/

	/*myRacer.matrix = rotateY(myRacer.matrix, 45 * (PI / 180));
	myRacer.matrix.print();
	myRacer.matrix = rotateY(myRacer.matrix, 45 * (PI / 180));
	myRacer.matrix.print();

	myRacer.matrix.matrixToModel(myRacer.model);*/

	
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		// getting deltaTime
		deltaTime = myEngine->Timer();

		if (myEngine->KeyHeld(Key_W)) {
			myRacer.accMovement(-deltaTime);
		} 
		else if (myEngine->KeyHeld(Key_S)) {
			myRacer.accMovement(deltaTime);
		}
		if (myEngine->KeyHeld(Key_A)) {
			myRacer.accRotation(-deltaTime);
		}
		if (myEngine->KeyHeld(Key_D)) {
			myRacer.accRotation(deltaTime);
		}

		myRacer.gravity(deltaTime);
		

		myRacer.friction(deltaTime);

		mycamera->SetPosition(myRacer.model->GetX(), myRacer.model->GetY() + 10, myRacer.model->GetZ() - 20);

		myRacer.trackColision(trackCurve, deltaTime);

		//myRacer.matrix = rotateOn(myRacer.matrix, { 1,1,1 }, 0.01);

		myRacer.applyVectors(deltaTime);

		
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
