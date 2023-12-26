// SpheresIo_Assinment.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <iostream>
#include <sstream>
#include "Vectors.h"
using namespace tle;
using namespace std;

Vector PositionCompare(IModel* model1, IModel* model2) {
	Vector relativeV;
	relativeV.x = model1->GetX() - model2->GetX();
	relativeV.y = model1->GetY() - model2->GetY();
	relativeV.z = model1->GetZ() - model2->GetZ();
	return relativeV;
}

float Distance(Vector vec1, Vector vec2) {
	return sqrt((vec1.x - vec2.x) * (vec1.x - vec2.x) + (vec1.y - vec2.y) * (vec1.y - vec2.y) + (vec1.z - vec2.z) * (vec1.z - vec2.z));
}

enum MovementStates {
	Paused, MovingFront, MovingBack
};

enum States{
	Pause, Playing, GameOver, GameWon
};

enum FoodType {
	BasicFood, HyperFood
};

typedef struct Food {
	IModel* model;
	Vector vect3;
	float scale;
	FoodType type;


};

typedef struct Player {
	IModel* model;
	Vector vect3;
	MovementStates state;
	float radius = 5.0f;
};

typedef struct CamTransform {
	float x = 0;
	float y = 0;
	float z = 0;
	float xRot = 0;
	float yRot = 0;
	float zRot = 0;
};

void CamSetTransform(ICamera* cam, CamTransform newTransform, CamTransform oldTransform) {
	cam->SetPosition(newTransform.x, newTransform.y, newTransform.z);

	cam->RotateZ(-oldTransform.zRot);
	cam->RotateY(-oldTransform.yRot);
	cam->RotateX(-oldTransform.xRot);

	cam->RotateX(newTransform.xRot);
	cam->RotateY(newTransform.yRot);
	cam->RotateZ(newTransform.zRot);
	
}

float clamp(float x, float min, float max) {
	if (x > max) { return max; }
	if (x < min) { return min; }
	return x;
}

float random(int min, int max) {
	return ((rand() % (max - min + 1)) + min);
}

bool SphereRectCollision(IModel* sphere, float radius, IModel* rect, float xSize, float ySize = -1, float zSize = -1) {
	xSize = xSize / 2;

	if (ySize < 0) { ySize = xSize; } else {	ySize = ySize / 2;}
	if (zSize < 0) { zSize = xSize; } else {	zSize = zSize / 2;}

	float conrnerDist = sqrt((ySize * ySize) + (xSize * xSize) + (zSize * zSize));
	
	if (conrnerDist + radius > PositionCompare(sphere, rect).length) {
		Vector closest;
		closest.vectorSet(
			clamp(sphere->GetX(), rect->GetX() - xSize, rect->GetX() + xSize),
			clamp(sphere->GetY(), rect->GetY() - ySize, rect->GetY() + ySize),
			clamp(sphere->GetZ(), rect->GetZ() - zSize, rect->GetZ() + zSize));
		Vector spherePos;
		spherePos.vectorSet(sphere->GetX(), sphere->GetY(), sphere->GetZ());
		if (Distance(closest,spherePos) < radius) {
			return true;
		}
	}
	return false;
}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	//myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media\\Assinment1_Assets" );
	myEngine->AddMediaFolder("./media");

	/**** Set up your scene here ****/

	// Declaring Constants
	float pi = 3.14159265359f;


	// Loading Meshs
	IMesh* islandMesh = myEngine->LoadMesh("island.x");
	IMesh* miniCubeMesh = myEngine->LoadMesh("minicube.x");
	IMesh* skyMesh = myEngine->LoadMesh("sky.x");
	IMesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");
	IMesh* waterMesh = myEngine->LoadMesh("water.x");

	// Setting Up Scene Enviroment
	IModel* water = waterMesh->CreateModel(0, -5, 0);
	IModel* island = islandMesh->CreateModel(0, -5, 0);
	IModel* skybox = skyMesh->CreateModel(0, -960, 0);

	// Creating debug camera
	ICamera* myCamera = myEngine->CreateCamera(kManual);

	CamTransform camview0, camView1, camView2;
	camView1.y = 200;
	camView1.xRot = 90;

	camView2.x = 150;
	camView2.y = 150;
	camView2.z = -150;
	camView2.xRot = 45;
	camView2.yRot = -45;

	CamSetTransform(myCamera, camView1, camview0);
	camview0 = camView1;

	// food spawn
	const int MAX_FOOD = 12;
	IModel* food[MAX_FOOD];
	for (int i = 0; i < MAX_FOOD; i++){
		food[i] = miniCubeMesh->CreateModel(random(-100, 100), 2, random(-100, 100));
	}

	// Player Setup
	Player player;
	player.model = sphereMesh->CreateModel(0, 10, 0);
	player.vect3.vectorSet(0, 0, 1);
	player.state = Paused;
	player.radius = 10;

	float kDegRotationSpeed = 250;
	float kRadRotationSpeed = kDegRotationSpeed * (pi / 180);
	float kSphereSpeed = 50;

	int score = 0;

	stringstream outText;
	IFont* myFont = myEngine->LoadFont("Comic Sans", 36);
	IFont* largeFont = myEngine->LoadFont("Comic Sans", 72);

	States gameState = Playing;

	float deltaTime = myEngine->Timer();


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		deltaTime = myEngine->Timer();

		//Game Controller / logic
		if (myEngine->KeyHit(Key_P)) {
			gameState == Playing ? gameState = Pause : gameState = Playing;
		}
		if (myEngine->KeyHit(Key_Escape)) {
			myEngine->Stop();
		}
		if (gameState == Paused) {
			largeFont->Draw("GAME PAUSED", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kRed, kCentre, kVCentre);
		}
		
		if (player.model->GetX() > 100 || player.model->GetX() < -100 || player.model->GetZ() > 100 || player.model->GetZ() < -100) {
			gameState = GameOver;
		}
		if (score >= 120) {
			gameState = GameWon;
		}

		// Possible Game States
		switch (gameState)
		{
		// Paused state
		case Paused:
			break;

		// Playing State
		case Playing:

			// Movment Player Input
			player.state = Paused;
			if (myEngine->KeyHeld(Key_W)) {
				player.state = MovingFront;
			}
			if (myEngine->KeyHeld(Key_S)) {
				player.state = MovingBack;
			}
			if (myEngine->KeyHeld(Key_A)) {
				player.model->RotateY(-kDegRotationSpeed * deltaTime);
				player.vect3.rotateY(-kRadRotationSpeed * deltaTime);
			}
			if (myEngine->KeyHeld(Key_D)) {
				player.model->RotateY(kDegRotationSpeed * deltaTime);
				player.vect3.rotateY(kRadRotationSpeed * deltaTime);
			}

			// Moving Player
			switch (player.state) {
			case MovingFront:
				player.vect3.setLength(kSphereSpeed * deltaTime);
				player.vect3.move(player.model);
				break;
			case MovingBack:
				player.vect3.setLength(kSphereSpeed * deltaTime);
				player.vect3.move(player.model, -1);
				break;
			case Paused: break;
			default:
				break;
			}

			// Food Collision
			for (int i = 0; i < MAX_FOOD; i++) {
				if (SphereRectCollision(player.model, player.radius, food[i], 2)) {
					food[i]->SetPosition(random(-100, 100), 2, random(-100, 100));
					score += 10;
					if (score % 40 == 0) {
						player.model->Scale(1.2);
						player.radius *= 1.2;
						kSphereSpeed *= 0.8;
					}
				}
			}

			break;

		// Game Over State
		case GameOver:
			largeFont->Draw("You lost :( better luck next time", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kRed, kCentre, kVCentre);
			largeFont->Draw("press esc to exit", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2 + 70, kRed, kCentre, kVCentre);
			break;

		//Game Won State
		case GameWon:
			largeFont->Draw("Congratz! You Win!", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kRed, kCentre, kVCentre);
			largeFont->Draw("press esc to exit", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2 + 70, kRed, kCentre, kVCentre);
			break;

		default:
			break;
		}
		
		// Camera Controller
		if (myEngine->KeyHit(Key_1)) {
			CamSetTransform(myCamera, camView1, camview0);
			camview0 = camView1;
		}
		if (myEngine->KeyHit(Key_2)) {
			CamSetTransform(myCamera, camView2, camview0);
			camview0 = camView2;
		}

		// Score Display
		outText << "Score: " << score;
		myFont->Draw(outText.str(), myEngine->GetWidth() - 200, 20);
		outText.str("");
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
