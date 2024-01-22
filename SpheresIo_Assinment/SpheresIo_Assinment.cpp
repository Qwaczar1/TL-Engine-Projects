// SpheresIo_Assinment.cpp: A program using the TL-Engine
/*
Developed by Naya Zhukova (petr zhukov)
G number : 21054529

This program is the First assignment for moduel : CO1301
Of the Course : BSc Computing
*/

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <iostream>
#include <sstream>
#include "Vectors.h"
#include "Particles.h"
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

enum PlayerStates {
	Regular, Hyper
};

typedef struct Player {
	IModel* model;
	Vector vect3;
	Vector posVect;
	MovementStates Mstate;
	PlayerStates Pstate = Regular;
	float radius = 5.0f;
	float hyperTimer = 0.0;
	bool alive = true;

	void findPosVect() {
		posVect = { model->GetX(), model->GetY(), model->GetZ() };
	}

	void movementLogic(float speed) {
		switch (Mstate) {
		case MovingFront:
			vect3.setLength(speed);
			vect3.move(model);
			break;
		case MovingBack:
			vect3.setLength(speed);
			vect3.move(model, -1);
			break;
		case Paused: break;
		default:
			break;
		}
	}

	void checkIfOutOfBounds(float range) {
		findPosVect();
		if (posVect.x > range || posVect.x < -range || posVect.z > range || posVect.z < -range) {
			alive = false;
			model->MoveY(-1000);
		}
	}
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

	// Setting Rand Seed
	srand(time(NULL));

	// Loading Meshs
	IMesh* islandMesh = myEngine->LoadMesh("island.x");
	IMesh* foodMesh = myEngine->LoadMesh("minicube.x");
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

	// Player Setup
	Player player;
	player.model = sphereMesh->CreateModel(0, 10, 0);
	player.vect3.vectorSet(0, 0, 1);
	player.Mstate = Paused;
	player.radius = 10;

	// Enemy Setup
	Player enemy;
	enemy.model = sphereMesh->CreateModel(80, 10, 80);
	enemy.model->SetSkin("enemysphere.jpg");
	enemy.vect3.vectorSet(0, 0, 1);
	enemy.Mstate = Paused;
	enemy.radius = 10;	
	Vector closestFood = { 0, 0, 0 };
	Vector playerEnemyVect;

	// food spawn
	Vector systemVect = { 1, 0, 0 };
	Vector systemPositionVect = { 0, 4, 0 };
	particleSyst foodSystem = setupParticleSystem(foodMesh, 12, 0, 0, 10, 0, 0, systemVect, systemPositionVect, simpleRectangle, 48, 48);

	//creating 1 special food
	foodSystem.updateSystem();
	for (int i = 0; i < foodSystem.size; i++)
	{
		if (SphereRectCollision(player.model, player.radius, foodSystem.particles[i].model, 4) && !SphereRectCollision(enemy.model, enemy.radius, foodSystem.particles[i].model, 4)) {
			for (bool validFood = false; !validFood;) {
				foodSystem.respawnParticle(i);
				if (!SphereRectCollision(player.model, player.radius, foodSystem.particles[i].model, 4) && !SphereRectCollision(enemy.model, enemy.radius, foodSystem.particles[i].model, 4)) {
					validFood = true;
				}
			}
		}
	}
	foodSystem.particles[0].type = 1;
	foodSystem.particles[0].model->SetSkin("hypercube.jpg");


	float kDegRotationSpeed = 250;
	float kRadRotationSpeed = kDegRotationSpeed * (pi / 180);
	float kSphereSpeed = 50;

	int score = 0;
	int enemyScore = 0;

	stringstream playerScoreDisplay;
	stringstream enemyScoreDisplay;
	IFont* scoreFont = myEngine->LoadFont("Comic Sans", 36);
	IFont* anouncementFont = myEngine->LoadFont("Comic Sans", 72);

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
			anouncementFont->Draw("GAME PAUSED", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kRed, kCentre, kVCentre);
		}
		if (score >= 120) {
			gameState = GameWon;
			enemy.alive = false;
		}
		if (!player.alive || enemyScore >= 120) {
			gameState = GameOver;
		}

		player.checkIfOutOfBounds(100);
		enemy.checkIfOutOfBounds(100);

		// Possible Game States
		switch (gameState)
		{
		// Paused state
		case Paused:
			break;

		// Game State
		case Playing:

			if (player.alive) {
				// Movment Player Input
				player.Mstate = Paused;
				if (myEngine->KeyHeld(Key_W)) {
					player.Mstate = MovingFront;
				}
				if (myEngine->KeyHeld(Key_S)) {
					player.Mstate = MovingBack;
				}
				if (myEngine->KeyHeld(Key_A)) {
					player.model->RotateY(-kDegRotationSpeed * deltaTime);
					player.vect3.rotateY(-kRadRotationSpeed * deltaTime);
				}
				if (myEngine->KeyHeld(Key_D)) {
					player.model->RotateY(kDegRotationSpeed * deltaTime);
					player.vect3.rotateY(kRadRotationSpeed * deltaTime);
				}

				// Player Movement States
				player.movementLogic(kSphereSpeed * deltaTime * 10 / player.radius);

				// Player States

				switch (player.Pstate) {
				case Regular:
					player.model->SetSkin("regularsphere.jpg");
					break;

				case Hyper:
					for (int i = 0; i < foodSystem.size; i++)
					{
						player.model->SetSkin("hypersphere.jpg");

						foodSystem.particles[i].vector = {
							foodSystem.particles[i].model->GetX() - player.model->GetX(),
							foodSystem.particles[i].model->GetY() - player.model->GetY(),
							foodSystem.particles[i].model->GetZ() - player.model->GetZ() };
						foodSystem.particles[i].vector.getLength();
						if (foodSystem.particles[i].vector.length > 50) {
							foodSystem.particles[i].vector = { 0,0,0 };
						}
						else {
							foodSystem.particles[i].vector.setLength(-0.05);
						}

					}
					player.hyperTimer -= deltaTime;
					if (player.hyperTimer < 0) {
						player.Pstate = Regular;
						for (int i = 0; i < foodSystem.size; i++) {
							foodSystem.particles[i].vector = { 0,0,0 };
						}
					}
					break;

				default:
					break;
				}
			}

			// Player ~ Enemy colision
			player.findPosVect();
			enemy.findPosVect();
			// cout << "distance = " << player.posVect.findDist(enemy.posVect) << "   range = " << player.radius + enemy.radius << "\n";
			playerEnemyVect = player.posVect.findDifrence(enemy.posVect);
			playerEnemyVect.getLength();
			if (playerEnemyVect.length < player.radius + enemy.radius) {
				
				if (score - enemyScore > 40) {
					enemy.alive = false;
					enemy.model->MoveY(-100);
				}
				else if (enemyScore - score > 40) {
					player.alive = false;
					player.model->MoveY(-100);
				}
				else {
					playerEnemyVect.setLength(playerEnemyVect.length - player.radius - enemy.radius);
					playerEnemyVect.move(player.model, -1);
					playerEnemyVect.move(enemy.model, 1);
				}

			}

			break;

		// Game Over State
		case GameOver:
			anouncementFont->Draw("You lost :( better luck next time", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kRed, kCentre, kVCentre);
			anouncementFont->Draw("press esc to exit", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2 + 70, kRed, kCentre, kVCentre);
			break;

		//Game Won State
		case GameWon:
			anouncementFont->Draw("Congratz! You Win!", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kRed, kCentre, kVCentre);
			anouncementFont->Draw("press esc to exit", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2 + 70, kRed, kCentre, kVCentre);
			break;

		default:
			break;
		}

		// Making enemy funcion during gameover case.
		if (gameState == Playing || GameOver) {

			// Food Systems
			foodSystem.updateSystem();

			// Food Collision
			// Player
			for (int i = 0; i < foodSystem.size; i++) {
				if (SphereRectCollision(player.model, player.radius, foodSystem.particles[i].model, 4)) {
					for (bool validFood = false; !validFood;) {
						foodSystem.respawnParticle(i);
						if (!SphereRectCollision(player.model, player.radius, foodSystem.particles[i].model, 4) && !SphereRectCollision(enemy.model, enemy.radius, foodSystem.particles[i].model, 4)) {
							validFood = true;
						}
					}
					if (foodSystem.particles[i].type == 1) {
						player.Pstate = Hyper;
						player.hyperTimer = 5;
					}
					score += 10;
					if (score % 10 == 0) {
						player.model->Scale(1 + 0.5 / score);
						player.radius *= 1 + 0.5 / score;
					}
				}
			}
			// Enemy
			for (int i = 0; i < foodSystem.size; i++) {
				if (SphereRectCollision(enemy.model, enemy.radius, foodSystem.particles[i].model, 4)) {
					for (bool validFood = false; !validFood;) {
						foodSystem.respawnParticle(i);
						if (!SphereRectCollision(player.model, player.radius, foodSystem.particles[i].model, 4) && !SphereRectCollision(enemy.model, enemy.radius, foodSystem.particles[i].model, 4)) {
							validFood = true;
						}
					}
					if (foodSystem.particles[i].type == 1) {
						enemy.Pstate = Hyper;
						enemy.hyperTimer = 5;
					}
					enemyScore += 10;
					if (enemyScore % 10 == 0) {
						enemy.model->Scale(1 + 0.5 / enemy.radius);
						enemy.radius *= 1 + 0.5 / enemy.radius;
					}
				}
			}

			// Enemy AI
			// Finding Closest Food
			if (enemy.alive) {
				enemy.findPosVect();
				closestFood = { foodSystem.particles[0].model->GetX() - enemy.model->GetX(), 0, foodSystem.particles[0].model->GetZ() - enemy.model->GetZ() };

				for (int i = 0; i < foodSystem.size; i++)
				{
					closestFood.getLength();
					if (closestFood.length > enemy.posVect.findDist({ foodSystem.particles[i].model->GetX(), 0, foodSystem.particles[i].model->GetZ() })) {
						closestFood = { foodSystem.particles[i].model->GetX() - enemy.model->GetX(), 0, foodSystem.particles[i].model->GetZ() - enemy.model->GetZ() };
					}
				}
				// Moving towards closest food
				enemy.Mstate = Paused;
				if (enemy.vect3.normalize().dotProduct(closestFood.normalize()) > 0.999) {
					enemy.Mstate = MovingFront;
					//cout << "facing food. dot product = " << enemy.vect3.normalize().dotProduct(closestFood.normalize()) << "\n";
				}
				else {
					if (enemy.vect3.crossProduct({ 0,1,0 }).dotProduct(closestFood) > 0) {
						//cout << " turning Left\n";
						enemy.vect3.rotateY(-kRadRotationSpeed * deltaTime);
						enemy.model->RotateY(-kDegRotationSpeed * deltaTime);
					}
					else {
						//cout << " turning Right\n";
						enemy.vect3.rotateY(kRadRotationSpeed * deltaTime);
						enemy.model->RotateY(kDegRotationSpeed * deltaTime);
					}

				}

				enemy.movementLogic(kSphereSpeed * deltaTime * 10 / enemy.radius);

				// Enemy States

				switch (enemy.Pstate) {
				case Regular:
					enemy.model->SetSkin("enemysphere.jpg");
					break;

				case Hyper:
					for (int i = 0; i < foodSystem.size; i++)
					{
						enemy.model->SetSkin("hypersphere.jpg");

						foodSystem.particles[i].vector = {
							foodSystem.particles[i].model->GetX() - enemy.model->GetX(),
							foodSystem.particles[i].model->GetY() - enemy.model->GetY(),
							foodSystem.particles[i].model->GetZ() - enemy.model->GetZ() };
						foodSystem.particles[i].vector.getLength();
						if (foodSystem.particles[i].vector.length > 50) {
							foodSystem.particles[i].vector = { 0,0,0 };
						}
						else {
							foodSystem.particles[i].vector.setLength(-0.05);
						}

					}
					enemy.hyperTimer -= deltaTime;
					if (enemy.hyperTimer < 0) {
						enemy.Pstate = Regular;
						for (int i = 0; i < foodSystem.size; i++) {
							foodSystem.particles[i].vector = { 0,0,0 };
						}
					}
					break;

				default:
					break;
				}
			}
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

		if (score > enemyScore) {
			// Player is winning
			playerScoreDisplay << "Player Score: " << score;
			scoreFont->Draw(playerScoreDisplay.str(), myEngine->GetWidth() - 260, 20);
			playerScoreDisplay.str("");

			enemyScoreDisplay << "Enemy Score: " << enemyScore;
			scoreFont->Draw(enemyScoreDisplay.str(), myEngine->GetWidth() - 260, 50);
			enemyScoreDisplay.str("");
		}
		else {
			// Enemy is winning
			playerScoreDisplay << "Player Score: " << score;
			scoreFont->Draw(playerScoreDisplay.str(), myEngine->GetWidth() - 260, 50);
			playerScoreDisplay.str("");

			enemyScoreDisplay << "Enemy Score: " << enemyScore;
			scoreFont->Draw(enemyScoreDisplay.str(), myEngine->GetWidth() - 260, 20);
			enemyScoreDisplay.str("");
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
