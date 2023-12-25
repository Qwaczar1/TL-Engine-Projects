// Lab4_KeepingScore_Project.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;
#include <sstream>
using namespace std;

const float kSphereYPos = 10.0f;
const float kCubeYPos = 10.0f;
const float kCubeZPos = 160.0f;
const float kCubeOrigin = -200.0f;
const float kCubeDestination = 200.0f;
const int kCubeArraySize = 10;
const float kCameraYPos = 100.0f;
const float kCameraZPos = -40.0f;
const float kCameraAngle = 40.0f;
const float kSphereMaxZPos = 170.0f;
const float kSphereRadius = 10.0f;
const float kCubeBoundaryOffset = 10.0f;


void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");

	/**** Set up your scene here ****/
	IMesh* floorMesh = myEngine->LoadMesh("Floor.x");
	IModel* floor = floorMesh->CreateModel();

	IMesh* sphereMesh = myEngine->LoadMesh("Sphere.x");
	IModel* sphere = sphereMesh->CreateModel(0, kSphereYPos, 0);
	sphere->SetSkin("Redball.jpg");

	IMesh* cubeMesh = myEngine->LoadMesh("Cube.x");
	IModel* cube[kCubeArraySize];


	ICamera* camera = myEngine->CreateCamera(kManual, 0, kCameraYPos, kCameraZPos);
	camera->RotateLocalX(kCameraAngle);

	enum Spheretates { idle, shooting, returning };
	Spheretates sphereState = idle;

	enum GameStates { playing, gamewon, gameover };
	GameStates gameState = playing;

	float sphereSpeed = 0.3;
	float sphereRotation = sphereSpeed / 4;
	float cubeSpeed = 0.1;
	int score = 0;
	float xPos = kCubeOrigin;
	float xInc = 100;
	//intiating array of models
	for (int i = 0; i < kCubeArraySize; i++) {
		cube[i] = cubeMesh->CreateModel(xPos, kCubeYPos, kCubeZPos);
		cube[i]->Scale(2);
		xPos -= xInc;
	}

	stringstream outText;
	IFont* myFont = myEngine->LoadFont("Comic Sans", 36);
	IFont* largeFont = myEngine->LoadFont("Comic Sans", 72);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		for (int i = 0; i < kCubeArraySize; i++) {
			cube[i]->MoveX(cubeSpeed);
		}
		if (cube[kCubeArraySize - 1]->GetX() >= kCubeDestination) {
			gameState = gameover;
		}

		//checking sphere states
		if (sphereState == idle) {	//waiting to be shot
			if (myEngine->KeyHit(Key_Space)) {
				sphereState = shooting;
			}
		}
		else if (sphereState == shooting) { //move forward after being shot
			sphere->MoveZ(sphereSpeed);
			sphere->RotateLocalX(sphereRotation);
			if (sphere->GetZ() >= kSphereMaxZPos) {
				gameState = gameover;
			}
			for (int i = 0; i < kCubeArraySize; i++) {//check for collision with each cube using sphere to box 
				if ((sphere->GetX() > cube[i]->GetX() - 20 && sphere->GetX() < cube[i]->GetX() + 20) && (sphere->GetZ() > cube[i]->GetZ() - 20 && sphere->GetZ() < cube[i]->GetZ() + 20)) {
					cube[i]->SetSkin("Wood2.jpg");
					score += 1;
					sphereState = returning;
					if (score >= kCubeArraySize) {
						gameState = gamewon;
					}
				}
			}
		}
		else if (sphereState == returning) { //returning to initial position
			sphere->MoveZ(-sphereSpeed);
			sphere->RotateLocalX(-sphereRotation);
			if (sphere->GetZ() <= 0) {
				sphereState = idle;
			}
		}

		//dipslay score here
		outText << "Sphere Position: " << sphere->GetZ();
		myFont->Draw(outText.str(), 20, 20);
		outText.str("");
		
		outText << "Score: " << score;
		myFont->Draw(outText.str(), myEngine->GetWidth() - 200, 20);
		outText.str("");

		//checking game states
		if (gameState == gameover) {
			sphereSpeed = 0;
			cubeSpeed = 0;
			//display game over message here
			largeFont->Draw("You Lost :(", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kMagenta ,kCentre, kVCentre);
		}
		else if (gameState == gamewon) {
			sphereSpeed = 0;
			cubeSpeed = 0;
			//display game won message here
			myEngine->Stop();
			largeFont->Draw("You Won!! :O", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kMagenta, kCentre, kVCentre);
		}

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}