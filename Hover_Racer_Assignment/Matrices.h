#pragma once
#include "Vectors.h"
#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;
using namespace std;

enum axis {
	X, Y, Z
};
struct Matrix2D {
	float matrix[4][4];
	
	void martixFromModel(IModel* model) {
		model->GetMatrix(&matrix[0][0]);
	}

	void matrixToModel(IModel* model) {
		model->SetMatrix(matrix[0]);
	}



	void matrixFromVectors(Vector x, Vector y, Vector z) {
		matrix[0][0] = { x.x };
		matrix[0][1] = { x.y };
		matrix[0][2] = { x.z };
		
		matrix[1][0] = { y.x };
		matrix[1][1] = { y.y };
		matrix[1][2] = { y.z };
		
		matrix[2][0] = { z.x };
		matrix[2][1] = { z.y };
		matrix[2][2] = { z.z };
	}

	Vector vectorizeX() {
		return { matrix[0][0],matrix[0][1],matrix[0][2] };
	}
	Vector vectorizeY() {
		return { matrix[1][0],matrix[1][1],matrix[1][2] };
	}
	Vector vectorizeZ() {
		return { matrix[2][0],matrix[2][1],matrix[2][2] };
	}

	Matrix2D operator + (Vector input) {
		Matrix2D output;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				output.matrix[i][j] = matrix[i][j];
			}
		}
		output.matrix[3][0] += input.x;
		output.matrix[3][1] += input.y;
		output.matrix[3][2] += input.z;
		return output;
	}

	Matrix2D operator + (Matrix2D input) {
		Matrix2D output;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				output.matrix[i][j] = matrix[i][j] + input.matrix[i][j];
			}
		}
		return output;
	}

	Matrix2D operator * (float input) {
		Matrix2D output;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				output.matrix[i][j] = matrix[i][j] * input;
			}
		}
		return output;
	}

	Matrix2D operator * (Matrix2D input) {
		Matrix2D output;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				output.matrix[i][j] = 0;
				for (int n = 0; n < 4; n++)
				{
					output.matrix[i][j] += matrix[i][n] * input.matrix[n][i];
				}
			}
		}
		return output;
	}

	Matrix2D operator * (float input[4][4]) {
		Matrix2D output;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				output.matrix[i][j] = 0;
				for (int n = 0; n < 4; n++)
				{
					output.matrix[i][j] += matrix[i][n] * input[n][i];
				}
			}
		}
		return output;
	}

	void print() {
		cout << "\n";
		for (int i = 0; i < 4; i++)
		{
			cout << " [ ";
			for (int j = 0; j < 4; j++)
			{
				cout << matrix[i][j] << "  ";
			}
			cout << "]\n";
		}
	}
};

Matrix2D rotateX(Matrix2D matrix, float angle) {
	float roationMatrix[4][4] = {
		{1, 0, 0, 0},
		{0, cos(-angle), -sin(angle), 0},
		{0, sin(-angle), cos(-angle), 0},
		{0, 0, 0, 1} };
	return matrix * roationMatrix;
}
Matrix2D rotateY(Matrix2D matrix, float angle) {
	float roationMatrix[4][4] = {
		{cos(angle), 0, -sin(angle), 0},
		{0, 1, 0, 0},
		{sin(angle), 0, cos(angle), 0},
		{0, 0, 0, 1} };
	return matrix * roationMatrix;
}
Matrix2D rotateZ(Matrix2D matrix, float angle) {
	float roationMatrix[4][4] = {
		{cos(angle), sin(angle), 0, 0},
		{-sin(angle), cos(angle), 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1} };
	return matrix * roationMatrix;
}

Matrix2D rotateBy(Matrix2D matrix, Vector rotation) {
	return rotateZ(rotateY(rotateX(matrix, rotation.x), rotation.y), rotation.z);
}

Matrix2D rotateOn(Matrix2D matrix, Vector vect, float angle) {
	Vector newX = matrix.vectorizeX();
	newX.rotateOn(vect, angle);

	Vector newY = matrix.vectorizeX();
	newX.rotateOn(vect, angle);

	Vector newZ = matrix.vectorizeX();
	newX.rotateOn(vect, angle);

	matrix.matrixFromVectors(newX, newY, newZ);
	return matrix;
}

Matrix2D rotateTo(Matrix2D matrix, axis xyz, Vector target) {
	Vector axis;
	switch (xyz)
	{
	case X:
		axis = matrix.vectorizeX().crossProduct(target);
		break;
	case Y:
		axis = matrix.vectorizeX().crossProduct(target);
		break;
	case Z:
		axis = matrix.vectorizeX().crossProduct(target);
		break;
	default:
		break;
	}
	float angle = axis.angleDiffrence(target);

	return rotateOn(matrix, axis, angle);
}