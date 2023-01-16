// Luxoft_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Tests.h"
#include "IO.h"
#include "Container.h"

Input initData;
Output result;

int main(int argc, char* argv[])
{
	// parsing input
	std::cin >> initData;
	//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	//initData = Tests::generateRandomInput({ 450, 450 }, 1000);

	bool finish = false;
	std::vector<Container> containers;

	while (!finish)
	{
		finish = true;
		containers.emplace_back(initData.containerSize);
		for (auto boxIt = initData.rectsSizes.begin(); boxIt != initData.rectsSizes.end();) {
			//fill the container in descending order of rect size
			if (containers.back().tryToInsertRect(*boxIt)) {
				// exit the loop after it fails to fit all of the remaining rects
				finish = false;
				boxIt = initData.rectsSizes.erase(boxIt);
			}
			else {
				++boxIt;
			}
		}
	}
	containers.pop_back();

	// fill the program output structure
	result.containerSize = initData.containerSize;
	for (size_t containerIndex = 0; containerIndex < containers.size(); containerIndex++) {
		result.rects.resize(result.rects.size() + 1);
		auto boxes = containers[containerIndex].boxes();
		for (auto& resBox : boxes) {
			result.rects.back().push_back(resBox);
		}
	}

	std::cout << result;

	/*std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;*/
}