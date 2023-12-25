// Lab3_FieldOfSpikes_Project.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <ctime>
#include <iostream>
using namespace tle;
using namespace std;

float random(int min, int max) {
	srand(time(0));
	return ((rand() % (max - min + 1)) + min);
}

float max(float a, float b) {
	return ((a > b) ? a : b);
}

float vector2Distance(float X1, float Z1, float X2, float Z2) {

}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media\\Lab3_Assets" );

	/**** Set up your scene here ****/

	ICamera* myCamera = myEngine->CreateCamera(kManual);
	myCamera->SetPosition(0, 40, -50);
	myCamera->RotateX(20);

	IMesh* floorMesh = myEngine->LoadMesh("floor.x");
	IModel* floor = floorMesh->CreateModel();

	IMesh* skyboxMesh = myEngine->LoadMesh("skybox.x");
	IModel* skybox = skyboxMesh->CreateModel(0, -960, 0);

	IMesh* planeMesh = myEngine->LoadMesh("plane.x");
	IModel* plane = planeMesh->CreateModel(0, 10, 0);

	IMesh* ballMesh = myEngine->LoadMesh("ball.x");
	IModel* ball;

	IMesh* spikeMesh = myEngine->LoadMesh("spike.x");
	const int spikeAmount = 10;
	IModel* spike[spikeAmount];
	int posX = 0;
	int posZ = 0;
	int spikesMinZ = -1000;
	int spikesMaxZ = 1000;
	int spikesMinX = -1000;
	int spikesMaxX = 1000;

	for (size_t i = 0; i < spikeAmount; i++)
	{
		posX = ((rand() % (spikesMaxX - (spikesMinX) + 1)) + (spikesMinX));
		posZ = ((rand() % (spikesMaxZ - (spikesMinZ) + 1)) + (spikesMinZ));
		spike[i] = spikeMesh->CreateModel(posX, 0, posZ);
	}

	bool ballfired = false;
	int ballDistance = 0;
	float mouseXPos = 0;
	float totalRotation = 0;
	bool firedDirectionSet = true;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		plane->MoveLocalZ(0.05);
		int mouseMoveX = myEngine->GetMouseMovementX();
		mouseXPos += mouseMoveX * 0.0002;
		plane->RotateY(mouseXPos);
		totalRotation += (mouseXPos);

		myCamera->SetPosition(plane->GetX(), plane->GetY() + 10, plane->GetZ());
		myCamera->RotateY(mouseXPos);
		myCamera->MoveLocalZ(-50);
		

		if (myEngine->KeyHit(Key_Space)) {
			if (ballfired) {
				ballMesh->RemoveModel(ball);
				ballfired = false;
				ballDistance = 0;
			}
			ball = ballMesh->CreateModel(plane->GetLocalX(), plane->GetLocalY(), plane -> GetLocalZ());
			ball->Scale(0.05);
			ballfired = true;
			ball->RotateY(totalRotation);
		}

		if (ballfired) {
			ball->MoveLocalZ(3);
			ballDistance += 1;
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
