// VectorsTesting.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include "Vectors.h"
#include <iostream>
#include <sstream>
using namespace std;
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/
	ICamera* mycamera = myEngine->CreateCamera(kFPS);

	IMesh* cubeMesh = myEngine->LoadMesh("Cube.x");
	IModel* cube = cubeMesh->CreateModel();

	Vector movementVect;
	movementVect.vectorSet(0.01, 0, 0);
	Vector axisVect;
	axisVect.vectorSet(0, 0.5, 0.5);


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		
		movementVect.rotateOn(axisVect, 0.001);
		movementVect.move(cube);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
