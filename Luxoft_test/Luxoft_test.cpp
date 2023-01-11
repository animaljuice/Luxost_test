// Luxoft_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Tests.h"
#include "IO.h"
#include "Container.h"

Input initData;
Output result;

int main(int argc, char* argv[])
{
	std::cin >> initData;

	initData = Tests::generateRandomInput({ 400, 400 }, 100);

	Container c(initData.containerSize);
	for (auto boxIt = initData.rectsSizes.rbegin(); boxIt != initData.rectsSizes.rend(); boxIt++){
		if (c.tryToAddRect(*boxIt)) {

		}
	}

	result.containerSize = c.size();
	result.rects.resize(1);
	auto boxes = c.boxes();
	for (auto& resBox : boxes) {
		result.rects.front().push_back(resBox);
	}

	std::cout << result;
}