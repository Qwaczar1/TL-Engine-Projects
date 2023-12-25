// Lab1 Giant Project.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/

	IMesh* cubemesh = myEngine->LoadMesh("Cube.x");

	IModel* cube0 = cubemesh->CreateModel(10, 5, 0);
	IModel* cube1 = cubemesh->CreateModel(-10, 5, 0);
	IModel* cube2 = cubemesh->CreateModel(10, 15, 0);
	IModel* cube3 = cubemesh->CreateModel(-10, 15, 0);
	IModel* cube4 = cubemesh->CreateModel(0, 25, 0);
	IModel* cube5 = cubemesh->CreateModel(0, 35, 0);
	IModel* cube6 = cubemesh->CreateModel(0, 45, 0);
	IModel* cube7 = cubemesh->CreateModel(5, 55, 0);
	IModel* cube8 = cubemesh->CreateModel(-5, 55, 0);
	IModel* cube9 = cubemesh->CreateModel(5, 65, 0);
	IModel* cube10 = cubemesh->CreateModel(-5, 65, 0);
	IModel* cube11 = cubemesh->CreateModel(10, 35, 0);
	IModel* cube12 = cubemesh->CreateModel(-10, 35, 0);
	IModel* cube13 = cubemesh->CreateModel(20, 35, 0);
	IModel* cube14 = cubemesh->CreateModel(-20, 35, 0);

	ICamera* myCamera = myEngine->CreateCamera(kFPS);


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
