// Lab6_Vectors_Project.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;
using namespace std;

const int PARTICLESKINSLENGTH = 5;
const string particleSkins[] = {"particleSkin1.tga", "particleSkin2.tga", "particleSkin3.tga", "particleSkin4.tga", "particleSkin5.tga"};

enum eParticleMotionState{
	Moving, Paused
};

enum eParticleState {
	Default, ChangeSkin
};

typedef struct Vector3 {
	float xMovement = 0;
	float yMovement = 0;
	float zMovement = 0;
	float length = sqrt((xMovement * xMovement) + (yMovement * yMovement) + (zMovement * zMovement));
};

typedef struct BasicParicle {
	IModel* model;
	Vector3 vect3;
	eParticleMotionState motionState;
	eParticleState state;
	int skin;
};

void ParticleChangeColor(IModel* model, int colorNum, BasicParicle& particle) {
	if (colorNum > PARTICLESKINSLENGTH) {
		particle.skin = (particle.skin + 1) % PARTICLESKINSLENGTH;
		model->SetSkin(particleSkins[particle.skin]);
	}	else {
		particle.skin = colorNum;
		particle.model->SetSkin(particleSkins[colorNum]);
	}
}

void SetVector(Vector3* vect, float x, float y, float z) {
	vect->xMovement = x;
	vect->yMovement = y;
	vect->zMovement = z;
}

Vector3 Vector3Add(Vector3 vect1, Vector3 vect2) {
	Vector3 vectorSum;
	vectorSum.xMovement = vect1.xMovement + vect2.xMovement;
	vectorSum.yMovement = vect1.yMovement + vect2.yMovement;
	vectorSum.zMovement = vect1.zMovement + vect2.zMovement;
	return vectorSum;
}

Vector3 Vecotr3SetLength(Vector3 vect, float length) {
	Vector3 newVector;
	float lengthRatio = length / vect.length;
	newVector.xMovement = vect.xMovement * lengthRatio;
	newVector.yMovement = vect.yMovement * lengthRatio;
	newVector.zMovement = vect.zMovement * lengthRatio;
	return newVector;
}

void VectorMove(IModel* model, Vector3 vect) {
	model->Move(vect.xMovement, vect.yMovement, vect.zMovement);
}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media\\Lab6_Assets" );

	/**** Set up your scene here ****/
	ICamera* myCamera = myEngine->CreateCamera(kManual, 0, 0, -60);

	IMesh* starsMesh = myEngine->LoadMesh("stars.x");
	IModel* stars = starsMesh->CreateModel();
	IMesh* particleMesh = myEngine->LoadMesh("particle.x");

	Vector3 particleVect3;
	particleVect3.xMovement = 0.0f;
	particleVect3.yMovement = -0.005f;
	particleVect3.zMovement = 0.0f;

	const int particleAmount = 5;
	const float kInitialY = 30;
	const float kFinalY = -30;
	const float kMaxX = 40;
	const float kSpread = 20;
	BasicParicle particle[particleAmount];
	for (int i = 0; i < particleAmount; i++){
		particle[i].model = particleMesh->CreateModel(kMaxX - (i * kSpread), kInitialY, 0);
		particle[i].vect3 = particleVect3;
		particle[i].motionState = Moving;
		particle[i].skin = 0;
		ParticleChangeColor(particle[i].model, i, particle[i]);
	}

	Vector3 windVect3;
	windVect3.xMovement = -0.005f;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		//particle->Move(particleVect3.xMovement, particleVect3.yMovement, particleVect3.zMovement);
		
		for (int i = 0; i < particleAmount; i++){
			switch (particle[i].motionState){
			case Moving: 
				VectorMove(particle[i].model, Vector3Add(particle[i].vect3, windVect3));
				if (particle[i].model->GetY() < kFinalY) {
					particle[i].model->SetY(kInitialY);
				}
				if (particle[i].model->GetX() > 55) {
					particle[i].model->SetX(-54);
				}
				if (particle[i].model->GetX() < -55) {
					particle[i].model->SetX(54);
				}
				break;
			case Paused:
				break;
			default:
				break;
			}
		}

		if (myEngine->KeyHit(Key_0)){
			switch (particle[0].motionState){
			case Moving: particle[0].motionState = Paused; break;
			case Paused: particle[0].motionState = Moving; break;
			}
		}
		if (myEngine->KeyHit(Key_1)){
			switch (particle[1].motionState){
			case Moving: particle[1].motionState = Paused; break;
			case Paused: particle[1].motionState = Moving; break;
			}
		}
		if (myEngine->KeyHit(Key_2)){
			switch (particle[2].motionState){
			case Moving: particle[2].motionState = Paused; break;
			case Paused: particle[2].motionState = Moving; break;
			}
		}
		if (myEngine->KeyHit(Key_3)){
			switch (particle[3].motionState){
			case Moving: particle[3].motionState = Paused; break;
			case Paused: particle[3].motionState = Moving; break;
			}
		}
		if (myEngine->KeyHit(Key_4)){
			switch (particle[4].motionState){
			case Moving: particle[4].motionState = Paused; break;
			case Paused: particle[4].motionState = Moving; break;
			}
		}

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
