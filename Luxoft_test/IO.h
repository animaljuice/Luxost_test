#pragma once

#include "Rect.h"
#include <iostream>
#include <set>

struct Input
{
	Size containerSize;
	std::multiset<Size> rectsSizes;

	friend std::istream& operator>>(std::istream& input, Input& in);
};

struct Output {
	Size containerSize;
	std::vector<std::vector<Rect>> rects;

	friend std::ostream& operator<<(std::ostream& os, const Output& dt);
};

inline std::ostream& operator<<(std::ostream& os, const Output& out)
{
	os << out.rects.size() << ',' << out.containerSize << std::endl;
	for (size_t boxIndex = 0; boxIndex < out.rects.size(); boxIndex++) {
		for (size_t rectIndex = 0; rectIndex < out.rects[boxIndex].size(); rectIndex++) {
			os << boxIndex << ',' << out.rects[boxIndex][rectIndex] << std::endl;
		}
	}
	return os;
}

inline std::istream& operator>>(std::istream& is, Input& in) {
	std::string inLine;
	while (!is.eof())
	{
		// TODO: perhaps it's worth to check the correctness of the input data, but it's not first priority in this work
		is >> inLine;
		auto commaIndex = inLine.find(',');
		static bool isContainer = true;
		auto firstNumber = std::atoi(inLine.substr(0, commaIndex).c_str());
		auto secondNumber = std::atoi(inLine.substr(commaIndex + 1).c_str());
		Size currentSize(firstNumber, secondNumber);
		if (isContainer) {
			in.containerSize = currentSize;
			isContainer = false;
		}
		else {
			if (in.containerSize.minSide() >= currentSize.minSide()) {
				in.rectsSizes.insert(currentSize);
			}
		}
	}

	return is;
}
