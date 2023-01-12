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

	bool finish = false;
	std::vector<Container> containers;
	while (!finish)
	{
		finish = true;
		containers.emplace_back(initData.containerSize);
		for (auto boxIt = initData.rectsSizes.begin(); boxIt != initData.rectsSizes.end();) {
			if (containers.back().tryToAddRect(*boxIt)) {
				finish = false;
				boxIt = initData.rectsSizes.erase(boxIt);
			}
			else {
				++boxIt;
			}
		}
	}
	containers.pop_back();

	result.containerSize = initData.containerSize;
	for (size_t containerIndex = 0; containerIndex < containers.size(); containerIndex++){
		result.rects.resize(result.rects.size() + 1);
		auto boxes = containers[containerIndex].boxes();
		for (auto& resBox : boxes) {
			result.rects.back().push_back(resBox);
		}
	}	

	std::cout << result;
}