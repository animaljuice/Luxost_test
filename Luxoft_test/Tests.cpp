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
		auto randW = rand() % int(result.containerSize.maxSide() * 0.5);
		auto randH = rand() % int(result.containerSize.maxSide() * 0.5);

		auto newIt = result.rectsSizes.insert(Size(randW == 0 ? 1 : randW, randH == 0 ? 1 : randH));
	}

	return result;
}
