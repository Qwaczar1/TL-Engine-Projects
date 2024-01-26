// Bezier Curve Testing.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include "Vectors.h";
#include "Matrices.h";
#include "Bezier.h";
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");

	/**** Set up your scene here ****/
	ICamera* myCamera = myEngine->CreateCamera(kFPS);

	IMesh* cubeMesh = myEngine->LoadMesh("Cube.x");
	IMesh* sphereMesh = myEngine->LoadMesh("sphere.x");

	IModel* sphere = sphereMesh->CreateModel(0,0,0);

	Curve myCurve;

	myCurve.newSegment({ 0,0,0 }, 0, { 100,0,0 }, 0, { 100,0,100 }, 0, { 0,0,100 }, 0);
	myCurve.newSegment({ 0,0,100 }, 0, { -100,0,100 }, 0, { -100,0,200 }, 0, { 0,0,200 }, 0);
	myCurve.newSegment({ 0,0,200 }, 0, { 100,0,200 }, 0, { 100,0,300 }, 0, { 0,0,300 }, 0);
	myCurve.newSegment({ 0,0,300 }, 0, { -100,0,300 }, 0, { -100,0,400 }, 0, { 0,0,400 }, 0);

	IModel* cubes[1000];
	Vector temp;

	for (int i = 0; i <= 40; i++)
	{
		cubes[i] = cubeMesh->CreateModel();
		temp = myCurve.posOnCurve(i / 10.0f);
		temp.move(cubes[i]);
		cubes[i]->Scale(0.1);
	}
	cubes[0]->Scale(10);

	Vector spherePos;


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		
		
		spherePos.vectorSet(sphere->GetX(), sphere->GetY(), sphere->GetZ());
		temp = myCurve.closestPoint(spherePos, 100).posVect;
		cubes[0]->SetPosition(temp.x, temp.y, temp.z);
		

		if (myEngine->KeyHeld(Key_W)) {
			sphere->MoveZ(0.1);
		}
		if (myEngine->KeyHeld(Key_S)) {
			sphere->MoveZ(-0.1);
		}
		if (myEngine->KeyHeld(Key_A)) {
			sphere->MoveX(-0.1);
		}
		if (myEngine->KeyHeld(Key_D)) {
			sphere->MoveX(0.1);
		}

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
