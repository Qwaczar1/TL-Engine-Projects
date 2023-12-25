// Lab1 Positioning.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");

	/**** Set up your scene here ****/
	IMesh* gridMesh = myEngine->LoadMesh("Grid.x");
	IModel* mesh = gridMesh->CreateModel();
	IMesh* cubeMesh = myEngine->LoadMesh("Cube.x");
	IModel* cube = cubeMesh->CreateModel(1, 2, 5);

	ICamera* myCamera = myEngine->CreateCamera(kFPS);
	myCamera->SetPosition(0, 0, 0);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		for (int delay = 0; delay < 100000; delay++) {
			
		}
		cube->RotateY(0.05);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
