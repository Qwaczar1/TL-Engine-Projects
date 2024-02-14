// Hover_Racer_Assignment.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

#include "Bezier.h"
#include "Vectors.h"

#define PI 3.14159265359f

struct Racer {
	IModel* model;
	Vector facingVector = {0,0,1,1};
	Vector normalVector = { 0,1,0,1 };
	Vector movementVector = { 0,0,0 };
	Vector rotationVector = { 0,0,0 };
	Vector positionvector = { 0,0,0 };
	float baseSpeed = 10;


	void accMovement(float factor) {
		movementVector = movementVector + facingVector * ((1 / (movementVector.length/10 + 0.1)) * factor * baseSpeed);
		movementVector.getLength();
	}

	void accRotation(float factor) {
	    rotationVector.rotateOn(normalVector, factor * ((1 / ((rotationVector.length / (movementVector.length + 1)) / 10 + 0.1))));
		rotationVector.getLength();
	}

	// Applies a downwords vector
	void gravity(float factor) {
		movementVector.y -= factor * 20;
		movementVector.getLength();
	}

	void friction(float factor) {
		// motion friction
		float force = factor * 10;
		if (movementVector.length > force) {
			movementVector.setLength(movementVector.length - force);
		}
		else {
			movementVector = { 0,0,0,0 };
		}
		// rotation friction
		force = factor * 2;
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
		CurvePoint closestPoint = track.closestPoint({ model->GetX(), model->GetY(), model->GetZ() }, 500);

		// Getting a track normal vector for the closest track point.
		Vector trackNormal = { -closestPoint.facingVect.z, 0, closestPoint.facingVect.x };
		trackNormal.rotateOn(closestPoint.facingVect, closestPoint.rotation);
		trackNormal = trackNormal.normalize().crossProduct(closestPoint.facingVect.normalize());

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

				trackNormal = trackNormal.normalize();
				normalVector = normalVector.normalize();
				cout << "\n \n trackNormal : x= " << trackNormal.x << "  y= " << trackNormal.y << "  z= " << trackNormal.z;
				cout << "\n normalVector : x= " << normalVector.x << "  y= " << normalVector.y << "  z= " << normalVector.z;
				Vector angleDif = vectAngleDifrence(trackNormal, normalVector);
				rotationVector = rotationVector + angleDif;
				cout << "\n angleDif : x= " << angleDif.x << "  y= " << angleDif.y << "  z= " << angleDif.z;

			}
		}
	}

	void applyVectors(float factor) {

		//   Turns all of the acting vectors to the racer making it move/rotate according to the vectors.

		movementVector.move(model, factor);
		/*cout << "\n Current direction : x= " << facingVector.x << "  y= " << facingVector.y << "  z= " << facingVector.z;
		cout << "\n input x rotation: " << rotationVector.y * (PI / 180);		*/
		facingVector.rotateX(rotationVector.x * (PI / 180));
		facingVector.rotateY(rotationVector.y * (PI / 180));
		facingVector.rotateZ(rotationVector.z * (PI / 180));

		normalVector.rotateX(rotationVector.x * (PI / 180));
		normalVector.rotateY(rotationVector.y * (PI / 180));
		normalVector.rotateZ(rotationVector.z * (PI / 180));


		model->RotateX(rotationVector.x);
		model->RotateY(rotationVector.y);
		model->RotateZ(rotationVector.z);
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

	IMesh* testMesh = myEngine->LoadMesh("TestTrack1.x");
	IModel* TestModel = testMesh->CreateModel(0,0,0);
	IMesh* cubeMesh = myEngine->LoadMesh("Cube.x");
	IMesh* racerMesh = myEngine->LoadMesh("racer.x");
	/*IModel* racerModel = racerMesh->CreateModel();
	racerModel->RotateLocalY(180);*/

	Racer myRacer;
	myRacer.model = racerMesh->CreateModel();
	myRacer.model->RotateLocalY(180);


	Curve trackCurve;
	trackCurve.newSegment({ 0,0,0 }, 0, { 0, 0, 55.2125 }, 0, { -43.7876, 0, 100 }, 0, { -100, 0, 100 }, 0);
	trackCurve.newSegment({ -100, 0, 100}, 0, { -154.213, 0, 100}, 0, { -200, 0, 55.2125 }, 0, { -200, 0, 0}, 0);
	trackCurve.newSegment({ -200,0,0 }, 0, { -200, 0, -55.2125 }, 0, { -154.213, 0, -100 }, 0, { -100, 0, -100 }, 0);
	trackCurve.newSegment({ -100, 0, -100}, 0, { -43.7876, 0, -100}, 0, { 0, 0, -55.2125 }, 0, { 0, 0, 0}, 0);

	IModel* cubes[4000];
	Vector temp;

	for (int i = 0; i <= trackCurve.size * 500; i++)
	{
		cubes[i] = cubeMesh->CreateModel();
		temp = trackCurve.posOnCurve(i / 500.0f);
		temp.move(cubes[i]);
		cubes[i]->Scale(0.1);
	}

	float deltaTime = myEngine->Timer();

	mycamera->RotateLocalX(20);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		// getting deltaTime
		deltaTime = myEngine->Timer();

		if (myEngine->KeyHeld(Key_W)) {
			myRacer.accMovement(deltaTime);
		} 
		else if (myEngine->KeyHeld(Key_S)) {
			myRacer.accMovement(-deltaTime);
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
		myRacer.applyVectors(deltaTime);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
