#pragma once

#include "Contour.h"
#include "structs.h"
#include "Rect.h"

class Container {
	Size _m_size;
	std::vector<Contour> _m_emptyAreas;
	std::vector<Rect> _m_boxes;

public:
	Container(const Size& containerSize);
	bool tryToAddRect(const Size &rect);
	Size size() const {
		return _m_size;
	}

	std::vector<Rect> boxes() const {
		return _m_boxes;
	}
};