// Lab2_SquarePath_Project.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

double square(double x) { return(x * x); }
void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/

	IMesh* floormesh = myEngine->LoadMesh("floor.x");
	IModel* floor = floormesh->CreateModel(0, 0, 0);

	IMesh* sphereMesh = myEngine->LoadMesh("Sphere.x");
	IModel* sphere = sphereMesh->CreateModel(0, 10, 0);

	IMesh* cubeMesh = myEngine->LoadMesh("Cube.x");

	ICamera* myCamera = myEngine->CreateCamera(kFPS);
	myCamera->SetPosition(0, 500, 0);
	myCamera->RotateLocalX(90);

	float speedMultiplyer = 1;
	float basespeed = 0.01;
	float zSpeed = 0;
	float xSpeed = 0;

	float xPosition[] = { 10, 50, -90, 90, -50 };
	float zPosition[] = { 100, -100, 30, 31, -100 };

	int positionIndex = 0;
	int positionsLen = sizeof(xPosition) / sizeof(0) + 2;

	float offset[2];

	IModel* checkpoint1 = cubeMesh->CreateModel(xPosition[0], 0, zPosition[0]);
	IModel* checkpoint2 = cubeMesh->CreateModel(xPosition[1], 0, zPosition[1]);
	IModel* checkpoint3 = cubeMesh->CreateModel(xPosition[2], 0, zPosition[2]);
	IModel* checkpoint4 = cubeMesh->CreateModel(xPosition[3], 0, zPosition[3]);
	IModel* checkpoint5 = cubeMesh->CreateModel(xPosition[4], 0, zPosition[4]);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		if (myEngine->KeyHit(Key_W) && sphere->GetLocalY() < 30) {
			sphere->SetLocalY(sphere->GetLocalY() + 5);
		}
		if (myEngine->KeyHit(Key_S) && sphere->GetLocalY() > 10) {
			sphere->SetLocalY(sphere->GetLocalY() - 5);
		}
		if (myEngine->KeyHit(Key_D)) {
			speedMultiplyer = speedMultiplyer * 1.1;
		}
		if (myEngine->KeyHit(Key_A)) {
			speedMultiplyer = speedMultiplyer * 0.9;
		}

		offset[1] = abs(xPosition[positionIndex] - sphere->GetX());
		offset[0] = abs(zPosition[positionIndex] - sphere->GetZ());

		if (offset[0] < basespeed) { xSpeed = 0; }
		else {
			xSpeed = basespeed / (square(offset[0]) * (1 / (square(offset[0]) + square(offset[1]))));
		}
		if (offset[1] < basespeed) { zSpeed = 0; }
		else {
			zSpeed = basespeed / (square(offset[1]) * (1 / (square(offset[0]) + square(offset[1]))));
		}
		
		if (sphere->GetX() > xPosition[positionIndex]) { xSpeed = -xSpeed; }
		if (sphere->GetZ() > zPosition[positionIndex]) { zSpeed = -zSpeed; }
		
		sphere->SetX(sphere->GetX() + xSpeed);
		sphere->SetZ(sphere->GetZ() + zSpeed);

		if (offset[0] < basespeed * 5 && offset[1] < basespeed * 5) {
			if (positionIndex == positionsLen) { positionIndex = 0; }
			else { ++positionIndex; }
		}

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
