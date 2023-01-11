#include "Tests.h"
#include <chrono>
#include <random>

Input Tests::generateRandomInput(const Size& conteinerSize, int boxCount)
{
	Input result;
	result.containerSize = conteinerSize;

	srand(std::chrono::system_clock::now().time_since_epoch().count());
	auto asd = rand() % int(result.containerSize.maxSide() * 0.5);
	for (int boxIndex = 0; boxIndex < boxCount; boxIndex++)
	{
		result.rectsSizes.insert(Size(double(rand() % int(result.containerSize.maxSide() * 0.5)),
			double(rand() % int(result.containerSize.maxSide() * 0.5))));
	}

	return result;
}
