#include "image_math.h"

float conv_mat3(const mat3& matA, const mat3& matB)
{
	float result = 0;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			result += matA[i][j] * matB[2-i][2-j];
	return result;
}

float conv_mat4(const mat4& matA, const mat4& matB)
{
	float result = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result += matA[i][j] * matB[j][i];
	return result;
}

// 0-255
float sigmoid(float x)
{
	return 255 / (1 + pow(2.718, (-x + 100) / 40));
}
