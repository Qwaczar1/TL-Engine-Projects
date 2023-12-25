// Lab3_CameraControl_Project.cpp: A program using the TL-Engine

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

	IMesh* cubeMesh = myEngine->LoadMesh("Sphere.x");
	IModel* cube = cubeMesh->CreateModel();

	ICamera* myCamera = myEngine->CreateCamera();
	const float kCameraSpeed = 0.01f;
	const float kCameraRotation = 0.1f;
	bool mouseCaptureToggle = false;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		/**** Update your scene each frame here ****/

		if (myEngine->KeyHeld(Key_W)) { myCamera->SetLocalZ(myCamera->GetLocalZ() + kCameraSpeed); }
		if (myEngine->KeyHeld(Key_S)) { myCamera->SetLocalZ(myCamera->GetLocalZ() - kCameraSpeed); }
		if (myEngine->KeyHeld(Key_A)) { myCamera->SetLocalX(myCamera->GetLocalX() - kCameraSpeed); }
		if (myEngine->KeyHeld(Key_D)) { myCamera->SetLocalX(myCamera->GetLocalX() + kCameraSpeed); }
		if (myEngine->KeyHeld(Key_Q)) { myCamera->SetLocalY(myCamera->GetLocalY() - kCameraSpeed); }
		if (myEngine->KeyHeld(Key_E)) { myCamera->SetLocalY(myCamera->GetLocalY() + kCameraSpeed); }

		if (myEngine->KeyHit(Key_Tab)) { 
			mouseCaptureToggle = !mouseCaptureToggle; 
			if (mouseCaptureToggle) { myEngine->StartMouseCapture(); }
			else { myEngine->StopMouseCapture(); }
		}
		myCamera->RotateY(myEngine->GetMouseMovementX() * kCameraRotation);
		myCamera->RotateX(myEngine->GetMouseMovementY() * kCameraRotation);

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
