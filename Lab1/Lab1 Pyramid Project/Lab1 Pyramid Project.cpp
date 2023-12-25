// Lab1 Pyramid Project.cpp: A program using the TL-Engine

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

	IMesh* cubeMesh = myEngine->LoadMesh("Cube.x");

	IModel* cube0 = cubeMesh->CreateModel(0, 5, 0);
	IModel* cube1 = cubeMesh->CreateModel(10, 5, 10);
	IModel* cube2 = cubeMesh->CreateModel(10, 5, 0);
	IModel* cube3 = cubeMesh->CreateModel(10, 5, -10);
	IModel* cube4 = cubeMesh->CreateModel(0, 5, 10);
	IModel* cube5 = cubeMesh->CreateModel(-10, 5, -10);
	IModel* cube6 = cubeMesh->CreateModel(-10, 5, 10);
	IModel* cube7 = cubeMesh->CreateModel(-10, 5, 0);
	IModel* cube8 = cubeMesh->CreateModel(0, 5, -10);

	IModel* cube9 = cubeMesh->CreateModel(5, 15, 5);
	IModel* cube10 = cubeMesh->CreateModel(5, 15, -5);
	IModel* cube11 = cubeMesh->CreateModel(-5, 15, 5);
	IModel* cube12 = cubeMesh->CreateModel(-5, 15, -5);

	IModel* cube13 = cubeMesh->CreateModel(0, 25, 0);

	IMesh* sphereMesh = myEngine->LoadMesh("Sphere.x");
	IModel* sphere = sphereMesh->CreateModel(0, 40, 0);

	ICamera* myCamera = myEngine->CreateCamera(kFPS);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		sphere->RotateX(0.05);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
