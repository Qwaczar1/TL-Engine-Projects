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

	Curve myCurve;
	myCurve.newPoint(0, 0, 0);
	myCurve.newPoint(0, 0, 100);
	myCurve.newPoint(100, 0, 100);
	myCurve.newPoint(100, 0, 0);

	myCurve.newPoint(100, 0, -100);
	myCurve.newPoint(200, 0, -100);
	myCurve.newPoint(200, 0, 0);

	IModel* cubes[100];
	Vector temp;

	for (int i = 0; i <= 100; i++)
	{
		cubes[i] = cubeMesh->CreateModel();
		temp = myCurve.posOnCureve(i / 50.0f);
		temp.move(cubes[i]);
		cout << "\n position: x=" << temp.x << "  y=" << temp.y << "  z=" << temp.z;
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
