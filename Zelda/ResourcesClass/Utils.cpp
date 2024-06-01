#include "Utils.h"

bool CheckBounds(int firstX, int firstY, int secondX, int secondY, float distance)
{
	float deltaX = firstX - secondX;
	float deltaY = firstY - secondY;
	float euclideanDistance = sqrt(deltaX * deltaX + deltaY * deltaY);

	return euclideanDistance <= distance;
}

unsigned int WindowWidth()
{
	return WINDOWWIDTH;
}

unsigned int WindowHeight()
{
	return WINDOWHEIGHT;
}

int getRandomInt(int X)
{
	// Crea un generador de n�meros aleatorios usando el dispositivo aleatorio
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define la distribuci�n en el rango [0, X]
	std::uniform_int_distribution<> distrib(50, X-50);

	// Genera y devuelve el n�mero aleatorio
	return distrib(gen);
}