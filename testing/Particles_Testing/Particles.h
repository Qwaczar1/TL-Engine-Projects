#pragma once
#include <TL-Engine.h>
#include <iostream>
#include <sstream>
#include "Vectors.h"
using namespace std;
using namespace tle;

enum spawner_Shapes {
	cone, rectangle, sphere, vector
};

struct particle {
	Vector vector;
	IModel* model;
	int age;
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
	float spawn_interval = 0;
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

	void moveParticles() {
		for (size_t i = 0; i < size; i++)
		{
			particles[i].vector.move(particles[i].model);
		}
	}

	void updateSystem() {

		if (size < maxsize)
		{
			cout << "\nSpawning new particle \n";
			switch (spawner_shape)
			{
			case cone:
				// Param1 serves as the cone's angle.  |  Param2 serves as the cone height  |  vect serves as the pointing direction of the spawner
				Vector particleVect = vect;
				//particleVect.printVector();
				particleVect.rotateOn(vect.crossProduct({ vect.y, vect.z, vect.x }), random(0, param1));
				//particleVect.printVector();
				particleVect.rotateOn(vect, random(0, 2 * pi));
				//particleVect.printVector();
				particleVect.setLength(random(minVelocity, maxVelocity));
				particles[size] = createParticle(particleVect, particleMesh, posVect);
				size++;
				//particleVect.printVector();
				break;
			case rectangle:
				// Param1 serves as the square's x length.  |  Param2 serves as the square's z length  |  vect serves as the pointing direction of the spawner
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
		}


		if (maxAge) {

		}
	}

};


particleSyst setupParticleSystem(IMesh* particleMesh, int MaxSize, float SpawnInterval, int maxAge, float spread, float minVelocity, float maxVelocity, Vector vect, Vector position, spawner_Shapes SpanerShape, float Param1, float Param2) {

	return { particleMesh, 0, MaxSize, SpawnInterval, maxAge, spread, minVelocity, maxVelocity, vect, position, SpanerShape, Param1, Param2 };

}

