#include "Tests.h"
#include <chrono>
#include <random>

Input Tests::generateRandomInput(const Size& conteinerSize, int boxCount)
{
	Input result;
	result.containerSize = conteinerSize;

	srand(std::chrono::system_clock::now().time_since_epoch().count());
	for (int boxIndex = 0; boxIndex < boxCount; boxIndex++)
	{
		auto randW = rand() % int(result.containerSize.maxSide() * 0.1);
		auto randH = rand() % int(result.containerSize.maxSide() * 0.1);

		result.rectsSizes.push_back(Size(randW == 0 ? 1 : randW, randH == 0 ? 1 : randH));
	}
	result.rectsSizes.emplace_back(conteinerSize.width() / 2, conteinerSize.height() / 2);
	result.sort();

	return result;
}
