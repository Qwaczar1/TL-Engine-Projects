#pragma once
#include "Vectors.h"
#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;
using namespace std;

struct Matrix2D {
	int rows = 4;
	int columbs = 4;
	float matrix[4][4];
	

	/*void initalize() {
		matrix = new float*[rows];
		for (int i = 0; i < rows; i++) {
			matrix[i] = new float[columbs];
			for (int j = 0; j < columbs; j++)
			{
				matrix[i][j] = 0;
			}
		}
	}*/
	

	void martixFromModel(IModel* model) {
		model->GetMatrix(&matrix[0][0]);
	}

	Matrix2D operator + (Matrix2D input) {
		Matrix2D output;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columbs; j++)
			{
				output.matrix[i][j] = matrix[i][j] + input.matrix[i][j];
			}
		}
		return output;
	}

	Matrix2D operator * (float input) {
		Matrix2D output;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columbs; j++)
			{
				output.matrix[i][j] = matrix[i][j] * input;
			}
		}
		return output;
	}

	Matrix2D operator * (Matrix2D input) {
		Matrix2D output;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columbs; j++)
			{
				output.matrix[i][j] = 0;
				for (int n = 0; n < columbs; n++)
				{
					output.matrix[i][j] += matrix[i][n] * input.matrix[n][i];
				}
			}
		}
		return output;
	}

	//Matrix2D rotateX(float angle) {
	//	Matrix2D roationMatrix = {{1, 0, 0, 0}, {0, cos(-angle), -sin(angle), 0},	{0, sin(-angle), cos(-angle), 0}, {0, 0, 0, 1}};
	//	return matrix * roationMatrix;
	//}

	void print() {
		for (int i = 0; i < rows; i++)
		{
			cout << " [ ";
			for (int j = 0; j < columbs; j++)
			{
				cout << matrix[i][j] << "  ";
			}
			cout << "]\n";
		}
		cout << "\n";
	}
};