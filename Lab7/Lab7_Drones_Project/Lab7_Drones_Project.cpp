// Lab7_Drones_Project.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <iostream>
using namespace tle;
using namespace std;

const float kGameSpeed = 0.05f;

enum eDroneMotionState {
	Moving, Paused
};

typedef struct Vector3 {
	float x;
	float y;
	float z;
	float length = sqrt((x * x) + (y * y) + (z * z));
};

void SetVector(Vector3 vect, float x, float y, float z) {
	vect.x = x;
	vect.y = y;
	vect.z = z;
}

Vector3 Vector3Add(Vector3 vect1, Vector3 vect2) {
	Vector3 vectorSum;
	vectorSum.x = vect1.x + vect2.x;
	vectorSum.y = vect1.y + vect2.y;
	vectorSum.z = vect1.z + vect2.z;
	return vectorSum;
}

Vector3 Vecotr3SetLength(Vector3 vect, float length) {
	Vector3 newVector;
	float lengthRatio = length / vect.length;
	newVector.x = vect.x * lengthRatio;
	newVector.y = vect.y * lengthRatio;
	newVector.z = vect.z * lengthRatio;
	return newVector;
}

void VectorMove(IModel* model, Vector3 vect) {
	model->Move(vect.x, vect.y, vect.z);
}

Vector3 PositionCompare(IModel* model1, IModel* model2) {
	Vector3 relativeV;
	relativeV.x = model1->GetX() - model2->GetX();
	relativeV.y = model1->GetY() - model2->GetY();
	relativeV.z = model1->GetZ() - model2->GetZ();
	relativeV.length = sqrt((relativeV.x * relativeV.x) + (relativeV.y * relativeV.y) + (relativeV.z * relativeV.z));
	relativeV.x = relativeV.x / relativeV.length;
	relativeV.y = relativeV.y / relativeV.length;
	relativeV.z = relativeV.z / relativeV.length;
	return relativeV;
}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media\\Lab7_Assets" );

	/**** Set up your scene here ****/
	ICamera* myCamera = myEngine->CreateCamera(kManual);
	myCamera->SetPosition(0, 600, -250);
	myCamera->RotateLocalX(45);

	IMesh* bulletMesh = myEngine->LoadMesh("bullet.x");
	IMesh* droneMesh = myEngine->LoadMesh("drone.x");
	IMesh* groundMesh = myEngine->LoadMesh("ground.x");

	IModel* ground = groundMesh->CreateModel();
	ground->Scale(2);

	IModel* playerDrone = droneMesh->CreateModel(0, 60, 0);
	IModel* enemyDrone = droneMesh->CreateModel(0, 60, 400);
	enemyDrone->SetSkin("Red_Drone.jpg");
	const float kDroneSpeed = kGameSpeed;
	Vector3 vectorPlayer;
	eDroneMotionState playerState;

	Vector3 vectDist;

	IModel* bullet;
	bool bulletShot = false;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		
		SetVector(vectorPlayer, 0, 0, 0);
		vectorPlayer.x = 0;
		vectorPlayer.y = 0;
		vectorPlayer.z = 0;
		playerState = Paused;
		if (myEngine->KeyHeld(Key_W)) {
			vectorPlayer.z = 1;
			playerState = Moving;
		}
		if (myEngine->KeyHeld(Key_A)) {
			playerDrone->RotateY(-kDroneSpeed * 5);
		}
		if (myEngine->KeyHeld(Key_D)) {
			playerDrone->RotateY(kDroneSpeed * 5);
		}
		switch (playerState){
		case Moving: playerDrone->MoveLocal(vectorPlayer.x, vectorPlayer.y, vectorPlayer.z);
			break;
		case Paused:
			break;
		default:
			break;
		}
		enemyDrone->LookAt(playerDrone);

		vectDist = PositionCompare(playerDrone, enemyDrone);
		cout << "Distance is = " << vectDist.length<< '\n';

		if (vectDist.length < 250) { bulletShot = true; }

		if (bulletShot) {

		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
