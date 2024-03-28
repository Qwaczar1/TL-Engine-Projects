// Hover_Racer_Assignment.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

#include "Bezier.h"
#include "Vectors.h"
#include "Matrices.h"
#include "SnazzyCam.h"
#include "Particles.h"

#define PI 3.14159265359f

#define GameSpeed 20f
#define trackScale 90
#define ENEMIEAMOUNT 5

enum GameStates
{
	Demo, CountDown, Stage0, Stage1, RaceComplete
};

enum ControllState
{
	Idle, Manual, AI1
};

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
	//IModel* cubes[6];
	Matrix2D matrix;
	Vector movementVector = Vector( 0,0,0 );
	particleSyst exhaust;
	float rotationVector = 0;
	float baseSpeed = 1.0f;

	ControllState control = Idle;

	void accMovement(float factor) {
		if (factor > 0) { factor *= 0.5f; }
		movementVector = movementVector + matrix.vectorizeZ() * baseSpeed * factor;
		movementVector.getLength();
	}

	void accRotation(float factor) {
	    //rotationVector.rotateOn(matrix.vectorizeY(), factor * ((1 / ((rotationVector.length * (movementVector.length + 1)) / 10 + 0.1))));
		rotationVector = rotationVector + 0.1 * baseSpeed * -factor;
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
			movementVector = Vector( 0,0,0);
		}
		// rotation friction
		force = (factor * rotationVector * rotationVector) + (factor * 0.001);
		if (rotationVector > force || rotationVector < - force) {
			rotationVector = rotationVector * 0.98f;
		}
		else {
			rotationVector = 0;
		}
	}

	void trackColision(Curve track, float factor) {

		//  Finds out if te racer is in colision with the Bezier curve Track, and applies an outwards force to get the racer out of the track.

		float targetDistance = 10;
		CurvePoint closestPoint = track.closestPoint(matrix.vectorizePos(), 100);

		// Getting a track normal vector for the closest track point.

		Vector trackNormal = Vector(0,0,0);
		//cout << "closest t = " << closestPoint.t;
		if (closestPoint.t > 3 && closestPoint.t < 5)
		{
			trackNormal = closestPoint.posVect.subtractVector({ (0.898 - 1.4994) * trackScale, (-0.015654 + 0.544593) * trackScale, (-1.705 - 0.492173) * trackScale }).normalize() * -1;
			trackNormal.rotateOn(closestPoint.facingVect, closestPoint.rotation);
		}
		else {
			trackNormal = { -closestPoint.facingVect.z, 0, closestPoint.facingVect.x };
			trackNormal = trackNormal.normalize();
			trackNormal.rotateOn(closestPoint.facingVect, closestPoint.rotation);
			trackNormal = trackNormal.normalize().crossProduct(closestPoint.facingVect.normalize());
		}

		//viewVector(cubes[0], cubes[1], cubes[2], trackNormal, closestPoint.posVect);
		//viewVector(cubes[3], cubes[4], cubes[5], closestPoint.facingVect, closestPoint.posVect);

		//cout << "\n dotProduct: " << matrix.vectorizeY().normalize().dotProduct(trackNormal.normalize());
		if (matrix.vectorizeY().normalize().dotProduct(trackNormal.normalize()) < 0.999999) {
			matrix = rotateTo(matrix, Y, trackNormal.normalize());

			//cout << "\nmatrix: \n";
			//matrix.print();
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
		if (racerPos.length < 8) {
			if (dotProduct < 0.0) {
				float pushPower = abs(dotProduct) * racerPos.length;
				//cout << "\n Push Power = " << pushPower;
				trackNormal.setLength(pushPower);
				//cout << "\n Current Normal : x= " << trackNormal.x << "  y= " << trackNormal.y << "  z= " << trackNormal.z;
				gravity(-factor);
				movementVector = movementVector + trackNormal;

				
			}
		}
		else {
			racerPos.setLength(racerPos.length - 8);
			movementVector = movementVector - racerPos;
			
		}
	}

	void applyVectors(float factor) {

		//   Turns all of the acting vectors to the racer making it move/rotate according to the vectors.
		matrix = matrix + (movementVector * factor * 20);
		matrix = rotateOn(matrix, matrix.vectorizeY(), rotationVector);
		
		matrix.matrixToModel(model);
	}

	void phys(float deltaTime, Curve trackCurve) {
		gravity(deltaTime);
		friction(deltaTime);
		trackColision(trackCurve, deltaTime);
		applyVectors(deltaTime);
	}

	void AIbrain(float deltaTime, Curve trackCurve) {
		CurvePoint target = trackCurve.closestPoint(matrix.vectorizePos(), 100);
		target = trackCurve.posOnCurve(target.t + 0.1);
		if (matrix.vectorizeZ().dotProduct(target.posVect - matrix.vectorizePos()) > 0) {
			accMovement(deltaTime);
		}
		else {
			accMovement(-deltaTime);
		}

		if (matrix.vectorizeX().dotProduct(target.posVect - matrix.vectorizePos()) > 0.1) {
			accRotation(-deltaTime);
		}
		else if (matrix.vectorizeX().dotProduct(target.posVect - matrix.vectorizePos()) < -0.1) {
			accRotation(deltaTime);
		}
	}

	void manualControl(float deltaTime, I3DEngine* myEngine) {
		if (myEngine->KeyHeld(Key_W)) {
			accMovement(-deltaTime);
		}
		else if (myEngine->KeyHeld(Key_S)) {
			accMovement(deltaTime);
		}
		if (myEngine->KeyHeld(Key_A)) {
			accRotation(-deltaTime);
		}
		if (myEngine->KeyHeld(Key_D)) {
			accRotation(deltaTime);
		}
	}

	void operate(float deltaTime, I3DEngine* myEngine, Curve trackCurve) {
		exhaust.posVect = matrix.vectorizePos() + (matrix.vectorizeY() * 0.48) + (matrix.vectorizeZ() * 1.5);
		exhaust.vect = matrix.vectorizeZ();
		exhaust.updateSystem();
		switch (control)
		{
		case Idle:
			phys(deltaTime, trackCurve);
			break;
		case Manual:
			manualControl(deltaTime, myEngine);
			phys(deltaTime, trackCurve);
			break;
		case AI1:
			AIbrain(deltaTime, trackCurve);
			phys(deltaTime, trackCurve);
			break;
		default:
			break;
		}
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
	myEngine->AddMediaFolder("./Media/Track");

	/**** Set up your scene here ****/

	IMesh* TrackMesh = myEngine->LoadMesh("HoverRacerTrackV2Track.x");
	IMesh* TubeMesh = myEngine->LoadMesh("HoverRacerTrackV2Tube.x");
	IMesh* IslandMesh = myEngine->LoadMesh("HoverRacerTrackV2Island.x");
	IMesh* SupportsMesh = myEngine->LoadMesh("HoverRacerTrackV2Supports.x");
	IMesh* TopWaterMesh = myEngine->LoadMesh("HoverRacerTrackV2TopWater.x");
	IMesh* BottomWaterMesh = myEngine->LoadMesh("HoverRacerTrackV2BottomWater.x");
	
	IModel* Track = TrackMesh->CreateModel(0,0,0);
	Track->RotateY(-90);
	IModel* Tube = TubeMesh->CreateModel(0,0,0);
	Tube->RotateY(-90);
	IModel* Island = IslandMesh->CreateModel(0,0,0);
	Island->RotateY(-90);
	IModel* Suports = SupportsMesh->CreateModel(0,0,0);
	Suports->RotateY(-90);
	IModel* TopWater = TopWaterMesh->CreateModel(0,0,0);
	TopWater->RotateY(-90);
	IModel* BottomWater = BottomWaterMesh->CreateModel(0,0,0);
	BottomWater->RotateY(-90);


	IMesh* cubeMesh = myEngine->LoadMesh("Cube.x");
	IMesh* racerMesh = myEngine->LoadMesh("racer.x");
	/*IModel* racerModel = racerMesh->CreateModel();
	racerModel->RotateLocalY(180);*/

	/*for (int i = 0; i < 6; i++)
	{
		myRacer.cubes[i] = cubeMesh->CreateModel();
		myRacer.cubes[i]->Scale(0.02);
	}
	*/
	
#pragma region settingUpTrackCurve

	Curve trackCurve;
	trackCurve.newSegment({ 0.618465,2.1018,0.010918 }, 0, {0.290103, 1.62184, 0.010918}, 0, {-0.152559, 1.69943, 0.016881}, 0, {-0.843004, 1.70675, 0.016881}, 0);
	trackCurve.newSegment({ -0.843004, 1.70675, 0.016881 }, 0, { -1.47163, 1.71341, 0.016881 }, 0, { -2.14434, 1.47552, 0.019571 }, 0, { -2.14196, 0.743453, 0.019571 }, -0.1);
	trackCurve.newSegment({ -2.14196, 0.743453, 0.019571 }, -0.1, { -2.14002, 0.148229, 0.019571 }, -0.1, { -2.28075, -0.039035, 0.007296 }, -0.1, { -1.43852, -0.50615, 0.007296 }, 0);

	//LOOP
	trackCurve.newSegment({ -1.43852, -0.50615, 0.007296 }, 0, { -0.5963, -0.973264, 0.007296 }, 0, { -0.836632, -0.986419, 1.1 }, 0, { -1.53644, -0.598294, 1.1 }, 0.1);
	trackCurve.newSegment({ -1.53644, -0.598294, 1.1 }, 0.1, { -2.14049, -0.263278, 1.1 }, 0.2, { -2.3426, -0.191956, 0.015372 }, 0.3, { -1.65757, -0.649594, 0.015372 }, 0.4);

	trackCurve.newSegment({ -1.65757,-0.649594,0.015372 }, 0, { -0.527314,-1.40467,0.015372 }, 0, { 0.291053,-1.34521,-0.038012 }, 0.1, { 0,-2.12294,0.219752 }, 0.3);
	trackCurve.newSegment({ 0,-2.12294,0.219752 }, 0.3, { -0.291432,-2.90168,0.477852 }, 0.5, { -1.85921,-2.91416,0.416732 }, 0.4, { -2.21183,-2.12294,0.504054 }, 0.4);
	trackCurve.newSegment({ -2.21183,-2.12294,0.504054 }, 0.4, { -2.38925,-1.72483,0.547991 }, 0.45, { -1.31862,-0.182002,0.440243 }, 0.45, { -0.928724,0.037519,0.376895 }, 0.4);
	trackCurve.newSegment({ -0.928724,0.037519,0.376895 }, 0.4, { -0.266648,0.410284,0.269324 }, 0, { 0.464526,-0.528374,0.234164 }, -0.4, { 1,0,0.131925 }, -0.5);
	trackCurve.newSegment({ 1,0,0.131925 }, -0.5, { 1.5267,0.519715,0.031361 }, -0.3, { 0.887217,1.17088,0.002681 }, -0.1, { 1,1.90886,0.010659 }, 0);
	trackCurve.newSegment({ 1,1.90886,0.010659 }, 0, { 1.07112,2.37421,0.015689 }, 0, { 1.62465,2.91317,0 }, 0, { 1.36005,3.05959,0 }, 0);
	trackCurve.newSegment({ 1.36005,3.05959,0 }, 0, { 1.10641,3.19994,0 }, 0, { 0.946825,2.58176,0.010918 }, 0, { 0.618465,2.1018,0.010918 }, 0);

	trackCurve.move({ 0.898, -0.015654, -1.705 });

	trackCurve.scale(trackScale);

	/*IModel* cubes[1000];
	Vector temp2;

	for (int i = 0, o = 0; i <= trackCurve.size * 50; i++)
	{
		cubes[i] = cubeMesh->CreateModel();
		o = i / 200.0f;
		temp2 = trackCurve.posOnCurve(i / 50.0f).posVect;
		cout << "\n";
		temp2.print();
		temp2.move(cubes[i]);
		cubes[i]->Scale(0.05);
	}

	cubes[0]->SetLocalPosition((0.898 - 1.4994) * trackScale, (- 0.015654 + 0.544593) * trackScale, (-1.705 - 0.492173) * trackScale);
	cubes[0]->Scale(20);*/

#pragma endregion settingUpTrackCurve

	float deltaTime = myEngine->Timer();

	Vector temp;

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

	Vector viableStartPos[10] = {
		{0,2,0},
		{6,2,6},
		{6,2,-6},
		{12,2,0},
		{18,2,6},
		{18,2,-6},
		{24,2,0},
		{30,2,6},
		{30,2,-6},
		{36,2,0},
	};

	IMesh* ExhaustMesh = myEngine->LoadMesh("ExhaustParticle.x");

	Racer racers[ENEMIEAMOUNT + 1];
	for (int i = 0; i < ENEMIEAMOUNT + 1; i++)
	{
		racers[i].model = racerMesh->CreateModel(viableStartPos[i].x, viableStartPos[i].y, viableStartPos[i].z);
		racers[i].matrix = rotateY(racers[i].matrix, PI / 2);
		racers[i].matrix = racers[i].matrix + viableStartPos[i];
		racers[i].exhaust = setupParticleSystem(ExhaustMesh, 50, 1, 40, 0, 0.05, 0.06, racers[i].matrix.vectorizeZ(), racers[i].matrix.vectorizePos(), cone, 0, 0);
		racers[i].control = Idle;
	}
	racers[0].control = Manual;

	SnazzyCam myCam;
	myCam.myEngine = myEngine;
	myCam.Camera = myEngine->CreateCamera(kManual);
	myCam.parent = racers[0].model;
	myCam.setState(FreeMoving);

	IFont* infoFont = myEngine->LoadFont("Comic Sans", 15);
	stringstream info;

	float totalTime = 0;
	
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		// getting deltaTime
		
		myCam.cameraLogic(deltaTime);

		if (myEngine->KeyHit(Key_Space)) {
			for (int i = 1; i < ENEMIEAMOUNT + 1; i++) {
				racers[i].control = AI1;
			}

		}

		//racers[0].operate(deltaTime, myEngine, trackCurve);
		for (int i = 0; i < ENEMIEAMOUNT+1; i++)
		{
			racers[i].operate(deltaTime, myEngine, trackCurve);
			for (int j = 0; j < ENEMIEAMOUNT+1; j++)
			{
				if (i != j) {
					temp = (racers[i].matrix.vectorizePos() - racers[j].matrix.vectorizePos());
					temp.getLength();
					if (temp.length < 3) {
						temp.setLength((3 - temp.length) / 2);
						racers[i].movementVector = racers[i].movementVector + temp;
					}
				}
			}
		}

		// Data Hud
		info << "Version 0.5\n";
		info << "FPS: " << 1 / deltaTime << "\n";
		totalTime += deltaTime;
		info << "Timer: " << int(totalTime / 60) << ":" << int(totalTime) % 60 << "\n";
		infoFont->Draw(info.str(), 20, 20);
		info.str("");
		

		deltaTime = myEngine->Timer();
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
