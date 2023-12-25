// ControlledSphere Project.cpp: A program using the TL-Engine

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

	IMesh* sphereMesh = myEngine->LoadMesh("Sphere.x");
	IModel* sphere = sphereMesh->CreateModel();

	ICamera* myCamera = myEngine->CreateCamera(kFPS);

	int tick = 0;
	float yVelocity = 0.01;
	float xVelocity = 0.01;
	int prevTime = clock();
	int timeDif;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		
		/**** Update your scene each frame here ****/
		
		if (sphere->GetY() > 30 || sphere->GetY() < 0) {
			yVelocity = -yVelocity;
		}

		if (sphere->GetX() > 40 || sphere->GetX() < -40) {
			xVelocity = -xVelocity;
		}

		timeDif = clock() - prevTime;
		prevTime = clock();

		sphere->SetPosition( sphere->GetX() + (xVelocity * timeDif), sphere->GetY() + (yVelocity * timeDif), 0);
		sphere->RotateY(-xVelocity * timeDif * 5);
		sphere->RotateX(yVelocity * timeDif * 5);

		tick++;
		
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
