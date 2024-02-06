// MatrixTesting.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include "Matrices.h";
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
	IMesh* mesh = myEngine->LoadMesh("Box.x");

	IModel* box = mesh->CreateModel();

	Matrix2D boxMatrix;/*
	boxMatrix.initalize();*/
	boxMatrix.print();
	cout << "-------------------------------------------\n";
	boxMatrix.martixFromModel(box);
	boxMatrix.print();
	cout << "-------------------------------------------\n";
	box->RotateY(45);
	boxMatrix.martixFromModel(box);
	boxMatrix.print();

	//boxMatrix = boxMatrix * 5;
	//boxMatrix.print();
	//boxMatrix = boxMatrix * boxMatrix;
	//boxMatrix.print();

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
