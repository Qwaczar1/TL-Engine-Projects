// Bezier Curve Testing.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include "Vectors.h";
#include "Matrices.h";
#include "Bezier.h";
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/
	ICamera* myCamera = myEngine->CreateCamera(kFPS);

	IMesh* cubeMesh = myEngine->LoadMesh("Cube.x");

	Curve myCurve;
	myCurve.newPoint(0, 0, 0);
	myCurve.newPoint(0, 0, 10);
	myCurve.newPoint(10, 0, 10);
	myCurve.newPoint(10, 0, 0);

	IModel* cubes[100];

	for (int i = 0; i < 10; i++)
	{
		cubes[i] = cubeMesh->CreateModel();
		myCurve.posOnCureve(i / 10.0f).move(cubes[i]);
	}


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}