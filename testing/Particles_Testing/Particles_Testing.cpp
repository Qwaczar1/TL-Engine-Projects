// Particles_Testing.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include "Vectors.h"
#include "Particles.h"
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
	IMesh* particleMesh = myEngine->LoadMesh("Box.x");

	Vector systemVect = { 1, 0, 0 };
	Vector systemPositionVect = { 0, 0, 0 };
	particleSyst particlesTest = setupParticleSystem(particleMesh, 20, 0, 0, 10, 0, 0, systemVect, systemPositionVect, simpleRectangle, 20, 20);

	srand(time(NULL));

	stringstream outText;
	IFont* myFont = myEngine->LoadFont("Comic Sans", 36);


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		particlesTest.updateSystem();


		outText << "Particles: " << particlesTest.size;
		myFont->Draw(outText.str(), myEngine->GetWidth() - 200, 20);
		outText.str("");
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}