// Lab5_GravityCube_Project.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

typedef struct xyzPos {
	float x;
	float y;
	float z;
};

float random(int min, int max) {
	return ((rand() % (max - min + 1)) + min);
}

float xyzDist(xyzPos pos1, xyzPos pos2){
	float x = pos1.x - pos2.x;
	float y = pos1.y - pos2.y;
	float z = pos1.z - pos2.z;
	return sqrt(x * x + y * y + z * z);
}

float clamp(float min, float max, float x) {
	if (x > max) { return max; }
	else if (x < min) { return min; }
	else { return x; }
}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media\\Lab5_Assets" );

	/**** Set up your scene here ****/
	srand(time(0));
	ICamera* myCamera = myEngine->CreateCamera(kFPS);
	myCamera->SetPosition(0, 0, -130);

	IMesh* backgroundMesh = myEngine->LoadMesh("background.x");
	IMesh* enemyCubeMesh = myEngine->LoadMesh("enemyCube.x");
	IMesh* platformMesh = myEngine->LoadMesh("platform.x");
	IMesh* playerCubeMesh = myEngine->LoadMesh("playerCube.x");

	IModel* background0 = backgroundMesh->CreateModel(0, 0, 10);
	IModel* platform0 = platformMesh->CreateModel();
	IModel* platform1 = platformMesh->CreateModel();
	platform0->AttachToParent(background0);
	platform1->AttachToParent(background0);
	platform0->SetLocalPosition(0, 60, -10);
	platform1->SetLocalPosition(0, -60, -10);

	IModel* background1 = backgroundMesh->CreateModel(260, 0, 10);
	IModel* platform2 = platformMesh->CreateModel();
	IModel* platform3 = platformMesh->CreateModel();
	platform2->AttachToParent(background1);
	platform3->AttachToParent(background1);
	platform2->SetLocalPosition(0, 60, -10);
	platform3->SetLocalPosition(0, -60, -10);

	const int enemyAmount = 2;
	xyzPos enemyPos[enemyAmount];
	IModel* enemy[enemyAmount];
	for (int i = 0; i < enemyAmount; i++) {
		enemy[i] = enemyCubeMesh->CreateModel(100, random(-40, 40), -10);
		i % 2 == 0 ? enemy[i]->AttachToParent(background0) : enemy[i]->AttachToParent(background1);
	}

	IModel* playerCube = playerCubeMesh->CreateModel(0, -40, 0);

	enum ePositionState {Bottom, Top, MovingUp, MovingDown};
	ePositionState playerPosition = Bottom;

	const float kGravity = -0.2f;
	const float kGameSpeed = -0.05f;
	float betweenPlatformTime = 80.0f / kGravity + 1;

	xyzPos playerPos;
	int gameSkin = 0;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/*** Frame Prep ***/
		playerPos.x = playerCube->GetX();
		playerPos.y = playerCube->GetY();
		playerPos.z = playerCube->GetZ();

		for (int i = 0; i < enemyAmount; i++){
			enemyPos[i].x = enemy[i]->GetX();
			enemyPos[i].y = enemy[i]->GetY();
			enemyPos[i].z = enemy[i]->GetZ();
		}

		/**** Update your scene each frame here ****/

		/* Player Logic */
		switch (playerPosition) {
		case Bottom: if (myEngine->KeyHit(Key_Space)) { playerPosition = MovingUp; } break;
		case Top: if (myEngine->KeyHit(Key_Space)) { playerPosition = MovingDown; } break;
		case MovingDown: if (playerPos.y + kGravity < -40) { 
			playerPosition = Bottom; 
		}else {playerCube->MoveY(kGravity);
			playerCube->RotateZ(90.0 / betweenPlatformTime);
		} break;
		case MovingUp: if (playerPos.y - kGravity > 40) {
			playerPosition = Top;
		}else { playerCube->MoveY(-kGravity);
		playerCube->RotateZ(-90.0 / betweenPlatformTime);} break;
		}

		/* Collision Detection */
		for (int i = 0; i < enemyAmount; i++){
			if (xyzDist(playerPos, enemyPos[i]) < 20) {
				myEngine->Stop();
			}
		}

		/* Player Skin Swapper */
		if (myEngine->KeyHit(Key_C)) { ++gameSkin; }
		switch (gameSkin) {
		case 0: playerCube->SetSkin("yellow.jpg"); break;
		case 1: playerCube->SetSkin("red.jpg"); break;
		default: gameSkin = 0;
		}

		/* Background Logic */
		if (background0->GetX() < -260) {
			background0->SetX(260);
		}
		else if (background1->GetX() < -260) {
			background1->SetX(260);
		}
		background0->MoveX(kGameSpeed);
		background1->MoveX(kGameSpeed);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
