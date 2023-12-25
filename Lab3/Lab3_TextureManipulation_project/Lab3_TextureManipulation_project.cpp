// Lab3_TextureManipulation_project.cpp: A program using the TL-Engine

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
	ICamera* myCamera = myEngine->CreateCamera(kFPS);

	IMesh* sphereMesh = myEngine->LoadMesh("Sphere.x");
	IModel* sphere = sphereMesh->CreateModel();
	sphere->SetSkin("Pluto.jpg");

	string skins[] = { "Clouds.jpg", "EarthHi.jpg", "EarthNight.jpg", "EarthPlain.jpg",
		"Jupiter.jpg", "Mars.jpg", "Saturn.jpg", "Pluto.jpg" };
	int skinsIndex = 0;
	int skinsTotal = 7;

	float xSpeed = 0.05;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		sphere->SetPosition(sphere->GetX() + xSpeed, 0, 0);
		sphere->RotateY(-xSpeed*5);

		if (sphere->GetX() > 40 || sphere->GetX() < -40) {
			sphere->SetSkin(skins[skinsIndex]);
			if (skinsIndex >= skinsTotal) { skinsIndex = 0; }
			else { skinsIndex++; }
			xSpeed = -xSpeed;
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
