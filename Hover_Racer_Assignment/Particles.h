#pragma once
#include <TL-Engine.h>
#include <iostream>
#include <sstream>
#include "Vectors.h"
using namespace std;
using namespace tle;

enum spawner_Shapes {
	cone, rectangle, simpleRectangle, sphere, vector
};

struct particle {
	Vector vector;
	IModel* model;
	int age = 0;
	int type = 0;
};

particle createParticle(Vector vect, IMesh* mesh, Vector position) {
	particle newParticle;
	newParticle.vector = vect;
	newParticle.model = mesh->CreateModel(position.x, position.y, position.z);
	return newParticle;
}

float random(float LO, float HI) {
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}

struct particleSyst {
	//assignable variables
	IMesh* particleMesh;
	int size = 0;
	int maxsize = 0;
	int spawn_interval = 0;
	int maxAge = 10;
	float spread = 0;
	float minVelocity;
	float maxVelocity;
	Vector vect;
	Vector posVect;
	spawner_Shapes spawner_shape = sphere;
	float param1 = 1;
	float param2 = 1;

	//internal variables
	particle particles[500];
	int step = 0;
	float pi = 3.14159265359f;
	bool spawnedParticle = false;
	bool validPos = false;
	int spawnAttempt = 0;

	void moveParticles() {
		for (size_t i = 0; i < size; i++)
		{
			particles[i].vector.move(particles[i].model);
		}
	}

	void spawnParticles(int i = -1) {

		Vector particleVect = vect;
		Vector particlePos;

		switch (spawner_shape)
		{
		case cone:
			// Param1 serves as the cone's angle.  |  Param2 serves as the cone height  |  vect serves as the pointing direction of the spawner
			
			particleVect.rotateOn(vect.crossProduct({ vect.y, vect.z, vect.x }), random(0, param1));
			particleVect.rotateOn(vect, random(0, 2 * pi));
			particleVect.setLength(random(minVelocity, maxVelocity));
			if (i < 0) {
				particles[size] = createParticle(particleVect, particleMesh, posVect);
				size++;
			}
			else {
				particles[i].model->SetPosition(posVect.x, posVect.y, posVect.z);
				particles[i].vector = particleVect;
				particles[i].age = 0;
			}
			//particleVect.printVector();
			break;

		case rectangle:
			// Param1 serves as the square's x length.  |  Param2 serves as the square's z length  |  vect serves as the pointing direction of the spawner
			break;

		case simpleRectangle:
			//Same as the rectangle, but the rect is locked on the XZ plane
			// Param1 serves as the square's x length.  |  Param2 serves as the square's z length  |  vect serves as the pointing direction of the spawner

			spawnAttempt = 0;

			float distance;
			
			validPos = (spread == 0) ? true : false;

			if (validPos) {
				particlePos = { random(-2 * param1, 2 * param1), random(-2 * param2, 2 * param2), 0 };
			}
			
			while (!validPos && spawnAttempt < 10)
			{
				validPos = true;
				spawnAttempt++;
				particlePos = { random(-2 * param1, 2 * param1), 0, random(-2 * param2, 2 * param2) };
				for (int i = 0; i < size; i++)
				{
					distance = particlePos.findDist({ particles[i].model->GetX() ,particles[i].model->GetY() ,particles[i].model->GetZ() });

					if (distance < spread) {
						validPos = false;
						//cout << "Invalid Possition! Attempt : " << spawnAttempt << "\n";
					}
				}
			}
			//cout << "Found Position! \n";

			particlePos = particlePos.addVector(posVect);
			particleVect.setLength(random(minVelocity, maxVelocity));
			if (i < 0) {
				particles[size] = createParticle(particleVect, particleMesh, particlePos);
				size++;
			}
			else {
				particles[i].model->SetPosition(particlePos.x, particlePos.y, particlePos.z);
				particles[i].vector = particleVect;
				particles[i].age = 0;
			}
			break;

		case sphere:
			// Param1 serves as the sphere's Radius.  |  Param2 serves no prupos  |  vect serves as the pointing direction of the spawner
			break;

		case vector:
			// Param1 serves as the cone's Radius.  |  Param2 serves as the cone height  |  vect serves as the pointing direction of the spawner
			break;

		default:
			break;
		}
		spawnedParticle = true;
	}

	void respawnParticle(int i) {
		spawnParticles(i);
	}

	void ageParticles() {
		for (int i = 0; i < size; i++)
		{
			if (particles[i].age >= maxAge) {
				respawnParticle(i);
			}
			particles[i].age++;
		}
	}

	void updateSystem() {
		if (maxAge != 0) {
			ageParticles();
		}
		if (spawn_interval == 0) {
			for (int i = 0; i < maxsize; i++){
				if (size < maxsize && spawnedParticle == false) {
					spawnParticles();
				}
				spawnedParticle = false;
			}
		}
		else {
			if (step % spawn_interval == 0) {
				if (size < maxsize && spawnedParticle == false) {
					spawnParticles();
				}
				spawnedParticle = false;
			}
		}
		
		moveParticles();
		step++;
	}
};


particleSyst setupParticleSystem(IMesh* particleMesh, int MaxSize, int SpawnInterval, int maxAge, float spread, float minVelocity, float maxVelocity, Vector vect, Vector position, spawner_Shapes SpanerShape, float Param1, float Param2) {

	return { particleMesh, 0, MaxSize, SpawnInterval, maxAge, spread, minVelocity, maxVelocity, vect, position, SpanerShape, Param1, Param2 };

}