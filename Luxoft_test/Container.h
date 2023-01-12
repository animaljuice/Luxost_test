#pragma once

#include "AAPolygon.h"
#include "size.h"
#include "Recti.h"

class Container {
	Size _m_size;
	std::vector<AAPolygon> _m_emptyAreas;
	std::vector<Recti> _m_boxes;

public:
	Container(const Size& containerSize);
	bool tryToAddRect(const Size &rect);
	Size size() const {
		return _m_size;
	}

	std::vector<Recti> boxes() const {
		return _m_boxes;
	}
};